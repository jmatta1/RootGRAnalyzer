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
#include<TGLabel.h>
#include<TH2F.h>
#include<TH1F.h>
#include<TGraph.h>
#include<TStyle.h>
#include<TColor.h>
#include<TPaveText.h>

//my includes
#include"guiSupport.h"

enum DisplayFunction{None, PIDCut, BGCut};
enum UpdateCallType{ Initial, Normal, Final};

TCut RayCut = "Rayid==0";

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

	//sequential spectrum display
	void prevSeqSpec();
	void nextSeqSpec();
	void endSeqSpec();

	//overall control operations
	void makeTreeFileActive(){mainFile->cd();}
	void makeAuxFileActive(){auxFile->cd();}
	void makeFriendFileActive(frFile->cd();}
	void resetToStart();
	void exitApp();
	
private:
	//some private helper functions
	void parseRunFileLine(const string& line, RunData& tempData);
	
	//functions for constructing names of various constructs
	string makeTreeName(int runN);
	string makePIDCutName(int runN);
	string makePIDSpecName(int runN);
	string makeBaseCutName(int runN);
	string makeBGGraphName(int runN);
	string makeBGCutName(int runN);
	string makeTRCutName(int runN);
	string makeBGSpecName(int runN);

	//functions for retrieving various things from files
	TTree* retrieveTree(int runN);
	TCutG* retrievePIDCut(int runN);
	TH2F* retrievePIDSpec(int runN);
	TCut* retrieveBaseCut(int runN);
	TGraph* retrieveBGGraph(int runN);
	TCut* retrieveBGCut(int runN);
	TCut* retrieveTRCut(int runN);
	TH2F* retrieveBGSpec(int runN);

	//display functions for sequential displays
	void updateDisplay(const UpdateCallType& tp);
	void updatePIDDisp(const UpdateCallType& tp);
	void updateBGDisp(const UpdateCallType& tp);

	//Gui stuff
	TGMainFrame *menu;
	TCanvas *whiteBoard;
	
	//sequential display stuff
	int dispNum;
	DisplayFunction dispFunc;
	
	//analysis stuff
	RunData* runs;
	int numRuns;
	TFile* mainFile;
	TFile* auxFile;
	TFile* frFile;
};

