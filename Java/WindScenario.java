import java.io.File;

import javax.xml.parsers.DocumentBuilder;
import javax.xml.parsers.DocumentBuilderFactory;

import org.w3c.dom.Document;
import org.w3c.dom.Element;
import org.w3c.dom.Node;
import org.w3c.dom.NodeList;

public class WindScenario {
	// wind resources
	public double ks[];
	public double c[];
	public double omegas[];
	public double thetas[][];

	// Farm parameters
	public double CT;
	public double farmRadius;
	public double PRated;
	public double R;
	public double eta;
	public double k;
	public double lambda;
	public double vCin;
	public double vCout;
	public double vRated;
	public double width;
	public double height;
	public int nturbines;
	public double wakeFreeEnergy;
    public double obstacles[][]; // rows of [xmin, ymin, xmax, ymax]

	// Optimization parameters
    protected static double fac=Math.PI/180;
    protected double coSinMidThetas[][];
    public double rkRatio;
    public double krRatio;
    public double vints[];
    protected double wblcdfValues[];
    protected double wblcdfAccuracy;
    protected double cMax, cMin;
    public double atan_k;
    public double trans_CT;
    public double minDist;

    WindScenario(String xmlFileName) throws Exception {
    	File fXmlFile = new File(xmlFileName);
    	DocumentBuilderFactory dbFactory = DocumentBuilderFactory.newInstance();
    	DocumentBuilder dBuilder = dbFactory.newDocumentBuilder();
    	Document doc = dBuilder.parse(fXmlFile);

    	doc.getDocumentElement().normalize();

        c=new double[24];
        ks=new double[24];
        omegas=new double[24];
        thetas=new double[24][2];

        for (int i=0; i<24; i++) {
            thetas[i][0]=i*15.0;
            thetas[i][1]=(i+1)*15.0;
        }
    	NodeList angleList = doc.getElementsByTagName("angle");
    	for (int i=0; i<angleList.getLength(); i++) {
    		Node angle=angleList.item(i);
    		if (angle.getNodeType()==Node.ELEMENT_NODE) {
    			Element eAngle=(Element)angle;
    			c[i]=Double.parseDouble(eAngle.getAttribute("c"));
    			ks[i]=Double.parseDouble(eAngle.getAttribute("k"));
    			omegas[i]=Double.parseDouble(eAngle.getAttribute("omega"));
    		}
    	}

    	NodeList obstacleList = doc.getElementsByTagName("obstacle");
    	obstacles=new double[obstacleList.getLength()][4];
    	for (int i=0; i<obstacleList.getLength(); i++) {
    		Node obstacle=obstacleList.item(i);
    		if (obstacle.getNodeType()==Node.ELEMENT_NODE) {
    			Element eobstacle=(Element)obstacle;
    			obstacles[i][0]=Double.parseDouble(eobstacle.getAttribute("xmin"));
    			obstacles[i][1]=Double.parseDouble(eobstacle.getAttribute("ymin"));
    			obstacles[i][2]=Double.parseDouble(eobstacle.getAttribute("xmax"));
    			obstacles[i][3]=Double.parseDouble(eobstacle.getAttribute("ymax"));
    		}
    	}

    	width=Double.parseDouble(((Node)(doc.getElementsByTagName("Width").item(0))).getTextContent());
    	height=Double.parseDouble(((Node)(doc.getElementsByTagName("Height").item(0))).getTextContent());
    	nturbines=Integer.parseInt(((Node)(doc.getElementsByTagName("NTurbines").item(0))).getTextContent());
    	wakeFreeEnergy=Double.parseDouble(((Node)(doc.getElementsByTagName("WakeFreeEnergy").item(0))).getTextContent());

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

        initOptimizationParameters();
    }

    void initOptimizationParameters() {
        coSinMidThetas=new double[thetas.length][2];
        for (int thets=0; thets<thetas.length; thets++) {
            double theta=(thetas[thets][0]+thetas[thets][1])/2.0*fac;
            coSinMidThetas[thets][0]=Math.cos(theta);
            coSinMidThetas[thets][1]=Math.sin(theta);
        }
        rkRatio=R/k;
        krRatio=k/R;
        vints=new double[(int)(2.0*vRated-7.0+1.0)];
        for (int i=0; i<vints.length; i++) {
          vints[i]=3.5+(double)i*0.5;
        }
        atan_k=Math.atan(k);
        trans_CT=1.0-Math.sqrt(1.0-CT);
        minDist=64.0*R*R;
    }

    double getCosMidThetas(int thetIndex) {
    	return coSinMidThetas[thetIndex][0];
    }
    
    double getSinMidThetas(int thetIndex) {
    	return coSinMidThetas[thetIndex][1];
    }

    
	double getWblcdfVints(double c, int vintIndex, int ksIndex) {
		return wblcdfValues[(int)((c-cMin)/wblcdfAccuracy)*(vints.length+1)*ks.length+vintIndex*ks.length+ksIndex];
	}

	double getWblcdfVrated(double c, int ksIndex) {
		return wblcdfValues[(int)((c-cMin)/wblcdfAccuracy)*(vints.length+1)*ks.length+vints.length*ks.length+ksIndex];

	}

	static public double wblcdf(double x, double sc, double sh) {
		return 1.0-Math.exp(-fastPow(x/sc,sh));
	}

	static public double fastPow(double a, double b) {
		if (Math.abs(b-2)<0.0001) {
			return a*a;
		} if (Math.abs(b-1)<0.0001) {
			return a;
		} if (Math.abs(b)<0.0001) {
			return 1;
		} else {
			return Math.pow(a,b);
		}
	}

}
