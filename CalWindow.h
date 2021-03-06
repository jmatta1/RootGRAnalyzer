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
#include<TGraphErrors.h>

//my includes
#include"bicubicinterp.h"
#include"guiSupport.h"

//TODO, modify the widths of the text entries
//TODO, fix up deconstructor
//TODO, fix up reset function

enum UpdateCallType{ Initial, Normal, Final};
enum WidgetNumberings{ PolOrderEntry = 0};

TCut RayCut;

class CalWindow
{
	RQ_OBJECT("CalWindow")
public:
	CalWindow(const TGWindow* parent, UInt_t width, UInt_t height);
	~CalWindow();
	
	//fit control
	void setFitFunc();
	void getFitData();
	void doQuickFit();
	
	//calibration control
	void transferFit();
	void transferAllFits();
	void removeFit();
	void assignFitToState(int stNum);
	void unAssignFitToState(int stNum);
	bool getCalibration();
	void showCalibration();
	void hideCalibration();
	void exportCalibrations();
	
	//sequential spectrum display
	void prevSeqSpec();
	void nextSeqSpec();
	void toggleSpec();
	
	//file operations
	void readRunData();
	void openBigFile();
	void openAuxFile();
	void openFriendFile();
	void openInterpFile();
	void readStateData();
	
	//overall control operations
	void clearLog();
	void resetToStart();
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
	bool checkUpToInterpFile();
	bool checkUpToStates();
	bool checkForFitStart();
	void doScatteringCalcs();
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
	
	//display functions for sequential displays
	void displaySubSpec(const UpdateCallType& tp);
	void sepFitAndDraw();
	
	//analysis stuff
	RunData* runs;
	FitData* tempFits;
	StateData** states;
	double quickFitPos[7];
	double tempHistVals[7];
	double tempParams[18];
	bool statesSet;
	FitData** fitList;
	int* numFits;
	StateFit** assigns;
	int* numAssigned;
	TGraphErrors* calGraph;
	TF1** calFits;
	int* calOrd;
	int numRuns;
	int numStates;
	TFile* mainFile;
	TFile* auxFile;
	TFile* frFile;
	BiCubicInterpolation* interp;
	int currBGOrder;
	int numPeaks;
	bool quickFitFirstTime;

	//display stuff
	int dispNum;
	bool sclToggle;
	TH2F* subSpec;
	TH1D* cutSpec;

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
	TGHorizontalFrame* calOrderFrame;//holds the calibration order getter stuff
	
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
	TGVerticalFrame* stateVert[9];//organizers that hold the cells
	TGLabel* colLabels[9];//column labels
	TGTextEntry** exEntry;//excitation energy holders
	TGTextEntry** fpmEntry;//calculated focal plane momenta
	TGTextEntry** calfpmEntry;//focal plain momenta from calibration
	TGTextEntry** calxfpEntry;//xfp from calculated focal plane momentum
	TGTextEntry** aFitEntry;//assigned fit centroids
	TGTextEntry** difEntry;//difference between assigned fit centroid and calculated centroid
	TGComboBox** fitBox;//combo boxes for holding unassigned fits
	TGTextButton** assignButtons;//buttons to assign a selected fit to a state
	TGTextButton** unAssignButtons;//buttons to unassign a fit from a state
	
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
	TGLabel* calOrderLabel;
	TGLabel* calCtrlLabel;
	
	//action buttons
	TGNumberEntry* bgPolOrderGetter;//used for getting the order of the polynomial background
	TGNumberEntry* numPeaksGetter;//used for getting the number of peaks to fit
	TGNumberEntry* calOrderGetter;//used for getting the order of the cal function
	TGTextButton* setFunc;
	TGTextButton* getFit;
	TGTextButton* quickFit;
	TGTextButton* doCal;
	TGTextButton* showCal;
	TGTextButton* hideCal;
	TGComboBox* tempFitBox;
	TGTextButton* useFit;
	TGTextButton* useAllFits;
	TGComboBox* fitListBox;
	TGTextButton* rmFit;
	TGTextButton* exportCal;
	//sequential display buttons
	TGTextButton *prevSpec;
	TGTextButton *nextSpec;
	TGTextButton *toggleScale;

	//file ops buttons
	TGTextButton *rdRunData;
	TGTextButton *opBigFile;
	TGTextButton *opAuxFile;
	TGTextButton *opFrFile;
	TGTextButton *opInterpFile;
	TGTextButton *rdStateData;
	
	//overall control buttons
	TGTextButton *clLogBut;
	TGTextButton *resBut;
	TGTextButton *exBut;
};

