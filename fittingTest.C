#include<fstream>
#include<string>
#include<iostream>
#include<sstream>
using namespace std;

#include<TF1.h>
#include<TGraphErrors.h>
#include<TGraph2DErrors.h>
#include<TF2.h>

#include"testFuncs.h"


enum { MaxNumStates = 25, NumAngleCuts=9};

int lengths[9]={7,7,7,7,7,7,7,6,7};

const char* fileName = "testPoints.csv";

double xVal[9][7];
double exVal[9][7];
double yVal[9][7];
double eyVal[9][7];
double zVal[9][7];
double ezVal[9][7];

double tempAberParams[numParams];
double tempAngleArr[NumAngleCuts];
double tempAngleErrArr[NumAngleCuts];
double tempOldExArr[MaxNumStates];
double tempOldExErrArr[MaxNumStates];
double tempCorrExArr[MaxNumStates];
double tempCorrExErrArr[MaxNumStates];
double tempAngleFitParams[exOrder+1][NumAngleCuts];
double tempAngleFitParamErrs[exOrder+1][NumAngleCuts];
TGraph2DErrors corrGraph;
TF2 corrFit("CorrectionFit", &correctionFunction, 0.0, 55.0, -1.0, 1.0, numParams);
TGraph2D residGraph;

void parsePtLine(const string& line, int angleInd, int ptNum)
{
	istringstream conv;
	//read x
	int ind = line.find(',');
	string temp = line.substr(0,ind);
	string tempLine = line.substr(ind+1);
	conv.str(temp);
	conv>>(xVal[angleInd][ptNum]);
	conv.clear();
	//read ex
	ind = tempLine.find(',');
	temp = tempLine.substr(0,ind);
	tempLine = tempLine.substr(ind+1);
	conv.str(temp);
	conv>>(exVal[angleInd][ptNum]);
	conv.clear();
	//read y
	ind = tempLine.find(',');
	temp = tempLine.substr(0,ind);
	tempLine = tempLine.substr(ind+1);
	conv.str(temp);
	conv>>(yVal[angleInd][ptNum]);
	conv.clear();
	//read ey
	ind = tempLine.find(',');
	temp = tempLine.substr(0,ind);
	tempLine = tempLine.substr(ind+1);
	conv.str(temp);
	conv>>(eyVal[angleInd][ptNum]);
	conv.clear();
	//read z
	ind = tempLine.find(',');
	temp = tempLine.substr(0,ind);
	tempLine = tempLine.substr(ind+1);
	conv.str(temp);
	conv>>(zVal[angleInd][ptNum]);
	conv.clear();
	//read ez
	ind = tempLine.find(',');
	temp = tempLine.substr(0,ind);
	tempLine = tempLine.substr(ind+1);
	conv.str(temp);
	conv>>(ezVal[angleInd][ptNum]);
	conv.clear();
}

void loadArrays()
{
	ifstream input;
	input.open(fileName);
	string line;
	getline(input, line);
	int ptCount = 0;
	int angleIndex=0;
	while(!input.eof() && angleIndex<9)
	{
		parsePtLine(line, angleIndex, ptCount);
		getline(input, line);
		++ptCount;
		if(ptCount>=lengths[angleIndex])
		{
			++angleIndex;
			ptCount=0;
		}
	}
}

void initFitSingleAngle(int numFitPts, int angleNum)
{
	//first create the TGraphErrors
	TGraphErrors* tempAngleFitPts = new TGraphErrors(numFitPts,tempOldExArr,tempCorrExArr,tempOldExErrArr, tempCorrExErrArr);
	tempAngleFitPts->Draw();
	//write the polynomial in old ex energy that we are using
	ostringstream formNamer;
	formNamer<<"pol"<<exOrder<<"(0)";
	string tempFormula = formNamer.str();
	//create the fitting function
	TF1* tempAngleFitFunc = new TF1("tempAngleFitFormula",tempFormula.c_str(),0.6*tempOldExArr[0],1.4*tempOldExArr[numFitPts-1]);
	tempAngleFitFunc->SetParameters(tempAngleFitInitParams);
	//do the fit using with no plotting, quiet mode, and using the range that was specified in the function
	tempAngleFitPts->Fit(tempAngleFitFunc,"Q O R","");
	cout<<"\n\nFit report for angle: "<<angleNum<<" with "<<numFitPts<<" points"<<endl;
	tempAngleFitFunc->Print();
	//grab the fitted parameters
	double* angleFitParams = tempAngleFitFunc->GetParameters();
	double* angleFitParamErrs = tempAngleFitFunc->GetParErrors();
	//transfer the parameters into the temporary array for them
	for(int i=0; i<=exOrder; ++i)
	{
		tempAngleFitParams[i][angleNum] = angleFitParams[i];
		tempAngleFitParamErrs[i][angleNum] = angleFitParamErrs[i];
	}
	//clean up after ourselves
	delete tempAngleFitFunc;
	delete tempAngleFitPts;
}

