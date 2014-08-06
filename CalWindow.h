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
#include<TGraph.h>
#include<TStyle.h>
#include<TColor.h>
#include<TPaveText.h>
#include<TSystem.h>
#include<TFitResult.h>
#include<TMath.h>

//my includes
#include"bicubicinterp.h"
#include"guiSupport.h"

enum UpdateCallType{ Initial, Normal, Final};
TCut RayCut;

class CalWindow
{
	RQ_OBJECT("CalibrationWindow")
public:
	CalWindow(const TGWindow* parent, UInt_t width, UInt_t height);
	~CalWindow();
	
	//sequential spectrum display
	void prevSeqSpec();
	void nextSeqSpec();
	void endSeqSpec();
	
	//file operations
	void readRunData();
	void openBigFile();
	void openAuxFile();
	void openFriendFile();
	void openInterpFile();
	void readStateData();
	
	//overall control operations
	void makeTreeFileActive(){mainFile->cd();}
	void makeAuxFileActive(){auxFile->cd();}
	void makeFriendFileActive(){frFile->cd();}
	void clearLog();
	void resetToStart();
	void exitApp();

	void doClose();

private:
		//some private helper functions
	void parseRunFileLine(const string& line, RunData& tempData);
	void pushToLog();
	bool checkUpToRunData();
	bool checkUpToMainFile();
	bool checkUpToAuxFile();
	bool checkUpToFrFile();
	bool checkUpToInterpFile();
	void doScatteringCalcs();
	double calculateBGNorm(int runN);
	
	double calcCrossSection(double counts, const RunData& data, double thWidth, double phWidth);
	
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
	void updateDisplay(const UpdateCallType& tp);
	void updatePIDDisp(const UpdateCallType& tp);
	void updateBGDisp(const UpdateCallType& tp);
	void updateSubDisp(const UpdateCallType& tp);
	
	//analysis stuff
	RunData* runs;
	StateData* states;
	int numRuns;
	int numStates;
	TFile* mainFile;
	TFile* auxFile;
	TFile* frFile;
	BiCubicInterpolation* interp;

	//sequential display stuff
	int dispNum;

	/************************************************
	**GUI Declarations
	************************************************/

	//Main Gui stuff
	TSystem *sys;
	TGMainFrame *mainWindow;
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
	TGHorizontalFrame* fileActFrame;//holds the buttons for activating files
	TGHorizontalFrame* overallControlFrame;//holds the overall control buttons
	TGVerticalFrame* messageLogFrame;//frame that only holds the message log, necessary for the resizing trick
	
	//the message container for the window to try and make things so we do not need the terminal at all.
	TGTextView* messageLog;
	
	//the splitters allowing resizing of the canvas and message box
	TGHSplitter* split;
	
	//labels to store text
	TGLabel* fileLabel;
	TGLabel* fileControlLabel;
	TGLabel* controlLabel;
	TGLabel* sDLabel;
	TGLabel* cutLabel;
	TGLabel* shapeLabel;
	TGLabel* csLabel;
	
	//action buttons
	TGTextButton *startCal;
	//sequential display buttons
	TGTextButton *prevSpec;
	TGTextButton *nextSpec;
	
	//file ops buttons
	TGTextButton *rdRunData;
	TGTextButton *opBigFile;
	TGTextButton *opAuxFile;
	TGTextButton *opFrFile;
	TGTextButton *opInterpFile;
	TGTextButton *rdStateData;
	
	//file activation buttons
	TGTextButton *treeFocusBut;
	TGTextButton *auxFocusBut;
	TGTextButton *frFocusBut;
	
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

	//sys = new TUnixSystem();
	sys = gSystem;
	
	RayCut.SetTitle("Rayid==0");
	
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
	fileActFrame = new TGHorizontalFrame(bottomBarFrame,width,height);
	overallControlFrame = new TGHorizontalFrame(bottomBarFrame,width,height);
	messageLogFrame  = new TGVerticalFrame(organizerFrame,width,height, kChildFrame | kFixedHeight);
	
