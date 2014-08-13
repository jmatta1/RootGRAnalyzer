#ifndef DIALOGS_H
#define DIALOGS_H

#include<sstream>
using std::string;
using std::ostringstream;

#include<TGWindow.h>
#include<TGFrame.h>
#include<TGLabel.h>
#include<TGNumberEntry.h>
#include<TG3DLine.h>
#include<RQ_OBJECT.h>

struct CSBounds
{
	int numStates;
	float minTheta;
	float maxTheta;
	int thetaSegs;
	float phiWidth;
};

class BasicCSDialog
{
	RQ_OBJECT("BasicCSDialog")
public:
	BasicCSDialog(const TGWindow* parent, UInt_t width, UInt_t height);
	~BasicCSDialog();
	
	//show and hide functions
	void show();
	void hide();
	
	//title modifier
	void setBasicName();
	void setRunName(int runN);
	
	//Values function
	void getVals(CSBounds* bnds);
	void setVals(CSBounds* bnds);

private:
	//widgetNumberings
	enum {StatesEntry=1, MinEntry=2, MaxEntry=3, SegsEntry=4, PhiEntry=5, CancelButton=6, OKButton=7};
	//main window
	TGMainFrame *mainWindow;
	//sub frames
	//these are here so that I can know what is in the class, they should be deleted in the typical recursive fashion
	//when mainWindow is deleted though
	TGHorizontalFrame* overAllOrg; //the outermost frame in the dialog box
	TGVerticalFrame* mainOrganizer; //the organizer for the normal mode of operation buttons
	TGHorizontalFrame* statesFrame;//the frame for holding the number of states information getters
	TGHorizontalFrame* minThetaFrame;//the frame for holding the stuff to get the minimum theta
	TGHorizontalFrame* maxThetaFrame;//the frame for holding the stuff to get the maximum theta
	TGHorizontalFrame* segsFrame;//the frame for holding the stuff to get the number of segments in theta
	TGHorizontalFrame* phiFrame;//the frame to hold the stuff for getting the width in phi
	TGVerticalFrame* customBinsOrganizer; //the organizer for the stuff necessary for custom angular binning
	
	
	//number getters
	TGNumberEntry* numStatesEntry;//use for getting the number of states to retrieve
	TGNumberEntry* minThetaEntry;//used for getting the minimum theta
	TGNumberEntry* maxThetaEntry;//used for getting the maximum theta
	TGNumberEntry* numSegsEntry;//user for getting the number of segments in theta
	TGNumberEntry* phiWidthEntry;//used for getting the with of phi in milliradians
	
	//labels in the main frame
	TGLabel* statesMainLbl;
	TGLabel* minThMainLbl;
	TGLabel* maxThMainLbl;
	TGLabel* numSegsMainLbl;
	TGLabel* phiWidthMainLbl;
	
	//labels in each of the horizontal frames
	TGLabel* statesEntryLBL;
	TGLabel* minThEntryLBL;
	TGLabel* maxThEntryLBL;
	TGLabel* numSegsEntryLBL;
	TGLabel* phiWidthEntryLBL;
	
	//separators
	TGHorizontal3DLine* sep1;
	TGHorizontal3DLine* sep2;
	TGHorizontal3DLine* sep3;
	TGHorizontal3DLine* sep4;
};