void initFitSingleParamSet(int paramNum, int numAngles)
{
	//create the TGraphErrors for this run
	TGraphErrors* tempParamFitPts = new TGraphErrors(numAngles, tempAngleArr, tempAngleFitParams[paramNum], tempAngleErrArr, tempAngleFitParamErrs[paramNum]);
	//write the polynomial in angle that we are using
	ostringstream formNamer;
	formNamer<<"pol"<<thOrder<<"(0)";
	string tempFormula = formNamer.str();
	//create the fitting function
	TF1* tempParamFitFunc = new TF1("tempParamFitFormula",tempFormula.c_str(),1.4*tempAngleArr[0],1.4*tempAngleArr[numAngles-1]);
	//set a few initial guestimates for the constant and linear terms
	double x1 = tempAngleArr[0];
	double y1 = tempAngleFitParams[paramNum][0];
	double x2 = tempAngleArr[numAngles-1];
	double y2 = tempAngleFitParams[paramNum][numAngles-1];
	double slope = ((y2-y1)/(x2-x1));
	double offset = (y2-(slope*x2));
	tempParamFitInitParams[0]=offset;
	tempParamFitInitParams[1]=slope;
	tempParamFitFunc->SetParameters(tempParamFitInitParams);
	cout<<"\n\nFit report for paramset: "<<paramNum<<" with "<<numAngles<<" points"<<endl;
	//do the fit using with no plotting, quiet mode, and using the range that was specified in the function
	tempParamFitPts->Fit(tempParamFitFunc,"Q O R","");
	tempParamFitFunc->Print();
	//grab the fitted parameters we do not need the errors from this because these are serving as initial points for a more
	//global fit instead of as points feeding into another fit function
	double* paramFitParams = tempParamFitFunc->GetParameters();
	for(int i=0; i<=thOrder; ++i)
	{
		tempAberParams[i*exSize+paramNum] = paramFitParams[i];
	}
	//clean up the allocated objects
	delete tempParamFitFunc;
	delete tempParamFitPts;
}

void prepInitialParams()
{
	int numAngles=0;
	for(int i=0; i<9; ++i)
	{
		tempAngleArr[numAngles] = yVal[i][0];
		tempAngleErrArr[numAngles] = eyVal[i][0];
		++numAngles;
		int count = 0;
		for(int j=0; j<lengths[i]; ++j)
		{
			tempOldExArr[count] = xVal[i][j];
			tempOldExErrArr[count] = exVal[i][j];
			tempCorrExArr[count] = zVal[i][j];
			tempCorrExErrArr[count] = ezVal[i][j];
			++count;
		}
		//now fit the temporary data points
		initFitSingleAngle(count, numAngles-1);
	}
	cout<<"Done with angle fits, proceding to param range fits"<<endl;
	//now that we have the initial sets of parameters extracted, now we get the
	//theta dependence out of the parameters with additional fits
	for(int i=0; i<=exOrder; ++i)
	{
		initFitSingleParamSet(i,numAngles);
	}
	//tell the user what the initia fit values were
	cout<<"Done setting initial values, they are as follows\n"<<endl;
	for(int i=0; i<=thOrder; ++i)
	{
		for(int j=0; j<=exOrder; ++j)
		{
			cout<<"p"<<(i*exSize+j)<<"  a"<<i<<","<<j<<"     ";
			cout<<tempAberParams[i*exSize+j]<<endl;
		}
	}	
}

void loadGraph()
{
	//now iterate through all the correction points for this run and add them one by one to the graph
	int count=0;
	for(int i=0; i<9; ++i)
	{
		for(int j=0; j<lengths[i]; ++j)
		{
			corrGraph.SetPoint(count, xVal[i][j], yVal[i][j], zVal[i][j]);
			corrGraph.SetPointError(count, exVal[i][j], eyVal[i][j], ezVal[i][j]);
			++count;
		}
	}
}

void dispCorrPts()
{
	//now draw the graph
	corrGraph.SetMarkerSize(2);
	corrGraph.SetMarkerStyle(8);
	corrGraph.SetMargin(0.4);
	corrGraph.Draw("PCOL ERR");
}

void dispResids()
{
	//now draw the graph
	residGraph.SetMarkerSize(2);
	residGraph.SetMarkerStyle(8);
	residGraph.SetMargin(0.4);
	residGraph.Draw("PCOL");
}

void drawGraphOfPoints()
{
	corrGraph.SetMarkerSize(2);
	corrGraph.SetMarkerColor(1);
	corrGraph.SetMarkerStyle(8);
	corrGraph.SetMargin(0.4);
	corrGraph.Draw("SAME PO ERR");
}

void doFitCorr()
{
	dispCorrPts();
	prepInitialParams();
	corrFit.SetParameters(tempAberParams);
	corrGraph.Fit(&corrFit,"O");
	corrFit.SetRange(0.6*corrGraph.GetXmin(),1.4*corrGraph.GetYmin(),1.4*corrGraph.GetXmax(),1.4*corrGraph.GetYmax());
	corrFit.Draw("surf1");
	drawGraphOfPoints();
}

void improveFit()
{
	corrGraph.Fit(&corrFit,"O M");
	corrFit.SetRange(0.6*corrGraph.GetXmin(),1.4*corrGraph.GetYmin(),1.4*corrGraph.GetXmax(),1.4*corrGraph.GetYmax());
	corrFit.Draw("surf1");
	drawGraphOfPoints();
}

void getResids()
{
	//get the residuals
	int count=0;
	for(int i=0; i<9; ++i)
	{
		for(int j=0; j<lengths[i]; ++j)
		{
			double resid = (zVal[i][j]-corrFit.Eval(xVal[i][j],yVal[i][j]));
			residGraph.SetPoint(count, xVal[i][j], yVal[i][j], resid);
			++count;
		}
	}
}

void exportFunction()
{
	//now open the file
	ofstream output;
	output.open("testFunc.csv");
	output<<"run #, a00, a01, a02, a03, a04, a10, a11, .., a43, a44, where each term is: aij*(theta^i)*(ex^j)"<<endl;
	double* paramArray = corrFit.GetParameters();
	output<<"1153,"<<paramArray[0];
	for(int j=1; j<numParams; ++j)
	{
		output<<","<<paramArray[j];
	}
	output<<endl;
	output.close();
}

