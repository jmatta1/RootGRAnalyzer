
const int exOrder=4;
const int exSize=5;
const int thOrder=4;
const int numParams=25;

const double tempAngleFitInitParams[5]={0.0,1.0,0.0,0.0,0.0};
double tempParamFitInitParams[5]={0.0,0.0,0.0,0.0,0.0};

double correctionFunction(double *x, double *par)
{
	double result=0.0;
	double ex=x[0];
	double th=x[1];
	double tempEx=1.0;
	double tempTh=1.0;
	for(int i=0; i<=thOrder; ++i)
	{
		tempEx=1.0;
		int intermediate = (i*exSize);
		for(int j=0; j<=exOrder; ++j)
		{
			result += (par[intermediate+j]*tempEx*tempTh);
			tempEx*=ex;
		}
		tempTh*=th;
	}
	return result;
}

inline double absVal(double val)
{
	return ((val<0)?(-val):(val));
}

struct CorrFunc
{
	int runNum;
	double params[numParams];
	
	double eval(double oldEx, double angle)
	{
		double result = 0.0;
		double tempEx=1.0;
		double tempTh=1.0;
		for(int i=0; i<=thOrder; ++i)
		{
			tempEx=1.0;
			int intermediate = (i*exSize);
			for(int j=0; j<=exOrder; ++j)
			{
				result += (params[intermediate+j]*tempEx*tempTh);
				tempEx*=oldEx;
			}
			tempTh*=angle;
		}
		return result;
	}
};
