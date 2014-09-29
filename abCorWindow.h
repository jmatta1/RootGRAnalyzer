#ifndef CAL_WINDOW_H
#define CAL_WINDOW_H
//stl includes
#include<iostream>
#include<sstream>
#include<fstream>
#include<iostream>
using std::string;
using std::ostringstream;
using std::istringstream;
using std::ofstream;
using std::ifstream;
using std::cout;
using std::cin;
using std::endl;
using std::flush;
using std::ios_base;

//root includes
#include<TCanvas.h>
#include<TRootEmbeddedCanvas.h>
#include<TGFrame.h>
#include<TGButton.h>
#include<RQ_OBJECT.h>
#include<TGClient.h>
#include<TString.h>
#include<TGFileDialog.h>
#include<TFile.h>
#include<TTree.h>
#include<TObject.h>
#include<TCutG.h>
#include<TCut.h>
#include<TLine.h>
#include<TGLabel.h>
#include<TH2F.h>
#include<TH1F.h>
#include<TF1.h>
#include<TGraph.h>
#include<TStyle.h>
#include<TColor.h>
#include<TPaveText.h>
#include<TSystem.h>
#include<TFitResult.h>
#include<TGTextView.h>
#include<TGSplitter.h>
#include<TGNumberEntry.h>
#include<TMath.h>
#include<TGTab.h>
#include<TGComboBox.h>
#include<TApplication.h>
#include<TFitEditor.h>
#include<TROOT.h>
#include<TPluginManager.h>
#include<TList.h>

//my includes
#include"bicubicinterp.h"
#include"guiSupport.h"

//TODO, fix up deconstructor
//TODO, fix up reset function

enum UpdateCallType{ Initial, Normal, Final};
enum WidgetNumberings{ PolOrderEntry = 0};

enum {NumAngleCuts=9};

TCut RayCut;

class AberrationCorrectionWindow
{
	RQ_OBJECT("AberrationCorrectionWindow")
public:
	AberrationCorrectionWindow(const TGWindow* parent, UInt_t width, UInt_t height);
	~AberrationCorrectionWindow();
	
	//fit control
	void setFitFunc();
	void getFitData();
	
	//Fit control
	void transferFit();
	void removeFit();
	void assignFitToState(int stNum);
	void unAssignFitToState(int stNum);
	void rebinDispSpec();
	void getPeakFindResults();
	void performPeakFind();
	
	//sequential spectrum display
	void prevSeqSpec();
	void nextSeqSpec();
	void prevCutSpec();
	void nextCutSpec();
	void toggleSpec();
	
	//file operations
	void readRunData();
	void openBigFile();
	void openAuxFile();
	void openFriendFile();
	void readStateData();
	
	//overall control operations
	void clearLog();
	void exitApp();

	void DoClose();

private:
		//some private helper functions
	void parseRunFileLine(const string& line, RunData& tempData);
	void pushToLog();
	bool checkUpToRunData();
	bool checkUpToMainFile();
	bool checkUpToAuxFile();
	bool checkUpToFrFile();
	bool checkUpToStates();
	bool checkForFitStart();
	void updateStateInfo();
	void generateStateInfo();
	float invertCalFunc(int i);
	void updateComboBoxes();
	
	//functions for constructing names of various constructs
	string makeTreeName(int runN);
	string makePIDCutName(int runN);
	string makePIDSpecName(int runN);
	string makeBaseCutName(int runN);
	string makeBGGraphName(int runN);
	string makeBGCutName(int runN);
	string makeTRCutName(int runN);
	string makeBGSpecName(int runN);
	string makeOrigShapeSpecName(int runN);
	string makeFirstOrderShapeSpecName(int runN);
	string makeFirstOrderShapeTreeName(int runN);
	string makeTRpBGSpecName(int runN);
	string makeBGOnlySpecName(int runN);
	string makeScaledBGSpecName(int runN);
	string makeSubSpecName(int runN);
	string makeExSubSpecName(int runN);

	//functions for retrieving various things from files
	TTree* retrieveTree(int runN);
	TCutG* retrievePIDCut(int runN);
	TH2F* retrievePIDSpec(int runN);
	TCut* retrieveBaseCut(int runN);
	TGraph* retrieveBGGraph(int runN);
	TCut* retrieveBGCut(int runN);
	TCut* retrieveTRCut(int runN);
	TH1F* retrieveBGSpec(int runN);
	TH2F* retrieveOrigShapeSpec(int runN);
	TH2F* retrieveFirstOrderShapeSpec(int runN);
	TTree* retrieveFirstOrderShapeTree(int runN);
	TH2F* retrieveTRpBGSpec(int runN);
	TH2F* retrieveBGOnlySpec(int runN);
	TH2F* retrieveScaledBGSpec(int runN);
	TH2F* retrieveSubSpec(int runN);
	TH2F* retrieveExSubSpec(int runN);

	//functions for retrieving various things from files
	//and then throwing out error messages if the pointer comes back null
	TTree* testTree(int runN);
	TCutG* testPIDCut(int runN);
	TH2F* testPIDSpec(int runN);
	TCut* testBaseCut(int runN);
	TGraph* testBGGraph(int runN);
	TCut* testBGCut(int runN);
	TCut* testTRCut(int runN);
	TH1F* testBGSpec(int runN);
	TH2F* testOrigShapeSpec(int runN);
	TH2F* testFirstOrderShapeSpec(int runN);
	TTree* testFirstOrderShapeTree(int runN);
	TH2F* testTRpBGSpec(int runN);
	TH2F* testBGOnlySpec(int runN);
	TH2F* testScaledBGSpec(int runN);
	TH2F* testSubSpec(int runN);
	TH2F* testExSubSpec(int runN);
	
	//display functions for sequential displays
	void displaySubSpec(const UpdateCallType& tp);
	void sepFitAndDraw();
	
	//analysis stuff
	RunData* runs;
	FitData* tempFits;
	StateData** states;
	bool statesSet;
	CorrectionPoint** corrPts;
	int* numPoints;
	int numRuns;
	int numStates;
	TFile* mainFile;
	TFile* auxFile;
	TFile* frFile;
	int currBGOrder;
	int numPeaks;

	//display stuff
	int dispNum;
	bool sclToggle;
	TH2F* subSpec;
	TH1D* cutSpec;
	double minAngle;
	double maxAngle;
	int angleInd;

	/************************************************
	**GUI Declarations
	************************************************/

	//Main Gui stuff
	TSystem *sys;
	TGMainFrame *mainWindow;
	TFitEditor* fitDiag;
	//The string stream for building messages for the logger
	ostringstream logStrm;
	//main canvas for display
	TRootEmbeddedCanvas *canvas;//holds the canvas we use for drawing
	TCanvas *whiteBoard;//holds the pointer to the actual TCanvas in canvas
	//Frames for organization
	TGVerticalFrame* organizerFrame;//the outermost organizer of the window
	TGHorizontalFrame* canvasFrame;//holds the canvas and bottom bar
	TGVerticalFrame* bottomFrame;//holds the canvas and bottom bar
	TGVerticalFrame* sideBarFrame;//holds the sidebar
	TGHorizontalFrame* fileOpsRowFrame;//holds the row of buttons at the bottom for file operations
	TGHorizontalFrame* bottomBarFrame;//holds the subframes for file control and overall control
	TGHorizontalFrame* specDispFrame;//holds the buttons for controlling spectra
	TGHorizontalFrame* overallControlFrame;//holds the overall control buttons
	TGVerticalFrame* messageLogFrame;//frame that only holds the message log, necessary for the resizing trick
	TGHorizontalFrame* orderFrame;//holds the polynomial order getter stuff
	TGHorizontalFrame* numPksFrame;//holds the number of peaks getter stuff
	TGHorizontalFrame* iterationsFrame;//holds the iterations order getter stuff
	
	//tab system for going back and forth between canvas and state info
	TGTab *tabFrame;
	//frames for holding the components of tabs
	TGCompositeFrame* whiteBoardCon;
	TGCompositeFrame* stateDataCon;
	//stuff for the state info tab
	TGCanvas* stateCan;//canvas for the state information
	TGCompositeFrame* stateCanCon;//container for the state information canvas
	TGLayoutHints* colLayout;//layout to apply to the columns
	TGLayoutHints* cellLayout;//layout to apply to the cells in the columns
	TGHorizontalFrame* stateHoriz;//organizer that holds the columns
	TGVerticalFrame* stateVert[5];//organizers that hold the cells
	TGLabel* colLabels[5];//column labels
	TGTextEntry** exEntry;//excitation energy holders
	TGComboBox** aFitBox;//assigned fit centroids
	TGTextButton** unAssignButtons;//buttons to unassign a fit from a state
	TGComboBox** fitBox;//combo boxes for holding unassigned fits
	TGTextButton** assignButtons;//buttons to assign a selected fit to a state
	
