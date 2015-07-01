#include "CompetitionEvaluator.h"
#include "json/json.h"
#include "curl/curl.h"
#include <sstream>

static size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
  ((std::string*)userp)->append((char*)contents, size * nmemb);
  return size*nmemb;
}

CompetitionEvaluator::CompetitionEvaluator() {
  tspe=NULL;
  tpositions=NULL;
}

void CompetitionEvaluator::initialize(CompetitionScenario& sc, const char* token) {
  scenario=CompetitionScenario(sc);
  wakeFreeEnergy=0;
  energyCapture=0;
  if (tspe) delete tspe;
  if (tpositions) delete tpositions;
  tspe=NULL;
  tpositions=NULL;
  tfitnesses=NULL;
  nEvals=0;
  energyCost = std::numeric_limits<double>::max();
  user_token = token;
  CURL *curl;
  CURLcode res;
  std::string readBuffer;

  curl = curl_easy_init();

  struct curl_slist *headers_orig = NULL;
  struct curl_slist *headers;

  std::string post_data;

  if (curl) {
    headers = curl_slist_append(headers_orig, "Content-Type:application/json");

    post_data = "{\"api_token\":\"" + user_token + "\"}";

    curl_easy_setopt(curl, CURLOPT_URL, "http://windflo.com/runs/");
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, post_data.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
  }

  res = curl_easy_perform(curl);
  if(res != CURLE_OK) {
    fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
  }

  curl_easy_cleanup(curl);
  curl_slist_free_all(headers_orig);
  curl_slist_free_all(headers);

  Json::Value root;
  Json::Reader reader;
  bool parsingSuccessful = reader.parse( readBuffer, root, false );
  if ( !parsingSuccessful ) {
    fprintf(stderr, "json parse failed: %s\n", reader.getFormattedErrorMessages().c_str());
  }

  run_token = root["token"].asString();

}

CompetitionEvaluator::~CompetitionEvaluator() {
  if (tspe) delete tspe;
  if (tpositions) delete tpositions;
  if (tfitnesses) delete tfitnesses;
}

double CompetitionEvaluator::evaluate(Matrix<double>* layout) {
  if (tpositions) delete tpositions;
  tpositions=new Matrix<double>(layout);
  if (tspe) delete tspe;
  if (tfitnesses) delete tfitnesses;
  energyCapture=0;
  wakeFreeRatio=0;

  CURL *curl;
  CURLcode res;
  std::string readBuffer;
  std::string post_data;

  curl = curl_easy_init();
  struct curl_slist *headers_orig = NULL;
  struct curl_slist *headers;
  if (curl) {
    headers = curl_slist_append(headers_orig, "Content-Type:application/json");

    post_data = post_data + "{\"api_token\":\"" + user_token + "\",";
    post_data = post_data + "\"run\":\"" + run_token + "\",";
    post_data = post_data + "\"scenario\":" + std::to_string(scenario.num) + ",";
    post_data = post_data + "\"xs\":[";
    for (int i=0; i<tpositions->rows; ++i) {
      post_data = post_data + std::to_string(tpositions->get(i, 0));
      if (i<tpositions->rows-1) {
        post_data = post_data + ",";
      }
    }
    post_data = post_data + "],\"ys\":[";
    for (int i=0; i<tpositions->rows; ++i) {
      post_data = post_data + std::to_string(tpositions->get(i, 1));
      if (i<tpositions->rows-1) {
        post_data = post_data + ",";
      }
    }
    post_data = post_data + "]}";

    curl_easy_setopt(curl, CURLOPT_URL, "http://windflo.com/evaluate/");
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, post_data.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
  }

  res = curl_easy_perform(curl);
  if(res != CURLE_OK) {
    fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
  }

  curl_easy_cleanup(curl);
  curl_slist_free_all(headers_orig);
  curl_slist_free_all(headers);

  Json::Value root;
  Json::Reader reader;
  bool parsingSuccessful = reader.parse( readBuffer, root, false );
  if ( !parsingSuccessful ) {
    fprintf(stderr, "json parse failed: %s\n", reader.getFormattedErrorMessages().c_str());
  }

  tspe=new Matrix<double>(24, tpositions->rows);
  tfitnesses=new Matrix<double>(tpositions->rows, 1);

  energyCapture = root["energy_output"].asDouble();
  energyCost = root["energy_cost"].asDouble();
  wakeFreeRatio = root["wake_free_ratio"].asDouble();
  nEvals = root["evals"].asInt();
  Json::Value turbine_fitnesses = root["turbine_fitnesses"];
  for (int i=0; i < turbine_fitnesses.size(); ++i) {
    Json::Value turbine_fitness = turbine_fitnesses[i];
    tfitnesses->set(i, 0, turbine_fitness[0].asDouble());
  }
  Json::Value energy_outputs = root["energy_outputs"];
  for (int i=0; i < energy_outputs.size(); ++i) {
    Json::Value direction = energy_outputs[i];
    for (int j=0; j < direction.size(); ++j) {
      tspe->set(i, j, direction[j].asDouble());
    }
  }

  return energyCost;
}

Matrix<double>* CompetitionEvaluator::getEnergyOutputs() {
  if (!tspe) return NULL;
  Matrix<double>* res = new Matrix<double>(tspe);
  return res;
}

Matrix<double>* CompetitionEvaluator::getTurbineFitnesses() {
  if (!tspe) return NULL;
  Matrix<double>* res = new Matrix<double>(tfitnesses);
  return res;
}

bool CompetitionEvaluator::checkConstraint(Matrix<double>* layout) {
  static const double minDist=64.0*scenario.R*scenario.R;
  for (int i=0; i<layout->rows; i++) {
    // check boundaries
    if (tpositions->get(i, 0) < 0 || tpositions->get(i, 0) > scenario.width ||
        tpositions->get(i, 1) < 0 || tpositions->get(i, 1) > scenario.height) {
      return false;
    }
    // check for obstacles
    for (int j=0; j<scenario.obstacles.rows; j++) {
      if (layout->get(i, 0) > scenario.obstacles.get(j, 0) &&
	  layout->get(i, 0) < scenario.obstacles.get(j, 2) &&
	  layout->get(i, 1) > scenario.obstacles.get(j, 1) &&
	  layout->get(i, 1) < scenario.obstacles.get(j, 3)) {
	printf("Obstacle %d [%f, %f, %f, %f] violated by turbine %d (%f, %f)\n", 
	       j, scenario.obstacles.get(j, 0), scenario.obstacles.get(j, 1),
	       scenario.obstacles.get(j, 2), scenario.obstacles.get(j, 3), i);
	return false;
      }
    }
    // checking security distance constraint
    for (int j=0; j<layout->rows; j++) {
      if (i!=j) {
	// calculate the sqared distance between both turbs
	double dist=(layout->get(i, 0)-layout->get(j, 0))*(layout->get(i, 0)-layout->get(j, 0))+
	  (layout->get(i, 1)-layout->get(j, 1))*(layout->get(i, 1)-layout->get(j, 1));
	if (dist<minDist) {
          //printf("dist:\t%f\t<\t%f\t(%d,%d)\n",dist,minDist,i,j);
	  return false;
	}
      }
    }
  }
  return true;
}
