#ifndef ADJUST_DIALOGS_H
#define ADJUST_DIALOGS_H

char* GPNameList[] = {"H", "P", "W"};
char* PBNameList[] = {"a ", "b ", "c ", "d "};

#include<sstream>
#include<iostream>
using std::ostringstream;
using std::cout;
using std::endl;
#include<TGWindow.h>
#include<TGFrame.h>
#include<TGLabel.h>
#include<TGNumberEntry.h>
#include<TGButton.h>
#include<RQ_OBJECT.h>

class ParamAdjustmentDialog
{
	RQ_OBJECT("ParamAdjustmentDialog");
public:
	ParamAdjustmentDialog(const TGWindow* parent, UInt_t width, UInt_t height);
	~ParamAdjustmentDialog();
	
	void initWithParams(int numPeaks, int nP, double* vals);
	
	void updateParams(double* values);
	void retrieveData(double* values, double* lowerBnds, double* upperBnds, bool* useBnds);
	
	void show();
	void hide();
	
private:

	void clearPrevious();	
	
	int numParams;
	
	//main window
	TGMainFrame *mainWindow;
	TGVerticalFrame* mainOrganizer; //the organizer for all the sub frames
	TGHorizontalFrame* labelFrame; //the frame with the list of labels for columns
	TGVerticalFrame* paramOrganizer; //the organizer for all the sub frames
	TGHorizontalFrame** paramFrames;//the list of frames for parameters one per parameter
	
	//column labels
	TGLabel* paramColLbl;
	TGLabel* lbColLbl;
	TGLabel* valColLbl;
	TGLabel* ubColLbl;
	TGLabel* bcbColLbl;
	
	//row by row items
	TGLabel** paramNames;//labels for parameter names
	TGNumberEntry** lowerBounds;//number entries for parameter lower bounds
	TGNumberEntry** paramValues;//number entries for parameter starting values
	TGNumberEntry** upperBounds;//number entries for parameter upper bounds
	TGCheckButton** useBounds;//buttons indicating the use of bounds for a parameter
	
};

ParamAdjustmentDialog::ParamAdjustmentDialog(const TGWindow* parent, UInt_t width, UInt_t height)
{
	numParams=0;
	paramFrames=NULL;
	paramNames=NULL;
	lowerBounds=NULL;
	paramValues=NULL;
	upperBounds=NULL;
	useBounds=NULL;
	
	//create the main window for the gui
	mainWindow = new TGMainFrame(parent,width,height);
	//create the main organizer
	mainOrganizer = new TGVerticalFrame(mainWindow, 10, 10);
	//create the column label frame
	labelFrame = new TGHorizontalFrame(mainOrganizer, 10, 10);
	//create the parameter frame organizer
	paramOrganizer = new TGVerticalFrame(mainOrganizer, 10, 10);
	
	//create the column labels
	paramColLbl = new TGLabel(labelFrame, "Name");
	lbColLbl = new TGLabel(labelFrame, "Lower Bound");
	valColLbl = new TGLabel(labelFrame, "Param Value");
	ubColLbl = new TGLabel(labelFrame, "Upper Bound");
	bcbColLbl = new TGLabel(labelFrame, "Apply Bounds?");
	labelFrame->AddFrame(paramColLbl, new TGLayoutHints(kLHintsNormal,5,5,3,4) );
	labelFrame->AddFrame(lbColLbl, new TGLayoutHints(kLHintsNormal,5,5,3,4) );
	labelFrame->AddFrame(valColLbl, new TGLayoutHints(kLHintsNormal,5,5,3,4) );
	labelFrame->AddFrame(ubColLbl, new TGLayoutHints(kLHintsNormal,5,5,3,4) );
	labelFrame->AddFrame(bcbColLbl, new TGLayoutHints(kLHintsNormal,5,5,3,4) );
	
	mainOrganizer->AddFrame(labelFrame, new TGLayoutHints(kLHintsNormal,5,5,3,4) );
	mainOrganizer->AddFrame(paramOrganizer, new TGLayoutHints(kLHintsNormal,5,5,3,4) );
	
	mainWindow->AddFrame(mainOrganizer, new TGLayoutHints(kLHintsNormal,5,5,3,4) );
	
	//map the subwindows
	mainWindow->MapSubwindows();
	//init the layout engine
	mainWindow->Resize(mainWindow->GetDefaultSize());
	
	mainWindow->SetMWMHints(kMWMDecorAll | kMWMDecorResizeH  | kMWMDecorMaximize | kMWMDecorMinimize | kMWMDecorMenu,
               kMWMFuncAll |  kMWMFuncResize    | kMWMFuncMaximize | kMWMFuncMinimize,
               kMWMInputModeless);
    
    //map the main frame
	mainWindow->MapWindow();
}