	//the message container for the window to try and make things so we do not need the terminal at all.
	TGTextView* messageLog;
	
	//the splitters allowing resizing of the canvas and message box
	TGHSplitter* split;
	
	//labels to store text
	TGLabel* fileLabel;
	TGLabel* fileControlLabel;
	TGLabel* controlLabel;
	TGLabel* sDLabel;
	TGLabel* fitCtrlLabel;
	TGLabel* orderLabel;
	TGLabel* numPksLabel;
	TGLabel* iterationsLabel;
	TGLabel* calCtrlLabel;
	
	//action buttons
	TGNumberEntry* bgPolOrderGetter;//used for getting the order of the polynomial background
	TGNumberEntry* numPeaksGetter;//used for getting the number of peaks to fit
	TGNumberEntry* iterationsGetter;//used for getting the order of the cal function
	TGTextButton* setFunc;
	TGTextButton* getFit;
	TGTextButton* doRebin;
	TGComboBox* tempFitBox;
	TGTextButton* useFit;
	TGComboBox* fitListBox;
	TGTextButton* rmFit;
	TGTextButton* doPeakFind;
	TGTextButton* getFoundPeaks;
	//sequential display buttons
	TGTextButton *prevSpec;
	TGTextButton *nextSpec;
	TGTextButton *prevCut;
	TGTextButton *nextCut;
	TGTextButton *toggleScale;
	
	//file ops buttons
	TGTextButton *rdRunData;
	TGTextButton *opBigFile;
	TGTextButton *opAuxFile;
	TGTextButton *opFrFile;
	TGTextButton *rdStateData;
	
	//overall control buttons
	TGTextButton *clLogBut;
	TGTextButton *exBut;
};

