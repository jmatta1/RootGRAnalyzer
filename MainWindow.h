#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H
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
#include"guiSupport.h"
#include"BasicCSDialog.h"

enum DisplayFunction{None, PIDCut, BGCut, ShapeDisp, SubbedSpecs, BasicCSInfoSimple, BasicCSInfoPerRun};
enum UpdateCallType{ Initial, Normal, Final};

TCut RayCut;

//define the class that draws the window and handles button clicks and the like
class MainWindow
{
	RQ_OBJECT("MainWindow")
public:
	MainWindow(const TGWindow* parent, UInt_t width, UInt_t height);
	~MainWindow();
	
	//file operations
	void readRunData();
	void buildBigFile();
	void openBigFile();
	void makeAuxFile();
	void openAuxFile();
	void makeFriendFile();
	void openFriendFile();
	
	//cut operations
	void getPIDCuts();
	void showPIDCut();
	void getBGCuts();
	void showBGCut();

	//shape operations
	void getFirstOrdShapes();
	void showFirstOrdShapes();
	void makeBGSubSpecs();
	void showSubbedShapes();

	//cs operations
	void getBasicCSParamsSimple();
	void getBasicCSParamsPerRun();
	void simpleGetCS();

	//sequential spectrum display
	void prevSeqSpec();
	void nextSeqSpec();
	void endSeqSpec();

	//overall control operations
	void makeTreeFileActive(){mainFile->cd();}
	void makeAuxFileActive(){auxFile->cd();}
	void makeFriendFileActive(){frFile->cd();}
	void resetToStart();
	void exitApp();
	
private:
	//some private helper functions
	void parseRunFileLine(const string& line, RunData& tempData);
	bool checkUpToRunData();
	bool checkUpToMainFile();
	bool checkUpToAuxFile();
	bool checkUpToFrFile();
	
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
	void grabBasicCsInfoPerRun(const UpdateCallType& tp);
	void grabBasicCsInfoSimple(const UpdateCallType& tp);
	
	//analysis stuff
	RunData* runs;
	int numRuns;
	CSBounds* csBnds;
	int numBnds;
	TFile* mainFile;
	TFile* auxFile;
	TFile* frFile;

	//sequential display stuff
	int dispNum;
	DisplayFunction dispFunc;

	//Main Gui stuff
	BasicCSDialog* basicInfoGrabber;
	TSystem *sys;
	TGMainFrame *mainWindow;
	//main canvas for display
	TRootEmbeddedCanvas *canvas;//holds the canvas we use for drawing
	TCanvas *whiteBoard;//holds the pointer to the actual TCanvas in canvas
	//Frames for organization
	TGVerticalFrame* organizerFrame;//the outermost organizer of the window
	TGHorizontalFrame* cSBFrame;//holds the canvas and sidebar
	TGVerticalFrame* sideBarFrame;//holds the sidebar
	TGHorizontalFrame fileOpsRowFrame;//holds the row of buttons at the bottom for file operations
	
	
	
};

MainWindow::MainWindow(const TGWindow* parent, UInt_t width, UInt_t height)
{
	runs=NULL;
	numRuns=0;
	csBnds=NULL;
	numBnds = 0;
	mainFile=NULL;
	auxFile=NULL;
	frFile=NULL;
	dispNum = 0;
	dispFunc = None;

	//sys = new TUnixSystem();
	sys = gSystem;
	
	RayCut.SetTitle("Rayid==0");
	
	basicInfoGrabber = new BasicCSDialog(parent,width,height);
	
	//create the area for the menu items
	menu = new TGMainFrame(parent,width,height);
	//create the canvas
	whiteBoard = new TCanvas("canvas","White Board For Analysis");
	
	
	gStyle->SetOptStat(0);
	
	TGVerticalFrame *fullFrame = new TGVerticalFrame(menu, width, height);
	TGHorizontalFrame *overallFrame = new TGHorizontalFrame(fullFrame, width, height);
	
	/******************************************
	** File operations buttons
	******************************************/
	TGVerticalFrame *fileFrame = new TGVerticalFrame(overallFrame, width, height);
	TGLabel* fileLabel = new TGLabel(fileFrame, "File Operations");
	fileFrame->AddFrame(fileLabel, new TGLayoutHints(kLHintsExpandX,5,5,3,4));
	//read run data button
	TGTextButton *rdRunData = new TGTextButton(fileFrame,"Read Run Data");
	rdRunData->Connect("Clicked()","MainWindow",this,"readRunData()");
	fileFrame->AddFrame(rdRunData, new TGLayoutHints(kLHintsExpandX,5,5,3,4));
	
	//build combined root file
	TGTextButton *buildFile = new TGTextButton(fileFrame,"Build Combined Root File");
	buildFile->Connect("Clicked()","MainWindow",this,"buildBigFile()");
	fileFrame->AddFrame(buildFile, new TGLayoutHints(kLHintsExpandX,5,5,3,4));
	
	//open combined root file
	TGTextButton *opBigFile = new TGTextButton(fileFrame,"Open Combined Root File");
	opBigFile->Connect("Clicked()","MainWindow",this,"openBigFile()");
	fileFrame->AddFrame(opBigFile, new TGLayoutHints(kLHintsExpandX,5,5,3,4));
	
	//CreateAuxFile button
	TGTextButton *createAuxFile = new TGTextButton(fileFrame,"Make Auxilliary File");
	createAuxFile->Connect("Clicked()","MainWindow",this,"makeAuxFile()");
	fileFrame->AddFrame(createAuxFile, new TGLayoutHints(kLHintsExpandX,5,5,3,4));
	
	//Open Aux File button
	TGTextButton *opAuxFile = new TGTextButton(fileFrame,"Open Auxilliary File");
	opAuxFile->Connect("Clicked()","MainWindow",this,"openAuxFile()");
	fileFrame->AddFrame(opAuxFile, new TGLayoutHints(kLHintsExpandX,5,5,3,4));
	
	//Create friend file button
	TGTextButton *createFrFile = new TGTextButton(fileFrame,"Make Friend Tree File");
	createFrFile->Connect("Clicked()","MainWindow",this,"makeFriendFile()");
	fileFrame->AddFrame(createFrFile, new TGLayoutHints(kLHintsExpandX,5,5,3,4));
	
	//Open friend file button
	TGTextButton *opFrFile = new TGTextButton(fileFrame,"Open Friend Tree File");
	opFrFile->Connect("Clicked()","MainWindow",this,"openFriendFile()");
	fileFrame->AddFrame(opFrFile, new TGLayoutHints(kLHintsExpandX,5,5,3,4));
	
	//add the file frame to the overall frame
	overallFrame->AddFrame(fileFrame,new TGLayoutHints(kLHintsExpandX,2,2,2,2));
	
	/******************************************
	** Cut operations buttons
	******************************************/
	TGVerticalFrame *cutFrame = new TGVerticalFrame(overallFrame, width, height);
	TGLabel* cutLabel = new TGLabel(cutFrame, "Cut and Shape Operations");
	cutFrame->AddFrame(cutLabel, new TGLayoutHints(kLHintsExpandX,5,5,3,4));
	//Get PIDs button
	TGTextButton *getPIDs = new TGTextButton(cutFrame,"Get PID Cuts");
	getPIDs->Connect("Clicked()","MainWindow",this,"getPIDCuts()");
	cutFrame->AddFrame(getPIDs, new TGLayoutHints(kLHintsExpandX,5,5,3,4));
	
	//display PIDs button
	TGTextButton *dispPID = new TGTextButton(cutFrame,"Show PID Cuts");
	dispPID->Connect("Clicked()","MainWindow",this,"showPIDCut()");
	cutFrame->AddFrame(dispPID, new TGLayoutHints(kLHintsExpandX,5,5,3,4));
	
	//Get BG Cuts Button
	TGTextButton *getBG = new TGTextButton(cutFrame,"Get BG Cuts");
	getBG->Connect("Clicked()","MainWindow",this,"getBGCuts()");
	cutFrame->AddFrame(getBG, new TGLayoutHints(kLHintsExpandX,5,5,3,4));
	
	//Display BG Cuts Button
	TGTextButton *dispBG = new TGTextButton(cutFrame,"Show BG Cuts");
	dispBG->Connect("Clicked()","MainWindow",this,"showBGCut()");
	cutFrame->AddFrame(dispBG, new TGLayoutHints(kLHintsExpandX,5,5,3,4));
	
	
	/******************************************
	** Shape operations buttons
	******************************************/
	//Get shapes button
	TGTextButton *getShapesBut = new TGTextButton(cutFrame,"Get Shapes");
	getShapesBut->Connect("Clicked()","MainWindow",this,"getFirstOrdShapes()");
	cutFrame->AddFrame(getShapesBut, new TGLayoutHints(kLHintsExpandX,5,5,3,4));

	//make bg sub spectra button
	TGTextButton *makeBGSubBut = new TGTextButton(cutFrame,"Make Frnd Tree Specs");
	makeBGSubBut->Connect("Clicked()","MainWindow",this,"makeBGSubSpecs()");
	cutFrame->AddFrame(makeBGSubBut, new TGLayoutHints(kLHintsExpandX,5,5,3,4));

	//display shapes button
	TGTextButton *showShapesBut = new TGTextButton(cutFrame,"Show Shapes");
	showShapesBut->Connect("Clicked()","MainWindow",this,"showFirstOrdShapes()");
	cutFrame->AddFrame(showShapesBut, new TGLayoutHints(kLHintsExpandX,5,5,3,4));
	
	//display bgsub shapes button
	TGTextButton *showbgSubBut = new TGTextButton(cutFrame,"Show BG-sub Shapes");
	showbgSubBut->Connect("Clicked()","MainWindow",this,"showSubbedShapes()");
	cutFrame->AddFrame(showbgSubBut, new TGLayoutHints(kLHintsExpandX,5,5,3,4));
	
	//add the cut frame to the overall frame
	overallFrame->AddFrame(cutFrame,new TGLayoutHints(kLHintsExpandX,2,2,2,2));
	
	
	/******************************************
	** Get Cross-Sections buttons
	******************************************/
	TGVerticalFrame *csFrame = new TGVerticalFrame(overallFrame, width, height);
	TGLabel* csLabel = new TGLabel(csFrame, "Cross-Section Extraction");
	csFrame->AddFrame(csLabel, new TGLayoutHints(kLHintsExpandX,5,5,3,4));
	//Simple get cross-sections button
	TGTextButton *getCSParamsAllRunsBut = new TGTextButton(csFrame,"Get CS Params All Runs");
	getCSParamsAllRunsBut->Connect("Clicked()","MainWindow",this,"getBasicCSParamsSimple()");
	csFrame->AddFrame(getCSParamsAllRunsBut, new TGLayoutHints(kLHintsExpandX,5,5,3,4));
	//Simple get cross-sections button
	TGTextButton *getCSParamsPerRunBut = new TGTextButton(csFrame,"Get CS Params Per Run");
	getCSParamsPerRunBut->Connect("Clicked()","MainWindow",this,"getBasicCSParamsPerRun()");
	csFrame->AddFrame(getCSParamsPerRunBut, new TGLayoutHints(kLHintsExpandX,5,5,3,4));
	//Simple get cross-sections button
	TGTextButton *scsBut = new TGTextButton(csFrame,"Simple CS Extraction");
	scsBut->Connect("Clicked()","MainWindow",this,"simpleGetCS()");
	csFrame->AddFrame(scsBut, new TGLayoutHints(kLHintsExpandX,5,5,3,4));
	
	//add the cs frame to the overall frame
	overallFrame->AddFrame(csFrame,new TGLayoutHints(kLHintsExpandX,2,2,2,2));
	
	//add the overall frame to the full frame	
	fullFrame->AddFrame(overallFrame,new TGLayoutHints(kLHintsExpandX,2,2,2,2));
	
	
	/******************************************
	** Sequential Display buttons
	******************************************/
	TGHorizontalFrame *extraFrame = new TGHorizontalFrame(fullFrame, width, height);
	TGVerticalFrame *dispFrame = new TGVerticalFrame(extraFrame, width, height);
	TGLabel* auxLabel = new TGLabel(dispFrame, "Sequential Display Buttons");
	dispFrame->AddFrame(auxLabel, new TGLayoutHints(kLHintsExpandX,5,5,3,4));
	
	TGHorizontalFrame *auxButtonsFrame = new TGHorizontalFrame(dispFrame, width, height);
	//show previous spectrum
	TGTextButton *prevSpec = new TGTextButton(auxButtonsFrame,"Prev Spec");
	prevSpec->Connect("Clicked()","MainWindow",this,"prevSeqSpec()");
	auxButtonsFrame->AddFrame(prevSpec, new TGLayoutHints(kLHintsExpandX,5,5,3,4));
	//show next spectrum
	TGTextButton *nextSpec = new TGTextButton(auxButtonsFrame,"Next Spec");
	nextSpec->Connect("Clicked()","MainWindow",this,"nextSeqSpec()");
	auxButtonsFrame->AddFrame(nextSpec, new TGLayoutHints(kLHintsExpandX,5,5,3,4));
	//end display
	TGTextButton *endSpec = new TGTextButton(auxButtonsFrame,"End Display");
	endSpec->Connect("Clicked()","MainWindow",this,"endSeqSpec()");
	auxButtonsFrame->AddFrame(endSpec, new TGLayoutHints(kLHintsExpandX,5,5,3,4));

	dispFrame->AddFrame(auxButtonsFrame,new TGLayoutHints(kLHintsExpandX,2,2,2,2));
	extraFrame->AddFrame(dispFrame,new TGLayoutHints(kLHintsExpandX,2,2,2,2));
	
	/******************************************
	** Overall Control buttons
	******************************************/
	TGVerticalFrame *ctrlFrame = new TGVerticalFrame(extraFrame, width, height);
	TGLabel* fancyLabel = new TGLabel(ctrlFrame, "Overall Control");
	ctrlFrame->AddFrame(fancyLabel, new TGLayoutHints(kLHintsExpandX,5,5,3,4));
	TGHorizontalFrame *overallControlFrame = new TGHorizontalFrame(ctrlFrame, width, height);
	//reset button
	TGTextButton *resBut = new TGTextButton(overallControlFrame,"Reset Setup");
	resBut->Connect("Clicked()","MainWindow",this,"resetToStart()");
	overallControlFrame->AddFrame(resBut, new TGLayoutHints(kLHintsExpandX,5,5,3,4));
	//Kill application button
	TGTextButton *exBut = new TGTextButton(overallControlFrame,"Exit");
	exBut->Connect("Clicked()","MainWindow",this,"exitApp()");
	overallControlFrame->AddFrame(exBut, new TGLayoutHints(kLHintsExpandX,5,5,3,4));
	
	ctrlFrame->AddFrame(overallControlFrame,new TGLayoutHints(kLHintsExpandX,2,2,2,2));
	extraFrame->AddFrame(ctrlFrame,new TGLayoutHints(kLHintsExpandX,2,2,2,2));
	
	fullFrame->AddFrame(extraFrame,new TGLayoutHints(kLHintsExpandX,2,2,2,2));
	
	/******************************************
	** File Control buttons
	******************************************/
	TGLabel* fileControlLabel = new TGLabel(fullFrame, "File Control (for debugging)");
	fullFrame->AddFrame(fileControlLabel, new TGLayoutHints(kLHintsExpandX,5,5,3,4));
	TGHorizontalFrame *fileControlFrame = new TGHorizontalFrame(fullFrame, width, height);
	//make tree file the current dir
	TGTextButton *treeFocusBut = new TGTextButton(fileControlFrame,"Activate Tree File");
	treeFocusBut->Connect("Clicked()","MainWindow",this,"makeTreeFileActive()");
	fileControlFrame->AddFrame(treeFocusBut, new TGLayoutHints(kLHintsExpandX,5,5,3,4));
	//make aux file the current dir
	TGTextButton *auxFocusBut = new TGTextButton(fileControlFrame,"Activate Aux File");
	auxFocusBut->Connect("Clicked()","MainWindow",this,"makeAuxFileActive()");
	fileControlFrame->AddFrame(auxFocusBut, new TGLayoutHints(kLHintsExpandX,5,5,3,4));
	//make friend file the current dir
	TGTextButton *frFocusBut = new TGTextButton(fileControlFrame,"Activate Friend File");
	frFocusBut->Connect("Clicked()","MainWindow",this,"makeFriendFileActive()");
	fileControlFrame->AddFrame(frFocusBut, new TGLayoutHints(kLHintsExpandX,5,5,3,4));
	
	fullFrame->AddFrame(fileControlFrame,new TGLayoutHints(kLHintsExpandX,2,2,2,2));
	
	//add the full frame to the window
	menu->AddFrame(fullFrame,new TGLayoutHints(kLHintsExpandX,2,2,2,2));
	
	//set the name of the main frame
	menu->SetWindowName("Analysis Menu");
	
	//go through the steps to draw this beast
	//map the subwindows
	menu->MapSubwindows();
	//init the layout engine
	menu->Resize(menu->GetDefaultSize());
	//map the main frame
	menu->MapWindow();
}