ParamAdjustmentDialog::~ParamAdjustmentDialog()
{
	mainWindow->Cleanup();
	delete mainWindow;
}

void ParamAdjustmentDialog::show()
{
	mainWindow->MapWindow();
}

void ParamAdjustmentDialog::hide()
{
	mainWindow->UnmapWindow();
}

void ParamAdjustmentDialog::clearPrevious()
{
	paramOrganizer->RemoveAll();
	
	if (paramNames != NULL)
	{	delete[] paramNames;	}
	
	if (lowerBounds != NULL)
	{	delete[] lowerBounds;	}

	if (paramValues != NULL)
	{	delete[] paramValues;	}
	
	if (upperBounds != NULL)
	{	delete[] upperBounds;	}
	
	if (useBounds != NULL)
	{	delete[] useBounds;	}
	
	if (paramFrames != NULL)
	{	delete[] paramFrames;}
}

void ParamAdjustmentDialog::retrieveData(double* values, double* lowerBnds, double* upperBnds, bool* useBnds)
{
	for(int i=0; i<numParams; ++i)
	{
		lowerBnds[i] = lowerBounds[i]->GetNumber();
		values[i]    = paramValues[i]->GetNumber();
		upperBnds[i] = upperBounds[i]->GetNumber();
		useBnds[i]   = useBounds[i]->IsOn();
	}
}

void ParamAdjustmentDialog::updateParams(double* values)
{
	for(int i=0; i<numParams; ++i)
	{
		paramValues[i]->SetNumber(values[i]);
	}
}