AberrationCorrectionWindow::AberrationCorrectionWindow(const TGWindow* parent, UInt_t width, UInt_t height)
{
	runs=NULL;
	states=NULL;
	numRuns=0;
	numStates=0;
	mainFile=NULL;
	auxFile=NULL;
	frFile=NULL;
	dispNum = 0;
	sclToggle=false;
	subSpec=NULL;
	cutSpec=NULL;
	fitDiag=NULL;
	currBGOrder=0;
	numPeaks=0;
	numPoints=NULL;
	corrPts=NULL;
	statesSet=false;
	maxAngle=0.05;
	minAngle=-0.05;
	angleInd=4;

	tempFits = new FitData[5];

	//sys = new TUnixSystem();
	sys = gSystem;
	
	RayCut.SetTitle("Rayid==0");
	
	//paramDialog = new ParamAdjustmentDialog(parent,width,height);
	
	//create the main window for the gui
	mainWindow = new TGMainFrame(parent,width,height);
	
	//set up some stuff to prevent the X11 BadDrawable error
	mainWindow->SetCleanup(kDeepCleanup);
	mainWindow->Connect("CloseWindow()","AberrationCorrectionWindow",this,"DoClose()");
	mainWindow->DontCallClose();
	
	//create the frames for organizing everything
	organizerFrame = new TGVerticalFrame(mainWindow,width,height);
	canvasFrame = new TGHorizontalFrame(organizerFrame,width,height);
	sideBarFrame = new TGVerticalFrame(canvasFrame,width,height);
	bottomFrame = new TGVerticalFrame(organizerFrame,width,height);
	fileOpsRowFrame = new TGHorizontalFrame(bottomFrame,width,height);
	bottomBarFrame = new TGHorizontalFrame(bottomFrame,width,height);
	specDispFrame = new TGHorizontalFrame(bottomBarFrame,width,height);
	overallControlFrame = new TGHorizontalFrame(bottomBarFrame,width,height);
	messageLogFrame  = new TGVerticalFrame(organizerFrame,width,height, kChildFrame | kFixedHeight);
	orderFrame = new TGHorizontalFrame(sideBarFrame, width, height);
	numPksFrame = new TGHorizontalFrame(sideBarFrame, width, height);
	iterationsFrame = new TGHorizontalFrame(sideBarFrame, width, height);
	
	//create and connect the buttons in the side bar
	/******************************************
	** Fit Control Stuff
	******************************************/
	fitCtrlLabel = new TGLabel(sideBarFrame, "Fit Control");
	sideBarFrame->AddFrame(fitCtrlLabel, new TGLayoutHints(kLHintsExpandX,2,2,2,2));
	//numer entry for getting the bg order
	bgPolOrderGetter = new TGNumberEntry( orderFrame, 1.0, 5, PolOrderEntry, TGNumberFormat::kNESInteger, 
			TGNumberFormat::kNEAAnyNumber, TGNumberFormat::kNELLimitMinMax, -1, 2);
	orderLabel = new TGLabel(orderFrame, "BG Pol Ord");
	orderFrame->AddFrame(bgPolOrderGetter, new TGLayoutHints(kLHintsLeft | kLHintsTop, 5, 5, 3, 4) );
	orderFrame->AddFrame(orderLabel, new TGLayoutHints(kLHintsNormal, 5, 5, 3, 4) );
	//number entry for getting the number of peaks
	numPeaksGetter = new TGNumberEntry( numPksFrame, 1.0, 5, PolOrderEntry, TGNumberFormat::kNESInteger, 
			TGNumberFormat::kNEAAnyNumber, TGNumberFormat::kNELLimitMinMax, 1, 5);
	numPksLabel = new TGLabel(numPksFrame, "NumPeaks");
	numPksFrame->AddFrame(numPeaksGetter, new TGLayoutHints(kLHintsLeft | kLHintsTop, 5, 5, 3, 4) );
	numPksFrame->AddFrame(numPksLabel, new TGLayoutHints(kLHintsNormal, 5, 5, 3, 4) );
	//button that sets the fit panel function from the two previous numbers
	setFunc = new TGTextButton(sideBarFrame,"Set Fit Func");
	setFunc->Connect("Clicked()","AberrationCorrectionWindow",this,"setFitFunc()");
	//button that gets the fit data from the fit panel
	getFit = new TGTextButton(sideBarFrame,"Get Fit Data");
	getFit->Connect("Clicked()","AberrationCorrectionWindow",this,"getFitData()");
	//buttons to do and retrieve peakfind info
	doPeakFind = new TGTextButton(sideBarFrame,"Find Peaks");
	doPeakFind->Connect("Clicked()","AberrationCorrectionWindow",this,"performPeakFind()");
	getFoundPeaks = new TGTextButton(sideBarFrame,"Get Found Peaks");
	getFoundPeaks->Connect("Clicked()","AberrationCorrectionWindow",this,"getPeakFindResults()");
	//add the sidebar buttons to the sidebar frame
	sideBarFrame->AddFrame(orderFrame, new TGLayoutHints(kLHintsExpandX,2,2,2,2));
	sideBarFrame->AddFrame(numPksFrame, new TGLayoutHints(kLHintsExpandX,2,2,2,2));
	sideBarFrame->AddFrame(setFunc, new TGLayoutHints(kLHintsExpandX,2,2,2,2));
	sideBarFrame->AddFrame(getFit, new TGLayoutHints(kLHintsExpandX,2,2,2,2));
	sideBarFrame->AddFrame(doPeakFind, new TGLayoutHints(kLHintsExpandX,2,2,2,2));
	sideBarFrame->AddFrame(getFoundPeaks, new TGLayoutHints(kLHintsExpandX,2,2,2,2));
	
	/******************************************
	** Fit Control
	******************************************/
	//add the combo box that holds temporary fit information
	tempFitBox = new TGComboBox(sideBarFrame);
	tempFitBox->AddEntry("Retrieved Fits",0);
	tempFitBox->Select(0);
	tempFitBox->Resize(100,20);
	useFit = new TGTextButton(sideBarFrame,"Use Fit");
	useFit->Connect("Clicked()","AberrationCorrectionWindow",this,"transferFit()");
	fitListBox = new TGComboBox(sideBarFrame);
	fitListBox->AddEntry("Fits",0);
	fitListBox->Select(0);
	fitListBox->Resize(100,20);
	rmFit = new TGTextButton(sideBarFrame,"Remove Fit");
	rmFit->Connect("Clicked()","AberrationCorrectionWindow",this,"removeFit()");
	iterationsGetter = new TGNumberEntry( iterationsFrame, 1.0, 5, PolOrderEntry, TGNumberFormat::kNESInteger, 
			TGNumberFormat::kNEAAnyNumber, TGNumberFormat::kNELLimitMinMax, 1, 40);
	iterationsLabel = new TGLabel(iterationsFrame, "Rebin Ratio");
	iterationsFrame->AddFrame(iterationsGetter, new TGLayoutHints(kLHintsLeft | kLHintsTop, 5, 5, 3, 4) );
	iterationsFrame->AddFrame(iterationsLabel, new TGLayoutHints(kLHintsNormal, 5, 5, 3, 4) );
	doRebin = new TGTextButton(sideBarFrame,"Rebin");
	doRebin->Connect("Clicked()","AberrationCorrectionWindow",this,"rebinDispSpec()");
	
	sideBarFrame->AddFrame(tempFitBox, new TGLayoutHints(kLHintsExpandX,2,2,2,2));
	sideBarFrame->AddFrame(useFit, new TGLayoutHints(kLHintsExpandX,2,2,2,2));
	sideBarFrame->AddFrame(fitListBox, new TGLayoutHints(kLHintsExpandX,2,2,2,2));
	sideBarFrame->AddFrame(rmFit, new TGLayoutHints(kLHintsExpandX,2,2,2,2));
	sideBarFrame->AddFrame(iterationsFrame, new TGLayoutHints(kLHintsExpandX,2,2,2,2));
	sideBarFrame->AddFrame(doRebin, new TGLayoutHints(kLHintsExpandX,2,2,2,2));
	
	//add the sidebar to the canvas frame
	canvasFrame->AddFrame(sideBarFrame,  new TGLayoutHints(kLHintsTop | kLHintsLeft | kLHintsExpandY, 2,2,2,2) );
	
	/******************************************
	** Tabbed Object Creation
	******************************************/
	tabFrame =  new TGTab(canvasFrame, 10, 10);
	whiteBoardCon = new TGCompositeFrame(tabFrame, 10, 10);
	stateDataCon = new TGCompositeFrame(tabFrame, 10, 10);
	tabFrame->AddTab("Histogram", whiteBoardCon);
	tabFrame->AddTab("State Assignment", stateDataCon);
	/******************************************
	** Canvas Creation
	******************************************/
	//create the embedded canvas
	canvas = new TRootEmbeddedCanvas("canvas",whiteBoardCon, 200, 200);
	whiteBoard = canvas->GetCanvas();
	whiteBoard->SetTitle("Whiteboard For Analysis");
	gStyle->SetOptStat(0);
	//add the canvas to its frame
	whiteBoardCon->AddFrame(canvas, new TGLayoutHints(kLHintsExpandX | kLHintsExpandY , 2,2,2,2) );
	/******************************************
	** State info creation / initial setup
	******************************************/
	stateCan = new TGCanvas(stateDataCon, 10, 10);
	stateCanCon = new TGCompositeFrame(stateCan->GetViewPort(),50,50);
	stateCanCon->SetCleanup(kDeepCleanup);
	stateCan->GetViewPort()->SetCleanup(kDeepCleanup);
	stateCan->SetContainer(stateCanCon);
	
	colLayout = new TGLayoutHints(kLHintsTop | kLHintsExpandX, 1, 1, 1, 0);
	cellLayout = new TGLayoutHints(kLHintsTop | kLHintsExpandX, 1, 1, 1, 1);
	
	stateHoriz = new TGHorizontalFrame(stateCanCon);
	for(int i=0; i<5; ++i)
	{
		stateVert[i] = new TGVerticalFrame(stateHoriz, 10, 10);
		colLabels[i] = new TGLabel(stateVert[i], corColNames[i]);
		stateVert[i]->AddFrame(colLabels[i], new TGLayoutHints(kLHintsTop | kLHintsCenterX, 1, 1, 1, 0));
		stateHoriz->AddFrame(stateVert[i], colLayout);
	}
	
	stateCanCon->AddFrame(stateHoriz, cellLayout);
	
	stateDataCon->AddFrame(stateCan, new TGLayoutHints(kLHintsExpandX | kLHintsExpandY,2,2,2,2));
	canvasFrame->AddFrame(tabFrame, new TGLayoutHints(kLHintsExpandX | kLHintsExpandY , 2,2,2,2) );
	
	/******************************************
	** File operations
	******************************************/
	//create the file ops label and add it
	fileLabel = new TGLabel(fileOpsRowFrame, "File Ops: ");
	fileOpsRowFrame->AddFrame(fileLabel, new TGLayoutHints(kLHintsCenterY, 2,2,2,2));
	//create and connect the file operations buttons
	//read run data button
	rdRunData = new TGTextButton(fileOpsRowFrame,"Read Run Data");
	rdRunData->Connect("Clicked()","AberrationCorrectionWindow",this,"readRunData()");
	//open merge root file
	opBigFile = new TGTextButton(fileOpsRowFrame,"Open Merge File");
	opBigFile->Connect("Clicked()","AberrationCorrectionWindow",this,"openBigFile()");
	//Open Aux File button
	opAuxFile = new TGTextButton(fileOpsRowFrame,"Open Aux File");
	opAuxFile->Connect("Clicked()","AberrationCorrectionWindow",this,"openAuxFile()");
	//Open friend file button
	opFrFile = new TGTextButton(fileOpsRowFrame,"Open Friend File");
	opFrFile->Connect("Clicked()","AberrationCorrectionWindow",this,"openFriendFile()");
	//Read State Data Button
	rdStateData = new TGTextButton(fileOpsRowFrame,"Read State Data");
	rdStateData->Connect("Clicked()","AberrationCorrectionWindow",this,"readStateData()");
	
	
	//add file operations buttons to their frame
	fileOpsRowFrame->AddFrame(rdRunData, new TGLayoutHints(kLHintsExpandX ,3,3,2,2));
	fileOpsRowFrame->AddFrame(opBigFile, new TGLayoutHints(kLHintsExpandX,3,3,2,2));
	fileOpsRowFrame->AddFrame(opAuxFile, new TGLayoutHints(kLHintsExpandX,3,3,2,2));
	fileOpsRowFrame->AddFrame(opFrFile, new TGLayoutHints(kLHintsExpandX,3,3,2,2));
	fileOpsRowFrame->AddFrame(rdStateData, new TGLayoutHints(kLHintsExpandX,3,3,2,2));
	
	//add the file operations frame to its frame
	bottomFrame->AddFrame(fileOpsRowFrame, new TGLayoutHints(kLHintsExpandX | kLHintsTop, 2,2,2,2) );
	
	
	/******************************************
	** Sequential Display buttons
	******************************************/
	sDLabel = new TGLabel(specDispFrame, "Spectrum Control");
	specDispFrame->AddFrame(sDLabel, new TGLayoutHints(kLHintsExpandX,2,2,2,2));
	//create and connect the sequential display buttons
	//show previous spectrum
	prevSpec = new TGTextButton(specDispFrame,"Prev Run");
	prevSpec->Connect("Clicked()","AberrationCorrectionWindow",this,"prevSeqSpec()");
	//show next spectrum
	nextSpec = new TGTextButton(specDispFrame,"Next Run");
	nextSpec->Connect("Clicked()","AberrationCorrectionWindow",this,"nextSeqSpec()");
	//show previous cut
	prevCut = new TGTextButton(specDispFrame,"Prev Angle");
	prevCut->Connect("Clicked()","AberrationCorrectionWindow",this,"prevCutSpec()");
	//show Next cut
	nextCut = new TGTextButton(specDispFrame,"Next Angle");
	nextCut->Connect("Clicked()","AberrationCorrectionWindow",this,"nextCutSpec()");
	//toggle spectrum axis
	toggleScale = new TGTextButton(specDispFrame,"Toggle Scale");
	toggleScale->Connect("Clicked()","AberrationCorrectionWindow",this,"toggleSpec()");
	
	//add the sequential display buttons to their frame
	specDispFrame->AddFrame(prevSpec, new TGLayoutHints(kLHintsExpandX,2,2,2,2));
	specDispFrame->AddFrame(nextSpec, new TGLayoutHints(kLHintsExpandX,2,2,2,2));
	specDispFrame->AddFrame(prevCut, new TGLayoutHints(kLHintsExpandX,2,2,2,2));
	specDispFrame->AddFrame(nextCut, new TGLayoutHints(kLHintsExpandX,2,2,2,2));
	specDispFrame->AddFrame(toggleScale, new TGLayoutHints(kLHintsExpandX,2,2,2,2));
	
	//add the file activation frame to its frame
	bottomBarFrame->AddFrame(specDispFrame, new TGLayoutHints(kLHintsExpandX | kLHintsLeft, 2,2,2,2) );
	
	/******************************************
	** Overall Control buttons
	******************************************/
	controlLabel = new TGLabel(overallControlFrame, "Overall Control: ");
	overallControlFrame->AddFrame(controlLabel, new TGLayoutHints(kLHintsLeft | kLHintsCenterY,2,2,2,2));
	//create and connect the overall control buttons
	//clear log button
	clLogBut = new TGTextButton(overallControlFrame,"Clear Log");
	clLogBut->Connect("Clicked()","AberrationCorrectionWindow",this,"clearLog()");
	//Kill application button
	exBut = new TGTextButton(overallControlFrame,"Exit");
	exBut->Connect("Clicked()","AberrationCorrectionWindow",this,"exitApp()");
	
	
	//add the overall control buttons to their frame
	overallControlFrame->AddFrame(clLogBut, new TGLayoutHints(kLHintsExpandX,3,3,2,2));
	overallControlFrame->AddFrame(exBut, new TGLayoutHints(kLHintsExpandX,3,3,2,2));
	//add the overall control frame to its frame
	bottomBarFrame->AddFrame(overallControlFrame, new TGLayoutHints(kLHintsExpandX, 2,2,2,2) );
	
	//add the bottomBarFrame to its frame
	bottomFrame->AddFrame(bottomBarFrame, new TGLayoutHints(kLHintsExpandX | kLHintsBottom, 2,2,2,2) );
	
	//add the canvasFrame to the overall frame
	organizerFrame->AddFrame(canvasFrame,  new TGLayoutHints(kLHintsRight | kLHintsExpandX | kLHintsExpandY, 2,2,2,2) );
	
	//add the bottom frame to its frame 
	organizerFrame->AddFrame(bottomFrame, new TGLayoutHints(kLHintsExpandX, 2,2,2,2) );
	
	/******************************************
	** Message Log
	******************************************/
	//create the message log
	messageLog = new TGTextView(messageLogFrame,1,100,corInitMessage);
	//the splitter between the menu bar and the message log
	split = new TGHSplitter(organizerFrame);
	split->SetFrame(messageLogFrame,false);
	organizerFrame->AddFrame(split,  new TGLayoutHints( kLHintsExpandX  , 2,2,2,2) );
	//add the message log to the organizer frame
	messageLogFrame->AddFrame(messageLog,  new TGLayoutHints( kLHintsExpandX | kLHintsExpandY, 2,2,2,2) );
	messageLogFrame->SetHeight(104);
	organizerFrame->AddFrame(messageLogFrame,  new TGLayoutHints( kLHintsExpandX | kLHintsCenterX, 2,2,2,2) );
	
	//add the overall frame to the mainwindow
	mainWindow->AddFrame(organizerFrame,  new TGLayoutHints(kLHintsExpandX | kLHintsExpandY | kLHintsBottom, 2,2,2,2) );
	
	//set the name of the main frame
	mainWindow->SetWindowName("Aberration Correction GUI");
	//go through the steps to draw this beast
	//map the subwindows
	mainWindow->MapSubwindows();
	//init the layout engine
	mainWindow->Resize(mainWindow->GetDefaultSize());
	mainWindow->Resize(850,650);
	//map the main frame
	mainWindow->MapWindow();
}