	//create and connect the buttons in the side bar
	
	
	
	//add the sidebar buttons to the sidebar frame

	
	/******************************************
	** Sequential Display buttons
	******************************************/
	sDLabel = new TGLabel(sideBarFrame, "Sequential Display Buttons");
	sideBarFrame->AddFrame(sDLabel, new TGLayoutHints(kLHintsExpandX,2,2,2,2));
	//create and connect the sequential display buttons
	//show previous spectrum
	prevSpec = new TGTextButton(sideBarFrame,"Prev Run");
	prevSpec->Connect("Clicked()","CalWindow",this,"prevSeqSpec()");
	//show next spectrum
	nextSpec = new TGTextButton(sideBarFrame,"Next Run");
	nextSpec->Connect("Clicked()","CalWindow",this,"nextSeqSpec()");
	
	//add the sequential display buttons to their frame
	sideBarFrame->AddFrame(prevSpec, new TGLayoutHints(kLHintsExpandX,2,2,2,2));
	sideBarFrame->AddFrame(nextSpec, new TGLayoutHints(kLHintsExpandX,2,2,2,2));
	
	//add the sidebar to the canvas frame
	canvasFrame->AddFrame(sideBarFrame,  new TGLayoutHints(kLHintsTop | kLHintsLeft | kLHintsExpandY, 2,2,2,2) );
	
	/******************************************
	** Canvas Creation
	******************************************/
	//create the embedded canvas
	canvas = new TRootEmbeddedCanvas("canvas",canvasFrame, 200, 200);
	whiteBoard = canvas->GetCanvas();
	whiteBoard->SetTitle("Whiteboard For Analysis");
	gStyle->SetOptStat(0);
	//add the canvas to its frame
	canvasFrame->AddFrame(canvas, new TGLayoutHints(kLHintsExpandX | kLHintsExpandY , 2,2,2,2) );
	
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
	** File Control buttons
	******************************************/
	fileControlLabel = new TGLabel(fileActFrame, "File Activation: ");
	fileActFrame->AddFrame(fileControlLabel, new TGLayoutHints(kLHintsLeft | kLHintsCenterY,2,2,2,2));
	//create and connect the file activation buttons
	//make tree file the current dir
	treeFocusBut = new TGTextButton(fileActFrame,"Tree File");
	treeFocusBut->Connect("Clicked()","CalWindow",this,"makeTreeFileActive()");
	//make aux file the current dir
	auxFocusBut = new TGTextButton(fileActFrame,"Aux File");
	auxFocusBut->Connect("Clicked()","CalWindow",this,"makeAuxFileActive()");
	//make friend file the current dir
	frFocusBut = new TGTextButton(fileActFrame,"Friend File");
	frFocusBut->Connect("Clicked()","CalWindow",this,"makeFriendFileActive()");
	
	//add the file activation buttons to their frame
	fileActFrame->AddFrame(treeFocusBut, new TGLayoutHints(kLHintsExpandX,3,3,2,2));
	fileActFrame->AddFrame(auxFocusBut, new TGLayoutHints(kLHintsExpandX,3,3,2,2));
	fileActFrame->AddFrame(frFocusBut, new TGLayoutHints(kLHintsExpandX,3,3,2,2));
	
	//add the file activation frame to its frame
	bottomBarFrame->AddFrame(fileActFrame, new TGLayoutHints(kLHintsExpandX | kLHintsLeft, 2,2,2,2) );
	
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
	mainWindow->SetWindowName("Analysis GUI");
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
	if (states != NULL)
	{
		logStrm<<"\nState data already loaded, to load new run data, use the reset button";
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
		if(line.length()>2)//to handle blank lines at the end of a file
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
	states = new StateData[numStates];
	//now read line by line to get the run data
	getline(input, line);
	int count = 0;
	while(!input.eof() && count<numStates)
	{
		istringstream conv;
		conv.str(line);
		states[count].index = count;
		conv>>(states[count].en);
		states[count].assignedFit = NULL;
		getline(input, line);
		++count;
	}
	logStrm<<"\nState data has been loaded";
	pushToLog();
	displaySubSpec(Initial);
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
	}
	
