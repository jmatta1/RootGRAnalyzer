#ifndef ADJUST_DIALOGS_H
#define ADJUST_DIALOGS_H

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
#include<TGTab.h>
#include<TGClient.h>
#include<TGCanvas.h>
#include<TGComboBox.h>
#include<RQ_OBJECT.h>

#include"guiSupport.h"
#include"MatrixFrame.h"

class AssignmentDialog
{
	RQ_OBJECT("AssignmentDialog");
public:
	AssignmentDialog(const TGWindow* parent, UInt_t width, UInt_t height);
	~AssignmentDialog();
	
	void setStates(StateData* sts, int numSt);
	void addFit(const FitData& fitDat){}
	
	void assignStates(){}
	void fitStates(){}
	void removeFit(){}
	
private:
	
	int numStates;
	StateData* states;
	int numAssignments;
	StateFit* assignments;
	int numFits;
	FitData* fits;

	//main window
	TGMainFrame *mainWindow;
	TGVerticalFrame* overallOrg;
	TGTab *tabFrame;
	TGCanvas* assignCanvas;
	TGHorizontalFrame* buttonContainer;
	//frames for holding the contents of individual tabs
	TGCompositeFrame* assignTabFrame;
	TGCompositeFrame* fitDataTabFrame;
	TGCompositeFrame* predictionTabFrame;
	//frames for holding the contents of the TGCanvas
	TGCompositeFrame* assignContainer;
	//frames for making labels in assign container
	TGHorizontalFrame* assignHoriz;
	TGVerticalFrame** assignVert;
	TGLabel** colLabels;
	TGTextEntry** exEntry;
	TGTextEntry** fpmEntry;
	TGTextEntry** calfpmEntry;
	TGTextEntry** aFitEntry;
	TGTextEntry** difEntry;
	TGComboBox** fitBox;
	TGTextButton** assignButtons;
	TGTextButton** unAssignButtons;
	//buttons and other stuff
	TGTextButton *assignButton;
	TGTextButton *fitButton;
	TGNumberEntry* polOrder;
	TGLabel* polOrdLbl;
	TGTextButton *rmvFitButton;
	TGComboBox* fitsBox;
	
};