BasicCSDialog::BasicCSDialog(const TGWindow* parent, UInt_t width, UInt_t height)
{
	/***************************************
	****************************************
	** Graphical Setup
	****************************************
	***************************************/
	//make the main frame
	mainWindow = new TGMainFrame(parent,width,height);
	//make the sub frames
	mainOrganizer = new TGVerticalFrame(mainWindow, 10, 10);
	statesFrame = new TGHorizontalFrame(mainOrganizer, 10, 10);
	minThetaFrame = new TGHorizontalFrame(mainOrganizer, 10, 10);
	maxThetaFrame = new TGHorizontalFrame(mainOrganizer, 10, 10);
	segsFrame = new TGHorizontalFrame(mainOrganizer, 10, 10);
	phiFrame = new TGHorizontalFrame(mainOrganizer, 10, 10);
	
	//make the number entry boxes and add each on to its frame
	numStatesEntry = new TGNumberEntry( statesFrame, 1.0, 5, StatesEntry, TGNumberFormat::kNESInteger,
			TGNumberFormat::kNEAPositive, TGNumberFormat::kNELLimitMin, 1, 10000);
	statesFrame->AddFrame(numStatesEntry, new TGLayoutHints(kLHintsLeft | kLHintsTop, 5, 5, 3, 4) );
	
	minThetaEntry = new TGNumberEntry( minThetaFrame, -0.9, 5, MinEntry, TGNumberFormat::kNESRealTwo,
			TGNumberFormat::kNEAAnyNumber, TGNumberFormat::kNELLimitMinMax, -5.0, 5.0);
	minThetaFrame->AddFrame(minThetaEntry, new TGLayoutHints(kLHintsLeft | kLHintsTop, 5, 5, 3, 4) );
	
	maxThetaEntry = new TGNumberEntry( maxThetaFrame, 0.9, 5, MaxEntry, TGNumberFormat::kNESRealTwo, 
			TGNumberFormat::kNEAAnyNumber, TGNumberFormat::kNELLimitMinMax, -5.0, 5.0);
	maxThetaFrame->AddFrame(maxThetaEntry, new TGLayoutHints(kLHintsLeft | kLHintsTop, 5, 5, 3, 4) );
	
	numSegsEntry = new TGNumberEntry( segsFrame, 6.0, 5, SegsEntry, TGNumberFormat::kNESInteger, 
			TGNumberFormat::kNEAPositive, TGNumberFormat::kNELLimitMin, 1, 10000);
	segsFrame->AddFrame(numSegsEntry, new TGLayoutHints(kLHintsLeft | kLHintsTop, 5, 5, 3, 4) );
	
	phiWidthEntry = new TGNumberEntry( phiFrame, 40.0, 5, PhiEntry, TGNumberFormat::kNESRealTwo,
			TGNumberFormat::kNEAPositive, TGNumberFormat::kNELLimitMinMax, 0.01, 3141.59);
	phiFrame->AddFrame(phiWidthEntry, new TGLayoutHints(kLHintsLeft | kLHintsTop, 5, 5, 3, 4) );
	
	//make the mainOrganizer frame labels
	statesMainLbl =  new TGLabel(mainOrganizer, "Number of States to Retrieve");
	minThMainLbl =  new TGLabel(mainOrganizer, "Minimum Theta");
	maxThMainLbl =  new TGLabel(mainOrganizer, "Maximum Theta");
	numSegsMainLbl =  new TGLabel(mainOrganizer, "Number of Segments in Theta");
	phiWidthMainLbl =  new TGLabel(mainOrganizer, "Width in Phi (phi = 0.0 +- phiWidth/2)");
	
	//make the in frame labels and add them to the appropriate horizontal frame
	statesEntryLBL = new TGLabel(statesFrame, "Integer");
	statesFrame->AddFrame(statesEntryLBL, new TGLayoutHints(kLHintsNormal, 5, 5, 3, 4) );
	
	minThEntryLBL = new TGLabel(minThetaFrame, "(degrees) (I tend to use -0.9)");
	minThetaFrame->AddFrame(minThEntryLBL, new TGLayoutHints(kLHintsNormal, 5, 5, 3, 4) );
	
	maxThEntryLBL = new TGLabel(maxThetaFrame, "(degrees) (I tend to use 0.9)");
	maxThetaFrame->AddFrame(maxThEntryLBL, new TGLayoutHints(kLHintsNormal, 5, 5, 3, 4) );
	
	numSegsEntryLBL = new TGLabel(segsFrame, "Integer (I tend to use 6)");
	segsFrame->AddFrame(numSegsEntryLBL, new TGLayoutHints(kLHintsNormal, 5, 5, 3, 4) );
	
	phiWidthEntryLBL = new TGLabel(phiFrame, "(milliRadians)");
	phiFrame->AddFrame(phiWidthEntryLBL, new TGLayoutHints(kLHintsNormal, 5, 5, 3, 4) );
	
	//make the separators
	sep1 = new TGHorizontal3DLine(mainOrganizer,2,4);
	sep2 = new TGHorizontal3DLine(mainOrganizer,2,4);
	sep3 = new TGHorizontal3DLine(mainOrganizer,2,4);
	sep4 = new TGHorizontal3DLine(mainOrganizer,2,4);
	
	
	//now add everything to the mainOrganizer frame in the appropriate order
	mainOrganizer->AddFrame(statesMainLbl, new TGLayoutHints(kLHintsNormal,5,5,3,4) );
	mainOrganizer->AddFrame(statesFrame, new TGLayoutHints(kLHintsNormal,5,5,3,4) );
	
	mainOrganizer->AddFrame(sep1, new TGLayoutHints(kLHintsExpandX,5,5,3,4) );
	
	mainOrganizer->AddFrame(minThMainLbl, new TGLayoutHints(kLHintsNormal,5,5,3,4) );
	mainOrganizer->AddFrame(minThetaFrame, new TGLayoutHints(kLHintsNormal,5,5,3,4) );
	
	mainOrganizer->AddFrame(sep2, new TGLayoutHints(kLHintsExpandX,5,5,3,4) );
	
	mainOrganizer->AddFrame(maxThMainLbl, new TGLayoutHints(kLHintsNormal,5,5,3,4) );
	mainOrganizer->AddFrame(maxThetaFrame, new TGLayoutHints(kLHintsNormal,5,5,3,4) );
	
	mainOrganizer->AddFrame(sep3, new TGLayoutHints(kLHintsExpandX,5,5,3,4) );
	
	mainOrganizer->AddFrame(numSegsMainLbl, new TGLayoutHints(kLHintsNormal,5,5,3,4) );
	mainOrganizer->AddFrame(segsFrame, new TGLayoutHints(kLHintsNormal,5,5,3,4) );
	
	mainOrganizer->AddFrame(sep4, new TGLayoutHints(kLHintsExpandX,5,5,3,4) );
	
	mainOrganizer->AddFrame(phiWidthMainLbl, new TGLayoutHints(kLHintsNormal,5,5,3,4) );
	mainOrganizer->AddFrame(phiFrame, new TGLayoutHints(kLHintsNormal,5,5,3,4) );
	
	//set the name for dialog
	mainWindow->SetWindowName("Info For All Runs");
	//now add the mainOrganizer frame to this dialog
	mainWindow->AddFrame(mainOrganizer, new TGLayoutHints( kLHintsExpandX | kLHintsTop ,5,5,3,4) );
	//map the subwindows
	mainWindow->MapSubwindows();
	//init the layout engine
	mainWindow->Resize(mainWindow->GetDefaultSize());
	
	mainWindow->SetMWMHints(kMWMDecorAll | kMWMDecorResizeH  | kMWMDecorMaximize | kMWMDecorMinimize | kMWMDecorMenu,
               kMWMFuncAll |  kMWMFuncResize    | kMWMFuncMaximize | kMWMFuncMinimize,
               kMWMInputModeless);
}