void ParamAdjustmentDialog::initWithParams(int numPeaks, int nP, double* vals)
{
	if(nP<(3*numPeaks))
	{
		cout<<"Error, fewer parameters than the number of peaks allows"<<endl;
		return;
	}
	
	int numBgParams=(nP-3*numPeaks);
	
	if( numBgParams>4 )
	{
		cout<<"Error, to many background parameters"<<endl;
		return;
	}
	//set the number of parameters
	numParams=nP;
	clearPrevious();
	
	if(nP==0)
	{ return; }
	
	paramFrames = new TGHorizontalFrame*[nP];
	paramNames = new TGLabel*[nP];
	lowerBounds = new TGNumberEntry*[nP];
	paramValues = new TGNumberEntry*[nP];
	upperBounds = new TGNumberEntry*[nP];
	useBounds = new TGCheckButton*[nP];
	
	ostringstream nmr;
	
	//loop through each peak setting up its parameters
	for(int i=0; i<numPeaks; ++i)
	{
		for(int j=0; j<3; ++j)
		{
			int ind = i*3 + j;
			//create the frame
			paramFrames[ind] = new TGHorizontalFrame(paramOrganizer, 10, 10);
			//make the string for the param label
			nmr.clear();
			nmr.str("");
			nmr<<GPNameList[j]<<i;
			//make the parts of the frame
			paramNames[ind] = new TGLabel(paramFrames[ind], nmr.str().c_str());
			lowerBounds[ind] = new TGNumberEntry( paramFrames[ind], 0.5*vals[ind], 8, -1, TGNumberFormat::kNESRealOne,
				TGNumberFormat::kNEAAnyNumber, TGNumberFormat::kNELNoLimits, 0.0, 1.0);
			paramValues[ind] = new TGNumberEntry( paramFrames[ind], vals[ind], 8, -1, TGNumberFormat::kNESRealOne,
				TGNumberFormat::kNEAAnyNumber, TGNumberFormat::kNELNoLimits, 0.0, 1.0);
			upperBounds[ind] = new TGNumberEntry( paramFrames[ind], 2.0*vals[ind], 8, -1, TGNumberFormat::kNESRealTwo,
				TGNumberFormat::kNEAAnyNumber, TGNumberFormat::kNELNoLimits, 0.0, 1.0);
			useBounds[ind] = new TGCheckButton(paramFrames[ind], "Use Bnds?");
			//add the parts to the frame
			paramFrames[ind]->AddFrame(paramNames[ind], new TGLayoutHints(kLHintsNormal,5,5,3,4) );
			paramFrames[ind]->AddFrame(lowerBounds[ind], new TGLayoutHints(kLHintsNormal,5,5,3,4) );
			paramFrames[ind]->AddFrame(paramValues[ind], new TGLayoutHints(kLHintsNormal,5,5,3,4) );
			paramFrames[ind]->AddFrame(upperBounds[ind], new TGLayoutHints(kLHintsNormal,5,5,3,4) );
			paramFrames[ind]->AddFrame(useBounds[ind], new TGLayoutHints(kLHintsNormal,5,5,3,4) );
			//add the frame to the organizer
			paramOrganizer->AddFrame(paramFrames[ind], new TGLayoutHints(kLHintsNormal,5,5,3,4) );
		}
	}
	
	//now loop through the background setting it up its params
	for(int i=0; i<numBgParams; ++i)
	{
		int ind = (3*numPeaks+i);
		//create the frame
		paramFrames[ind] = new TGHorizontalFrame(paramOrganizer, 10, 10);
		//make the parts of the frame
		paramNames[ind] = new TGLabel(paramFrames[ind], PBNameList[i]);
		lowerBounds[ind] = new TGNumberEntry( paramFrames[ind], 0.5*vals[ind], 8, -1, TGNumberFormat::kNESRealTwo,
			TGNumberFormat::kNEAAnyNumber, TGNumberFormat::kNELNoLimits, 0.0, 1.0);
		paramValues[ind] = new TGNumberEntry( paramFrames[ind], vals[ind], 8, -1, TGNumberFormat::kNESRealTwo,
			TGNumberFormat::kNEAAnyNumber, TGNumberFormat::kNELNoLimits, 0.0, 1.0);
		upperBounds[ind] = new TGNumberEntry( paramFrames[ind], 2.0*vals[ind], 8, -1, TGNumberFormat::kNESRealTwo,
			TGNumberFormat::kNEAAnyNumber, TGNumberFormat::kNELNoLimits, 0.0, 1.0);
		useBounds[ind] = new TGCheckButton(paramFrames[ind], "Use Bnds?");
		//add the parts to the frame
		paramFrames[ind]->AddFrame(paramNames[ind], new TGLayoutHints(kLHintsNormal,5,5,3,4) );
		paramFrames[ind]->AddFrame(lowerBounds[ind], new TGLayoutHints(kLHintsNormal,5,5,3,4) );
		paramFrames[ind]->AddFrame(paramValues[ind], new TGLayoutHints(kLHintsNormal,5,5,3,4) );
		paramFrames[ind]->AddFrame(upperBounds[ind], new TGLayoutHints(kLHintsNormal,5,5,3,4) );
		paramFrames[ind]->AddFrame(useBounds[ind], new TGLayoutHints(kLHintsNormal,5,5,3,4) );
		//add the frame to the organizer
		paramOrganizer->AddFrame(paramFrames[ind], new TGLayoutHints(kLHintsNormal,5,5,3,4) );
	}
	mainWindow->SetWindowName("Parameter Adjuster");
	//map the subwindows
	mainWindow->MapSubwindows();
	//init the layout engine
	mainWindow->Resize(mainWindow->GetDefaultSize());
	//map the main frame
	mainWindow->MapWindow();
}

#endif //ADJUST_DIALOGS_H