AssignmentDialog::AssignmentDialog(const TGWindow* parent, UInt_t width, UInt_t height)
{
	//create the high level stuff
	mainWindow = new TGMainFrame(parent, width, height);
	overallOrg = new TGVerticalFrame(mainWindow, 10, 10);
	tabFrame = new TGTab(overallOrg, 10, 10);
	assignTabFrame = new TGCompositeFrame(tabFrame, 10, 10);
	fitDataTabFrame = new TGCompositeFrame(tabFrame, 10, 10);
	predictionTabFrame = new TGCompositeFrame(tabFrame, 10, 10);
	assignCanvas = new TGCanvas(assignTabFrame, 10, 10);
	buttonContainer = new TGHorizontalFrame(overallOrg, 10, 10);
	
	//create and assign canvas containers
	assignContainer = new TGCompositeFrame(assignCanvas->GetViewPort(),50,50);
	assignContainer->SetCleanup(kDeepCleanup);
	assignCanvas->GetViewPort()->SetCleanup(kDeepCleanup);
	assignCanvas->SetContainer(assignContainer);
	
	//create the column name labels for the assignment tab
	assignHoriz = new TGHorizontalFrame(assignContainer);
	assignVert = new TGVerticalFrame*[8];
	for(int i=0; i<8; ++i)
	{
		assignVert[i] = new TGVerticalFrame(assignHoriz, 10, 10);
		
	}
	
	colLabels = new TGLabel*[7];
	colLabels[0] = new TGLabel(assignVert[0], "Ex");
	assignVert[0]->AddFrame(colLabels[0], new TGLayoutHints( kLHintsCenterX ,1,1,1,1));
	
	colLabels[1] = new TGLabel(assignVert[1], "Calc P_fp");
	assignVert[1]->AddFrame(colLabels[1], new TGLayoutHints( kLHintsCenterX ,1,1,1,1));
	
	colLabels[2] = new TGLabel(assignVert[2], "Cal P_fp");
	assignVert[2]->AddFrame(colLabels[2], new TGLayoutHints( kLHintsCenterX ,1,1,1,1));
	
	colLabels[3] = new TGLabel(assignVert[3], "Assigned Cent.");
	assignVert[3]->AddFrame(colLabels[3], new TGLayoutHints( kLHintsCenterX ,1,1,1,1));
	
	colLabels[4] = new TGLabel(assignVert[4], "Difference");
	assignVert[4]->AddFrame(colLabels[4], new TGLayoutHints( kLHintsCenterX ,1,1,1,1));
	
	colLabels[5] = new TGLabel(assignVert[5], "Select Fit");
	assignVert[5]->AddFrame(colLabels[5], new TGLayoutHints( kLHintsCenterX ,1,1,1,1));
	
	colLabels[6] = new TGLabel(assignVert[6], "Assign Buttons");
	assignVert[6]->AddFrame(colLabels[6], new TGLayoutHints( kLHintsCenterX ,1,1,1,1));
	
	colLabels[7] = new TGLabel(assignVert[7], "Unassign Buttons");
	assignVert[7]->AddFrame(colLabels[7], new TGLayoutHints( kLHintsCenterX ,1,1,1,1));
	
	for(int i=0; i<8; ++i)
	{
		assignHoriz->AddFrame(assignVert[i], new TGLayoutHints( kLHintsTop | kLHintsCenterX, 5, 5, 5, 5));
	}
	
	assignContainer->AddFrame(assignHoriz, new TGLayoutHints(kLHintsNormal, 1, 1, 1, 1));
	
	//add the canvases to the frames
	assignTabFrame->AddFrame(assignCanvas, new TGLayoutHints(kLHintsExpandX | kLHintsExpandY,5,5,3,4));
	
	//create the tabs
	tabFrame->AddTab("State/Fit Assignment", assignTabFrame);
	tabFrame->AddTab("Fit Information", fitDataTabFrame);
	tabFrame->AddTab("State Positions", predictionTabFrame);
	
	//add the tab frame to the overall frame
	overallOrg->AddFrame(tabFrame, new TGLayoutHints(kLHintsExpandX | kLHintsExpandY,5,5,3,4));
	
	//create the buttons and such for the button container
	assignButton = new TGTextButton(buttonContainer,"Assign States");
	assignButton->Connect("Clicked()","AssignmentDialog",this,"assignStates()");
	fitButton = new TGTextButton(buttonContainer,"Do Fitting");
	fitButton->Connect("Clicked()","AssignmentDialog",this,"fitStates()");
	polOrder = new TGNumberEntry( buttonContainer, 1.0, 5, -1, TGNumberFormat::kNESInteger,
			TGNumberFormat::kNEAPositive, TGNumberFormat::kNELLimitMinMax, 1, 2);
	polOrdLbl = new TGLabel(buttonContainer, "Fit Ord");
	rmvFitButton = new TGTextButton(buttonContainer,"Remove Fit");
	rmvFitButton->Connect("Clicked()","AssignmentDialog",this,"removeFit()");
	fitsBox = new TGComboBox(buttonContainer);
	fitsBox->AddEntry("Stored Fits",0);
	fitsBox->Select(0);
	fitsBox->Resize(100,20);
	
	//add the buttons to their frame
	buttonContainer->AddFrame(assignButton, new TGLayoutHints(kLHintsNormal,5,5,3,4));
	buttonContainer->AddFrame(polOrdLbl, new TGLayoutHints(kLHintsNormal,5,5,3,4));
	buttonContainer->AddFrame(polOrder, new TGLayoutHints(kLHintsNormal,5,5,3,4));
	buttonContainer->AddFrame(fitButton, new TGLayoutHints(kLHintsNormal,5,5,3,4));
	buttonContainer->AddFrame(fitsBox, new TGLayoutHints(kLHintsNormal,5,5,3,4));
	buttonContainer->AddFrame(rmvFitButton, new TGLayoutHints(kLHintsNormal,5,5,3,4));
	
	//add the button container to the overall organizer
	overallOrg->AddFrame(buttonContainer, new TGLayoutHints(kLHintsBottom,5,5,3,4));
		
	//set the name for dialog
	mainWindow->SetWindowName("State / Fit Assignment, Calibration, and Prediction");
	//now add the overallOrg frame to this dialog
	mainWindow->AddFrame(overallOrg, new TGLayoutHints(kLHintsExpandX | kLHintsExpandY,5,5,3,4));
	//map the subwindows
	mainWindow->MapSubwindows();
	//init the layout engine
	mainWindow->Resize(mainWindow->GetDefaultSize());
	
	mainWindow->SetMWMHints(kMWMDecorAll | kMWMDecorResizeH  | kMWMDecorMaximize | kMWMDecorMinimize | kMWMDecorMenu,
               kMWMFuncAll |  kMWMFuncResize    | kMWMFuncMaximize | kMWMFuncMinimize,
               kMWMInputModeless);
	//map the main frame
	mainWindow->MapWindow();
	mainWindow->Layout();
}