BasicCSDialog::~BasicCSDialog()
{
	mainWindow->Cleanup();
	delete mainWindow;
}

void BasicCSDialog::show()
{
	mainWindow->MapWindow();
}

void BasicCSDialog::hide()
{
	mainWindow->UnmapWindow();
}

void BasicCSDialog::setBasicName()
{
	mainWindow->SetWindowName("Info For All Runs");
}

void BasicCSDialog::setRunName(int runN)
{
	ostringstream titler;
	titler<<"Info For Run "<<runN;
	string title = titler.str();
	mainWindow->SetWindowName(title.c_str());
}

void BasicCSDialog::getVals(CSBounds* bnds)
{
	bnds->numStates = numStatesEntry->GetIntNumber();
	bnds->minTheta  = minThetaEntry->GetNumber();
	bnds->maxTheta  = maxThetaEntry->GetNumber();
	bnds->thetaSegs = numSegsEntry->GetIntNumber();
	bnds->phiWidth  = phiWidthEntry->GetNumber();
}

void BasicCSDialog::setVals(CSBounds* bnds)
{
	numStatesEntry->SetIntNumber( bnds->numStates);
	minThetaEntry->SetNumber(bnds->minTheta );
	maxThetaEntry->SetNumber(bnds->maxTheta);
	numSegsEntry->SetIntNumber(bnds->thetaSegs);
	phiWidthEntry->SetNumber(bnds->phiWidth);
}

#endif