AberrationCorrectionWindow::~AberrationCorrectionWindow()
{
	if(runs != NULL)
	{
		delete[] runs;
		runs = NULL;
	}
	if(auxFile != NULL)
	{
		delete auxFile;
		auxFile = NULL;
	}
	if(mainFile != NULL)
	{
		delete mainFile;
		mainFile = NULL;
	}
	if(frFile != NULL)
	{
		delete frFile;
		frFile = NULL;
	}
	if(states != NULL)
	{
		for(int i=0; i<numRuns; ++i)
		{
			if(states[i]!=NULL)
			{
				delete[] states[i];
			}
		}
		delete[] states;
		states = NULL;
	}
	
	if(corrPts != NULL)
	{
		for(int i=0; i<numRuns; ++i)
		{
			if(corrPts[i]!=NULL)
			{
				delete[] corrPts[i];
			}
		}
		delete[] corrPts;
		corrPts = NULL;
	}
	if(numPoints != NULL)
	{
		delete numPoints;
		numPoints = NULL;
	}
	//delete fitDialog;
	
	//mainWindow->Cleanup();
	mainWindow->CloseWindow();
	//delete mainWindow;
}

/******************************************
*******************************************
** File operations
*******************************************
******************************************/
void AberrationCorrectionWindow::readRunData()
{
	if (runs != NULL)
	{
		logStrm<<"\nRun data already loaded, to load new run data, use the reset button";
		pushToLog();
		return;
	}
	//get the file name using a file dialog
	static TString directory(".");
	TGFileInfo fileInfo;
	fileInfo.SetMultipleSelection(false);
	fileInfo.fFileTypes = csvDataType;
	fileInfo.fIniDir = StrDup(directory);
	
	//make the open file dialog
	//quite frankly this creeps me the hell out, just creating an object like this
	//but apparently the parent object will delete it on its own
	new TGFileDialog(gClient->GetRoot(), mainWindow, kFDOpen, &fileInfo);
	//cout<<fileInfo.fFilename<<", "<<fileInfo.fIniDir;
	directory = fileInfo.fIniDir;
	
	if(TString(fileInfo.fFilename).IsNull())
	{
		return;
	}
	
	ifstream input;
	input.open(fileInfo.fFilename);
	
	//count the number of lines
	string line;
	while (getline(input, line))
	{
		if(line.length()>4)//to handle blank lines at the end of a file
		{
			++numRuns;
		}
		else
		{
			break;
		}
	}
	//jump back to the beginning
	input.clear();
	input.seekg(0,ios_base::beg);
	//allocate the runs array
	runs = new RunData[numRuns];
	states = new StateData*[numRuns];
	corrPts = new CorrectionPoint*[numRuns];
	numPoints = new int[NumAngleCuts*numRuns];
	for(int i =0; i<numRuns; ++i)
	{
		states[i] = NULL;
		for(int j=0; j<NumAngleCuts; ++j)
		{
			numPoints[i*NumAngleCuts+j]=0;
		}
	}
	//now read line by line to get the run data
	getline(input, line);
	int count = 0;
	while(!input.eof() && count<numRuns)
	{
		parseRunFileLine(line, (runs[count]) );
		getline(input, line);
		++count;
	}
	logStrm<<"\nRun data has been loaded";
	pushToLog();
}

void AberrationCorrectionWindow::openBigFile()
{
	if(!checkUpToRunData())
	{	return; }
	if (mainFile != NULL)
	{
		logStrm<<"\nMerge file already loaded, to load a different one, use the reset button";
		pushToLog();
		return;
	}
	
	//get the file name using a file dialog
	static TString directory(".");
	TGFileInfo fileInfo;
	fileInfo.SetMultipleSelection(false);
	fileInfo.fFileTypes = combRootFileType;
	fileInfo.fIniDir = StrDup(directory);
	
	//make the open file dialog
	//quite frankly this creeps me the hell out, just creating an object like this
	//but apparently the parent object will delete it on its own
	new TGFileDialog(gClient->GetRoot(), mainWindow, kFDOpen, &fileInfo);
	if(TString(fileInfo.fFilename).IsNull())
	{
		return;
	}
	//make sure the file name ends with the extension
	string temp = string(fileInfo.fFilename);
	if( (temp.size()-11) != ( temp.rfind("_merge.root") ) )
	{
		temp.append("_merge.root");
	}
	directory = fileInfo.fIniDir;
	
	//open the big file
	mainFile = new TFile(temp.c_str(),"READ");
	logStrm<<"\nMerge file opened in read only mode";
	pushToLog();
}

