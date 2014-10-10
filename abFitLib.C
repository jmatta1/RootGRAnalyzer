/*
To use this file,
1) Load root.
2) in CINT type ".L abFitLib.C+"

the user should only be using a few functions

loadRun( runNumber, FileName)
fitCorrFunc()
improveFit()
dispCorrPts()
fitDisplay()
exportFunction(fileName)

*/
#include<fstream>
#include<string>
#include<iostream>
#include<sstream>
using namespace std;

#include<TF1.h>
#include<TGraphErrors.h>
#include<TGraph2DErrors.h>
#include<TF2.h>
#include<TCanvas.h>

#include"guiSupport.h"

enum { MaxNumStates = 25, NumAngleCuts=11};

//global variables
//hold raw correction point data
double xVal[NumAngleCuts][MaxNumStates];
double exVal[NumAngleCuts][MaxNumStates];
double yVal[NumAngleCuts][MaxNumStates];
double eyVal[NumAngleCuts][MaxNumStates];
double zVal[NumAngleCuts][MaxNumStates];
double ezVal[NumAngleCuts][MaxNumStates];
//holds the number of states in each angle
int lengths[NumAngleCuts];
//holds stuff for the current fit
double tempAberParams[numParams];
double tempAngleArr[NumAngleCuts];
double tempAngleErrArr[NumAngleCuts];
double tempOldExArr[MaxNumStates];
double tempOldExErrArr[MaxNumStates];
double tempCorrExArr[MaxNumStates];
double tempCorrExErrArr[MaxNumStates];
double tempAngleFitParams[exOrder+1][NumAngleCuts];
double tempAngleFitParamErrs[exOrder+1][NumAngleCuts];
TGraph2DErrors* corrGraph=NULL;
TF2* corrFit=NULL;
TGraph2D* residGraph=NULL;
//TGraphErrors tempPlot;

TCanvas* canvas=NULL;

int numAngles=0;
int currRunNumber=0;

const double tempAngleFitInitParams[5]={0.0,1.0,0.0,0.0,0.0};
double tempParamFitInitParams[5]={0.0,0.0,0.0,0.0,0.0};

//function prototypes
void parsePtLine(const string& line, int &tempRunNum, double &tempOldEx, double &tempOldErr,
			double &tempAngle, double &tempAngleErr, double &tempCorrEx, double &tempCorrErr);
void loadRun(int runNumber, string filename);
void fitCorrFunc();
void prepInitialParams();
void initFitSingleAngle(int numFitPts, int angleNum);
void initFitSingleParamSet(int paramNum);
void dispCorrPts();
void dispResids();
void drawGraphOfPoints();
void getResids();
void improveFit();
void loadGraph();
void fitDisplay();

void fitCorrFunc()
{
	dispCorrPts();
	prepInitialParams();
	corrFit->SetParameters(tempAberParams);
	corrGraph->Fit(corrFit,"O");
	getResids();
	fitDisplay();
}

void improveFit()
{
	corrGraph->Fit(corrFit,"O M");
	corrFit->SetRange(0.6*corrGraph->GetXmin(),1.4*corrGraph->GetYmin(),1.4*corrGraph->GetXmax(),1.4*corrGraph->GetYmax());
	corrFit->Draw("surf1");
	getResids();
	fitDisplay();
}

void fitDisplay()
{
	canvas->Clear();
	corrFit->SetRange(0.6*corrGraph->GetXmin(),1.4*corrGraph->GetYmin(),1.4*corrGraph->GetXmax(),1.4*corrGraph->GetYmax());
	canvas->Divide(2,1);
	canvas->cd(1);
	corrFit->Draw("surf1");
	drawGraphOfPoints();
	canvas->cd(2);
	dispResids();
}