AssignmentDialog::~AssignmentDialog()
{
	mainWindow->Cleanup();
//	delete assignContainer;
//	delete fitDataContainer;
//	delete predictionContainer;
	
	delete mainWindow;
}

void AssignmentDialog::setStates(StateData* sts, int numSt)
{
	numStates = numSt;
	states = new StateData[numSt];
	for(int i=0; i<numSt; ++i)
	{
		states[i] = sts[i];
	}
	exEntry = new TGTextEntry*[numSt];
	fpmEntry = new TGTextEntry*[numSt];
	calfpmEntry = new TGTextEntry*[numSt];
	aFitEntry = new TGTextEntry*[numSt];
	difEntry = new TGTextEntry*[numSt];
	
	//create the columns
	for(int i=0; i<numSt; ++i)
	{
		ostringstream namer;
		namer.str("");
		namer.clear();
		exEntry[i] = new TGTextEntry(assignVert[0]);
		exEntry[i]->SetEnabled(false);
		namer<<sts[i].en<<" MeV";
		exEntry[i]->SetText(namer.str().c_str(),false);
		assignVert[0]->AddFrame(exEntry[i], new TGLayoutHints(kLHintsNormal, 1, 1, 1, 1));
		
		namer.str("");
		namer.clear();
		fpmEntry[i] = new TGTextEntry(assignVert[1]);
		fpmEntry[i]->SetEnabled(false);
		namer<<sts[i].fpMom<<" MeV/c";
		fpmEntry[i]->SetText(namer.str().c_str(),false);
		assignVert[1]->AddFrame(fpmEntry[i], new TGLayoutHints(kLHintsNormal, 1, 1, 1, 1));
		
		calfpmEntry[i] = new TGTextEntry(assignVert[2]);
		calfpmEntry[i]->SetEnabled(false);
		calfpmEntry[i]->SetText(namer.str().c_str(),false);
		assignVert[2]->AddFrame(calfpmEntry[i], new TGLayoutHints(kLHintsNormal, 1, 1, 1, 1));
		
		aFitEntry[i] = new TGTextEntry(assignVert[3]);
		aFitEntry[i]->SetEnabled(false);
		assignVert[3]->AddFrame(aFitEntry[i], new TGLayoutHints(kLHintsNormal, 1, 1, 1, 1));
		
		difEntry[i] = new TGTextEntry(assignVert[4]);
		difEntry[i]->SetEnabled(false);
		assignVert[4]->AddFrame(difEntry[i], new TGLayoutHints(kLHintsNormal, 1, 1, 1, 1));
	}
	mainWindow->MapSubwindows();
	mainWindow->MapWindow();
	mainWindow->Layout();
}

#endif //ADJUST_DIALOGS_H