void AberrationCorrectionWindow::openAuxFile()
{
	if(!checkUpToMainFile())
	{	return; }
	if(auxFile != NULL)
	{
		logStrm<<"\nAux file already loaded, to load a different one, use the reset button";
		pushToLog();
		return;
	}
	//get the file name using a file dialog
	static TString directory(".");
	TGFileInfo fileInfo;
	fileInfo.SetMultipleSelection(false);
	fileInfo.fFileTypes = auxRootFileType;
	fileInfo.fIniDir = StrDup(directory);
	
	//make the open file dialog
	//quite frankly this creeps me the hell out, just creating an object like this
	//but apparently the parent object will delete it on its own
	new TGFileDialog(gClient->GetRoot(), mainWindow, kFDOpen, &fileInfo);
	if(TString(fileInfo.fFilename).IsNull())
	{
		return;
	}
	//make sure the file name ends with the extension
	string temp = string(fileInfo.fFilename);
	if( (temp.size()-9) != ( temp.rfind("_aux.root") ) )
	{
		temp.append("_aux.root");
	}
	directory = fileInfo.fIniDir;
	
	//open the aux file
	auxFile = new TFile(temp.c_str(),"UPDATE");
	logStrm<<"\nAux File Opened";
	pushToLog();
}

void AberrationCorrectionWindow::openFriendFile()
{
	if(!checkUpToAuxFile())
	{	return; }
	if( frFile != NULL )
	{
		logStrm<<"\nFriend file already loaded, to load a different one, use the reset button";
		pushToLog();
		return;
	}
	//get the file name using a file dialog
	static TString directory(".");
	TGFileInfo fileInfo;
	fileInfo.SetMultipleSelection(false);
	fileInfo.fFileTypes = frRootFileType;
	fileInfo.fIniDir = StrDup(directory);
	
	//make the open file dialog
	//quite frankly this creeps me the hell out, just creating an object like this
	//but apparently the parent object will delete it on its own
	new TGFileDialog(gClient->GetRoot(), mainWindow, kFDOpen, &fileInfo);
	if(TString(fileInfo.fFilename).IsNull())
	{
		return;
	}
	//make sure the file name ends with the extension
	string temp = string(fileInfo.fFilename);
	if( (temp.size()-10) != ( temp.rfind("_frnd.root") ) )
	{
		temp.append("_frnd.root");
	}
	directory = fileInfo.fIniDir;
	
	//open the aux file
	frFile = new TFile(temp.c_str(),"UPDATE");
	logStrm<<"\nFriend File Opened";
	pushToLog();
}

void AberrationCorrectionWindow::readStateData()
{
	if(!checkUpToFrFile())
	{ return; }
	if (statesSet)
	{
		logStrm<<"\nState data already loaded, to load new run data, use the reset button";
		pushToLog();
		return;
	}
	statesSet = true;
	//get the file name using a file dialog
	static TString directory(".");
	TGFileInfo fileInfo;
	fileInfo.SetMultipleSelection(false);
	fileInfo.fFileTypes = csvDataType;
	fileInfo.fIniDir = StrDup(directory);
	
	//make the open file dialog
	//quite frankly this creeps me the hell out, just creating an object like this
	//but apparently the parent object will delete it on its own
	new TGFileDialog(gClient->GetRoot(), mainWindow, kFDOpen, &fileInfo);
	//cout<<fileInfo.fFilename<<", "<<fileInfo.fIniDir;
	directory = fileInfo.fIniDir;
	
	if(TString(fileInfo.fFilename).IsNull())
	{
		return;
	}
	
	ifstream input;
	input.open(fileInfo.fFilename);
	
	//count the number of lines
	string line;
	while (getline(input, line))
	{
		if(line.length()>0)//to handle blank lines at the end of a file
		{
			++numStates;
		}
		else
		{
			break;
		}
	}
	//jump back to the beginning
	input.clear();
	input.seekg(0,ios_base::beg);
	//allocate the runs array
	for(int i=0; i<numRuns; ++i)
	{
		states[i] = new StateData[numStates];
		corrPts[i] = new CorrectionPoint[NumAngleCuts*numStates];
		for(int j=0; j<numStates; ++j)
		{
			corrPts[i][j].stateIndex=-1;
			corrPts[i][j].correctEx=-1.0;
		}
	}
	//now read line by line to get the run data
	getline(input, line);
	int count = 0;

	while(!input.eof() && count<numStates)
	{
		istringstream conv;
		conv.str(line);
		float temp;
		conv>>temp;
		for(int i=0; i<numRuns; ++i)
		{
			states[i][count].en = (temp/1000.0);
		}
		getline(input, line);
		++count;
	}
	logStrm<<"\nState data has been loaded";
	pushToLog();
	
	//generate state window
	generateStateInfo();
	//display the spectrum
	displaySubSpec(Initial);
}

/******************************************
*******************************************
** fit / state window operations
*******************************************
******************************************/
void AberrationCorrectionWindow::generateStateInfo()
{
	//make the arrays
	exEntry = new TGTextEntry*[numStates];
	aFitBox = new TGComboBox*[numStates];
	unAssignButtons = new TGTextButton*[numStates];
	fitBox = new TGComboBox*[numStates];
	assignButtons = new TGTextButton*[numStates];

	//create the individual pieces
	for(int i=0; i<numStates; ++i)
	{
		exEntry[i] = new TGTextEntry(stateVert[0]);
		exEntry[i]->SetEnabled(false);
		stateVert[0]->AddFrame(exEntry[i], cellLayout);
		
		aFitBox[i] = new TGComboBox(stateVert[1]);
		aFitBox[i]->SetEnabled(false);
		aFitBox[i]->Resize(100,22);
		stateVert[1]->AddFrame(aFitBox[i], cellLayout);
		
		ostringstream connection;
		connection<<"unAssignFitToState(="<<i<<")";
		unAssignButtons[i] = new TGTextButton(stateVert[2],"Unassign");
		unAssignButtons[i]->Connect("Clicked()","AberrationCorrectionWindow",this,connection.str().c_str());
		stateVert[2]->AddFrame(unAssignButtons[i], cellLayout);
		
		fitBox[i] = new TGComboBox(stateVert[3]);
		fitBox[i]->SetEnabled(false);
		fitBox[i]->Resize(100,22);
		stateVert[3]->AddFrame(fitBox[i], cellLayout);
		
		connection.str("");
		connection.clear();
		connection<<"assignFitToState(="<<i<<")";
		assignButtons[i] = new TGTextButton(stateVert[4],"Assign");
		assignButtons[i]->Connect("Clicked()","AberrationCorrectionWindow",this,connection.str().c_str());
		stateVert[4]->AddFrame(assignButtons[i], cellLayout);
	}
	
	//now write in the excitation energies (since that only needs to be done once)
	ostringstream cell;
	for(int i=0; i<numStates; ++i)
	{
		cell.str("");
		cell.clear();
		cell<<states[0][i].en<<" MeV";
		exEntry[i]->SetText(cell.str().c_str());
	}
	
	mainWindow->MapSubwindows();
	mainWindow->MapWindow();
	mainWindow->Layout();

}

void AberrationCorrectionWindow::updateStateInfo()
{
	
	mainWindow->MapSubwindows();
	mainWindow->MapWindow();
	mainWindow->Layout();
}

void AberrationCorrectionWindow::updateComboBoxes()
{

}

void AberrationCorrectionWindow::assignFitToState(int stNum)
{

}

void AberrationCorrectionWindow::unAssignFitToState(int stNum)
{

}

void AberrationCorrectionWindow::rebinDispSpec()
{
	int rebinVal = iterationsGetter->GetIntNumber();
	cutSpec->Rebin(rebinVal);
	cutSpec->Draw();
	whiteBoard->Update();
}

/******************************************
*******************************************
** Fit operations
*******************************************
******************************************/
void AberrationCorrectionWindow::setFitFunc()
{
	if(!checkUpToStates())
	{return;}
	//get the polynomial order
	currBGOrder = bgPolOrderGetter->GetIntNumber();
	numPeaks = numPeaksGetter->GetIntNumber();
	
	//now construct the fit function
	int paramCounter = 0;
	ostringstream formulaBuilder;
	//add a gaussian for each peak
	for(int j=0; j<numPeaks; ++j)
	{
		formulaBuilder<<"gaus("<<paramCounter<<")";
		if( currBGOrder==-1 && j!=(numPeaks-1))
		{
			formulaBuilder<<" + ";
		}
		else if (currBGOrder != -1)
		{
			formulaBuilder<<" + ";
		}
		paramCounter+=3;
	}
	//now add the appropriate polynomial background
	//add polynomial background to the fit if requested
	if( currBGOrder!=-1)
	{
		formulaBuilder<<"pol"<<currBGOrder<<"("<<paramCounter<<")";
	}
	string formula = formulaBuilder.str();
	
	fitDiag->SetFunction(formula.c_str());
}