MainWindow::MainWindow(const TGWindow* parent, UInt_t width, UInt_t height)
{
	runs=NULL;
	numRuns=0;
	mainFile=NULL;
	auxFile=NULL;
	frFile=NULL;
	dispNum = 0;
	dispFunc = None;

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
	fileFrame->AddFrame(fileLabel, new TGLayoutHints(kLHintsCenterX,5,5,3,4));
	//read run data button
	TGTextButton *rdRunData = new TGTextButton(fileFrame,"Read Run Data");
	rdRunData->Connect("Clicked()","MainWindow",this,"readRunData()");
	fileFrame->AddFrame(rdRunData, new TGLayoutHints(kLHintsCenterX,5,5,3,4));
	
	//build combined root file
	TGTextButton *buildFile = new TGTextButton(fileFrame,"Build Combined Root File");
	buildFile->Connect("Clicked()","MainWindow",this,"buildBigFile()");
	fileFrame->AddFrame(buildFile, new TGLayoutHints(kLHintsCenterX,5,5,3,4));
	
	//open combined root file
	TGTextButton *opBigFile = new TGTextButton(fileFrame,"Open Combined Root File");
	opBigFile->Connect("Clicked()","MainWindow",this,"openBigFile()");
	fileFrame->AddFrame(opBigFile, new TGLayoutHints(kLHintsCenterX,5,5,3,4));
	
	//CreateAuxFile button
	TGTextButton *createAuxFile = new TGTextButton(fileFrame,"Make Auxilliary File");
	createAuxFile->Connect("Clicked()","MainWindow",this,"makeAuxFile()");
	fileFrame->AddFrame(createAuxFile, new TGLayoutHints(kLHintsCenterX,5,5,3,4));
	
	//Open Aux File button
	TGTextButton *opAuxFile = new TGTextButton(fileFrame,"Open Auxilliary File");
	opAuxFile->Connect("Clicked()","MainWindow",this,"openAuxFile()");
	fileFrame->AddFrame(opAuxFile, new TGLayoutHints(kLHintsCenterX,5,5,3,4));
	
	//Create friend file button
	TGTextButton *createFrFile = new TGTextButton(fileFrame,"Make Friend Tree File");
	createFrFile->Connect("Clicked()","MainWindow",this,"makeFriendFile()");
	fileFrame->AddFrame(createFrFile, new TGLayoutHints(kLHintsCenterX,5,5,3,4));
	
	//Open friend file button
	TGTextButton *opFrFile = new TGTextButton(fileFrame,"Open Friend Tree File");
	opFrFile->Connect("Clicked()","MainWindow",this,"openFriendFile()");
	fileFrame->AddFrame(opFrFile, new TGLayoutHints(kLHintsCenterX,5,5,3,4));
	
	//add the file frame to the overall frame
	overallFrame->AddFrame(fileFrame,new TGLayoutHints(kLHintsCenterX,2,2,2,2));
	
	/******************************************
	** Cut operations buttons
	******************************************/
	TGVerticalFrame *cutFrame = new TGVerticalFrame(overallFrame, width, height);
	TGLabel* cutLabel = new TGLabel(cutFrame, "Cut and Shape Operations");
	cutFrame->AddFrame(cutLabel, new TGLayoutHints(kLHintsCenterX,5,5,3,4));
	//Get PIDs button
	TGTextButton *getPIDs = new TGTextButton(cutFrame,"Get PID Cut(s)");
	getPIDs->Connect("Clicked()","MainWindow",this,"getPIDCuts()");
	cutFrame->AddFrame(getPIDs, new TGLayoutHints(kLHintsCenterX,5,5,3,4));
	
	//display PIDs button
	TGTextButton *dispPID = new TGTextButton(cutFrame,"Show PID Cut(s)");
	dispPID->Connect("Clicked()","MainWindow",this,"showPIDCut()");
	cutFrame->AddFrame(dispPID, new TGLayoutHints(kLHintsCenterX,5,5,3,4));
	
	//Get BG Cuts Button
	TGTextButton *getBG = new TGTextButton(cutFrame,"Get BG Cut(s)");
	getBG->Connect("Clicked()","MainWindow",this,"getBGCuts()");
	cutFrame->AddFrame(getBG, new TGLayoutHints(kLHintsCenterX,5,5,3,4));
	
	//Display BG Cuts Button
	TGTextButton *dispBG = new TGTextButton(cutFrame,"Show BG Cut(s)");
	dispBG->Connect("Clicked()","MainWindow",this,"showBGCut()");
	cutFrame->AddFrame(dispBG, new TGLayoutHints(kLHintsCenterX,5,5,3,4));
	
	
	/******************************************
	** Shape operations buttons
	******************************************/
	//Get shapes button
	TGTextButton *getShapesBut = new TGTextButton(cutFrame,"Get 1st Order Shapes(s)");
	getShapesBut->Connect("Clicked()","MainWindow",this,"getFirstOrdShapes()");
	cutFrame->AddFrame(getShapesBut, new TGLayoutHints(kLHintsCenterX,5,5,3,4));
	
	//display shapes button
	TGTextButton *showShapesBut = new TGTextButton(cutFrame,"Show 1st Order Shapes(s)");
	showShapesBut->Connect("Clicked()","MainWindow",this,"showFirstOrdShapes()");
	cutFrame->AddFrame(showShapesBut, new TGLayoutHints(kLHintsCenterX,5,5,3,4));
	
	//add the cut frame to the overall frame
	overallFrame->AddFrame(cutFrame,new TGLayoutHints(kLHintsCenterX,2,2,2,2));
	
	
	//add the overall frame to the full frame	
	fullFrame->AddFrame(overallFrame,new TGLayoutHints(kLHintsCenterX,2,2,2,2));
	
	
	
	/******************************************
	** Sequential Display buttons
	******************************************/
	TGHorizontalFrame *extraFrame = new TGHorizontalFrame(fullFrame, width, height);
	TGVerticalFrame *dispFrame = new TGVerticalFrame(extraFrame, width, height);
	TGLabel* auxLabel = new TGLabel(dispFrame, "Sequential Display Buttons");
	dispFrame->AddFrame(auxLabel, new TGLayoutHints(kLHintsCenterX,5,5,3,4));
	
	TGHorizontalFrame *auxButtonsFrame = new TGHorizontalFrame(dispFrame, width, height);
	//show previous spectrum
	TGTextButton *prevSpec = new TGTextButton(auxButtonsFrame,"Prev Spec");
	prevSpec->Connect("Clicked()","MainWindow",this,"prevSeqSpec()");
	auxButtonsFrame->AddFrame(prevSpec, new TGLayoutHints(kLHintsCenterX,5,5,3,4));
	//show next spectrum
	TGTextButton *nextSpec = new TGTextButton(auxButtonsFrame,"Next Spec");
	nextSpec->Connect("Clicked()","MainWindow",this,"nextSeqSpec()");
	auxButtonsFrame->AddFrame(nextSpec, new TGLayoutHints(kLHintsCenterX,5,5,3,4));
	//end display
	TGTextButton *endSpec = new TGTextButton(auxButtonsFrame,"End Display");
	endSpec->Connect("Clicked()","MainWindow",this,"endSeqSpec()");
	auxButtonsFrame->AddFrame(endSpec, new TGLayoutHints(kLHintsCenterX,5,5,3,4));

	dispFrame->AddFrame(auxButtonsFrame,new TGLayoutHints(kLHintsCenterX,2,2,2,2));
	extraFrame->AddFrame(dispFrame,new TGLayoutHints(kLHintsCenterX,2,2,2,2));
	
	/******************************************
	** Overall Control buttons
	******************************************/
	TGVerticalFrame *ctrlFrame = new TGVerticalFrame(extraFrame, width, height);
	TGLabel* fancyLabel = new TGLabel(ctrlFrame, "Overall Control");
	ctrlFrame->AddFrame(fancyLabel, new TGLayoutHints(kLHintsCenterX,5,5,3,4));
	TGHorizontalFrame *overallControlFrame = new TGHorizontalFrame(ctrlFrame, width, height);
	//reset button
	TGTextButton *resBut = new TGTextButton(overallControlFrame,"Reset Setup");
	resBut->Connect("Clicked()","MainWindow",this,"resetToStart()");
	overallControlFrame->AddFrame(resBut, new TGLayoutHints(kLHintsCenterX,5,5,3,4));
	//Kill application button
	TGTextButton *exBut = new TGTextButton(overallControlFrame,"Exit");
	exBut->Connect("Clicked()","MainWindow",this,"exitApp()");
	overallControlFrame->AddFrame(exBut, new TGLayoutHints(kLHintsCenterX,5,5,3,4));
	
	ctrlFrame->AddFrame(overallControlFrame,new TGLayoutHints(kLHintsCenterX,2,2,2,2));
	extraFrame->AddFrame(ctrlFrame,new TGLayoutHints(kLHintsCenterX,2,2,2,2));
	
	fullFrame->AddFrame(extraFrame,new TGLayoutHints(kLHintsCenterX,2,2,2,2));
	
	/******************************************
	** File Control buttons
	******************************************/
	TGLabel* fileControlLabel = new TGLabel(fullFrame, "File Control (for debugging)");
	fullFrame->AddFrame(fileControlLabel, new TGLayoutHints(kLHintsCenterX,5,5,3,4));
	TGHorizontalFrame *fileControlFrame = new TGHorizontalFrame(fullFrame, width, height);
	//make tree file the current dir
	TGTextButton *treeFocusBut = new TGTextButton(fileControlFrame,"Activate Tree File");
	treeFocusBut->Connect("Clicked()","MainWindow",this,"makeTreeFileActive()");
	fileControlFrame->AddFrame(treeFocusBut, new TGLayoutHints(kLHintsCenterX,5,5,3,4));
	//make aux file the current dir
	TGTextButton *auxFocusBut = new TGTextButton(fileControlFrame,"Activate Aux File");
	auxFocusBut->Connect("Clicked()","MainWindow",this,"makeAuxFileActive()");
	fileControlFrame->AddFrame(auxFocusBut, new TGLayoutHints(kLHintsCenterX,5,5,3,4));
	//make friend file the current dir
	TGTextButton *frFocusBut = new TGTextButton(fileControlFrame,"Activate Friend File");
	frFocusBut->Connect("Clicked()","MainWindow",this,"makeFriendFileActive()");
	fileControlFrame->AddFrame(frFocusBut, new TGLayoutHints(kLHintsCenterX,5,5,3,4));
	
	fullFrame->AddFrame(fileControlFrame,new TGLayoutHints(kLHintsCenterX,2,2,2,2));
	
	//add the full frame to the window
	menu->AddFrame(fullFrame,new TGLayoutHints(kLHintsCenterX,2,2,2,2));
	
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
	delete menu;
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
}