	clearLog();
	
	dispNum = 0;
	dispFunc = None;
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
	static TH2F* subSpec;
	static TH1D* cutSpec;
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
	int minBin = subSpec->GetXaxis()->FindBin(-0.25);
	int maxBin = subSpec->GetXaxis()->FindBin(0.25);
	cutSpec = subSpec->ProjectionX("angleCutProjection",minBin, maxBin);
	cutSpec->Draw();
	gPad->SetLogy(1);
	whiteBoard->Update();
	doScatteringCalcs();
}



void CalWindow::prevSeqSpec()
{
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


/******************************************
*******************************************
** Private helper functions
*******************************************
******************************************/

void CalWindow::doScatteringCalcs()
{
	double amuToMev = 931.4948236727373;
	//first get the angle
	double th = runs[dispNum].angle*3.1415926/180.0;
	//and the beam energy
	double bE = runs[dispNum].beamEn;
	//and the target half thickness
	double ht = runs[dispNum].thickness/2.0;
	//and the projectile's mass (converted to MeV/c^2)
	double pM = runs[dispNum].projMass*amuToMev;
	//and the target mass (converted to MeV/c^2)
	double tM = runs[dispNum].targetMass*amuToMev;
	
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
	
	logStrm<<"Run: "<<runs[dispNum].runNumber<<"  Angle: "<<(th*180.0/3.1415926)<<" deg   Degraded Beam Energy: "<<pKE;
	pushToLog();
	logStrm<<"Excitation En (MeV)| Scattering KE | Focal Plane KE | Focal Plane Momentum";
	pushToLog();
	for(int i=0; i<numStates; ++i)
	{
		//get the residual 'mass'
		double rM = tM + (states[i].en/1000.0);
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
		states[i].scatEn = scatKEn;
		
		//calculate the energy after passing through the foil
		double fpKE = interp->getVal(trav, scatKEn);
		//now translate that into a total energy
		double fpEn = fpKE + pM;
		//now into a momentum
		double fpMom = TMath::Sqrt(fpEn*fpEn-pM*pM);
		states[i].fpMom = fpMom;
		logStrm<<(states[i].en/1000.0)<<" | "<<scatKEn<<" | "<<fpKE<<" | "<<fpMom;
		pushToLog();
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

double CalWindow::calculateBGNorm(TGraph* regions)
{
	//first get the x values of the graph
	double* xVals = regions->GetX();
	double bgWidth = ( (xVals[1]-xVals[0]) + (xVals[3]-xVals[2]) );
	double trWidth = ( xVals[2]-xVals[1] );
	return (trWidth/bgWidth);
}


//returns d(sigma)/d(Omega) for that number of counts and rund data and angular region
double CalWindow::calcCrossSection(double counts, const RunData& data, double thWidth, double phWidth)
{
	const double electronCharge = 1.602176487e-19;
	//this is for the conversion from (mg/(cm^2)/AMU) --> (1/ mb)
	// mg/AMU = 6.02213665167516e20   and 1/cm^2 = 10^-27 1/mb
	const double massAndAreaConversion = 6.02213665167516e-7;
	const double nano = 0.000000001;
	const double pi = 3.141592653589793;
	const double d2rConv = pi/180.0;
	double solidAngle = (thWidth*d2rConv*(phWidth/1000.0));
	double eff = ((double)data.grAccept)/((double)data.grRequest);
	eff*= ((double)data.vdcEff);
	double incBeam = ( (((double)data.beamInt)*((double)data.biRange)*nano)/(1000.0* ((double)data.chargeState) * electronCharge) );
	double targNum = (( (((double) data.thickness) * massAndAreaConversion) )/((double)data.targetMass) );
	double denom = ( solidAngle*eff*incBeam*targNum );
	double cs = counts/denom;
	return cs;
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