void prepInitialParams()
{
	int angleCount=0;
	cout<<"NumAngles: "<<numAngles<<endl;
	for(int i=0; i<numAngles; ++i)
	{
		tempAngleArr[angleCount] = (yVal[i][0]/2.0);
		tempAngleErrArr[angleCount] = (eyVal[i][0]/2.0);
		++angleCount;
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
		initFitSingleAngle(count, i);
	}
	cout<<"Done with angle fits, proceding to param range fits"<<endl;
	//now that we have the initial sets of parameters extracted, now we get the
	//theta dependence out of the parameters with additional fits
	for(int i=0; i<=exOrder; ++i)
	{
		initFitSingleParamSet(i);
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

void dispCorrPts()
{
	canvas->Clear();
	//now draw the graph
	corrGraph->SetMarkerSize(2);
	corrGraph->SetMarkerStyle(8);
	corrGraph->SetMargin(0.4);
	corrGraph->Draw("PCOL ERR");
}

void dispResids()
{
	//now draw the graph
	residGraph->SetMarkerSize(2);
	residGraph->SetMarkerStyle(8);
	residGraph->SetMargin(0.4);
	residGraph->Draw("PCOL");
}

void drawGraphOfPoints()
{
	corrGraph->SetMarkerSize(2);
	corrGraph->SetMarkerColor(1);
	corrGraph->SetMarkerStyle(8);
	corrGraph->SetMargin(0.4);
	corrGraph->Draw("SAME PO ERR");
}

void getResids()
{
	//get the residuals
	int count=0;
	for(int i=0; i<numAngles; ++i)
	{
		for(int j=0; j<lengths[i]; ++j)
		{
			double resid = (zVal[i][j]-corrFit->Eval(xVal[i][j],yVal[i][j]));
			residGraph->SetPoint(count, xVal[i][j], yVal[i][j], resid);
			++count;
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

void initFitSingleParamSet(int paramNum)
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

void loadGraph()
{
	//now iterate through all the correction points for this run and add them one by one to the graph
	int count=0;
	for(int i=0; i<numAngles; ++i)
	{
		for(int j=0; j<lengths[i]; ++j)
		{
			corrGraph->SetPoint(count, xVal[i][j], yVal[i][j], zVal[i][j]);
			corrGraph->SetPointError(count, exVal[i][j], eyVal[i][j], ezVal[i][j]);
			++count;
		}
	}
}

void loadRun(int runNumber, string filename)
{
	currRunNumber = runNumber;
	if(canvas==NULL)
	{
		canvas = new TCanvas("Canvas");
	}
	
	if(corrGraph!=NULL)
	{
		delete corrGraph;
	}
	if(corrFit!=NULL)
	{
		delete corrFit;
	}
	if(residGraph!=NULL)
	{
		delete residGraph;
	}
	
	corrFit = new TF2("CorrectionFit", &correctionFunction, 0.0, 55.0, -1.0, 1.0, numParams);
	corrGraph = new TGraph2DErrors;
	residGraph = new TGraph2D;
	
	numAngles=0;//reset num angles
	ifstream input;
	input.open(filename.c_str());
	string line;
	getline(input, line);//skip the first line
	getline(input, line);
	int ptCount = 0;
	int angleIndex=-1;
	double prevAngle=-10000.0;
	while(!input.eof() && angleIndex<NumAngleCuts)
	{
		int tempRunNum;
		double tempOldEx;
		double tempOldErr;
		double tempAngle;
		double tempAngleErr;
		double tempCorrEx;
		double tempCorrErr;
		parsePtLine(line, tempRunNum, tempOldEx, tempOldErr, tempAngle, tempAngleErr, tempCorrEx, tempCorrErr);
		if(tempRunNum==runNumber)
		{
			//the point is in the run we want
			if(tempAngle<(prevAngle+0.01) && tempAngle>(prevAngle-0.01))
			{
				//the point is in the angle set that we have been in before
				//increment ptCount and leave angleIndex as before
				++ptCount;
			}
			else
			{
				//the angle has changed from before
				//first set the length of this angle
				if(angleIndex>-1)
				{
					lengths[angleIndex]=(ptCount+1);
				}
				//increment the number of angles
				++numAngles;
				//set previous angle to the new one
				prevAngle=tempAngle;
				//then increment angleIndex, set ptcount to 0 and proceed
				++angleIndex;
				ptCount=0;
			}
			if(angleIndex>=NumAngleCuts)
			{
				break;
			}
			if(ptCount>=MaxNumStates)
			{
				break;
			}
			xVal[angleIndex][ptCount]=tempOldEx;
			exVal[angleIndex][ptCount]=tempOldErr;
			yVal[angleIndex][ptCount]=tempAngle;
			eyVal[angleIndex][ptCount]=tempAngleErr;
			zVal[angleIndex][ptCount]=tempCorrEx;
			ezVal[angleIndex][ptCount]=tempCorrErr;
		}
		getline(input, line);
	}
	lengths[angleIndex]=(ptCount+1);
	loadGraph();

	//output of points for debugging/testing
/*	for(int i=0; i<numAngles; ++i)
	{
		for(int j=0; j<lengths[i]; ++j)
		{
			cout<<xVal[i][j]<<"  "<<exVal[i][j]<<"  "<<yVal[i][j]<<"  ";
			cout<<eyVal[i][j]<<"  "<<zVal[i][j]<<"  "<<ezVal[i][j]<<endl;
		}
	}
*/
}

void parsePtLine(const string& line, int &tempRunNum, double &tempOldEx, double &tempOldErr,
			double &tempAngle, double &tempAngleErr, double &tempCorrEx, double &tempCorrErr)
{
	istringstream conv;
	//read runNum
	int ind = line.find(',');
	string temp = line.substr(0,ind);
	string tempLine = line.substr(ind+1);
	conv.str(temp);
	conv>>tempRunNum;
	conv.clear();
	//read x
	ind = tempLine.find(',');
	temp = tempLine.substr(0,ind);
	tempLine = tempLine.substr(ind+1);
	conv.str(temp);
	conv>>tempOldEx;
	conv.clear();
	//read ex
	ind = tempLine.find(',');
	temp = tempLine.substr(0,ind);
	tempLine = tempLine.substr(ind+1);
	conv.str(temp);
	conv>>tempOldErr;
	conv.clear();
	//read y
	ind = tempLine.find(',');
	temp = tempLine.substr(0,ind);
	tempLine = tempLine.substr(ind+1);
	conv.str(temp);
	conv>>tempAngle;
	conv.clear();
	//read ey
	ind = tempLine.find(',');
	temp = tempLine.substr(0,ind);
	tempLine = tempLine.substr(ind+1);
	conv.str(temp);
	conv>>tempAngleErr;
	conv.clear();
	//read z
	ind = tempLine.find(',');
	temp = tempLine.substr(0,ind);
	tempLine = tempLine.substr(ind+1);
	conv.str(temp);
	conv>>tempCorrEx;
	conv.clear();
	//read ez
	ind = tempLine.find(',');
	temp = tempLine.substr(0,ind);
	tempLine = tempLine.substr(ind+1);
	conv.str(temp);
	conv>>tempCorrErr;
	conv.clear();
}

void exportFunction(const string &fileName)
{
	//now open the file
	ofstream output;
	output.open(fileName.c_str());
	output<<"run #, a00, a01, a02, a03, a04, a10, a11, .., a43, a44, where each term is: aij*(theta^i)*(ex^j)"<<endl;
	double* paramArray = corrFit->GetParameters();
	output<<currRunNumber<<","<<paramArray[0];
	for(int j=1; j<numParams; ++j)
	{
		output<<","<<paramArray[j];
	}
	output<<endl;
	output.close();
}