void AberrationCorrectionWindow::getFitData()
{
	if(!checkUpToStates())
	{return;}
	TList* funcList = fitDiag->GetListOfFittingFunctions();
	TF1* fit = reinterpret_cast<TF1*>(funcList->Last());
	if(fit==NULL)
	{
		logStrm<<"No fits to get"<<endl;
		pushToLog();
		return;
	}
	//read the fit data
	double* values = fit->GetParameters();
	//clear the temp fit combobox
	tempFitBox->RemoveEntries(1,6);
	for(int i=0; i<numPeaks; ++i)
	{
		tempFits[i].height = values[3*i+0];
		tempFits[i].centroid = values[3*i+1];
		tempFits[i].width = values[3*i+2];
		tempFits[i].isAssign = false;
		ostringstream namer;
		namer<<"Cent: "<<tempFits[i].centroid;
		tempFitBox->AddEntry(namer.str().c_str(),i+1);
	}
	
}

void AberrationCorrectionWindow::transferFit()
{

}

void AberrationCorrectionWindow::removeFit()
{

}

void AberrationCorrectionWindow::getPeakFindResults()
{

}

void AberrationCorrectionWindow::performPeakFind()
{

}

/******************************************
*******************************************
** Overall Control operations
*******************************************
******************************************/
void AberrationCorrectionWindow::clearLog()
{
	messageLog->Clear();
	messageLog->LoadBuffer("Message Log Cleared");
	messageLog->Update();
	messageLog->AddLine("");
	messageLog->ShowBottom();
}

void AberrationCorrectionWindow::DoClose()
{
	delete this;
}

void AberrationCorrectionWindow::exitApp()
{
	logStrm<<"\nGoodbye!\n";
	pushToLog();
	gApplication->Terminate(0);
}



/******************************************
*******************************************
** Display functions
*******************************************
******************************************/
void AberrationCorrectionWindow::displaySubSpec(const UpdateCallType& tp)
{
	if(tp == Initial)
	{
		subSpec = NULL;
		cutSpec = NULL;
		whiteBoard->Clear();
		whiteBoard->Update();
	}
	else if( tp == Normal )
	{
		if(subSpec!=NULL)
		{
			delete subSpec;
			subSpec=NULL;
		}
		if(cutSpec!=NULL)
		{
			delete cutSpec;
			cutSpec=NULL;
		}
		whiteBoard->Clear();
		whiteBoard->Update();
	}
	
	int runN = runs[dispNum].runNumber;
	
	//retrieve the spectrum
	subSpec = testExSubSpec( runN );
	if(subSpec == NULL)
	{
		logStrm<<"Lacking a subtracted excitation energy spectrum, something is wrong, exitting";
		pushToLog();
		gApplication->Terminate(0);
		return;
	}
	int minBin = subSpec->GetYaxis()->FindBin(minAngle);
	int maxBin = subSpec->GetYaxis()->FindBin(maxAngle);
	
	cutSpec = subSpec->ProjectionX("angleCutProjection",minBin, maxBin);
	cutSpec->Draw();
	sclToggle=true;
	gPad->SetLogy(1);
	
	logStrm<<"Now displaying run: "<<runN<<" with   "<<minAngle<<"<=theta<="<<maxAngle;
	pushToLog();
	
	if(fitDiag==NULL)
	{
		TPluginHandler *handler = gROOT->GetPluginManager()->FindHandler("TFitEditor");
		if (handler && handler->LoadPlugin() != -1)
		{
			fitDiag = reinterpret_cast<TFitEditor*>(handler->ExecPlugin(2, whiteBoard->GetCanvas(), cutSpec));
			if ( fitDiag == NULL){Error("FitPanel", "Unable to create the FitPanel");}
		}
		else{Error("FitPanel", "Unable to find the FitPanel plug-in");}
	}
	else
	{
		fitDiag->SetFitObject(dynamic_cast<TVirtualPad*>(whiteBoard->GetCanvas()), dynamic_cast<TObject*>(cutSpec), kButton1Down);
	}
	
	whiteBoard->Update();
	
	//update the state information
	updateStateInfo();
}



void AberrationCorrectionWindow::prevSeqSpec()
{
	if(!checkUpToStates())
	{return;}
	if(dispNum == 0)
	{
		logStrm<<"No previous run to display";
		pushToLog();
		return;
	}
	else
	{
		--dispNum;
		minAngle=-0.05;
		maxAngle=0.05;
		displaySubSpec(Normal);
		return;
	}
}

void AberrationCorrectionWindow::nextSeqSpec()
{
	if(!checkUpToStates())
	{return;}
	if(dispNum == (numRuns -1))
	{
		logStrm<<"No next run to display";
		pushToLog();
		return;
	}
	else
	{
		++dispNum;
		minAngle=-0.05;
		maxAngle=0.05;
		displaySubSpec(Normal);
		return;
	}
}

void AberrationCorrectionWindow::prevCutSpec()
{
	if(!checkUpToStates())
	{return;}
	if( minAngle<-0.8)
	{
		logStrm<<"No Previous Angle Cut to display";
		pushToLog();
		return;
	}
	else
	{
		minAngle-=0.2;
		maxAngle-=0.2;
		displaySubSpec(Normal);
		return;
	}
}

void AberrationCorrectionWindow::nextCutSpec()
{
	if(!checkUpToStates())
	{return;}
	if( maxAngle>0.8)
	{
		logStrm<<"No Next Angle Cut to display";
		pushToLog();
		return;
	}
	else
	{
		minAngle+=0.2;
		maxAngle+=0.2;
		displaySubSpec(Normal);
		return;
	}
}

void AberrationCorrectionWindow::toggleSpec()
{
	if(!checkUpToStates())
	{return;}
	if(sclToggle)
	{
		sclToggle=false;
		gPad->SetLogy(0);
		whiteBoard->Update();
	}
	else
	{
		sclToggle=true;
		gPad->SetLogy(1);
		whiteBoard->Update();
	}
}

/******************************************
*******************************************
** Private helper functions
*******************************************
******************************************/
void AberrationCorrectionWindow::pushToLog()
{
	//ind = tempLine.find(',');
	//temp = tempLine.substr(0,ind);
	//tempLine = tempLine.substr(ind+1);
	
	//first get the string from the string stream
	string temp = logStrm.str();
	//dump this to cout as well
	cout<<temp<<endl;
	//reset the string stream;
	logStrm.str("");
	//set up a loop to parse the string searching for \n characters
	int nlInd = temp.find('\n');
	while(nlInd != string::npos)
	{
		string firstPart = temp.substr(0,nlInd);
		temp = temp.substr(nlInd+1);
		messageLog->AddLine(firstPart.c_str());
		nlInd = temp.find('\n');
	}
	messageLog->AddLine(temp.c_str());
	messageLog->ShowBottom();
	messageLog->Update();
}

string AberrationCorrectionWindow::makeTreeName(int runN)
{
	ostringstream treeNamer;
	treeNamer<<"run"<<runN<<"Tree";
	string temp = treeNamer.str();
	return temp;
}

TTree* AberrationCorrectionWindow::retrieveTree(int runN)
{
	string treeName = makeTreeName(runN);
	return reinterpret_cast<TTree*>(mainFile->Get(treeName.c_str()));
}

TTree* AberrationCorrectionWindow::testTree(int runN)
{
	TTree* temp = retrieveTree(runN);
	if( temp==NULL )
	{
		logStrm<<"Missing a raw data tree, you might have the wrong merge file loaded";
		pushToLog();
		return NULL;
	}
	else
	{
		return temp;
	}
}


string AberrationCorrectionWindow::makePIDCutName(int runN)
{
	ostringstream cutNamer;
	cutNamer<<"pidCut"<<runN;
	string temp = cutNamer.str();
	return temp;
}

TCutG* AberrationCorrectionWindow::retrievePIDCut(int runN)
{
	string cName = makePIDCutName(runN);
	return reinterpret_cast<TCutG*>(auxFile->Get(cName.c_str()));
}