CalWindow::CalWindow(const TGWindow* parent, UInt_t width, UInt_t height)
{
	runs=NULL;
	states=NULL;
	numRuns=0;
	numStates=0;
	mainFile=NULL;
	auxFile=NULL;
	frFile=NULL;
	interp = NULL;
	dispNum = 0;
	sclToggle=false;
	subSpec=NULL;
	cutSpec=NULL;
	fitDiag=NULL;
	currBGOrder=0;
	numPeaks=0;
	numFits=NULL;
	statesSet=false;
	numAssigned=NULL;
	quickFitFirstTime = false;
	TGraphErrors* calGraph;

	tempFits = new FitData[5];

	//sys = new TUnixSystem();
	sys = gSystem;
	
	RayCut.SetTitle("Rayid==0");
	
	//paramDialog = new ParamAdjustmentDialog(parent,width,height);
	
	//create the main window for the gui
	mainWindow = new TGMainFrame(parent,width,height);
	
	//set up some stuff to prevent the X11 BadDrawable error
	mainWindow->SetCleanup(kDeepCleanup);
	mainWindow->Connect("CloseWindow()","CalWindow",this,"DoClose()");
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
	calOrderFrame = new TGHorizontalFrame(sideBarFrame, width, height);
	
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
	setFunc->Connect("Clicked()","CalWindow",this,"setFitFunc()");
	//button that gets the fit data from the fit panel
	getFit = new TGTextButton(sideBarFrame,"Get Fit Data");
	getFit->Connect("Clicked()","CalWindow",this,"getFitData()");
	//button that gives you the quick fit interface
	quickFit = new TGTextButton(sideBarFrame,"Quick Fit");
	quickFit->Connect("Clicked()","CalWindow",this,"doQuickFit()");
	//add the sidebar buttons to the sidebar frame
	sideBarFrame->AddFrame(orderFrame, new TGLayoutHints(kLHintsExpandX,2,2,2,2));
	sideBarFrame->AddFrame(numPksFrame, new TGLayoutHints(kLHintsExpandX,2,2,2,2));
	sideBarFrame->AddFrame(setFunc, new TGLayoutHints(kLHintsExpandX,2,2,2,2));
	sideBarFrame->AddFrame(getFit, new TGLayoutHints(kLHintsExpandX,2,2,2,2));
	sideBarFrame->AddFrame(quickFit, new TGLayoutHints(kLHintsExpandX,2,2,2,2));
	
	/******************************************
	** Calibration Control
	******************************************/
	calCtrlLabel = new TGLabel(sideBarFrame, "Calib. Control");
	sideBarFrame->AddFrame(calCtrlLabel, new TGLayoutHints(kLHintsExpandX,2,2,2,2));
	
	//add the combo box that holds temporary fit information
	tempFitBox = new TGComboBox(sideBarFrame);
	tempFitBox->AddEntry("Retrieved Fits",0);
	tempFitBox->Select(0);
	tempFitBox->Resize(100,20);
	useFit = new TGTextButton(sideBarFrame,"Use Fit");
	useFit->Connect("Clicked()","CalWindow",this,"transferFit()");
	useAllFits = new TGTextButton(sideBarFrame,"Use All Fits");
	useAllFits->Connect("Clicked()","CalWindow",this,"transferAllFits()");
	fitListBox = new TGComboBox(sideBarFrame);
	fitListBox->AddEntry("Fits",0);
	fitListBox->Select(0);
	fitListBox->Resize(100,20);
	rmFit = new TGTextButton(sideBarFrame,"Remove Fit");
	rmFit->Connect("Clicked()","CalWindow",this,"removeFit()");
	calOrderGetter = new TGNumberEntry( calOrderFrame, 1.0, 5, PolOrderEntry, TGNumberFormat::kNESInteger, 
			TGNumberFormat::kNEAAnyNumber, TGNumberFormat::kNELLimitMinMax, 1, 2);
	calOrderLabel = new TGLabel(calOrderFrame, "Cal Ord");
	calOrderFrame->AddFrame(calOrderGetter, new TGLayoutHints(kLHintsLeft | kLHintsTop, 5, 5, 3, 4) );
	calOrderFrame->AddFrame(calOrderLabel, new TGLayoutHints(kLHintsNormal, 5, 5, 3, 4) );
	doCal = new TGTextButton(sideBarFrame,"Do Calib.");
	doCal->Connect("Clicked()","CalWindow",this,"getCalibration()");
	showCal = new TGTextButton(sideBarFrame,"Show Calib.");
	showCal->Connect("Clicked()","CalWindow",this,"showCalibration()");
	hideCal = new TGTextButton(sideBarFrame,"Hide Calib.");
	hideCal->Connect("Clicked()","CalWindow",this,"hideCalibration()");
	exportCal = new TGTextButton(sideBarFrame,"Export Cals.");
	exportCal->Connect("Clicked()","CalWindow",this,"exportCalibrations()");
	
	sideBarFrame->AddFrame(tempFitBox, new TGLayoutHints(kLHintsExpandX,2,2,2,2));
	sideBarFrame->AddFrame(useFit, new TGLayoutHints(kLHintsExpandX,2,2,2,2));
	sideBarFrame->AddFrame(useAllFits, new TGLayoutHints(kLHintsExpandX,2,2,2,2));
	sideBarFrame->AddFrame(fitListBox, new TGLayoutHints(kLHintsExpandX,2,2,2,2));
	sideBarFrame->AddFrame(rmFit, new TGLayoutHints(kLHintsExpandX,2,2,2,2));
	sideBarFrame->AddFrame(calOrderFrame, new TGLayoutHints(kLHintsExpandX,2,2,2,2));
	sideBarFrame->AddFrame(doCal, new TGLayoutHints(kLHintsExpandX,2,2,2,2));
	sideBarFrame->AddFrame(showCal, new TGLayoutHints(kLHintsExpandX,2,2,2,2));
	sideBarFrame->AddFrame(hideCal, new TGLayoutHints(kLHintsExpandX,2,2,2,2));
	sideBarFrame->AddFrame(exportCal, new TGLayoutHints(kLHintsExpandX,2,2,2,2));
	
	//add the sidebar to the canvas frame
	canvasFrame->AddFrame(sideBarFrame,  new TGLayoutHints(kLHintsTop | kLHintsLeft | kLHintsExpandY, 2,2,2,2) );
	
	/******************************************
	** Tabbed Object Creation
	******************************************/
	tabFrame =  new TGTab(canvasFrame, 10, 10);
	whiteBoardCon = new TGCompositeFrame(tabFrame, 10, 10);
	stateDataCon = new TGCompositeFrame(tabFrame, 10, 10);
	tabFrame->AddTab("Histogram", whiteBoardCon);
	tabFrame->AddTab("Calibration", stateDataCon);
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
	for(int i=0; i<9; ++i)
	{
		stateVert[i] = new TGVerticalFrame(stateHoriz, 10, 10);
		colLabels[i] = new TGLabel(stateVert[i], calColNames[i]);
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
	rdRunData->Connect("Clicked()","CalWindow",this,"readRunData()");
	//open merge root file
	opBigFile = new TGTextButton(fileOpsRowFrame,"Open Merge File");
	opBigFile->Connect("Clicked()","CalWindow",this,"openBigFile()");
	//Open Aux File button
	opAuxFile = new TGTextButton(fileOpsRowFrame,"Open Aux File");
	opAuxFile->Connect("Clicked()","CalWindow",this,"openAuxFile()");
	//Open friend file button
	opFrFile = new TGTextButton(fileOpsRowFrame,"Open Friend File");
	opFrFile->Connect("Clicked()","CalWindow",this,"openFriendFile()");
	//Open Interpolation Button
	opInterpFile = new TGTextButton(fileOpsRowFrame,"Open Interpolation File");
	opInterpFile->Connect("Clicked()","CalWindow",this,"openInterpFile()");
	//Read State Data Button
	rdStateData = new TGTextButton(fileOpsRowFrame,"Read State Data");
	rdStateData->Connect("Clicked()","CalWindow",this,"readStateData()");
	
	
	//add file operations buttons to their frame
	fileOpsRowFrame->AddFrame(rdRunData, new TGLayoutHints(kLHintsExpandX ,3,3,2,2));
	fileOpsRowFrame->AddFrame(opBigFile, new TGLayoutHints(kLHintsExpandX,3,3,2,2));
	fileOpsRowFrame->AddFrame(opAuxFile, new TGLayoutHints(kLHintsExpandX,3,3,2,2));
	fileOpsRowFrame->AddFrame(opFrFile, new TGLayoutHints(kLHintsExpandX,3,3,2,2));
	fileOpsRowFrame->AddFrame(opInterpFile, new TGLayoutHints(kLHintsExpandX,3,3,2,2));
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
	prevSpec->Connect("Clicked()","CalWindow",this,"prevSeqSpec()");
	//show next spectrum
	nextSpec = new TGTextButton(specDispFrame,"Next Run");
	nextSpec->Connect("Clicked()","CalWindow",this,"nextSeqSpec()");
	//toggle spectrum axis
	toggleScale = new TGTextButton(specDispFrame,"Toggle Scale");
	toggleScale->Connect("Clicked()","CalWindow",this,"toggleSpec()");
	
	//add the sequential display buttons to their frame
	specDispFrame->AddFrame(prevSpec, new TGLayoutHints(kLHintsExpandX,2,2,2,2));
	specDispFrame->AddFrame(nextSpec, new TGLayoutHints(kLHintsExpandX,2,2,2,2));
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
	clLogBut->Connect("Clicked()","CalWindow",this,"clearLog()");
	//reset button
	resBut = new TGTextButton(overallControlFrame,"Reset");
	resBut->Connect("Clicked()","CalWindow",this,"resetToStart()");
	//Kill application button
	exBut = new TGTextButton(overallControlFrame,"Exit");
	exBut->Connect("Clicked()","CalWindow",this,"exitApp()");
	
	
	//add the overall control buttons to their frame
	overallControlFrame->AddFrame(clLogBut, new TGLayoutHints(kLHintsExpandX,3,3,2,2));
	overallControlFrame->AddFrame(resBut, new TGLayoutHints(kLHintsExpandX,3,3,2,2));
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
	messageLog = new TGTextView(messageLogFrame,1,100,calInitMessage);
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
	mainWindow->SetWindowName("Calibration GUI");
	//go through the steps to draw this beast
	//map the subwindows
	mainWindow->MapSubwindows();
	//init the layout engine
	mainWindow->Resize(mainWindow->GetDefaultSize());
	mainWindow->Resize(850,650);
	//map the main frame
	mainWindow->MapWindow();
}

CalWindow::~CalWindow()
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
	if(interp != NULL)
	{
		delete interp;
		interp = NULL;
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
	
	if(fitList != NULL)
	{
		for(int i=0; i<numRuns; ++i)
		{
			if(fitList[i]!=NULL)
			{
				delete[] fitList[i];
			}
		}
		delete[] fitList;
		fitList = NULL;
	}
	if(assigns != NULL)
	{
		for(int i=0; i<numRuns; ++i)
		{
			if(assigns[i]!=NULL)
			{
				delete[] assigns[i];
			}
		}
		delete[] assigns;
		assigns = NULL;
	}
	
	if(calFits != NULL)
	{
		for(int i=0; i<numRuns; ++i)
		{
			if(calFits[i]!=NULL)
			{
				delete[] calFits[i];
			}
		}
		delete[] calFits;
		calFits = NULL;
	}
	if(calOrd != NULL)
	{
		delete calOrd;
		calOrd = NULL;
	}
	if(numAssigned != NULL)
	{
		delete numAssigned;
		numAssigned = NULL;
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
void CalWindow::readRunData()
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
	fitList = new FitData*[numRuns];
	assigns = new StateFit*[numRuns];
	calFits = new TF1*[numRuns];
	calOrd = new int[numRuns];
	numAssigned = new int[numRuns];
	numFits = new int[numRuns];
	for(int i =0; i<numRuns; ++i)
	{
		states[i] = NULL;
		fitList[i] = NULL;
		assigns[i] = NULL;
		calFits[i] = NULL;
		calOrd[i] = 0;
		numAssigned[i] = 0;
		numFits[i] = 0;
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

void CalWindow::openBigFile()
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

void CalWindow::openAuxFile()
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

void CalWindow::openFriendFile()
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

void CalWindow::openInterpFile()
{
	if(!checkUpToFrFile())
	{	return; }
	if( interp != NULL )
	{
		logStrm<<"\nInterpolation file already loaded, to load a different one, use the reset button";
		pushToLog();
		return;
	}
	//get the file name using a file dialog
	static TString directory(".");
	TGFileInfo fileInfo;
	fileInfo.SetMultipleSelection(false);
	fileInfo.fFileTypes = interpDataType;
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
	directory = fileInfo.fIniDir;
	
	int* sizesBuff = new int[2];
	float* xVal = new float[1000];
	float* yVal = new float[1000];
	float* zVal = new float[1000000];
	
	ifstream inFile;
	inFile.open(temp.c_str(),std::fstream::in | std::fstream::binary);
	inFile.read(reinterpret_cast<char*>(sizesBuff),2*sizeof(int));//technically we do not really need these since both are always 1000
	inFile.read(reinterpret_cast<char*>(xVal),sizeof(float)*1000);
	logStrm<<"The min thickness is: "<<xVal[0]<<endl;
	pushToLog();
	inFile.read(reinterpret_cast<char*>(yVal),sizeof(float)*1000);
	inFile.read(reinterpret_cast<char*>(zVal),sizeof(float)*1000000);
	inFile.close();
	
	interp = new BiCubicInterpolation(xVal, 1000, yVal, 1000, zVal);
	logStrm<<"\nInterp File Opened";
	pushToLog();
	delete[] sizesBuff;
	delete[] xVal;
	delete[] yVal;
	delete[] zVal;
}

void CalWindow::readStateData()
{
	if(!checkUpToInterpFile())
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
		fitList[i] = new FitData[numStates];
		assigns[i] = new StateFit[numStates];
		calFits[i] = NULL;
		for(int j=0; j<numStates; ++j)
		{
			fitList[i][j].isAssign = false;
			assigns[i][j].isSet = false;
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
	
	//run scattering calcs
	doScatteringCalcs();
	//generate state window
	generateStateInfo();
	//display the spectrum
	displaySubSpec(Initial);
}

/******************************************
*******************************************
** calibration / state window operations
*******************************************
******************************************/
void CalWindow::generateStateInfo()
{
	/*for(int i=0; i<numStates; ++i)
	{
		cout<<i<<" | "<<(states[i].en)<<" | "<<(states[i].scatEn)<<" | "<<states[i].fpMom<<endl;
	}*/
	//make the arrays
	exEntry = new TGTextEntry*[numStates];
	fpmEntry = new TGTextEntry*[numStates];
	calxfpEntry = new TGTextEntry*[numStates];
	aFitEntry = new TGTextEntry*[numStates];
	calfpmEntry = new TGTextEntry*[numStates];
	difEntry = new TGTextEntry*[numStates];
	fitBox = new TGComboBox*[numStates];
	assignButtons = new TGTextButton*[numStates];
	unAssignButtons = new TGTextButton*[numStates];

	//create the individual pieces
	for(int i=0; i<numStates; ++i)
	{
		exEntry[i] = new TGTextEntry(stateVert[0]);
		exEntry[i]->SetEnabled(false);
		stateVert[0]->AddFrame(exEntry[i], cellLayout);
		
		fpmEntry[i] = new TGTextEntry(stateVert[1]);
		fpmEntry[i]->SetEnabled(false);
		stateVert[1]->AddFrame(fpmEntry[i], cellLayout);
		
		calxfpEntry[i] = new TGTextEntry(stateVert[2]);
		calxfpEntry[i]->SetEnabled(false);
		stateVert[2]->AddFrame(calxfpEntry[i], cellLayout);
		
		aFitEntry[i] = new TGTextEntry(stateVert[3]);
		aFitEntry[i]->SetEnabled(false);
		stateVert[3]->AddFrame(aFitEntry[i], cellLayout);
		
		calfpmEntry[i] = new TGTextEntry(stateVert[4]);
		calfpmEntry[i]->SetEnabled(false);
		stateVert[4]->AddFrame(calfpmEntry[i], cellLayout);
		
		difEntry[i] = new TGTextEntry(stateVert[5]);
		difEntry[i]->SetEnabled(false);
		stateVert[5]->AddFrame(difEntry[i], cellLayout);
		
		fitBox[i] = new TGComboBox(stateVert[6]);
		fitBox[i]->AddEntry("Poss. Fits",0);
		fitBox[i]->Select(0);
		fitBox[i]->Resize(100,exEntry[i]->GetHeight());
		stateVert[6]->AddFrame(fitBox[i], cellLayout);
		
		ostringstream connection;
		connection<<"assignFitToState(="<<i<<")";
		assignButtons[i] = new TGTextButton(stateVert[7],"Assign");
		assignButtons[i]->Connect("Clicked()","CalWindow",this,connection.str().c_str());
		stateVert[7]->AddFrame(assignButtons[i], cellLayout);
		
		connection.str("");
		connection.clear();
		connection<<"unAssignFitToState(="<<i<<")";
		unAssignButtons[i] = new TGTextButton(stateVert[8],"Unassign");
		unAssignButtons[i]->Connect("Clicked()","CalWindow",this,connection.str().c_str());
		stateVert[8]->AddFrame(unAssignButtons[i], cellLayout);
	}
	
	//now write in the excitation energies (since that only needs to be done once)
	ostringstream cell;
	for(int i=0; i<numStates; ++i)
	{
		cell.str("");
		cell.clear();
		cell<<states[0][i].en<<" MeV";
		exEntry[i]->SetText(cell.str().c_str());
		fpmEntry[i]->Resize(100,0);
		calxfpEntry[i]->Resize(100,0);
		aFitEntry[i]->Resize(100,0);
		calfpmEntry[i]->Resize(100,0);
		difEntry[i]->Resize(100,0);
	}
	
	mainWindow->MapSubwindows();
	mainWindow->MapWindow();
	mainWindow->Layout();

}

/*
TGVerticalFrame* stateVert[9];//organizers that hold the cells
TGTextEntry** exEntry;//excitation energy holders
TGTextEntry** fpmEntry;//calculated focal plane momenta
TGTextEntry** calxfpEntry;//xfp from calculated focal plane momentum
TGTextEntry** aFitEntry;//assigned fit centroids
TGTextEntry** calfpmEntry;//focal plain momenta from calibration
TGTextEntry** difEntry;//difference between assigned fit centroid and calculated centroid
TGComboBox** fitBox;//combo boxes for holding unassigned fits
TGTextButton** assignButtons;//buttons to assign a selected fit to a state
TGTextButton** unAssignButtons;//buttons to unassign a fit from a state
*/

void CalWindow::updateStateInfo()
{
	ostringstream cell;
	//first iterate through the state list and put in the focal plane momenta for that run
	for(int i=0; i<numStates; ++i)
	{	
		cell.str("");
		cell.clear();
		cell<<states[dispNum][i].fpMom<<" MeV/c";
		fpmEntry[i]->SetText(cell.str().c_str());
		//cout<<i<<" | "<<states[dispNum][i].en<<" | "<<states[dispNum][i].fpMom<<endl;
	}
	/*
	StateData** states;
	FitData** fitList;
	StateFit** assigns;
	TF1** calFits;
	*/	
	//now check if there are assigned states and/or calibration function and fill the necessary cells
	for(int i=0; i<numStates; ++i)
	{
		if(assigns[dispNum][i].isSet)
		{
			float cent = fitList[dispNum][assigns[dispNum][i].ftInd].centroid;
			cell.str("");
			cell.clear();
			cell<<cent;
			aFitEntry[i]->SetText(cell.str().c_str());
			if(calFits[dispNum]!=NULL)
			{	
				invertCalFunc(i);
				float calcP = states[dispNum][i].fpMom;
				float calP = calFits[dispNum]->Eval(cent);
				cell.str("");
				cell.clear();
				cell<<calP;
				calfpmEntry[i]->SetText(cell.str().c_str());
				cell.str("");
				cell.clear();
				cell<<(calP-calcP);
				difEntry[i]->SetText(cell.str().c_str());
			}
			else
			{
				calxfpEntry[i]->SetText("N/A");
				calfpmEntry[i]->SetText("N/A");
				difEntry[i]->SetText("N/A");
			}
		}
		else
		{
			if(calFits[dispNum]!=NULL)
			{	
				invertCalFunc(i);
			}
			else
			{
				calxfpEntry[i]->SetText("N/A");
			}
			aFitEntry[i]->SetText("N/A");
			calfpmEntry[i]->SetText("N/A");
			difEntry[i]->SetText("N/A");
		}
	}
	
	mainWindow->MapSubwindows();
	mainWindow->MapWindow();
	mainWindow->Layout();
}

float CalWindow::invertCalFunc(int i)
{
	double* params = calFits[dispNum]->GetParameters();
	if(calOrd[dispNum]==1)
	{
		float b = params[0];
		float m = params[1];
		ostringstream cell;
		float fpMom = states[dispNum][i].fpMom;
		float position = (fpMom-b)/m;
		cell.str("");
		cell.clear();
		cell<<position;
		calxfpEntry[i]->SetText(cell.str().c_str());
		return position;
	}
	else
	{
		float c1 = params[0];
		float b = params[1];
		float a = params[2];
		ostringstream cell;
		float fpMom = states[dispNum][i].fpMom;
		float c = c1 - fpMom;
		float position;
		if(a<0)
		{
			position = (((-1.0)*b-TMath::Sqrt(b*b-4.0*a*c))/(2.0*a));
		}
		else
		{
			position = (((-1.0)*b+TMath::Sqrt(b*b-4.0*a*c))/(2.0*a));
		}
		if(position>2000 || position<-2000)
		{
			if(a<0)
			{
				position = (((-1.0)*b+TMath::Sqrt(b*b-4.0*a*c))/(2.0*a));
			}
			else
			{
				position = (((-1.0)*b-TMath::Sqrt(b*b-4.0*a*c))/(2.0*a));
			}
		}
		cell.str("");
		cell.clear();
		cell<<position;
		calxfpEntry[i]->SetText(cell.str().c_str());
		return position;
	}
}

void CalWindow::updateComboBoxes()
{
	//first clear the fit lists and the possible fit lists
	fitListBox->RemoveEntries(1,numStates+1);
	fitListBox->Select(0);
	for(int i=0; i<numStates; ++i)
	{
		fitBox[i]->RemoveEntries(1,numStates+1);
		fitBox[i]->Select(0);
	}
	//iterate through fits and add each one to the fit list, then check if the fit is assigned
	//if the fit is not assigned add the fit to the poss fits combo boxes
	ostringstream cell;
	for(int i=0; i<numFits[dispNum]; ++i)
	{
		cell.str("");
		cell.clear();
		cell<<"Cent: "<<fitList[dispNum][i].centroid;
		string temp = cell.str();
		fitListBox->AddEntry(temp.c_str(),i+1);
		if(!(fitList[dispNum][i].isAssign))
		{
			for(int j=0; j<numStates; ++j)
			{
				fitBox[j]->AddEntry(temp.c_str(),i+1);
			}
		}
	}
}

void CalWindow::assignFitToState(int stNum)
{
	//first get the state to be assigned from the combo box
	int id = fitBox[stNum]->GetSelected();
	if(id==0)
	{
		logStrm<<"No Fit Selected To Assign";
		pushToLog();
		return;
	}
	//check if this state already has an assigned fit
	if(assigns[dispNum][stNum].isSet)
	{
		logStrm<<"State already has an assigned fit, unassign it first"<<endl;
		pushToLog();
		return;
	}
	int ind = (id-1);
	//otherwise, set the assignment
	assigns[dispNum][stNum].isSet = true;
	assigns[dispNum][stNum].ftInd = ind;
	fitList[dispNum][ind].isAssign = true;
	++numAssigned[dispNum];
	updateStateInfo();
	updateComboBoxes();
}

void CalWindow::unAssignFitToState(int stNum)
{
	//check if this state does not have an assigned fit
	if(!(assigns[dispNum][stNum].isSet))
	{
		logStrm<<"State does not have an assigned fit to unassign"<<endl;
		pushToLog();
		return;
	}
	
	assigns[dispNum][stNum].isSet = false;
	fitList[dispNum][assigns[dispNum][stNum].ftInd].isAssign = false;
	--numAssigned[dispNum];
	updateStateInfo();
	updateComboBoxes();
}

bool CalWindow::getCalibration()
{
	//get the order of polynomial to be used
	int calPolOrd = calOrderGetter->GetIntNumber();
	if( numAssigned[dispNum] < (calPolOrd+1) )
	{
		logStrm<<"Not enough peaks assigned for calibration of this order"<<endl;
		pushToLog();
		return false;
	}
	//if we can go ahead, first we construct a TGraph
	double* xVals = new double[numAssigned[dispNum]];
	double* yVals = new double[numAssigned[dispNum]];
	double* xErrs = new double[numAssigned[dispNum]];
	double* yErrs = new double[numAssigned[dispNum]];
	int ind =0;
	for(int i=0; i<numStates; ++i)
	{
		if(assigns[dispNum][i].isSet)
		{
			yVals[ind] = states[dispNum][i].fpMom;
			xVals[ind] = fitList[dispNum][assigns[dispNum][i].ftInd].centroid;
			xErrs[ind] = fitList[dispNum][assigns[dispNum][i].ftInd].width;
			yErrs[ind] = (0.005*states[dispNum][i].fpMom);
			++ind;
		}
	}
	//TGraph* temp = new TGraph( numAssigned[dispNum], xVals, yVals);
	calGraph = new TGraphErrors(numAssigned[dispNum], xVals, yVals, xErrs, yErrs);
	//now check if there is a fit function present if so, delete it
	if(calFits[dispNum]!=NULL)
	{
		delete calFits[dispNum];
		calFits[dispNum] = NULL;
	}
	
	//now create a new calibration function
	ostringstream namer;
	namer<<"run"<<runs[dispNum].runNumber<<"calfunc";
	calOrd[dispNum] = calPolOrd;
	calFits[dispNum] = new TF1(namer.str().c_str(),calFuncs[calPolOrd-1],-700,700);
	
	calGraph->Fit(calFits[dispNum], "QFN");
	calFits[dispNum]->Print();
	updateStateInfo();
		
	delete[] xVals;
	delete[] yVals;
	delete[] xErrs;
	delete[] yErrs;
	return true;
}

void CalWindow::showCalibration()
{
	bool proceed = getCalibration();
	if (!proceed)
	{
		return;
	}
	whiteBoard->Clear();
	gPad->SetLogy(0);
	ostringstream namer;
	namer<<"Calibration Points, Run "<<runs[dispNum].runNumber;
	calGraph->SetNameTitle("calGraph",namer.str().c_str());
	calGraph->Draw();
	calFits[dispNum]->Draw("same");
	whiteBoard->Update();
}

void CalWindow::hideCalibration()
{
	if(!checkUpToStates())
	{return;}
	gPad->SetLogy(1);
	displaySubSpec(Normal);
}

void CalWindow::exportCalibrations()
{
	//first check that all runs have calibrations
	/*for(int i=0; i < numRuns; ++i)
	{
		if(calFits[i]==NULL)
		{
			logStrm<<"Not all runs have calibrations"<<endl;
			pushToLog();
			return;
		}
	}*/
	logStrm<<"\nGive the file name to save this calibration data to";
	pushToLog();
	static TString directory(".");
	TGFileInfo fileInfo;
	fileInfo.SetMultipleSelection(false);
	fileInfo.fFileTypes = csvDataType;
	fileInfo.fIniDir = StrDup(directory);
	
	//make the open file dialog
	//quite frankly this creeps me the hell out, just creating an object like this
	//but apparently the parent object will delete it on its own
	new TGFileDialog(gClient->GetRoot(), mainWindow, kFDSave, &fileInfo);
	if(TString(fileInfo.fFilename).IsNull())
	{
		return;
	}
	//make sure the file name ends with the extension
	string temp = string(fileInfo.fFilename);
	if( (temp.size()-4) != ( temp.rfind(".csv") ) )
	{
		temp.append(".csv");
	}
	directory = fileInfo.fIniDir;
	
	//now open the file
	ofstream output;
	output.open(temp.c_str());
	
	//for( int i=0; i < numRuns; ++i)
	for( int i=0; i < numRuns; ++i)
	{
		if(calFits[i]!=NULL)
		{
			double* params = calFits[i]->GetParameters();
			output<<runs[i].runNumber<<","<<params[0]<<","<<params[1];
			if(calOrd[i]==2)
			{
				output<<","<<params[2];
			}
			else
			{
				output<<",0.0";
			}
			output<<endl;
		}
	}
	output.close();
}

/******************************************
*******************************************
** Fit operations
*******************************************
******************************************/
void CalWindow::setFitFunc()
{
	if(!checkUpToStates())
	{return;}
	//get the polynomial order
	currBGOrder = bgPolOrderGetter->GetIntNumber();
	numPeaks = numPeaksGetter->GetIntNumber();
	//calculate the indices into the grand array of functions
	int bgIndex = currBGOrder+1;
	int peakIndex = numPeaks-1;
	fitDiag->SetFunction(peakFitFormulas[peakIndex][bgIndex]);	
}

void CalWindow::getFitData()
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

void CalWindow::doQuickFit()
{
	if(!checkUpToStates())
	{return;}
	//get the polynomial order
	currBGOrder = bgPolOrderGetter->GetIntNumber();
	numPeaks = numPeaksGetter->GetIntNumber();
	//calculate the indices into the grand array of functions
	int bgIndex = currBGOrder+1;
	int peakIndex = numPeaks-1;
	//set the fit panel fit function while we are at it for the heck of it
	fitDiag->SetFunction(peakFitFormulas[peakIndex][bgIndex]);
	if(!quickFitFirstTime)
	{
		// tell the user what they need to do
		logStrm<<"\n    You need to click at the one bound of the fit region";
		pushToLog();
		logStrm<<"    Then the center of each peak in the region";
		pushToLog();
		logStrm<<"    (one center per peak in the num peaks number getter)";
		pushToLog();
		logStrm<<"    Finally you double click at the other bound of the fit region";
		pushToLog();
		quickFitFirstTime=true;
	}
	//whiteBoard->GetCanvas()->SetCrosshair(1);
	TGraph* fitData = (TGraph*)whiteBoard->WaitPrimitive("Graph","PolyLine");
	int numGraphPoints = fitData->GetN();
	if( numGraphPoints < (numPeaks+2) )
	{
		logStrm<<"Not enough points, you need to give numPeaks+2 ("<<numPeaks+2<<") Points, see instructions above\n";
		logStrm<<"Click the button to try again";
		pushToLog();
		delete fitData;
		return;
	}
	else if(numGraphPoints > (numPeaks+2) )
	{
		logStrm<<"Too many points, you need to give numPeaks+2 ("<<numPeaks+2<<") Points, see instructions above\n";
		logStrm<<"Click the button to try again";
		pushToLog();
		delete fitData;
		return;
	}
	double* positions = fitData->GetX();
	//transfer the quickFitPos array so we can sort it
	for(int i=0; i<numGraphPoints; ++i)
	{
		quickFitPos[i]=positions[i];
	}
	//now cleanup the fit data
	delete fitData;
	//sort the positions
	sortDoubles(quickFitPos,numGraphPoints);
	//fill the tempHistVals array with the histogram values at the clicked positions
	for(int i=0; i<numGraphPoints; ++i)
	{
		int binNum = cutSpec->GetXaxis()->FindBin(quickFitPos[i]);
		tempHistVals[i]=cutSpec->GetBinContent(binNum);
	}
	//calculate the estimated params using the array values
	int last = numGraphPoints-1;
	//first the straight line background
	double slope = ( (tempHistVals[last]-tempHistVals[0])/(quickFitPos[last]-quickFitPos[0]) );
	double intercept = ( tempHistVals[0]-(quickFitPos[0]*slope) );
	//or constant background
	double avgOffset = ( (tempHistVals[last]+tempHistVals[0])/2 );
	//now load the tempParams array with height, position, and width of every peak 
	for(int i=0; i<numPeaks; ++i)
	{
		double height;
		double position;
		double width;
		//first calculate the height guess for peak i, this is dependent on the background option
		switch(currBGOrder)
		{
		case -1:
			//no background
			height = tempHistVals[i+1];
			break;
		case 0:
			//constant background
			height = ( tempHistVals[i+1]-avgOffset );
			break;
		default:
			//linear or quadratic background (since I do not estimate the quadratic term)
			height = ( tempHistVals[i+1]- ( slope*quickFitPos[i+1]+intercept ) );
			break;
		}
		//guess the width by looking at the difference between the current peak and the points next to it
		double diff1 = ((quickFitPos[i+2]-quickFitPos[i+1])/2);
		double diff2 = ((quickFitPos[i+1]-quickFitPos[i+0])/2);
		double diff = (diff1<diff2)?diff1:diff2;
		width = diff/4.5; //the divider was pulled from the air testing shows it seems to work all right
		//guess the position via what the user entered
		position=quickFitPos[i+1];
		//load the temp param array
		tempParams[3*i] = height;
		tempParams[3*i+1] = position;
		tempParams[3*i+2] = width;
	}
	//temp params will always have space for quadratic background so simply put all terms
	if(currBGOrder==0)
	{
		tempParams[3*numPeaks]=avgOffset;
	}
	else
	{
		tempParams[3*numPeaks]=intercept;
	}
	tempParams[3*numPeaks+1]=slope;
	tempParams[3*numPeaks+2]=0.0;
	//whiteBoard->GetCanvas()->SetCrosshair(0);
	//make the TF1 to fit
	TF1* tempQuickFit = new TF1("tempQuickFit",peakFitFormulas[peakIndex][bgIndex],quickFitPos[0],quickFitPos[last]);
	tempQuickFit->SetParameters(tempParams);
	//fit the histogram
	cutSpec->Fit(tempQuickFit,"RI");
	whiteBoard->Update();
	//dump the peak centroids into the temporary fit holding area
	//as if the get fit data button was clicked when using the fit panel
	//read the fit data
	double* values = tempQuickFit->GetParameters();
	//clear the temp fit combobox
	tempFitBox->RemoveEntries(1,numStates+10);
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
	//cleanup
	delete tempQuickFit;
	//make the fit dialog point at the histogram again
	if(fitDiag!=NULL)
	{
		fitDiag->SetFitObject(dynamic_cast<TVirtualPad*>(whiteBoard->GetCanvas()), dynamic_cast<TObject*>(cutSpec), kButton1Down);
		setFitFunc();	
	}
}

void CalWindow::transferFit()
{
	if(!checkUpToStates())
	{return;}
	int id = tempFitBox->GetSelected();
	if(id==0)
	{
		logStrm<<"No Fit Selected";
		pushToLog();
		return;
	}
	if(numFits[dispNum]==numStates)
	{
		cout<<"Too Many fits cannot add more until one is removed"<<endl;
		return;
	}
	//other wise, add the fit to the various combo boxes, then update them
	tempFitBox->Select(0);
	tempFitBox->RemoveEntry(id);
	fitList[dispNum][numFits[dispNum]] = tempFits[id-1];
	++numFits[dispNum];
	updateComboBoxes();
}

void CalWindow::transferAllFits()
{
	if(!checkUpToStates())
	{return;}
	if((numFits[dispNum]+numPeaks)>=numStates)
	{
		cout<<"Too Many fits cannot add more until one is removed"<<endl;
		return;
	}
	if(numPeaks==0)
	{
		logStrm<<"No peaks to transfer";
		pushToLog();
		return;
	}
	for(int i=0; i<numPeaks; ++i)
	{
		fitList[dispNum][numFits[dispNum]] = tempFits[i];
		++numFits[dispNum];
	}
	tempFitBox->Select(0);
	tempFitBox->RemoveEntries(1,numStates+10);
	updateComboBoxes();
}

void CalWindow::removeFit()
{
	if(!checkUpToStates())
	{return;}
	int id = fitListBox->GetSelected();
	if(id==0)
	{
		logStrm<<"No Fit Selected";
		pushToLog();
		return;
	}
	
	int ind = id-1;
	
	if(fitList[dispNum][ind].isAssign)
	{
		logStrm<<"Cannot Delete an Assigned Fit"<<endl;
		pushToLog();
		return;
	}
	//other wise, remove the fit from the list, then update the combo boxes
	--numFits[dispNum];
	for(int i = ind; i<numFits[dispNum]; ++i)
	{
		fitList[dispNum][i] = fitList[dispNum][i+1];
	}
	fitList[dispNum][numFits[dispNum]].isAssign = false;
	updateComboBoxes();
}

/******************************************
*******************************************
** Overall Control operations
*******************************************
******************************************/
void CalWindow::clearLog()
{
	messageLog->Clear();
	messageLog->LoadBuffer("Message Log Cleared");
	messageLog->Update();
	messageLog->AddLine("");
	messageLog->ShowBottom();
}

void CalWindow::resetToStart()
{
	clearLog();
	
	if ( runs == NULL && mainFile == NULL && auxFile==NULL && frFile==NULL)
	{
		logStrm<<"\nNothing To Reset";
		pushToLog();
		return;
	}
	
	if(runs != NULL)
	{
		delete[] runs;
		runs = NULL;
		numRuns=0;
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
	if(interp != NULL)
	{
		delete interp;
		interp = NULL;
	}
	if(states != NULL)
	{
		delete[] states;
		states = NULL;
		numStates=0;
	}
	clearLog();
	dispNum = 0;
	logStrm<<"Reset to initial state";
	pushToLog();
	
}

void CalWindow::DoClose()
{
	delete this;
}

void CalWindow::exitApp()
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
void CalWindow::displaySubSpec(const UpdateCallType& tp)
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
	subSpec = testSubSpec( runN );
	if(subSpec == NULL)
	{
		logStrm<<"Lacking a sub spectrum, something is wrong, exitting";
		pushToLog();
		gApplication->Terminate(0);
		return;
	}
	int minBin = subSpec->GetYaxis()->FindBin(-0.10);
	int maxBin = subSpec->GetYaxis()->FindBin(0.10);
	
	cutSpec = subSpec->ProjectionX("angleCutProjection",minBin, maxBin);
	cutSpec->Draw();
	sclToggle=true;
	gPad->SetLogy(1);
	
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



void CalWindow::prevSeqSpec()
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
		displaySubSpec(Normal);
		return;
	}
}

void CalWindow::nextSeqSpec()
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
		displaySubSpec(Normal);
		return;
	}
}

void CalWindow::toggleSpec()
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

void CalWindow::doScatteringCalcs()
{
	double amuToMev = 931.4948236727373;
	for(int j=0; j<numRuns; ++j)
	{
		//first get the angle
		double th = runs[j].angle*3.1415926/180.0;
		//and the beam energy
		double bE = runs[j].beamEn;
		//and the target half thickness
		double ht = runs[j].thickness/2.0;
		//and the projectile's mass (converted to MeV/c^2)
		double pM = runs[j].projMass*amuToMev;
		//and the target mass (converted to MeV/c^2)
		double tM = runs[j].targetMass*amuToMev;
	
		//get the projectile energy on scattering
		double pKE = interp->getVal(ht, bE);
		//now calculate the total projectile energy
		double pE = pKE + pM;
		//now calculate the projectile momentum*c
		double pP = TMath::Sqrt( pE*pE - pM*pM );
		//calculate the constant term of the formula to be solved
		double cTerm = (2*pM*pM + tM*tM + 2*tM*pE);
		//now calculate the cosine of the angle
		double cosTh = TMath::Cos(th);
		double ppctSqr = pP*cosTh*pP*cosTh;
		//now calculate the thickness traversed
		double trav = (ht/cosTh);
		//the solution is obtained via the quadratic equation thus the numerator and denomenator logic coming up
		double denom = 8*(pE*pE + 2*pE*tM + tM*tM - ppctSqr);
		logStrm<<"Run: "<<runs[j].runNumber<<"  Angle: "<<(th*180.0/3.1415926)<<" deg   Degraded Beam Energy: "<<pKE<<"\n";
		pushToLog();
		//logStrm<<"Excitation En (MeV)| Scattering KE | Focal Plane KE | Focal Plane Momentum";
		//pushToLog();
		for(int i=0; i<numStates; ++i)
		{
			//get the residual 'mass'
			double rM = tM + states[j][i].en;
			//cout<<pM<<"  "<<pE<<"  "<<bE<<"  "<<pKE<<"  "<<tM<<"  "<<rM<<endl;
			//first calculate a repeated term
			double exTerm = (rM*rM - cTerm);
			//cout<<exTerm<<endl;
			//now start calculating the parts of the sqrt in teh quadratic equation
			double sqrtTerm1 = (exTerm*exTerm*16*(pE+tM)*(pE+tM));
			double sqrtTerm2 = (exTerm*exTerm+4*pM*pM*ppctSqr);
			double sqrtTerm3 = (4*pE*pE+8*pE*tM+4*tM*tM-4*ppctSqr);
			//now calculate teh components of the numerator
			double numTerm1 = 4*(pE+tM)*exTerm;
			//cout<<numTerm1<<endl;
			double numTerm2 = TMath::Sqrt(sqrtTerm1-(4*sqrtTerm2*sqrtTerm3));
			//cout<<numTerm2<<endl;
			//calculate the numerator
			double numer = (numTerm2-numTerm1);
			//cout<<numer<<endl;
			//cout<<denom<<endl;
			//cout<<(numer/denom)<<endl;
			double scatKEn = ((numer/denom) - pM);
			//caclulate the scattered kinetic energy
			states[j][i].scatEn = scatKEn;
	
			//calculate the energy after passing through the foil
			double fpKE = interp->getVal(trav, scatKEn);
			//now translate that into a total energy
			double fpEn = fpKE + pM;
			//now into a momentum
			double fpMom = TMath::Sqrt(fpEn*fpEn-pM*pM);
			states[j][i].fpMom = fpMom;
			//logStrm<<(states[j][i].en)<<" | "<<scatKEn<<" | "<<fpKE<<" | "<<fpMom<<" | "<<states[j][i].fpMom;
			//pushToLog();
		}
	}
}

void CalWindow::pushToLog()
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

string CalWindow::makeTreeName(int runN)
{
	ostringstream treeNamer;
	treeNamer<<"run"<<runN<<"Tree";
	string temp = treeNamer.str();
	return temp;
}

TTree* CalWindow::retrieveTree(int runN)
{
	string treeName = makeTreeName(runN);
	return reinterpret_cast<TTree*>(mainFile->Get(treeName.c_str()));
}

TTree* CalWindow::testTree(int runN)
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


string CalWindow::makePIDCutName(int runN)
{
	ostringstream cutNamer;
	cutNamer<<"pidCut"<<runN;
	string temp = cutNamer.str();
	return temp;
}

TCutG* CalWindow::retrievePIDCut(int runN)
{
	string cName = makePIDCutName(runN);
	return reinterpret_cast<TCutG*>(auxFile->Get(cName.c_str()));
}

TCutG* CalWindow::testPIDCut(int runN)
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


string CalWindow::makePIDSpecName(int runN)
{
	ostringstream histNamer;
	histNamer<<"h2PIDRun"<<runN;
	string temp = histNamer.str();
	return temp;
}

TH2F* CalWindow::retrievePIDSpec(int runN)
{
	string histName = makePIDSpecName(runN);
	return reinterpret_cast<TH2F*>(auxFile->Get(histName.c_str()));
}

TH2F* CalWindow::testPIDSpec(int runN)
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

string CalWindow::makeBaseCutName(int runN)
{
	ostringstream cutNamer;
	cutNamer<<"baseCut"<<runN;
	string temp = cutNamer.str();
	return temp;
}

TCut* CalWindow::retrieveBaseCut(int runN)
{
	string cName = makeBaseCutName(runN);
	return reinterpret_cast<TCut*>(auxFile->Get(cName.c_str()));
}

TCut* CalWindow::testBaseCut(int runN)
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

string CalWindow::makeBGGraphName(int runN)
{
	ostringstream graphNamer;
	graphNamer<<"bgGraph"<<runN;
	string temp = graphNamer.str();
	return temp;
}

TGraph* CalWindow::retrieveBGGraph(int runN)
{
	string gName = makeBGGraphName(runN);
	return reinterpret_cast<TGraph*>(auxFile->Get(gName.c_str()));
}

TGraph* CalWindow::testBGGraph(int runN)
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

string CalWindow::makeBGCutName(int runN)
{
	ostringstream cutNamer;
	cutNamer<<"bgCut"<<runN;
	string temp = cutNamer.str();
	return temp;
}

TCut* CalWindow::retrieveBGCut(int runN)
{
	string cName = makeBGCutName(runN);
	return reinterpret_cast<TCut*>(auxFile->Get(cName.c_str()));
}

TCut* CalWindow::testBGCut(int runN)
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

string CalWindow::makeTRCutName(int runN)
{
	ostringstream cutNamer;
	cutNamer<<"trCut"<<runN;
	string temp = cutNamer.str();
	return temp;
}

TCut* CalWindow::retrieveTRCut(int runN)
{
	string cName = makeTRCutName(runN);
	return reinterpret_cast<TCut*>(auxFile->Get(cName.c_str()));
}

TCut* CalWindow::testTRCut(int runN)
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

string CalWindow::makeBGSpecName(int runN)
{
	ostringstream bgNamer;
	bgNamer<<"h1YFPRun"<<runN;
	string temp = bgNamer.str();
	return temp;
}

TH1F* CalWindow::retrieveBGSpec(int runN)
{
	string histName = makeBGSpecName(runN);
	return reinterpret_cast<TH1F*>(auxFile->Get(histName.c_str()));
}

TH1F* CalWindow::testBGSpec(int runN)
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

string CalWindow::makeOrigShapeSpecName(int runN)
{
	ostringstream bgNamer;
	bgNamer<<"h2OrigShapeSpec"<<runN;
	string temp = bgNamer.str();
	return temp;
}

TH2F* CalWindow::retrieveOrigShapeSpec(int runN)
{
	string histName = makeOrigShapeSpecName(runN);
	return reinterpret_cast<TH2F*>(auxFile->Get(histName.c_str()));
}

TH2F* CalWindow::testOrigShapeSpec(int runN)
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

string CalWindow::makeFirstOrderShapeSpecName(int runN)
{
	ostringstream bgNamer;
	bgNamer<<"h2FirstOrderShapeSpec"<<runN;
	string temp = bgNamer.str();
	return temp;
}

TH2F* CalWindow::retrieveFirstOrderShapeSpec(int runN)
{
	string histName = makeFirstOrderShapeSpecName(runN);
	return reinterpret_cast<TH2F*>(auxFile->Get(histName.c_str()));
}

TH2F* CalWindow::testFirstOrderShapeSpec(int runN)
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

string CalWindow::makeFirstOrderShapeTreeName(int runN)
{
	ostringstream bgNamer;
	bgNamer<<"run"<<runN<<"_tree_shape1";
	string temp = bgNamer.str();
	return temp;
}

TTree* CalWindow::retrieveFirstOrderShapeTree(int runN)
{
	string histName = makeFirstOrderShapeTreeName(runN);
	return reinterpret_cast<TTree*>(frFile->Get(histName.c_str()));
}

TTree* CalWindow::testFirstOrderShapeTree(int runN)
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

string CalWindow::makeTRpBGSpecName(int runN)
{
	ostringstream bgNamer;
	bgNamer<<"h2TrpBG"<<runN;
	string temp = bgNamer.str();
	return temp;
}

TH2F* CalWindow::retrieveTRpBGSpec(int runN)
{
	string histName = makeTRpBGSpecName(runN);
	return reinterpret_cast<TH2F*>(auxFile->Get(histName.c_str()));
}

TH2F* CalWindow::testTRpBGSpec(int runN)
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

string CalWindow::makeBGOnlySpecName(int runN)
{
	ostringstream bgNamer;
	bgNamer<<"h2BgOnly"<<runN;
	string temp = bgNamer.str();
	return temp;
}

TH2F* CalWindow::retrieveBGOnlySpec(int runN)
{
	string histName = makeBGOnlySpecName(runN);
	return reinterpret_cast<TH2F*>(auxFile->Get(histName.c_str()));
}

TH2F* CalWindow::testBGOnlySpec(int runN)
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

string CalWindow::makeScaledBGSpecName(int runN)
{
	ostringstream bgNamer;
	bgNamer<<"h2ScaledBg"<<runN;
	string temp = bgNamer.str();
	return temp;
}

TH2F* CalWindow::retrieveScaledBGSpec(int runN)
{
	string histName = makeScaledBGSpecName(runN);
	return reinterpret_cast<TH2F*>(auxFile->Get(histName.c_str()));
}

TH2F* CalWindow::testScaledBGSpec(int runN)
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

string CalWindow::makeSubSpecName(int runN)
{
	ostringstream bgNamer;
	bgNamer<<"h2SubSpec"<<runN;
	string temp = bgNamer.str();
	return temp;
}

TH2F* CalWindow::retrieveSubSpec(int runN)
{
	string histName = makeSubSpecName(runN);
	return reinterpret_cast<TH2F*>(auxFile->Get(histName.c_str()));
}

TH2F* CalWindow::testSubSpec(int runN)
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

bool CalWindow::checkUpToRunData()
{
	if ( runs == NULL )
	{
		logStrm<<"\nLoad Run Data first";
		pushToLog();
		return false;
	}
	return true;
}

bool CalWindow::checkUpToMainFile()
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

bool CalWindow::checkUpToAuxFile()
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

bool CalWindow::checkUpToFrFile()
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

bool CalWindow::checkUpToInterpFile()
{
	if( !checkUpToFrFile() )
	{
		return false;
	}
	else if( interp == NULL )
	{
		logStrm<<"\nLoad an interp file first";
		pushToLog();
		return false;
	}
	return true;
}

bool CalWindow::checkUpToStates()
{
	if( !checkUpToInterpFile() )
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

void CalWindow::parseRunFileLine(const string& line, RunData& tempData)
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