void MainWindow::buildBigFile()
{
	if ( runs == NULL )
	{
		cout<<"\nLoadRun Data first"<<endl;
		return;
	}
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
	fileInfo.fFileTypes = rootFileType;
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
	if( (temp.size()-5) != ( temp.rfind(".root") ) )
	{
		temp.append(".root");
	}
	directory = fileInfo.fIniDir;
	
	//create the big file
	mainFile = new TFile(temp.c_str(),"RECREATE");
	//start the loop to read in each smaller file
	for(unsigned i=0; i<numRuns; ++i)
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
	if ( runs == NULL )
	{
		cout<<"\nLoad Run Data first"<<endl;
		return;
	}
	if (mainFile != NULL)
	{
		cout<<"\nCombined file already loaded, to load a different one, use the reset button"<<endl;
		return;
	}
	
	//get the file name using a file dialog
	static TString directory(".");
	TGFileInfo fileInfo;
	fileInfo.SetMultipleSelection(false);
	fileInfo.fFileTypes = rootFileType;
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
	if( (temp.size()-5) != ( temp.rfind(".root") ) )
	{
		temp.append(".root");
	}
	directory = fileInfo.fIniDir;
	
	//open the big file
	mainFile = new TFile(temp.c_str(),"READ");
	cout<<"\nCombined file opened in read only mode"<<endl;
}