TCutG* AberrationCorrectionWindow::testPIDCut(int runN)
{
	TCutG* temp = retrievePIDCut(runN);
	if( temp==NULL )
	{
		logStrm<<"Missing a PID cut, you might have the wrong aux file loaded";
		pushToLog();
		logStrm<<"or you might have forgotten to run Get PID Cut(s)";
		pushToLog();
		return NULL;
	}
	else
	{
		return temp;
	}
}


string AberrationCorrectionWindow::makePIDSpecName(int runN)
{
	ostringstream histNamer;
	histNamer<<"h2PIDRun"<<runN;
	string temp = histNamer.str();
	return temp;
}

TH2F* AberrationCorrectionWindow::retrievePIDSpec(int runN)
{
	string histName = makePIDSpecName(runN);
	return reinterpret_cast<TH2F*>(auxFile->Get(histName.c_str()));
}

TH2F* AberrationCorrectionWindow::testPIDSpec(int runN)
{
	TH2F* temp = retrievePIDSpec(runN);
	if( temp==NULL )
	{
		logStrm<<"Missing a PID spectrum, you might have the wrong aux file loaded";
		pushToLog();
		logStrm<<"or you might have forgotten to run Get PID Cut(s)";
		pushToLog();
		return NULL;
	}
	else
	{
		return temp;
	}
}

string AberrationCorrectionWindow::makeBaseCutName(int runN)
{
	ostringstream cutNamer;
	cutNamer<<"baseCut"<<runN;
	string temp = cutNamer.str();
	return temp;
}

TCut* AberrationCorrectionWindow::retrieveBaseCut(int runN)
{
	string cName = makeBaseCutName(runN);
	return reinterpret_cast<TCut*>(auxFile->Get(cName.c_str()));
}

TCut* AberrationCorrectionWindow::testBaseCut(int runN)
{
	TCut* temp = retrieveBaseCut(runN);
	if( temp==NULL )
	{
		logStrm<<"Missing a base cut, you might have the wrong aux file loaded";
		pushToLog();
		logStrm<<"or you might have forgotten to run Get PID Cut(s)";
		pushToLog();
		return NULL;
	}
	else
	{
		return temp;
	}
}

string AberrationCorrectionWindow::makeBGGraphName(int runN)
{
	ostringstream graphNamer;
	graphNamer<<"bgGraph"<<runN;
	string temp = graphNamer.str();
	return temp;
}

TGraph* AberrationCorrectionWindow::retrieveBGGraph(int runN)
{
	string gName = makeBGGraphName(runN);
	return reinterpret_cast<TGraph*>(auxFile->Get(gName.c_str()));
}

TGraph* AberrationCorrectionWindow::testBGGraph(int runN)
{
	TGraph* temp = retrieveBGGraph(runN);
	if( temp==NULL )
	{
		logStrm<<"Missing a Yfp positions definition, you might have the wrong aux file loaded";
		pushToLog();
		logStrm<<"or you might have forgotten to run Get BG Cut(s)";
		pushToLog();
		return NULL;
	}
	else
	{
		return temp;
	}
}

string AberrationCorrectionWindow::makeBGCutName(int runN)
{
	ostringstream cutNamer;
	cutNamer<<"bgCut"<<runN;
	string temp = cutNamer.str();
	return temp;
}

TCut* AberrationCorrectionWindow::retrieveBGCut(int runN)
{
	string cName = makeBGCutName(runN);
	return reinterpret_cast<TCut*>(auxFile->Get(cName.c_str()));
}

TCut* AberrationCorrectionWindow::testBGCut(int runN)
{
	TCut* temp = retrieveBGCut(runN);
	if( temp==NULL )
	{
		logStrm<<"Missing a BG Region Cut, you might have the wrong aux file loaded";
		pushToLog();
		logStrm<<"or you might have forgotten to run Get BG Cut(s)";
		pushToLog();
		return NULL;
	}
	else
	{
		return temp;
	}
}

string AberrationCorrectionWindow::makeTRCutName(int runN)
{
	ostringstream cutNamer;
	cutNamer<<"trCut"<<runN;
	string temp = cutNamer.str();
	return temp;
}

TCut* AberrationCorrectionWindow::retrieveTRCut(int runN)
{
	string cName = makeTRCutName(runN);
	return reinterpret_cast<TCut*>(auxFile->Get(cName.c_str()));
}

TCut* AberrationCorrectionWindow::testTRCut(int runN)
{
	TCut* temp = retrieveTRCut(runN);
	if( temp==NULL )
	{
		logStrm<<"Missing a True+BG Region Cut, you might have the wrong aux file loaded";
		pushToLog();
		logStrm<<"or you might have forgotten to run Get BG Cut(s)";
		pushToLog();
		return NULL;
	}
	else
	{
		return temp;
	}
}

string AberrationCorrectionWindow::makeBGSpecName(int runN)
{
	ostringstream bgNamer;
	bgNamer<<"h1YFPRun"<<runN;
	string temp = bgNamer.str();
	return temp;
}

TH1F* AberrationCorrectionWindow::retrieveBGSpec(int runN)
{
	string histName = makeBGSpecName(runN);
	return reinterpret_cast<TH1F*>(auxFile->Get(histName.c_str()));
}

TH1F* AberrationCorrectionWindow::testBGSpec(int runN)
{
	TH1F* temp = retrieveBGSpec(runN);
	if( temp==NULL )
	{
		logStrm<<"Missing a background spectrum, you might have the wrong aux file loaded";
		pushToLog();
		logStrm<<"or you might have forgotten to run Get BG Cut(s)";
		pushToLog();
		return NULL;
	}
	else
	{
		return temp;
	}
}

string AberrationCorrectionWindow::makeOrigShapeSpecName(int runN)
{
	ostringstream bgNamer;
	bgNamer<<"h2OrigShapeSpec"<<runN;
	string temp = bgNamer.str();
	return temp;
}

TH2F* AberrationCorrectionWindow::retrieveOrigShapeSpec(int runN)
{
	string histName = makeOrigShapeSpecName(runN);
	return reinterpret_cast<TH2F*>(auxFile->Get(histName.c_str()));
}

TH2F* AberrationCorrectionWindow::testOrigShapeSpec(int runN)
{
	TH2F* temp = retrieveOrigShapeSpec(runN);
	if( temp==NULL )
	{
		logStrm<<"Missing an original shape spectrum, you might have the wrong aux file loaded";
		pushToLog();
		return NULL;
	}
	else
	{
		return temp;
	}
}

string AberrationCorrectionWindow::makeFirstOrderShapeSpecName(int runN)
{
	ostringstream bgNamer;
	bgNamer<<"h2FirstOrderShapeSpec"<<runN;
	string temp = bgNamer.str();
	return temp;
}

TH2F* AberrationCorrectionWindow::retrieveFirstOrderShapeSpec(int runN)
{
	string histName = makeFirstOrderShapeSpecName(runN);
	return reinterpret_cast<TH2F*>(auxFile->Get(histName.c_str()));
}

TH2F* AberrationCorrectionWindow::testFirstOrderShapeSpec(int runN)
{
	TH2F* temp = retrieveFirstOrderShapeSpec(runN);
	if( temp==NULL )
	{
		logStrm<<"Missing a first order shape spectrum, you might have the wrong aux file loaded";
		pushToLog();
		return NULL;
	}
	else
	{
		return temp;
	}
}

string AberrationCorrectionWindow::makeFirstOrderShapeTreeName(int runN)
{
	ostringstream bgNamer;
	bgNamer<<"run"<<runN<<"_tree_shape1";
	string temp = bgNamer.str();
	return temp;
}

TTree* AberrationCorrectionWindow::retrieveFirstOrderShapeTree(int runN)
{
	string histName = makeFirstOrderShapeTreeName(runN);
	return reinterpret_cast<TTree*>(frFile->Get(histName.c_str()));
}

TTree* AberrationCorrectionWindow::testFirstOrderShapeTree(int runN)
{
	TTree* temp = retrieveFirstOrderShapeTree(runN);
	if( temp==NULL )
	{
		logStrm<<"Missing a first order shape tree, you might have the wrong friend file loaded";
		pushToLog();
		return NULL;
	}
	else
	{
		return temp;
	}
}

