//
//  CompetitionScenario.cpp
//

#include "CompetitionScenario.h"
#include <cstring>
#include <cstdio>
#include <string>
#include <cmath>
#include <curl/curl.h>
#include "json/json.h"

static size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
  ((std::string*)userp)->append((char*)contents, size * nmemb);
  return size*nmemb;
}

CompetitionScenario::CompetitionScenario(int sc) {
    num = sc;
    char urlbuff[100];
    sprintf(urlbuff, "http://windflo.com/scenarios/%d", sc);

    CURL *curl;
    CURLcode res;
    std::string readBuffer;

    curl = curl_easy_init();
    if (curl) {
      curl_easy_setopt(curl, CURLOPT_URL, urlbuff);
      curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
      curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
    }

    res = curl_easy_perform(curl);
    if(res != CURLE_OK) {
      fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
    }

    curl_easy_cleanup(curl);

    Json::Value root;
    Json::Reader reader;
    bool parsingSuccessful = reader.parse( readBuffer, root, false );
    if ( !parsingSuccessful ) {
      fprintf(stderr, "json parse failed: %s\n", reader.getFormattedErrorMessages().c_str());
    }

    width = root["width"].asDouble();
    height = root["height"].asDouble();

    Json::Value json_obstacles = root["obstacles"];
    obstacles = Matrix<double>(json_obstacles.size(), 4);

    for (int i=0; i < json_obstacles.size(); ++i) {
      Json::Value obstacle = json_obstacles[i];
      obstacles.set(i, 0, obstacle["xmin"].asDouble());
      obstacles.set(i, 1, obstacle["ymin"].asDouble());
      obstacles.set(i, 2, obstacle["xmax"].asDouble());
      obstacles.set(i, 3, obstacle["ymax"].asDouble());
    }

    // these are currently all the same across scenarios, so aren't in the xml files
    CT=0.8;
    PRated=1500.0;
    R=38.5;
    eta=-500.0;
    k=0.0750;
    lambda=140.86;
    vCin=3.5;
    vCout=20;
    vRated=14;
}

CompetitionScenario::CompetitionScenario(const CompetitionScenario& wsc):
num(wsc.num),
CT(wsc.CT),
PRated(wsc.PRated),
R(wsc.R),
eta(wsc.eta),
k(wsc.k),
lambda(wsc.lambda),
vCin(wsc.vCin),
vCout(wsc.vCout),
vRated(wsc.vRated),
width(wsc.width),
height(wsc.height),
obstacles(wsc.obstacles) {
  NbActiveScenario(1);
}