void MainWindow::makeAuxFile()
{
	if ( runs == NULL )
	{
		cout<<"\nLoad Run Data first"<<endl;
		return;
	}
	else if ( mainFile == NULL )
	{
		cout<<"\nLoad/create combined file first"<<endl;
		return;
	}
	else if(auxFile != NULL)
	{
		cout<<"\nAux file already loaded, to load a different one, use the reset button"<<endl;
		return;
	}
	//get the file name using a file dialog
	static TString directory(".");
	TGFileInfo fileInfo;
	fileInfo.SetMultipleSelection(false);
	fileInfo.fFileTypes = rootFileType;
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
	if( (temp.size()-5) != ( temp.rfind(".root") ) )
	{
		temp.append(".root");
	}
	directory = fileInfo.fIniDir;
	
	//create the aux file
	auxFile = new TFile(temp.c_str(),"RECREATE");
	cout<<"\nAux File Created"<<endl;
}

void MainWindow::openAuxFile()
{
	if ( runs == NULL )
	{
		cout<<"\nLoad Run Data first"<<endl;
		return;
	}
	else if ( mainFile == NULL )
	{
		cout<<"\nLoad/create combined file first"<<endl;
		return;
	}
	else if(auxFile != NULL)
	{
		cout<<"\nAux file already loaded, to load a different one, use the reset button"<<endl;
		return;
	}
	//get the file name using a file dialog
	static TString directory(".");
	TGFileInfo fileInfo;
	fileInfo.SetMultipleSelection(false);
	fileInfo.fFileTypes = rootFileType;
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
	if( (temp.size()-5) != ( temp.rfind(".root") ) )
	{
		temp.append(".root");
	}
	directory = fileInfo.fIniDir;
	
	//open the aux file
	auxFile = new TFile(temp.c_str(),"UPDATE");
	cout<<"\nAux File Opened"<<endl;
}