string AberrationCorrectionWindow::makeTRpBGSpecName(int runN)
{
	ostringstream bgNamer;
	bgNamer<<"h2TrpBG"<<runN;
	string temp = bgNamer.str();
	return temp;
}

TH2F* AberrationCorrectionWindow::retrieveTRpBGSpec(int runN)
{
	string histName = makeTRpBGSpecName(runN);
	return reinterpret_cast<TH2F*>(auxFile->Get(histName.c_str()));
}

TH2F* AberrationCorrectionWindow::testTRpBGSpec(int runN)
{
	TH2F* temp = retrieveTRpBGSpec(runN);
	if( temp==NULL )
	{
		logStrm<<"Missing a True+BG Theta vs Xfp spectrum, you might have the wrong aux file loaded";
		pushToLog();
		return NULL;
	}
	else
	{
		return temp;
	}
}

string AberrationCorrectionWindow::makeBGOnlySpecName(int runN)
{
	ostringstream bgNamer;
	bgNamer<<"h2BgOnly"<<runN;
	string temp = bgNamer.str();
	return temp;
}

TH2F* AberrationCorrectionWindow::retrieveBGOnlySpec(int runN)
{
	string histName = makeBGOnlySpecName(runN);
	return reinterpret_cast<TH2F*>(auxFile->Get(histName.c_str()));
}

TH2F* AberrationCorrectionWindow::testBGOnlySpec(int runN)
{
	TH2F* temp = retrieveBGOnlySpec(runN);
	if( temp==NULL )
	{
		logStrm<<"Missing a BG Theta vs Xfp spectrum, you might have the wrong aux file loaded";
		pushToLog();
		return NULL;
	}
	else
	{
		return temp;
	}
}

string AberrationCorrectionWindow::makeScaledBGSpecName(int runN)
{
	ostringstream bgNamer;
	bgNamer<<"h2ScaledBg"<<runN;
	string temp = bgNamer.str();
	return temp;
}

TH2F* AberrationCorrectionWindow::retrieveScaledBGSpec(int runN)
{
	string histName = makeScaledBGSpecName(runN);
	return reinterpret_cast<TH2F*>(auxFile->Get(histName.c_str()));
}

TH2F* AberrationCorrectionWindow::testScaledBGSpec(int runN)
{
	TH2F* temp = retrieveScaledBGSpec(runN);
	if( temp==NULL )
	{
		logStrm<<"Missing a BG Theta vs Xfp spectrum, you might have the wrong aux file loaded";
		pushToLog();
		return NULL;
	}
	else
	{
		return temp;
	}
}

string AberrationCorrectionWindow::makeSubSpecName(int runN)
{
	ostringstream bgNamer;
	bgNamer<<"h2SubSpec"<<runN;
	string temp = bgNamer.str();
	return temp;
}

TH2F* AberrationCorrectionWindow::retrieveSubSpec(int runN)
{
	string histName = makeSubSpecName(runN);
	return reinterpret_cast<TH2F*>(auxFile->Get(histName.c_str()));
}

string AberrationCorrectionWindow::makeExSubSpecName(int runN)
{
	ostringstream bgNamer;
	bgNamer<<"h2ExSubSpec"<<runN;
	string temp = bgNamer.str();
	return temp;
}

TH2F* AberrationCorrectionWindow::retrieveExSubSpec(int runN)
{
	string histName = makeExSubSpecName(runN);
	return reinterpret_cast<TH2F*>(auxFile->Get(histName.c_str()));
}

TH2F* AberrationCorrectionWindow::testExSubSpec(int runN)
{
	TH2F* temp = retrieveExSubSpec(runN);
	if( temp==NULL )
	{
		logStrm<<"Missing a Subtracted Theta vs Ex spectrum, you might have the wrong aux file loaded";
		pushToLog();
		return NULL;
	}
	else
	{
		return temp;
	}
}

TH2F* AberrationCorrectionWindow::testSubSpec(int runN)
{
	TH2F* temp = retrieveSubSpec(runN);
	if( temp==NULL )
	{
		logStrm<<"Missing a Subtracted Theta vs Xfp spectrum, you might have the wrong aux file loaded";
		pushToLog();
		return NULL;
	}
	else
	{
		return temp;
	}
}

bool AberrationCorrectionWindow::checkUpToRunData()
{
	if ( runs == NULL )
	{
		logStrm<<"\nLoad Run Data first";
		pushToLog();
		return false;
	}
	return true;
}

bool AberrationCorrectionWindow::checkUpToMainFile()
{
	if( !checkUpToRunData() )
	{
		return false;
	}
	else if ( mainFile == NULL )
	{
		logStrm<<"\nLoad merge file first";
		pushToLog();
		return false;
	}
	return true;
}

bool AberrationCorrectionWindow::checkUpToAuxFile()
{
	if( !checkUpToMainFile() )
	{
		return false;
	}
	else if ( auxFile == NULL )
	{
		logStrm<<"\nLoad auxiliary file first";
		pushToLog();
		return false;
	}
	return true;
}

bool AberrationCorrectionWindow::checkUpToFrFile()
{
	if( !checkUpToAuxFile() )
	{
		return false;
	}
	else if( frFile == NULL )
	{
		logStrm<<"\nLoad a friend file first";
		pushToLog();
		return false;
	}
	return true;
}

bool AberrationCorrectionWindow::checkUpToStates()
{
	if( !checkUpToFrFile() )
	{
		return false;
	}
	else if( !statesSet )
	{
		logStrm<<"\nLoad a states file first";
		pushToLog();
		return false;
	}
	return true;
}

void AberrationCorrectionWindow::parseRunFileLine(const string& line, RunData& tempData)
{
	istringstream conv;
	//read the run number
	int ind = line.find(',');
	string temp = line.substr(0,ind);
	string tempLine = line.substr(ind+1);
	conv.str(temp);
	conv>>tempData.runNumber;
	conv.clear();
	//read the angle
	ind = tempLine.find(',');
	temp = tempLine.substr(0,ind);
	tempLine = tempLine.substr(ind+1);
	conv.str(temp);
	conv>>tempData.angle;
	conv.clear();
	//read the beam energy 
	ind = tempLine.find(',');
	temp = tempLine.substr(0,ind);
	tempLine = tempLine.substr(ind+1);
	conv.str(temp);
	conv>>tempData.beamEn;
	conv.clear();
	//read the Charge State
	ind = tempLine.find(',');
	temp = tempLine.substr(0,ind);
	tempLine = tempLine.substr(ind+1);
	conv.str(temp);
	conv>>tempData.chargeState;
	conv.clear();
	//read the projectile mass
	ind = tempLine.find(',');
	temp = tempLine.substr(0,ind);
	tempLine = tempLine.substr(ind+1);
	conv.str(temp);
	conv>>tempData.projMass;
	conv.clear();
	//read the target mass
	ind = tempLine.find(',');
	temp = tempLine.substr(0,ind);
	tempLine = tempLine.substr(ind+1);
	conv.str(temp);
	conv>>tempData.targetMass;
	conv.clear();
	//read the target thickness
	ind = tempLine.find(',');
	temp = tempLine.substr(0,ind);
	tempLine = tempLine.substr(ind+1);
	conv.str(temp);
	conv>>tempData.thickness;
	conv.clear();
	//read the beam integral
	ind = tempLine.find(',');
	temp = tempLine.substr(0,ind);
	tempLine = tempLine.substr(ind+1);
	conv.str(temp);
	conv>>tempData.beamInt;
	conv.clear();
	//read the BI range
	ind = tempLine.find(',');
	temp = tempLine.substr(0,ind);
	tempLine = tempLine.substr(ind+1);
	conv.str(temp);
	conv>>tempData.biRange;
	conv.clear();
	//read the GR request
	ind = tempLine.find(',');
	temp = tempLine.substr(0,ind);
	tempLine = tempLine.substr(ind+1);
	conv.str(temp);
	conv>>tempData.grRequest;
	conv.clear();
	//read the GR accept
	ind = tempLine.find(',');
	temp = tempLine.substr(0,ind);
	tempLine = tempLine.substr(ind+1);
	conv.str(temp);
	conv>>tempData.grAccept;
	conv.clear();
	//read the vdc effeciency
	ind = tempLine.find(',');
	temp = tempLine.substr(0,ind);
	tempLine = tempLine.substr(ind+1);
	conv.str(temp);
	conv>>tempData.vdcEff;
	conv.clear();
}
#endif