MainWindow::~MainWindow()
{
	menu->Cleanup();
	delete whiteBoard;
	delete basicInfoGrabber;
	delete menu;
	//delete sys;
	//sys=NULL;
	
	if(runs != NULL)
	{
		delete[] runs;
		runs = NULL;
	}
	if(csBnds != NULL)
	{
		delete[] csBnds;
		csBnds = NULL;
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
}

/******************************************
*******************************************
** File operations
*******************************************
******************************************/
void MainWindow::readRunData()
{
	if (runs != NULL)
	{
		cout<<"\nRun data already loaded, to load new run data, use the reset button"<<endl;
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
	new TGFileDialog(gClient->GetRoot(), menu, kFDOpen, &fileInfo);
	//cout<<fileInfo.fFilename<<", "<<fileInfo.fIniDir<<endl;
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
	//cout<<numRuns<<endl;
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
	cout<<"\nRun data has been loaded"<<endl;
	csBnds = new CSBounds[numRuns];
}

void MainWindow::buildBigFile()
{
	if(!checkUpToRunData())
	{	return; }
	else if (mainFile != NULL)
	{
		cout<<"\nA combined file is already loaded, use reset to create another"<<endl;
		return;
	}
	else
	{
		cout<<"\nWarning, this function assumes that the big file"<<endl;
		cout<<"is saved in the same folder as the the original data"<<endl;
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
	new TGFileDialog(gClient->GetRoot(), menu, kFDSave, &fileInfo);
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
	
	//create the big file
	mainFile = new TFile(temp.c_str(),"RECREATE");
	//start the loop to read in each smaller file
	for(int i=0; i<numRuns; ++i)
	{
		//create the file name and the new tree name
		ostringstream namer;
		namer<<directory.Data()<<"/run"<<runs[i].runNumber<<".root";
		string name = makeTreeName(runs[i].runNumber);
		string fileName = namer.str();
		//create the new tree
		mainFile->cd();
		TTree* nTree = new TTree(name.c_str(), name.c_str());
		//nTree->SetAutoSave(1048576);
		//nTree->SetAutoFlush(10000);
		//cout<<mainFile<<" "<<nTree->GetCurrentFile()<<endl;
		//open the file with the old tree
		TFile* file = new TFile(fileName.c_str(),"READ");
		TTree* oTree = (TTree*)file->Get("h1000");
		mainFile->cd();
		cout<<"Starting to process run"<<runs[i].runNumber<<"..."<<std::flush;
		//loop through all the branches of the old tree adding them to the new tree
		TObjArray* branchList = (TObjArray*)(oTree->GetListOfBranches()->Clone());
		branchList->SetOwner(kFALSE);
		Long64_t numEnts = (oTree->GetEntries());
		int numBran = (branchList->GetLast()+1);
		float* vals = new float[numBran];
		for(int j=0; j<numBran; ++j)
		{
			const char* branName = branchList->At(j)->GetName();
			//construct the variable name
			ostringstream varNamer;
			varNamer<<branName<<"/F";
			nTree->Branch(branName,&(vals[j]),varNamer.str().c_str());
			oTree->SetBranchAddress(branName, &(vals[j]));
		}
		for(Long64_t k=0; k<numEnts; ++k)
		{
			oTree->GetEntry(k);
			nTree->Fill();
		}
		cout<<"  processed "<<numEnts<<" nTuples"<<endl;
		delete oTree;
		delete file;
		nTree->FlushBaskets();
		nTree->Write(name.c_str(),TObject::kOverwrite);
		file = nTree->GetCurrentFile();
		file->Flush();
		delete nTree;
		delete[] vals;
	}
	cout<<"Done Processing"<<endl;
	mainFile->Close();
	delete mainFile;
	mainFile = new TFile(temp.c_str(),"READ");
	cout<<"Combined file opened in read only mode"<<endl;
}

void MainWindow::openBigFile()
{
	if(!checkUpToRunData())
	{	return; }
	if (mainFile != NULL)
	{
		cout<<"\nCombined file already loaded, to load a different one, use the reset button"<<endl;
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
	new TGFileDialog(gClient->GetRoot(), menu, kFDOpen, &fileInfo);
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
	cout<<"\nCombined file opened in read only mode"<<endl;
}

void MainWindow::makeAuxFile()
{
	if(!checkUpToMainFile())
	{	return; }
	if(auxFile != NULL)
	{
		cout<<"\nAux file already loaded, to load a different one, use the reset button"<<endl;
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
	new TGFileDialog(gClient->GetRoot(), menu, kFDSave, &fileInfo);
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
	
	//create the aux file
	auxFile = new TFile(temp.c_str(),"RECREATE");
	cout<<"\nAux File Created"<<endl;
}

void MainWindow::openAuxFile()
{
	if(!checkUpToMainFile())
	{	return; }
	if(auxFile != NULL)
	{
		cout<<"\nAux file already loaded, to load a different one, use the reset button"<<endl;
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
	new TGFileDialog(gClient->GetRoot(), menu, kFDOpen, &fileInfo);
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
	cout<<"\nAux File Opened"<<endl;
}


void MainWindow::makeFriendFile()
{
	if(!checkUpToAuxFile())
	{	return; }
	else if( frFile != NULL )
	{
		cout<<"\nFriend file already loaded, to load a different one, use the reset button"<<endl;
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
	new TGFileDialog(gClient->GetRoot(), menu, kFDSave, &fileInfo);
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
	
	//create the aux file
	frFile = new TFile(temp.c_str(),"RECREATE");
	cout<<"\nFriend File Created"<<endl;
}

void MainWindow::openFriendFile()
{
	if(!checkUpToAuxFile())
	{	return; }
	if( frFile != NULL )
	{
		cout<<"\nFriend file already loaded, to load a different one, use the reset button"<<endl;
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
	new TGFileDialog(gClient->GetRoot(), menu, kFDOpen, &fileInfo);
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
	cout<<"\nFriend File Opened"<<endl;
}

/******************************************
*******************************************
** Cut operations
*******************************************
******************************************/

void MainWindow::getPIDCuts()
{
	if(!checkUpToAuxFile())
	{	return; }
	
	for(int i=0; i<numRuns; ++i)
	{
		cout<<"Loading run "<<runs[i].runNumber<<"  "<<(i+1)<<" / "<<numRuns<<endl;
		//make the PID cut name
		string cName = makePIDCutName(runs[i].runNumber);
		//retrive the tree
		TTree* temp = testTree(runs[i].runNumber);
		//check to make sure temp is present
		if(temp == NULL)
		{	return;		}
		//draw the histogram and set the axes correctly
		string histName = makePIDSpecName(runs[i].runNumber);
		string drawCommand = "Pi1:Pi2>>";
		drawCommand.append(histName);
		drawCommand.append("(200,0,600,200,0,600)");
		//cout<<histName<<"  "<<drawCommand<<endl;
		temp->Draw(drawCommand.c_str(),RayCut,"colz");
		TH2F* tempH2 = reinterpret_cast<TH2F*>(gDirectory->Get(histName.c_str()));
		ostringstream histTitle;
		histTitle<<"Run "<<runs[i].runNumber<<" Pid1:Pid2(Rayid==0)";
		tempH2->SetTitle(histTitle.str().c_str());
		auxFile->cd();
		tempH2->Write(histName.c_str(),TObject::kOverwrite);
		gPad->SetLogy(0);
		whiteBoard->SetLogz(1);
		whiteBoard->Update();
		cout<<"Please give PID cut."<<endl;
		//wait for the user to enter the cut then retrieve it
		TCutG* cut=(TCutG*)gPad->WaitPrimitive("CUTG","CutG");
		//cout<<cut->GetVarX()<<"  "<<cut->GetVarY()<<endl;
		//change the name of the cut to something more descriptive
		cut->SetName(cName.c_str());
		//make sure the cut has the correct x and y values
		cut->SetVarY("Pi1");
		cut->SetVarX("Pi2");
		//make the base cut
		TCut* baseCut = new TCut();
		(*baseCut) = RayCut && cut->GetName();
		string baseCutName = makeBaseCutName(runs[i].runNumber);
		baseCut->SetName(baseCutName.c_str());
		//cout<<cut->GetVarX()<<"  "<<cut->GetVarY()<<endl;
		//now save the cuts to auxFile
		auxFile->cd();
		cut->Write(cName.c_str(),TObject::kOverwrite);
		baseCut->Write(baseCutName.c_str(),TObject::kOverwrite);
		if( i != (numRuns-1))
		{
			cout<<"Cut saved, preparing for next cut"<<endl;
		}
		else
		{
			cout<<"Final cut saved. Erasing white board and exitting function"<<endl;
		}
		//now delete the cut and the histogram
		delete baseCut;
		delete cut;
		delete tempH2;
	}
	whiteBoard->Clear();
	whiteBoard->Update();
}

void MainWindow::showPIDCut()
{
	if(!checkUpToAuxFile())
	{	return; }
	
	dispNum = 0;
	dispFunc = PIDCut;
	updateDisplay(Initial);
}



void MainWindow::getBGCuts()
{
	if(!checkUpToAuxFile())
	{	return; }
	
	cout<<"\nWhen prompted to give regions, click at each of the four\nfollowing points specified, in any order"<<endl;
	cout<<"The points are:\nLower bnd of the lower bg region\nUpper bnd of the lower bg region (also the lower bnd of the true region)"<<endl;
	cout<<"Lower bnd of the upper bg region (also the upper bnd of the true region)\nUpper bnd of the upper bg region"<<endl;
	cout<<"Only the first 4 clicks will be used, double click when done selecting regions\n"<<endl;
	cout<<"\nWARNING: If you have already constructed bg subtracted histograms"<<endl;
	cout<<"you will need to reconstruct them after running this function"<<endl;
	
	for(int i=0; i<numRuns; ++i)
	{
		//first retrive the PID cut from the auxilliary file
		TCutG* pidCut = testPIDCut(runs[i].runNumber);
		//get the base cut
		TCut* baseCut = testBaseCut(runs[i].runNumber);
		//get the tree from the main file
		TTree* tree = testTree(runs[i].runNumber);
		if(pidCut == NULL || baseCut == NULL || tree == NULL)
		{	return;		}
		//build the draw command
		string histName = makeBGSpecName(runs[i].runNumber);
		ostringstream cmdBuilder;
		cmdBuilder<<"Yfp>>"<<histName<<"(200,-50,50)";
		string drawCommand = cmdBuilder.str();

		//draw the spectrum
		cout<<"Loading run "<<runs[i].runNumber<<"  "<<(i+1)<<" / "<<numRuns<<endl;
		tree->Draw(drawCommand.c_str(), (*baseCut));
		TH1F* tempH1 = reinterpret_cast<TH1F*>(gDirectory->Get(histName.c_str()));
		ostringstream titleBuilder;
		titleBuilder<<"run "<<runs[i].runNumber<<" Yfp( "<<baseCut->GetTitle()<<" )";
		tempH1->SetTitle(titleBuilder.str().c_str());
		TPad* pd = (TPad*)whiteBoard->cd(1);
		whiteBoard->SetLogy(1);
		whiteBoard->Update();
		//get the BG ranges
		cout<<"Please give regions"<<endl;
		TGraph* graph = (TGraph*)pd->WaitPrimitive("Graph","PolyLine");
		//get the array of x values (which are Yfp values)
		Double_t* xValOld = graph->GetX();
		//build the array of x vals and y vals for the stored graph
		double xValArr[4];
		double yValArr[4];
		for(int j=0; j<4; ++j)
		{xValArr[j] = xValOld[j];}
		//sort the x values
		sortDoubles(xValArr,4);
		//get the y values from the bg spectrum
		for(int j=0; j<4; ++j)
		{yValArr[j] = 1.5*(tempH1->GetBinContent(tempH1->FindBin(xValArr[j])));}
		
		//generate the 4 point graph that will actually be stored
		TGraph* bgGraph = new TGraph(4,xValArr,yValArr);
		string gName = makeBGGraphName(runs[i].runNumber);
		bgGraph->SetName(gName.c_str());
		//generate the background cut
		string bgCutName = makeBGCutName(runs[i].runNumber);
		ostringstream cutBuilder;
		cutBuilder<<"( ("<<xValArr[0]<<"<Yfp) && (Yfp<"<<xValArr[1]<<") ) ||  ( ("<<xValArr[2]<<"<Yfp) && (Yfp<"<<xValArr[3]<<") )";
		string backgroundCut = cutBuilder.str();
		//cout<<backgroundCut<<endl;
		TCut* bgCut = new TCut(bgCutName.c_str(),backgroundCut.c_str());
		//cout<<bgCut->GetTitle()<<endl;
		//generate the true cut
		string trCutName = makeTRCutName(runs[i].runNumber);
		cutBuilder.str("");
		cutBuilder<<"("<<xValArr[1]<<"<Yfp) && (Yfp<"<<xValArr[2]<<")";
		string trueCut = cutBuilder.str();
		//cout<<trueCut<<endl;
		TCut* trCut = new TCut(trCutName.c_str(),trueCut.c_str());
		//cout<<trCut->GetTitle()<<endl;
		//write everything to disk
		auxFile->cd();
		bgGraph->Write(gName.c_str(),TObject::kOverwrite);
		bgCut->Write(bgCutName.c_str(),TObject::kOverwrite);
		trCut->Write(trCutName.c_str(),TObject::kOverwrite);
		tempH1->Write(histName.c_str(),TObject::kOverwrite);
		auxFile->Flush();
		
		cout<<"True region: "<<trCut->GetTitle()<<endl;
		cout<<"Bgnd region: "<<bgCut->GetTitle()<<endl;
		if( i != (numRuns-1))
		{
			cout<<"Cut saved, preparing for next cut"<<endl;
		}
		else
		{
			cout<<"Final cut saved. Erasing white board and exitting function"<<endl;
		}
		delete graph;
		delete bgGraph;
		delete bgCut;
		delete trCut;
		delete tempH1;
		delete tree;
		delete baseCut;
		delete pidCut;
		whiteBoard->Update();
	}
	whiteBoard->Clear();
	whiteBoard->Update();
}

void MainWindow::showBGCut()
{
	if(!checkUpToAuxFile())
	{	return; }
	
	
	dispNum = 0;
	dispFunc = BGCut;
	updateDisplay(Initial);
}

/******************************************
*******************************************
** Shape operations buttons
*******************************************
******************************************/
void MainWindow::getFirstOrdShapes()
{
	if( !checkUpToFrFile() )
	{	return; }
	
	//cout<<"checkUpToFrFile == "<<checkUpToFrFile()<<" frFile = "<<frFile<<endl;
	
	cout<<"\nWhen prompted click along the top and bottom edges of the high counts region of the spectrum"<<endl;
	cout<<"You must click at least 4 times, preferably more\n"<<endl;
	cout<<"\nWARNING: If you have already constructed bg subtracted histograms"<<endl;
	cout<<"you will need to reconstruct them after running this function"<<endl;
	
	double* centers = new double[3*numRuns];
	
	for(int i=0; i<numRuns; ++i)
	{
		cout<<"Loading run "<<runs[i].runNumber<<"  "<<(i+1)<<" / "<<numRuns<<endl;
		//grab the tree
		TTree* temp = testTree( runs[i].runNumber );
		if(temp==NULL)
		{return;}
		//get the name for the histogram
		string histName = makeOrigShapeSpecName( runs[i].runNumber );
		//build the draw command, the cut does not need building as it is simple
		ostringstream drawCmd;
		drawCmd<<"Thscat:Xfp>>"<<histName<<"(240,-600,600,300,-3,3)";
		string drawStr = drawCmd.str();
		//draw the histogram and set the axes correctly
		temp->Draw(drawStr.c_str(),RayCut,"colz");
		TH2F* tempH2 = reinterpret_cast<TH2F*>(gDirectory->Get(histName.c_str()));
		ostringstream histTitler;
		histTitler<<"Run "<<runs[i].runNumber<<" Th_scatter:X_fp( Rayid==0 )";
		tempH2->SetTitle(histTitler.str().c_str());
		auxFile->cd();
		tempH2->Write(histName.c_str(),TObject::kOverwrite);
		whiteBoard->SetLogy(0);
		whiteBoard->SetLogz(1);
		whiteBoard->Update();
		cout<<"Give Top Edge"<<endl;
		//wait for the user to enter the cut then retrieve it
		TCutG* upper=(TCutG*)gPad->WaitPrimitive("Graph","PolyLine");
		//change the name of the cut to something more descriptive
		upper->SetName("top");
		cout<<"Give Bottom Edge"<<endl;
		//wait for the user to enter the cut then retrieve it
		TCutG* lower=(TCutG*)gPad->WaitPrimitive("Graph","PolyLine");
		//change the name of the cut to something more descriptive
		lower->SetName("bottom");
		
		TFitResultPtr temp1 = upper->Fit("pol2","SEQ");
		TFitResultPtr temp2 = lower->Fit("pol2","SEQ");
		whiteBoard->Update();
		
		const double* tParams = temp1->GetParams();
		const double* bParams = temp2->GetParams();
		
		//construct the polynomial that is the center of these two polynomials
		centers[3*i]   = (tParams[0]+bParams[0])/2;
		centers[3*i+1] = (tParams[1]+bParams[1])/2;
		centers[3*i+2] = (tParams[2]+bParams[2])/2;
	
		
		//cout<<centers[3*i]<<", "<<centers[3*i+1]<<", "<<centers[3*i+2]<<endl;
		
		//sleep for 2 seconds so the user can see the fitted edges
		sys->Sleep(2000);
		
		//now delete the file and cut
		delete temp;
		delete upper;
		delete lower;
		delete tempH2;
	}
	whiteBoard->Clear();
	whiteBoard->Update();
	
	cout<<"\nNow building friend trees with first order shape corrections."<<endl;
	cout<<"This could take some time depending on the number of events in the trees.\n"<<endl;
	
	for(int i=0; i<numRuns; ++i)
	{
		//force a new line
		cout<<"\n";
		//grab the tree, we do not need to test it because we tested it before so we
		//simply use retrieveTree rather than testTree
		TTree* orig = retrieveTree( runs[i].runNumber );
		//make the new tree
		frFile->cd();
		//check if the friend tree already exists
		TTree* test = retrieveFirstOrderShapeTree(runs[i].runNumber);
		string treeName = makeFirstOrderShapeTreeName(runs[i].runNumber);
		if(test!=NULL)
		{
			delete test;
			test=NULL;
			ostringstream nameBuilder;
			nameBuilder<<treeName<<";*";
			string toBeDeleted = nameBuilder.str();
			frFile->Delete(toBeDeleted.c_str());
			cout<<"Deleted original friend tree so we can build a replacement"<<endl;
		}
		TTree* frnd = new TTree(treeName.c_str(),treeName.c_str());
		//put the polynomial parameters for this run into quick variables
		float a = centers[3*i];
		float b = centers[3*i+1];
		float c = centers[3*i+2];
		//set up variables whose addresses will be used in reading individual events
		float theta;
		float xfp;
		orig->SetBranchAddress("Xfp",&xfp);
		orig->SetBranchAddress("Thscat",&theta);
		//set up the variable whose address will be used in writing the individual events
		float thCor;
		frnd->Branch("Thcorr",&thCor,"Thcorr/F");
		
		Long64_t numEnts = (orig->GetEntries());
		float avg = 0.0;
		cout<<"Preparing to apply first order shape corection to run"<<runs[i].runNumber<<"  "<<(i+1)<<" / "<<numRuns<<endl;
		//fill the new tree
		for(Long64_t j = 0; j<numEnts; ++j)
		{
			orig->GetEntry(j);
			avg = ( a + (b*xfp) + (c*xfp*xfp) );
			thCor = (theta - avg);
			frnd->Fill();
		}
		frFile->cd();
		string oldTreeName = makeTreeName(runs[i].runNumber);
		frnd->AddFriend( oldTreeName.c_str(), mainFile->GetName() );
		frnd->FlushBaskets();
		frnd->Write(treeName.c_str(),TObject::kOverwrite);
		TFile* file = frnd->GetCurrentFile();
		file->Flush();
		
		cout<<"Finished Constructing Friend Tree"<<endl;
		
		delete frnd;
		delete orig;
	}
	delete[] centers;
	cout<<"Done performing first order corrections"<<endl;
}

void MainWindow::showFirstOrdShapes()
{
	if(!checkUpToFrFile())
	{	return; }
	dispNum = 0;
	dispFunc = ShapeDisp;
	updateDisplay(Initial);
}

void MainWindow::makeBGSubSpecs()
{
	if( !checkUpToFrFile() )
	{	return; }
	
	cout<<"\n";
	
	//loop across loaded runs
	for(int i=0; i<numRuns; ++i)
	{
		cout<<"Preparing to construct specta for run"<<runs[i].runNumber<<"  "<<(i+1)<<" / "<<numRuns<<endl;
		int runN = runs[i].runNumber;
		//load the friend tree
		TTree* frnd = testFirstOrderShapeTree( runN );
		if(frnd==NULL)
		{	return; }
		
		//construct histograms, the shape corrected one, the true + bg, the bg, the scaled bg and the sub
		
		//first get the bggraph and the pid cut
		TGraph* bgGraph = testBGGraph(runN);
		TCutG* pidCut = testPIDCut(runN);
		
		if(bgGraph == NULL || pidCut == NULL)
		{
			return;
		}
		
		// now do stuff with the bgGraph
		double* bgPts = bgGraph->GetX();
		
		//now get the bg normalization
		double bgNorm = calculateBGNorm(bgGraph);
		
		//make all the histogram names
		string shapeHistName = makeFirstOrderShapeSpecName( runN );
		string trHistName = makeTRpBGSpecName( runN );
		string bgHistName = makeBGOnlySpecName( runN );
		string scaledBgHistName = makeScaledBGSpecName( runN );
		string subHistName = makeSubSpecName( runN );
		
		//construct the histogram titles
		ostringstream titler;
		titler<<"Run "<<runN<<" Thcorr:Xfp {No Cuts}";
		string shapeHistTitle = titler.str();
		titler.str("");
		
		titler<<"Run "<<runN<<" Thcorr:Xfp {Rayid==0 && PID && True Region}";
		string trHistTitle = titler.str();
		titler.str("");
		
		titler<<"Run "<<runN<<" Thcorr:Xfp {Rayid==0 && PID && BG Region}";
		string bgHistTitle = titler.str();
		titler.str("");
		
		titler<<"Run "<<runN<<" Thcorr:Xfp Scaled {Rayid==0 && PID && BG Region}";
		string scaledBGHistTitle = titler.str();
		titler.str("");
		
		titler<<"Run "<<runN<<" (True - Scaled BG) Thcorr:Xfp";
		string subHistTitle = titler.str();
		
		//make the actual histograms to hold things
		TH2F* shapeHist = new TH2F(shapeHistName.c_str(), shapeHistTitle.c_str(), 240, -600, 600, 300, -3, 3);
		TH2F* trHist = new TH2F(trHistName.c_str(), trHistTitle.c_str(), 240, -600, 600, 300, -3, 3);
		TH2F* bgHist = new TH2F(bgHistName.c_str(), bgHistTitle.c_str(), 240, -600, 600, 300, -3, 3);
		TH2F* scaledBGHist = new TH2F(scaledBgHistName.c_str(), scaledBGHistTitle.c_str(), 240, -600, 600, 300, -3, 3);
		TH2F* subHist = new TH2F(subHistName.c_str(), subHistTitle.c_str(), 240, -600, 600, 300, -3, 3);
		
		//now set up the branches of our new tree to retrieve everything
		float theta = 0.0, xfp = 0.0, yfp = 0.0, pi1 = 0.0, pi2 = 0.0, rayid = 0.0;
		frnd->SetBranchAddress("Thcorr",&theta);
		frnd->SetBranchAddress("Xfp",&xfp);
		frnd->SetBranchAddress("Yfp",&yfp);
		frnd->SetBranchAddress("Pi1",&pi1);
		frnd->SetBranchAddress("Pi2",&pi2);
		frnd->SetBranchAddress("Rayid",&rayid);
		
		Long64_t numEnts = (frnd->GetEntries());
		//now fill the histograms other than the subtracted spectrum
		for(Long64_t j = 0; j<numEnts; ++j)
		{
			//first get the entry
			frnd->GetEntry(j);
			//check the rayid cut as it is applied to everything
			if( rayid == 0)
			{
				//first fill the shape spectrum as it has no other cuts applied
				shapeHist->Fill(xfp,theta);
				//now test for the PID cut as everything else has that
				if( 1 == (pidCut->IsInside(pi2,pi1)) )
				{
					//test to see if it is in the background region
					if( (bgPts[0]<yfp && yfp<bgPts[1]) || (bgPts[2]<yfp && yfp<bgPts[3]) )
					{
						//increment the bg spectrum
						bgHist->Fill(xfp,theta);
						//increment the scaled bg spectrum using the scaling factor as the weight
						scaledBGHist->Fill(xfp,theta,bgNorm);
					}//otherwise test if we are in the true region
					else if( bgPts[1]<=yfp && yfp<=bgPts[2] )
					{
						//increment the tr+bg spectrum
						trHist->Fill(xfp,theta);
					}
				}
			}
		}
		
		//now the spectra should be filled so make the subtracted spectrum
		subHist->Add(trHist, scaledBGHist, 1, -1);
		
		//now save the spectra
		auxFile->cd();
		shapeHist->Write(shapeHistName.c_str(),TObject::kOverwrite);
		trHist->Write(trHistName.c_str(),TObject::kOverwrite);
		bgHist->Write(bgHistName.c_str(),TObject::kOverwrite);
		scaledBGHist->Write(scaledBgHistName.c_str(),TObject::kOverwrite);
		subHist->Write(subHistName.c_str(),TObject::kOverwrite);
		
		auxFile->Flush();
		delete frnd; 
		delete shapeHist;
		delete trHist;
		delete bgHist;
		delete scaledBGHist;
		delete subHist;
		delete bgGraph;
		delete pidCut;
	}
	cout<<"Done constructing histograms"<<endl;
}

void MainWindow::showSubbedShapes()
{
	if(!checkUpToFrFile())
	{	return; }
	dispNum = 0;
	dispFunc = SubbedSpecs;
	updateDisplay(Initial);
}

void MainWindow::getBasicCSParamsSimple()
{
	if(!checkUpToFrFile())
	{	return; }
	cout<<"\nOnce you have entered the parameters you can immediately press End Display\n"<<endl;
	dispNum = 0;
	dispFunc = BasicCSInfoSimple;
	updateDisplay(Initial);
}

void MainWindow::getBasicCSParamsPerRun()
{
	if(!checkUpToFrFile())
	{	return; }
	cout<<"\nTo support using the next and prev spectra functions here you will need to"<<endl;
	cout<<"all the way to the end of the list of runs using the next button before ending\n"<<endl;
	dispNum = 0;
	dispFunc = BasicCSInfoPerRun;
	updateDisplay(Initial);
}

void MainWindow::simpleGetCS()
{
	if(!checkUpToFrFile())
	{	return; }
	
	if(numBnds!=numRuns)
	{
		cout<<"Boundary Information has not been entered for every run"<<endl;
		return;
	}
	
	//first we need to find out a file name to save this data to
	cout<<"\nGive the file name to save this cross-section data to"<<endl;
	static TString directory(".");
	TGFileInfo fileInfo;
	fileInfo.SetMultipleSelection(false);
	fileInfo.fFileTypes = csvDataType;
	fileInfo.fIniDir = StrDup(directory);
	
	//make the open file dialog
	//quite frankly this creeps me the hell out, just creating an object like this
	//but apparently the parent object will delete it on its own
	new TGFileDialog(gClient->GetRoot(), menu, kFDSave, &fileInfo);
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
	
	//now output the information line of the csv file
	output<<"Run number, angle, state 1 cs, state 1 cs err, state 2 cs, state 2 cs err, ..."<<endl;
	
	cout<<"\nPlease be aware, the simple version of this function assumes that all of the input"<<endl;
	cout<<"That you give with regards to phi width etc holds for the entire set of runs\n"<<endl;
	
	int numStates=0;
	double thetaMin=0.0, thetaMax=0.0, delta=0.0, phiWidth=0.0;
	int numSegs=0;
	
	//next we need to iterate through the set of runs
	for(int i=0; i<numRuns; ++i)
	{
		numStates = csBnds[i].numStates;
		thetaMin = csBnds[i].minTheta;
		thetaMax = csBnds[i].maxTheta;
		numSegs = csBnds[i].thetaSegs;
		phiWidth = csBnds[i].phiWidth;
		delta = (thetaMax-thetaMin)/((float)numSegs);
		//now retrieve the background subtracted spectrum for this run
		TH2F* spec = testSubSpec(runs[i].runNumber);
		if(spec==NULL)
		{	return;	}
		
		//cout<<numStates<<"  "<<thetaMin<<"  "<<thetaMax<<"  "<<numSegs<<"  "<<delta<<"  "<<phiWidth;
		
		spec->Draw("colz");
		whiteBoard->SetLogy(0);
		whiteBoard->SetLogz(1);
		whiteBoard->Update();
		
		int numLines = (2*numStates);
		
		float* m = new float[numLines];
		float* b = new float[numLines];
		
		//get the lines defining the state bounds
		for(int j=0; j<numStates; ++j)
		{
			cout<<"Please draw the line that defines the lower bound in xfp for state "<<(j+1)<<endl;
			TLine* line = (TLine*)gPad->WaitPrimitive("TLine","Line");
			float x1 = line->GetX1();
			float x2 = line->GetX2();
			float y1 = line->GetY1();
			float y2 = line->GetY2();
			m[2*j] = ((x2 - x1)/(y2 - y1));
			b[2*j] = ( ((x2*y1)-(x1*y2)) / (y1-y2) );
			line->Delete();
			
			cout<<"Please draw the line that defines the upper bound in xfp for state "<<(j+1)<<endl;
			TLine* line = (TLine*)gPad->WaitPrimitive("TLine","Line");
			x1 = line->GetX1();
			x2 = line->GetX2();
			y1 = line->GetY1();
			y2 = line->GetY2();
			m[2*j+1] = ((x2 - x1)/(y2 - y1));
			b[2*j+1] = ( ((x2*y1)-(x1*y2)) / (y1-y2) );
			line->Delete();
		}
		
		//arrays for holding the 
		float xArr[5];
		float yArr[5];
		
		for(int j=0; j<numSegs; ++j)
		{
			float lowAngle = thetaMin + (j * delta);
			float highAngle = lowAngle + delta;
			//bottom left | bottom right
			yArr[0]=lowAngle; yArr[1]=lowAngle;
			//top right | top left
			yArr[2]=highAngle; yArr[3]=highAngle;
			//bottom left again
			yArr[4]=lowAngle;
			float central=(runs[i].angle + ((lowAngle+highAngle)/2));
			
			output<<runs[i].runNumber<<", "<<central<<", ";
			
			for(int k=0; k<numStates; ++k)
			{
				//calculate the x points of the cut from the given bounds
				//bottom left | bottom right
				xArr[0] = ( b[2*k]  + m[2*k]*yArr[0]);    xArr[1] = (b[2*k+1] + m[2*k+1]*yArr[1]);
				//top right | top left
				xArr[2] = (b[2*k+1] + m[2*k+1]*yArr[2]);  xArr[3] = ( b[2*k]  + m[2*k]*yArr[3]);
				//bottom left again
				xArr[4] = xArr[0];
			
				//now extract the points by using TCutG and its integrate member function
				TCutG* intBounds = new TCutG("bounds",5,xArr,yArr);
				intBounds->SetVarX("Xfp");
				intBounds->SetVarY("Thcorr");
				double counts = intBounds->IntegralHist(spec);
				double cntsErr = TMath::Sqrt(counts);
				output<< calcCrossSection(counts, runs[i], delta, phiWidth) <<", "<< calcCrossSection(cntsErr, runs[i], delta, phiWidth);
				if(k != (numStates - 1))
				{
					output<<", ";
				}
				else
				{
					output<<endl;
				}
			}
		}
		delete[] m;
		delete[] b;
		delete spec;
	}
	whiteBoard->Clear();
	whiteBoard->Update();
	cout<<"Done Getting Cross-Sections"<<endl;
}

/******************************************
*******************************************
** Sequential Display operations
*******************************************
******************************************/
void MainWindow::updatePIDDisp(const UpdateCallType& tp)
{
	static TH2F* hist;
	static TCutG* tempCG;
	if(tp==Initial)
	{
		hist = NULL;
		tempCG = NULL;
		whiteBoard->Clear();
		whiteBoard->Update();
	}
	else if(tp==Normal)
	{
		if(hist!=NULL)
		{
			delete hist;
			hist=NULL;
		}
		if(tempCG!=NULL)
		{
			delete tempCG;
			tempCG=NULL;
		}
		whiteBoard->Clear();
		whiteBoard->Update();
	}
	else if(tp==Final)
	{
		cout<<"Done with display, erasing whiteboard"<<endl;
		if(hist!=NULL)
		{
			delete hist;
			hist=NULL;
		}
		if(tempCG!=NULL)
		{
			delete tempCG;
			tempCG=NULL;
		}
		whiteBoard->Clear();
		whiteBoard->Update();
		return;
		break;	
	}
	
	//retrieve the tree and cuts
	hist = testPIDSpec( runs[dispNum].runNumber );
	tempCG = testPIDCut( runs[dispNum].runNumber );
	if(hist==NULL || tempCG == NULL)
	{
		cout<<"Backing out of display mode"<<endl;
		dispNum = 0;
		dispFunc = None;
		updatePIDDisp(Final);//to cleanup static vars
		return;
	}
	//otherwise display the pid spectrum
	whiteBoard->Divide(2,1);	
	whiteBoard->cd(1);
	hist->Draw("colz");
	gPad->SetLogz(1);
	gPad->SetLogy(0);
	tempCG->Draw("same");
	whiteBoard->Update();
	whiteBoard->cd(2);
	//build the options to draw it in color while applying the PID cut
	ostringstream optBuild;
	optBuild<<"colz ["<<tempCG->GetName()<<"]";
	string options = optBuild.str();
	hist->Draw( options.c_str() );
	gPad->SetLogz(1);
	gPad->SetLogy(0);
	whiteBoard->Update();
	whiteBoard->cd(1);
}

void MainWindow::updateBGDisp(const UpdateCallType& tp)
{
	static TH1F* origHist;
	static TH1F* lowHist;
	static TH1F* midHist;
	static TH1F* hiHist;
	static TGraph* regions;
	static TPaveText* text;
	if(tp==Initial)
	{
		origHist = NULL;
		lowHist = NULL;
		midHist = NULL;
		hiHist = NULL;
		regions = NULL;
		text = NULL;
		whiteBoard->Clear();
		whiteBoard->Update();
	}
	else if(tp==Normal)
	{
		if(origHist!=NULL)
		{
			delete origHist;
			origHist=NULL;
		}
		if(lowHist!=NULL)
		{
			delete lowHist;
			lowHist=NULL;
		}
		if(midHist!=NULL)
		{
			delete midHist;
			midHist=NULL;
		}
		if(hiHist!=NULL)
		{
			delete hiHist;
			hiHist=NULL;
		}
		if(regions!=NULL)
		{
			delete regions;
			regions=NULL;
		}
		if(text!=NULL)
		{
			delete text;
			text=NULL;
		}
		whiteBoard->Clear();
		whiteBoard->Update();
	}
	else if(tp==Final)
	{
		cout<<"Done with display, erasing whiteboard"<<endl;
		if(origHist!=NULL)
		{
			delete origHist;
			origHist=NULL;
		}
		if(lowHist!=NULL)
		{
			delete lowHist;
			lowHist=NULL;
		}
		if(midHist!=NULL)
		{
			delete midHist;
			midHist=NULL;
		}
		if(hiHist!=NULL)
		{
			delete hiHist;
			hiHist=NULL;
		}
		if(regions!=NULL)
		{
			delete regions;
			regions=NULL;
		}
		if(text!=NULL)
		{
			delete text;
			text=NULL;
		}
		whiteBoard->Clear();
		whiteBoard->Update();
		return;
		break;	
	}
	//retrieve the cuts and the tree
	origHist = testBGSpec(runs[dispNum].runNumber);
	regions = testBGGraph(runs[dispNum].runNumber);
	//test to make certain that everything is there
	if(origHist==NULL || regions==NULL)
	{
		cout<<"Backing out of display mode"<<endl;
		dispNum = 0;
		dispFunc = None;
		updateBGDisp(Final);//to cleanup static vars
		return;
	}
	//otherwise draw all
	whiteBoard->Divide(2,2);
	whiteBoard->cd(1);
	//first draw the background spectrum with the three regions higlighted
	//first get the x values of the graph
	double* xVals = regions->GetX();
	//next make and setup the three clones
	lowHist = (TH1F*)origHist->Clone("lowBGSpec");
	lowHist->GetXaxis()->SetRange(lowHist->FindBin(xVals[0]),lowHist->FindBin(xVals[1]));
	lowHist->SetFillColor(kCyan);
	hiHist = (TH1F*)origHist->Clone("hiBGSpec");
	hiHist->GetXaxis()->SetRange(lowHist->FindBin(xVals[2]),lowHist->FindBin(xVals[3]));
	hiHist->SetFillColor(kCyan);
	midHist = (TH1F*)origHist->Clone("trueSpec");
	midHist->GetXaxis()->SetRange(lowHist->FindBin(xVals[1]),lowHist->FindBin(xVals[2]));
	midHist->SetFillColor(kGreen);
	origHist->Draw();
	gPad->SetLogy(1);
	lowHist->Draw("same");
	midHist->Draw("same");
	hiHist->Draw("same");
	regions->Draw("same");
	whiteBoard->Update();
	whiteBoard->cd(2);
	origHist->Draw();
	gPad->SetLogy(1);
	lowHist->Draw("same");
	hiHist->Draw("same");
	regions->Draw("same");
	whiteBoard->Update();
	whiteBoard->cd(3);
	origHist->Draw();
	gPad->SetLogy(1);
	midHist->Draw("same");
	regions->Draw("same");
	whiteBoard->cd(4);
	text = new TPaveText(0.1,0.1,0.9,0.9,"NDC");
	ostringstream texBuilder;
	double bgWidth = ( (xVals[1]-xVals[0]) + (xVals[3]-xVals[2]) );
	double trWidth = ( xVals[2]-xVals[1] );
	texBuilder<<"The true region with is: "<<trWidth;
	text->AddText(texBuilder.str().c_str());
	texBuilder.str("");
	texBuilder<<"The bgnd region with is: "<<bgWidth;
	text->AddText(texBuilder.str().c_str());
	texBuilder.str("");
	texBuilder<<"The normalization is:    "<<(trWidth/bgWidth);
	text->AddText(texBuilder.str().c_str());
	text->Draw();
	whiteBoard->Update();
	whiteBoard->cd(1);
}

void MainWindow::updateShapeDisp(const UpdateCallType& tp)
{
	static TH2F* origShape;
	static TH2F* newShape;
	if(tp==Initial)
	{
		origShape = NULL;
		newShape = NULL;
		whiteBoard->Clear();
		whiteBoard->Update();
	}
	else if(tp==Normal)
	{
		if(origShape!=NULL)
		{
			delete origShape;
			origShape=NULL;
		}
		if(newShape!=NULL)
		{
			delete newShape;
			newShape=NULL;
		}
		whiteBoard->Clear();
		whiteBoard->Update();
	}
	else if(tp==Final)
	{
		cout<<"Done with display, erasing whiteboard"<<endl;
		if(origShape!=NULL)
		{
			delete origShape;
			origShape=NULL;
		}
		if(newShape!=NULL)
		{
			delete newShape;
			newShape=NULL;
		}
		whiteBoard->Clear();
		whiteBoard->Update();
		return;
		break;	
	}
	
	origShape = testOrigShapeSpec(runs[dispNum].runNumber);
	newShape = testFirstOrderShapeSpec(runs[dispNum].runNumber);
	
	if(origShape==NULL || newShape==NULL)
	{	
		cout<<"Backing out of display mode"<<endl;
		dispNum = 0;
		dispFunc = None;
		updateShapeDisp(Final);//to cleanup static vars
		return;
	}
	
	whiteBoard->Divide(2,1);
	whiteBoard->cd(1);
	origShape->Draw("colz");
	gPad->SetLogy(0);
	gPad->SetLogz(1);
	whiteBoard->cd(2);
	gPad->SetLogy(0);
	newShape->Draw("colz");
	gPad->SetLogz(1);
	whiteBoard->Update();
}

void MainWindow::updateSubDisp(const UpdateCallType& tp)
{
	static TH2F* trueSpec;
	static TH2F* bgSpec;
	static TH2F* scaledBgSpec;
	static TH2F* subSpec;
	if(tp==Initial)
	{
		trueSpec = NULL;
		bgSpec = NULL;
		scaledBgSpec = NULL;
		subSpec = NULL;
		whiteBoard->Clear();
		whiteBoard->Update();
	}
	else if(tp==Normal)
	{
		if(trueSpec!=NULL)
		{
			delete trueSpec;
			trueSpec=NULL;
		}
		if(bgSpec!=NULL)
		{
			delete bgSpec;
			bgSpec=NULL;
		}
		if(scaledBgSpec!=NULL)
		{
			delete scaledBgSpec;
			scaledBgSpec=NULL;
		}
		if(subSpec!=NULL)
		{
			delete subSpec;
			subSpec=NULL;
		}
		whiteBoard->Clear();
		whiteBoard->Update();
	}
	else if(tp==Final)
	{
		cout<<"Done with display, erasing whiteboard"<<endl;
		if(trueSpec!=NULL)
		{
			delete trueSpec;
			trueSpec=NULL;
		}
		if(bgSpec!=NULL)
		{
			delete bgSpec;
			bgSpec=NULL;
		}
		if(scaledBgSpec!=NULL)
		{
			delete scaledBgSpec;
			scaledBgSpec=NULL;
		}
		if(subSpec!=NULL)
		{
			delete subSpec;
			subSpec=NULL;
		}
		whiteBoard->Clear();
		whiteBoard->Update();
		return;
		break;	
	}
	
	int runN = runs[dispNum].runNumber;
	
	//retrieve the spectra
	trueSpec = testTRpBGSpec( runN );
	bgSpec = testBGOnlySpec( runN );
	scaledBgSpec = testScaledBGSpec( runN );
	subSpec = testSubSpec( runN );
	
	if(	trueSpec == NULL || bgSpec == NULL || scaledBgSpec == NULL || subSpec == NULL )
	{	
		cout<<"Backing out of display mode"<<endl;
		dispNum = 0;
		dispFunc = None;
		updateSubDisp(Final);//to cleanup static vars
		return;
	}
	
	//now draw everything
	whiteBoard->Divide(2,2);
	whiteBoard->cd(1);
	trueSpec->Draw("colz");
	gPad->SetLogy(0);
	gPad->SetLogz(1);
	whiteBoard->cd(2);
	bgSpec->Draw("colz");
	gPad->SetLogy(0);
	gPad->SetLogz(1);
	whiteBoard->cd(3);
	scaledBgSpec->Draw("colz");
	gPad->SetLogy(0);
	gPad->SetLogz(1);
	whiteBoard->cd(4);
	subSpec->Draw("colz");
	gPad->SetLogy(0);
	gPad->SetLogz(1);
	whiteBoard->Update();
}

void MainWindow::grabBasicCsInfoSimple(const UpdateCallType& tp)
{
	switch(tp)
	{
		case Initial:
			numBnds = 0;
			basicInfoGrabber->setBasicName();
			basicInfoGrabber->show();
			break;
		case Normal:
			cout<<"Press End Dsiplay to record these parameters"<<endl;
			break;
		case Final:
			for( int i=0; i<numRuns; ++i)
			{
				basicInfoGrabber->getVals((csBnds+i));
			}
			cout<<"These parameters are recorded for all runs"<<endl;
			numBnds = numRuns;
			basicInfoGrabber->hide();
			break;
	}
}

	BasicCSDialog* basicInfoGrabber;
	CSBounds* csBnds;
	int numBnds;

void MainWindow::grabBasicCsInfoPerRun(const UpdateCallType& tp)
{
	static int prevRun;
	static TH2F* subSpec;
	switch(tp)
	{
		case Initial:
			numBnds = 0;
			prevRun = 0;
			basicInfoGrabber->setRunName(runs[0].runNumber);
			basicInfoGrabber->show();
			break;
		case Normal:
			if(dispNum == (prevRun - 1))
			{//we went backwards put the values in the widget and set the title
				basicInfoGrabber->setVals( (csBnds+dispNum) );
				basicInfoGrabber->setRunName(runs[dispNum].runNumber);
			}
			else if (dispNum == (prevRun + 1))
			{//we went forward get the values from the widget and set the title
				basicInfoGrabber->getVals( (csBnds+prevRun) );
				basicInfoGrabber->setRunName(runs[dispNum].runNumber);
			}
			prevRun = dispNum;
			if(subSpec!=NULL)
			{
				delete subSpec;
				subSpec=NULL;
			}
			whiteBoard->Clear();
			whiteBoard->Update();
			break;
		case Final:
			basicInfoGrabber->getVals( (csBnds+dispNum) );
			basicInfoGrabber->hide();
			numBnds = (dispNum + 1);
			if(subSpec!=NULL)
			{
				delete subSpec;
				subSpec=NULL;
			}
			whiteBoard->Clear();
			whiteBoard->Update();
			return;
			break;
	}
	
	subSpec = testSubSpec( runs[dispNum].runNumber );
	
	if(	subSpec == NULL )
	{	
		cout<<"Spectrum display not possible for this run due to missing spectrum"<<endl;
		cout<<"You will need to fix this before running the cross-section grabber"<<endl;
	}
	else
	{
		subSpec->Draw("colz");
		gPad->SetLogy(0);
		gPad->SetLogz(1);
		whiteBoard->Update();
	}
}

void MainWindow::updateDisplay(const UpdateCallType& tp)
{
	switch(dispFunc)
	{
		case PIDCut:
			updatePIDDisp(tp);
			break;
		case BGCut:
			updateBGDisp(tp);
			break;
		case ShapeDisp:
			updateShapeDisp(tp);
			break;
		case SubbedSpecs:
			updateSubDisp(tp);
			break;
		case BasicCSInfoSimple:
			grabBasicCsInfoSimple(tp);
			break;
		case BasicCSInfoPerRun:
			grabBasicCsInfoPerRun(tp);
			break;
		case None:
			cout<<"In update display with no display function"<<endl;
			cout<<"This should not be possible"<<endl;
			break;
		default:
			cout<<"In update display with an invalid display function"<<endl;
			cout<<"This should not be possible"<<endl;
			break;
	}
}

void MainWindow::prevSeqSpec()
{
	if (dispFunc == None)
	{
		cout<<"Not in display mode"<<endl;
		return;
	}
	
	if(dispNum == 0)
	{
		cout<<"No previous spectrum to display"<<endl;
		return;
	}
	else
	{
		--dispNum;
		updateDisplay(Normal);
		return;
	}
}


void MainWindow::nextSeqSpec()
{
	if (dispFunc == None)
	{
		cout<<"Not in display mode"<<endl;
		return;
	}
	
	if(dispNum == (numRuns -1))
	{
		cout<<"No next spectrum to display"<<endl;
		return;
	}
	else
	{
		++dispNum;
		updateDisplay(Normal);
		return;
	}
}


void MainWindow::endSeqSpec()
{
	if (dispFunc == None)
	{
		cout<<"Not in display mode"<<endl;
		return;
	}
	else
	{
		updateDisplay(Final);
		dispNum = 0;
		dispFunc = None;
		return;
	}
}

/******************************************
*******************************************
** Overall Control operations
*******************************************
******************************************/
void MainWindow::resetToStart()
{
	if ( runs == NULL && mainFile == NULL && auxFile==NULL && frFile==NULL)
	{
		cout<<"\nNothing To Reset"<<endl;
		return;
	}
	
	if(runs != NULL)
	{
		delete[] runs;
		runs = NULL;
		numRuns=0;
	}
	if(csBnds != NULL)
	{
		delete[] csBnds;
		csBnds = NULL;
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
	dispNum = 0;
	dispFunc = None;
	cout<<"Reset to initial state"<<endl;
	
}

void MainWindow::exitApp()
{
	cout<<"\nGoodbye!\n"<<endl;
	gApplication->Terminate(0);
}



/******************************************
*******************************************
** Private helper functions
*******************************************
******************************************/
string MainWindow::makeTreeName(int runN)
{
	ostringstream treeNamer;
	treeNamer<<"run"<<runN<<"Tree";
	string temp = treeNamer.str();
	return temp;
}

TTree* MainWindow::retrieveTree(int runN)
{
	string treeName = makeTreeName(runN);
	return reinterpret_cast<TTree*>(mainFile->Get(treeName.c_str()));
}

TTree* MainWindow::testTree(int runN)
{
	TTree* temp = retrieveTree(runN);
	if( temp==NULL )
	{
		cout<<"Missing a raw data tree, you might have the wrong combined file loaded"<<endl;
		return NULL;
	}
	else
	{
		return temp;
	}
}


string MainWindow::makePIDCutName(int runN)
{
	ostringstream cutNamer;
	cutNamer<<"pidCut"<<runN;
	string temp = cutNamer.str();
	return temp;
}

TCutG* MainWindow::retrievePIDCut(int runN)
{
	string cName = makePIDCutName(runN);
	return reinterpret_cast<TCutG*>(auxFile->Get(cName.c_str()));
}

TCutG* MainWindow::testPIDCut(int runN)
{
	TCutG* temp = retrievePIDCut(runN);
	if( temp==NULL )
	{
		cout<<"Missing a PID cut, you might have the wrong aux file loaded"<<endl;
		cout<<"or you might have forgotten to run Get PID Cut(s)"<<endl;
		return NULL;
	}
	else
	{
		return temp;
	}
}


string MainWindow::makePIDSpecName(int runN)
{
	ostringstream histNamer;
	histNamer<<"h2PIDRun"<<runN;
	string temp = histNamer.str();
	return temp;
}

TH2F* MainWindow::retrievePIDSpec(int runN)
{
	string histName = makePIDSpecName(runN);
	return reinterpret_cast<TH2F*>(auxFile->Get(histName.c_str()));
}

TH2F* MainWindow::testPIDSpec(int runN)
{
	TH2F* temp = retrievePIDSpec(runN);
	if( temp==NULL )
	{
		cout<<"Missing a PID spectrum, you might have the wrong aux file loaded"<<endl;
		cout<<"or you might have forgotten to run Get PID Cut(s)"<<endl;
		return NULL;
	}
	else
	{
		return temp;
	}
}

string MainWindow::makeBaseCutName(int runN)
{
	ostringstream cutNamer;
	cutNamer<<"baseCut"<<runN;
	string temp = cutNamer.str();
	return temp;
}

TCut* MainWindow::retrieveBaseCut(int runN)
{
	string cName = makeBaseCutName(runN);
	return reinterpret_cast<TCut*>(auxFile->Get(cName.c_str()));
}

TCut* MainWindow::testBaseCut(int runN)
{
	TCut* temp = retrieveBaseCut(runN);
	if( temp==NULL )
	{
		cout<<"Missing a base cut, you might have the wrong aux file loaded"<<endl;
		cout<<"or you might have forgotten to run Get PID Cut(s)"<<endl;
		return NULL;
	}
	else
	{
		return temp;
	}
}

string MainWindow::makeBGGraphName(int runN)
{
	ostringstream graphNamer;
	graphNamer<<"bgGraph"<<runN;
	string temp = graphNamer.str();
	return temp;
}

TGraph* MainWindow::retrieveBGGraph(int runN)
{
	string gName = makeBGGraphName(runN);
	return reinterpret_cast<TGraph*>(auxFile->Get(gName.c_str()));
}

TGraph* MainWindow::testBGGraph(int runN)
{
	TGraph* temp = retrieveBGGraph(runN);
	if( temp==NULL )
	{
		cout<<"Missing a Yfp positions definition, you might have the wrong aux file loaded"<<endl;
		cout<<"or you might have forgotten to run Get BG Cut(s)"<<endl;
		return NULL;
	}
	else
	{
		return temp;
	}
}

string MainWindow::makeBGCutName(int runN)
{
	ostringstream cutNamer;
	cutNamer<<"bgCut"<<runN;
	string temp = cutNamer.str();
	return temp;
}

TCut* MainWindow::retrieveBGCut(int runN)
{
	string cName = makeBGCutName(runN);
	return reinterpret_cast<TCut*>(auxFile->Get(cName.c_str()));
}

TCut* MainWindow::testBGCut(int runN)
{
	TCut* temp = retrieveBGCut(runN);
	if( temp==NULL )
	{
		cout<<"Missing a BG Region Cut, you might have the wrong aux file loaded"<<endl;
		cout<<"or you might have forgotten to run Get BG Cut(s)"<<endl;
		return NULL;
	}
	else
	{
		return temp;
	}
}

string MainWindow::makeTRCutName(int runN)
{
	ostringstream cutNamer;
	cutNamer<<"trCut"<<runN;
	string temp = cutNamer.str();
	return temp;
}

TCut* MainWindow::retrieveTRCut(int runN)
{
	string cName = makeTRCutName(runN);
	return reinterpret_cast<TCut*>(auxFile->Get(cName.c_str()));
}

TCut* MainWindow::testTRCut(int runN)
{
	TCut* temp = retrieveTRCut(runN);
	if( temp==NULL )
	{
		cout<<"Missing a True+BG Region Cut, you might have the wrong aux file loaded"<<endl;
		cout<<"or you might have forgotten to run Get BG Cut(s)"<<endl;
		return NULL;
	}
	else
	{
		return temp;
	}
}

string MainWindow::makeBGSpecName(int runN)
{
	ostringstream bgNamer;
	bgNamer<<"h1YFPRun"<<runN;
	string temp = bgNamer.str();
	return temp;
}

TH1F* MainWindow::retrieveBGSpec(int runN)
{
	string histName = makeBGSpecName(runN);
	return reinterpret_cast<TH1F*>(auxFile->Get(histName.c_str()));
}

TH1F* MainWindow::testBGSpec(int runN)
{
	TH1F* temp = retrieveBGSpec(runN);
	if( temp==NULL )
	{
		cout<<"Missing a background spectrum, you might have the wrong aux file loaded"<<endl;
		cout<<"or you might have forgotten to run Get BG Cut(s)"<<endl;
		return NULL;
	}
	else
	{
		return temp;
	}
}

string MainWindow::makeOrigShapeSpecName(int runN)
{
	ostringstream bgNamer;
	bgNamer<<"h2OrigShapeSpec"<<runN;
	string temp = bgNamer.str();
	return temp;
}

TH2F* MainWindow::retrieveOrigShapeSpec(int runN)
{
	string histName = makeOrigShapeSpecName(runN);
	return reinterpret_cast<TH2F*>(auxFile->Get(histName.c_str()));
}

TH2F* MainWindow::testOrigShapeSpec(int runN)
{
	TH2F* temp = retrieveOrigShapeSpec(runN);
	if( temp==NULL )
	{
		cout<<"Missing an original shape spectrum, you might have the wrong aux file loaded"<<endl;
		return NULL;
	}
	else
	{
		return temp;
	}
}

string MainWindow::makeFirstOrderShapeSpecName(int runN)
{
	ostringstream bgNamer;
	bgNamer<<"h2FirstOrderShapeSpec"<<runN;
	string temp = bgNamer.str();
	return temp;
}

TH2F* MainWindow::retrieveFirstOrderShapeSpec(int runN)
{
	string histName = makeFirstOrderShapeSpecName(runN);
	return reinterpret_cast<TH2F*>(auxFile->Get(histName.c_str()));
}

TH2F* MainWindow::testFirstOrderShapeSpec(int runN)
{
	TH2F* temp = retrieveFirstOrderShapeSpec(runN);
	if( temp==NULL )
	{
		cout<<"Missing a first order shape spectrum, you might have the wrong aux file loaded"<<endl;
		return NULL;
	}
	else
	{
		return temp;
	}
}

string MainWindow::makeFirstOrderShapeTreeName(int runN)
{
	ostringstream bgNamer;
	bgNamer<<"run"<<runN<<"_tree_shape1";
	string temp = bgNamer.str();
	return temp;
}

TTree* MainWindow::retrieveFirstOrderShapeTree(int runN)
{
	string histName = makeFirstOrderShapeTreeName(runN);
	return reinterpret_cast<TTree*>(frFile->Get(histName.c_str()));
}

TTree* MainWindow::testFirstOrderShapeTree(int runN)
{
	TTree* temp = retrieveFirstOrderShapeTree(runN);
	if( temp==NULL )
	{
		cout<<"Missing a first order shape tree, you might have the wrong friend file loaded"<<endl;
		return NULL;
	}
	else
	{
		return temp;
	}
}

string MainWindow::makeTRpBGSpecName(int runN)
{
	ostringstream bgNamer;
	bgNamer<<"h2TrpBG"<<runN;
	string temp = bgNamer.str();
	return temp;
}

TH2F* MainWindow::retrieveTRpBGSpec(int runN)
{
	string histName = makeTRpBGSpecName(runN);
	return reinterpret_cast<TH2F*>(auxFile->Get(histName.c_str()));
}

TH2F* MainWindow::testTRpBGSpec(int runN)
{
	TH2F* temp = retrieveTRpBGSpec(runN);
	if( temp==NULL )
	{
		cout<<"Missing a True+BG Theta vs Xfp spectrum, you might have the wrong aux file loaded"<<endl;
		return NULL;
	}
	else
	{
		return temp;
	}
}

string MainWindow::makeBGOnlySpecName(int runN)
{
	ostringstream bgNamer;
	bgNamer<<"h2BgOnly"<<runN;
	string temp = bgNamer.str();
	return temp;
}

TH2F* MainWindow::retrieveBGOnlySpec(int runN)
{
	string histName = makeBGOnlySpecName(runN);
	return reinterpret_cast<TH2F*>(auxFile->Get(histName.c_str()));
}

TH2F* MainWindow::testBGOnlySpec(int runN)
{
	TH2F* temp = retrieveBGOnlySpec(runN);
	if( temp==NULL )
	{
		cout<<"Missing a BG Theta vs Xfp spectrum, you might have the wrong aux file loaded"<<endl;
		return NULL;
	}
	else
	{
		return temp;
	}
}

string MainWindow::makeScaledBGSpecName(int runN)
{
	ostringstream bgNamer;
	bgNamer<<"h2ScaledBg"<<runN;
	string temp = bgNamer.str();
	return temp;
}

TH2F* MainWindow::retrieveScaledBGSpec(int runN)
{
	string histName = makeScaledBGSpecName(runN);
	return reinterpret_cast<TH2F*>(auxFile->Get(histName.c_str()));
}

TH2F* MainWindow::testScaledBGSpec(int runN)
{
	TH2F* temp = retrieveScaledBGSpec(runN);
	if( temp==NULL )
	{
		cout<<"Missing a BG Theta vs Xfp spectrum, you might have the wrong aux file loaded"<<endl;
		return NULL;
	}
	else
	{
		return temp;
	}
}

string MainWindow::makeSubSpecName(int runN)
{
	ostringstream bgNamer;
	bgNamer<<"h2SubSpec"<<runN;
	string temp = bgNamer.str();
	return temp;
}

TH2F* MainWindow::retrieveSubSpec(int runN)
{
	string histName = makeSubSpecName(runN);
	return reinterpret_cast<TH2F*>(auxFile->Get(histName.c_str()));
}

TH2F* MainWindow::testSubSpec(int runN)
{
	TH2F* temp = retrieveSubSpec(runN);
	if( temp==NULL )
	{
		cout<<"Missing a Subtracted Theta vs Xfp spectrum, you might have the wrong aux file loaded"<<endl;
		return NULL;
	}
	else
	{
		return temp;
	}
}

double MainWindow::calculateBGNorm(TGraph* regions)
{
	//first get the x values of the graph
	double* xVals = regions->GetX();
	double bgWidth = ( (xVals[1]-xVals[0]) + (xVals[3]-xVals[2]) );
	double trWidth = ( xVals[2]-xVals[1] );
	return (trWidth/bgWidth);
}


//returns d(sigma)/d(Omega) for that number of counts and rund data and angular region
double MainWindow::calcCrossSection(double counts, const RunData& data, double thWidth, double phWidth)
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

bool MainWindow::checkUpToRunData()
{
	if ( runs == NULL )
	{
		cout<<"\nLoad Run Data first"<<endl;
		return false;
	}
	else if (dispFunc != None)
	{
		cout<<"\nEnd display mode first"<<endl;
		return false;
	}
	return true;
}

bool MainWindow::checkUpToMainFile()
{
	if( !checkUpToRunData() )
	{
		return false;
	}
	else if ( mainFile == NULL )
	{
		cout<<"\nLoad/create combined file first"<<endl;
		return false;
	}
	return true;
}

bool MainWindow::checkUpToAuxFile()
{
	if( !checkUpToMainFile() )
	{
		return false;
	}
	else if ( auxFile == NULL )
	{
		cout<<"\nLoad/create auxiliary file first"<<endl;
		return false;
	}
	return true;
}

bool MainWindow::checkUpToFrFile()
{
	if( !checkUpToAuxFile() )
	{
		return false;
	}
	else if( frFile == NULL )
	{
		cout<<"\nLoad/create a friend file first"<<endl;
		return false;
	}
	return true;
}

void MainWindow::parseRunFileLine(const string& line, RunData& tempData)
{
	istringstream conv;
	//read the run number
	int ind = line.find(',');
	string temp = line.substr(0,ind);
	string tempLine = line.substr(ind+1);
	conv.str(temp);
	conv>>tempData.runNumber;
	//cout<<tempData.runNumber<<" ";
	conv.clear();
	//read the angle
	ind = tempLine.find(',');
	temp = tempLine.substr(0,ind);
	tempLine = tempLine.substr(ind+1);
	conv.str(temp);
	conv>>tempData.angle;
	//cout<<tempData.angle<<" ";
	conv.clear();
	//read the beam energy 
	ind = tempLine.find(',');
	temp = tempLine.substr(0,ind);
	tempLine = tempLine.substr(ind+1);
	conv.str(temp);
	conv>>tempData.beamEn;
	//cout<<tempData.beamEn<<" ";
	conv.clear();
	//read the Charge State
	ind = tempLine.find(',');
	temp = tempLine.substr(0,ind);
	tempLine = tempLine.substr(ind+1);
	conv.str(temp);
	conv>>tempData.chargeState;
	//cout<<tempData.chargeState<<" ";
	conv.clear();
	//read the projectile mass
	ind = tempLine.find(',');
	temp = tempLine.substr(0,ind);
	tempLine = tempLine.substr(ind+1);
	conv.str(temp);
	conv>>tempData.projMass;
	//cout<<tempData.projMass<<" ";
	conv.clear();
	//read the target mass
	ind = tempLine.find(',');
	temp = tempLine.substr(0,ind);
	tempLine = tempLine.substr(ind+1);
	conv.str(temp);
	conv>>tempData.targetMass;
	//cout<<tempData.targetMass<<" ";
	conv.clear();
	//read the target thickness
	ind = tempLine.find(',');
	temp = tempLine.substr(0,ind);
	tempLine = tempLine.substr(ind+1);
	conv.str(temp);
	conv>>tempData.thickness;
	//cout<<tempData.thickness<<" ";
	conv.clear();
	//read the beam integral
	ind = tempLine.find(',');
	temp = tempLine.substr(0,ind);
	tempLine = tempLine.substr(ind+1);
	conv.str(temp);
	conv>>tempData.beamInt;
	//cout<<tempData.beamInt<<" ";
	conv.clear();
	//read the BI range
	ind = tempLine.find(',');
	temp = tempLine.substr(0,ind);
	tempLine = tempLine.substr(ind+1);
	conv.str(temp);
	conv>>tempData.biRange;
	//cout<<tempData.biRange<<" ";
	conv.clear();
	//read the GR request
	ind = tempLine.find(',');
	temp = tempLine.substr(0,ind);
	tempLine = tempLine.substr(ind+1);
	conv.str(temp);
	conv>>tempData.grRequest;
	//cout<<tempData.grRequest<<" ";
	conv.clear();
	//read the GR accept
	ind = tempLine.find(',');
	temp = tempLine.substr(0,ind);
	tempLine = tempLine.substr(ind+1);
	conv.str(temp);
	conv>>tempData.grAccept;
	//cout<<tempData.grAccept<<" ";
	conv.clear();
	//read the vdc effeciency
	ind = tempLine.find(',');
	temp = tempLine.substr(0,ind);
	tempLine = tempLine.substr(ind+1);
	conv.str(temp);
	conv>>tempData.vdcEff;
	//cout<<tempData.vdcEff<<endl;
	conv.clear();
}

#endif