void MainWindow::makeFriendFile()
{
	if ( runs == NULL )
	{
		cout<<"\nLoad Run Data first"<<endl;
		return;
	}
	else if ( mainFile == NULL )
	{
		cout<<"\nLoad/create combined file first"<<endl;
		return;
	}
	else if ( auxFile == NULL )
	{
		cout<<"\nLoad/create auxiliary file first"<<endl;
		return;
	}
	else if( frFile != NULL )
	{
		cout<<"\nFriend file already loaded, to load a different one, use the reset button"<<endl;
		return;
	}
	//get the file name using a file dialog
	static TString directory(".");
	TGFileInfo fileInfo;
	fileInfo.SetMultipleSelection(false);
	fileInfo.fFileTypes = rootFileType;
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
	if( (temp.size()-5) != ( temp.rfind(".root") ) )
	{
		temp.append(".root");
	}
	directory = fileInfo.fIniDir;
	
	//create the aux file
	frFile = new TFile(temp.c_str(),"RECREATE");
	cout<<"\nFriend File Created"<<endl;
}


void MainWindow::openFriendFile()
{
	if ( runs == NULL )
	{
		cout<<"\nLoad Run Data first"<<endl;
		return;
	}
	else if ( mainFile == NULL )
	{
		cout<<"\nLoad/create combined file first"<<endl;
		return;
	}
	else if ( auxFile == NULL )
	{
		cout<<"\nLoad/create auxiliary file first"<<endl;
		return;
	}
	else if( frFile != NULL )
	{
		cout<<"\nFriend file already loaded, to load a different one, use the reset button"<<endl;
		return;
	}
	//get the file name using a file dialog
	static TString directory(".");
	TGFileInfo fileInfo;
	fileInfo.SetMultipleSelection(false);
	fileInfo.fFileTypes = rootFileType;
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
	if( (temp.size()-5) != ( temp.rfind(".root") ) )
	{
		temp.append(".root");
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
	if ( runs == NULL )
	{
		cout<<"\nLoadRun Data first"<<endl;
		return;
	}
	else if ( mainFile == NULL )
	{
		cout<<"\nLoad/create combined file first"<<endl;
		return;
	}
	else if ( auxFile == NULL )
	{
		cout<<"\nLoad/create aux file first"<<endl;
		return;
	}
	else if (dispFunc != None)
	{
		cout<<"\nEnd display mode first"<<endl;
		return;
	}
	
	for(int i=0; i<numRuns; ++i)
	{
		cout<<"Loading run "<<runs[i].runNumber<<"  "<<(i+1)<<" / "<<numRuns<<endl;
		//make the PID cut name
		string cName = makePIDCutName(runs[i].runNumber);
		//retrive the tree
		TTree* temp = retrieveTree(runs[i].runNumber);
		//check to make sure temp is present
		if(temp == NULL)
		{
			cout<<"Missing a tree from the combined file"<<endl;
			cout<<"You probably loaded the wrong run data and combined file combination"<<endl;
			return;
		}
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
	if ( runs == NULL )
	{
		cout<<"\nLoadRun Data first"<<endl;
		return;
	}
	else if ( mainFile == NULL )
	{
		cout<<"\nLoad/create combined file first"<<endl;
		return;
	}
	else if ( auxFile == NULL )
	{
		cout<<"\nLoad/create aux file first"<<endl;
		return;
	}
	else if (dispFunc != None)
	{
		cout<<"\nEnd display mode first"<<endl;
		return;
	}
	
	dispNum = 0;
	dispFunc = PIDCut;
	updateDisplay(Initial);
}



void MainWindow::getBGCuts()
{
	if ( runs == NULL )
	{
		cout<<"\nLoadRun Data first"<<endl;
		return;
	}
	else if ( mainFile == NULL )
	{
		cout<<"\nLoad/create combined file first"<<endl;
		return;
	}
	else if ( auxFile == NULL )
	{
		cout<<"\nLoad/create aux file first"<<endl;
		return;
	}
	else if (dispFunc != None)
	{
		cout<<"\nEnd display mode first"<<endl;
		return;
	}
	
	cout<<"\nWhen prompted to give regions, click at each of the four\nfollowing points specified, in any order"<<endl;
	cout<<"The points are:\nLower bnd of the lower bg region\nUpper bnd of the lower bg region (also the lower bnd of the true region)"<<endl;
	cout<<"Lower bnd of the upper bg region (also the upper bnd of the true region)\nUpper bnd of the upper bg region"<<endl;
	cout<<"Only the first 4 clicks will be used, double click when done selecting regions\n"<<endl;
	
	for(int i=0; i<numRuns; ++i)
	{
		//first retrive the PID cut from the auxilliary file
		TCutG* pidCut = retrievePIDCut(runs[i].runNumber);
		if(pidCut == NULL)
		{
			cout<<"Missing a pidCut from the aux file"<<endl;
			cout<<"You probably loaded the wrong run data aux file combination"<<endl;
			return;
		}
		//get the base cut
		TCut* baseCut = retrieveBaseCut(runs[i].runNumber);
		if(baseCut == NULL)
		{
			cout<<"Missing a baseCut from the aux file"<<endl;
			cout<<"Since the PID cut is present (which we know by making it to this point)"<<endl;
			cout<<"And since the baseCut is generated and saved with the PID cuts"<<endl;
			cout<<"Something odd is wrong"<<endl;
			return;
		}
		//get the tree from the main file
		TTree* tree = retrieveTree(runs[i].runNumber);
		if(tree == NULL)
		{
			cout<<"Missing a tree from the combined file"<<endl;
			cout<<"You probably loaded the wrong run data combined file combination"<<endl;
			return;
		}
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
	if ( runs == NULL )
	{
		cout<<"\nLoadRun Data first"<<endl;
		return;
	}
	else if ( mainFile == NULL )
	{
		cout<<"\nLoad/create combined file first"<<endl;
		return;
	}
	else if ( auxFile == NULL )
	{
		cout<<"\nLoad/create aux file first"<<endl;
		return;
	}
	else if (dispFunc != None)
	{
		cout<<"\nEnd display mode first"<<endl;
		return;
	}
	
	dispNum = 0;
	dispFunc = BGCut;
	cout<<"doing first update display"<<endl;
	updateDisplay(Initial);
}

/******************************************
*******************************************
** Shape operations buttons
*******************************************
******************************************/
void MainWindow::getFirstOrdShapes()
{

}

void MainWindow::showFirstOrdShapes()
{

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
	switch(tp)
	{
	case Initial:
		hist = NULL;
		tempCG = NULL;
		whiteBoard->Clear();
		whiteBoard->Update();
		break;
	case Normal:
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
		break;
	case Final:
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
	hist = retrievePIDSpec( runs[dispNum].runNumber );
	tempCG = retrievePIDCut( runs[dispNum].runNumber );
	if(hist==NULL || tempCG == NULL)
	{
		cout<<"Missing a tree or PID cut"<<endl;
		cout<<"Check what run data, combined file, and aux file that you have"<<endl;
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
	tempCG->Draw("same");
	whiteBoard->Update();
	whiteBoard->cd(2);
	//build the options to draw it in color while applying the PID cut
	ostringstream optBuild;
	optBuild<<"colz ["<<tempCG->GetName()<<"]";
	string options = optBuild.str();
	hist->Draw( options.c_str() );
	gPad->SetLogz(1);
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
	switch(tp)
	{
	case Initial:
		origHist = NULL;
		lowHist = NULL;
		midHist = NULL;
		hiHist = NULL;
		regions = NULL;
		text = NULL;
		whiteBoard->Clear();
		whiteBoard->Update();
		break;
	case Normal:
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
		break;
	case Final:
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
	origHist = retrieveBGSpec(runs[dispNum].runNumber);
	regions = retrieveBGGraph(runs[dispNum].runNumber);
	//test to make certain that everything is there
	if(origHist==NULL || regions==NULL)
	{
		cout<<"Missing a spectrum or cut"<<endl;
		cout<<"Check what run data, combined file, and aux file that you have"<<endl;
		cout<<"Backing out of display mode"<<endl;
		dispNum = 0;
		dispFunc = None;
		updateBGDisp(Final);//to cleanup static vars
		return;
	}
	cout<<"Starting drawing"<<endl;
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

void MainWindow::updateDisplay(const UpdateCallType& tp)
{
	//I was using a switch statement for this but for some reason the interpretter was not handling it correctly
	//replacing it with the if statement got the job done
	if(dispFunc == PIDCut)
	{
		updatePIDDisp(tp);
		return;
	}
	else if(dispFunc == BGCut)
	{
		updateBGDisp(tp);
		return;
	}
	else if(dispFunc == None)
	{
		cout<<"In update display with no display function"<<endl;
		cout<<"This should not be possible"<<endl;
		return;
	}
	else
	{
		cout<<"In update display with an invalid display function"<<endl;
		cout<<"This should not be possible"<<endl;
		return;
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
	if ( runs == NULL && mainFile == NULL && auxFile==NULL)
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

string MainWindow::makePIDCutName(int runN)
{
	ostringstream cutNamer;
	cutNamer<<"pidCut"<<runN;
	string temp = cutNamer.str();
	return temp;
}

string MainWindow::makePIDSpecName(int runN)
{
	ostringstream histNamer;
	histNamer<<"h2PIDRun"<<runN;
	string temp = histNamer.str();
	return temp;
}

string MainWindow::makeBaseCutName(int runN)
{
	ostringstream cutNamer;
	cutNamer<<"baseCut"<<runN;
	string temp = cutNamer.str();
	return temp;
}

string MainWindow::makeBGGraphName(int runN)
{
	ostringstream graphNamer;
	graphNamer<<"bgGraph"<<runN;
	string temp = graphNamer.str();
	return temp;
}

string MainWindow::makeBGCutName(int runN)
{
	ostringstream cutNamer;
	cutNamer<<"bgCut"<<runN;
	string temp = cutNamer.str();
	return temp;
}

string MainWindow::makeTRCutName(int runN)
{
	ostringstream cutNamer;
	cutNamer<<"trCut"<<runN;
	string temp = cutNamer.str();
	return temp;
}

string MainWindow::makeBGSpecName(int runN)
{
	ostringstream bgNamer;
	bgNamer<<"h1YFPRun"<<runN;
	string temp = bgNamer.str();
	return temp;
}

TTree* MainWindow::retrieveTree(int runN)
{
	string treeName = makeTreeName(runN);
	return reinterpret_cast<TTree*>(mainFile->Get(treeName.c_str()));
}

TCutG* MainWindow::retrievePIDCut(int runN)
{
	string cName = makePIDCutName(runN);
	return reinterpret_cast<TCutG*>(auxFile->Get(cName.c_str()));
}

TH2F* MainWindow::retrievePIDSpec(int runN)
{
	string histName = makePIDSpecName(runN);
	return reinterpret_cast<TH2F*>(auxFile->Get(histName.c_str()));
}

TCut* MainWindow::retrieveBaseCut(int runN)
{
	string cName = makeBaseCutName(runN);
	return reinterpret_cast<TCut*>(auxFile->Get(cName.c_str()));
}

TGraph* MainWindow::retrieveBGGraph(int runN)
{
	string gName = makeBGGraphName(runN);
	return reinterpret_cast<TGraph*>(auxFile->Get(gName.c_str()));
}

TCut* MainWindow::retrieveBGCut(int runN)
{
	string cName = makeBGCutName(runN);
	return reinterpret_cast<TCut*>(auxFile->Get(cName.c_str()));
}

TCut* MainWindow::retrieveTRCut(int runN)
{
	string cName = makeTRCutName(runN);
	return reinterpret_cast<TCut*>(auxFile->Get(cName.c_str()));
}

TH1F* MainWindow::retrieveBGSpec(int runN)
{
	string histName = makeBGSpecName(runN);
	return reinterpret_cast<TH1F*>(auxFile->Get(histName.c_str()));
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
	//read the magnetic rigidity (skipping it)
	ind = tempLine.find(',');
	temp = tempLine.substr(0,ind);
	tempLine = tempLine.substr(ind+1);
	conv.str(temp);
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
