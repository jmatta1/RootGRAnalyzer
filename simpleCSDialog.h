#ifndef DIALOGS_H
#define DIALOGS_H

#include<TGWindow.h>
#include<TGFrame.h>
#include<TGLabel.h>
#include<TGNumberEntry.h>
#include<TG3DLine.h>

struct CSBounds
{
	int numStates;
	float minTheta;
	float maxTheta;
	int thetaSegs;
	float phiWidth;
	bool goodReturn;
};

class SimpleCSDialog : public TGTransientFrame
{
private:
	inline void update(int param);
	//not implemented
	SimpleCSDialog(const SimpleCSDialog&);
	SimpleCSDialog& operator=(const SimpleCSDialog&);
	
protected:
	//widgetNumberings
	enum {StatesEntry=1, MinEntry=2, MaxEntry=3, SegsEntry=4, PhiEntry=5, CancelButton=6, OKButton=7};
	//frames
	TGVerticalFrame* mainOrganizer; //the outermost frame in the dialog box
	TGHorizontalFrame* statesFrame;//the frame for holding the number of states information getters
	TGHorizontalFrame* minThetaFrame;//the frame for holding the stuff to get the minimum theta
	TGHorizontalFrame* maxThetaFrame;//the frame for holding the stuff to get the maximum theta
	TGHorizontalFrame* segsFrame;//the frame for holding the stuff to get the number of segments in theta
	TGHorizontalFrame* phiFrame;//the frame to hold the stuff for getting the width in phi
	TGHorizontalFrame* buttonFrame;//holds the ok and cancel buttons
	
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
	TGHorizontal3DLine* sep5;
	
	//Buttons
	TGTextButton* cancelBut;
	TGTextButton* okBut;
	
	//pointer to the class that will accept the data from the dialog
	CSBounds* bData;
	
public:
	SimpleCSDialog(const TGWindow *p, TGWindow *main, CSBounds* bndData);
	virtual ~SimpleCSDialog();
	
	virtual Bool_t ProcessMessage(Long_t msg, Long_t parm1, Long_t parm2);
	virtual void CloseWindow();

	void goodFinish();

	ClassDef(SimpleCSDialog,0) //Cross Section Bounds gathering dialog
};

SimpleCSDialog::SimpleCSDialog(const TGWindow *p, TGWindow *main, CSBounds* bndData):
		TGTransientFrame(p, main, 10, 10, kVerticalFrame)
{
	//create a simple cs bounds dialog
	SetCleanup(kDeepCleanup);
	Connect("CloseWindow()", "SimpleCSDialog", this, "CloseWindow()");
	DontCallClose();
	
	if (p!=NULL && main!=NULL)
	{
		MakeZombie();
		return;
	}
	bData = bndData;
	
	bData->goodReturn = false;
	bData->numStates = 1;
	bData->minTheta = -0.9;
	bData->maxTheta = 0.9;
	bData->thetaSegs = 6;
	bData->phiWidth = 40.0;
	
	/***************************************
	****************************************
	** Graphical Setup
	****************************************
	***************************************/
	//make the sub frames
	mainOrganizer = new TGVerticalFrame(this, 10, 10);
	statesFrame = new TGHorizontalFrame(mainOrganizer, 10, 10);
	minThetaFrame = new TGHorizontalFrame(mainOrganizer, 10, 10);
	maxThetaFrame = new TGHorizontalFrame(mainOrganizer, 10, 10);
	segsFrame = new TGHorizontalFrame(mainOrganizer, 10, 10);
	phiFrame = new TGHorizontalFrame(mainOrganizer, 10, 10);
	buttonFrame = new TGHorizontalFrame(mainOrganizer, 10, 10);
	
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
	sep5 = new TGHorizontal3DLine(mainOrganizer,2,4);
	
	//create, connect and add the buttons
	okBut = new TGTextButton(buttonFrame,"OK", OKButton);
	okBut->Connect("Clicked()","SimpleCSDialog",this,"goodFinish()");
	buttonFrame->AddFrame(okBut, new TGLayoutHints(kLHintsExpandX, 5, 5, 3, 4) );
	cancelBut = new TGTextButton(buttonFrame,"Cancel", CancelButton);
	cancelBut->Connect("Clicked()","SimpleCSDialog",this,"CloseWindow()");
	buttonFrame->AddFrame(cancelBut, new TGLayoutHints(kLHintsExpandX, 5, 5, 3, 4) );
	
	
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
	
	mainOrganizer->AddFrame(sep5, new TGLayoutHints(kLHintsExpandX,5,5,3,4) );
	
	mainOrganizer->AddFrame(buttonFrame, new TGLayoutHints(kLHintsExpandX,5,5,3,4) );
	
	//set the name for dialog
	SetWindowName("CS Info Dialog");
	//now add the mainOrganizer frame to this dialog
	AddFrame(mainOrganizer, new TGLayoutHints( kLHintsExpandX | kLHintsTop ,5,5,3,4) );
	//map the subwindows
	MapSubwindows();
	//init the layout engine
	TGDimension size = GetDefaultSize();
	Resize(size);
	//center relative to parent
	CenterOnParent();
	//make non-resizable
	SetWMSize(size.fWidth, size.fHeight);
	SetWMSizeHints(size.fWidth, size.fHeight, 10000, 10000, 1, 1);
	
	SetMWMHints(kMWMDecorAll | kMWMDecorResizeH  | kMWMDecorMaximize | kMWMDecorMinimize | kMWMDecorMenu,
               kMWMFuncAll |  kMWMFuncResize    | kMWMFuncMaximize | kMWMFuncMinimize,
               kMWMInputModeless);
	//now map this to the screen
	MapWindow();
	fClient->WaitFor(this);
}

SimpleCSDialog::~SimpleCSDialog()
{
	Cleanup();
	if(IsZombie()){return;}
	okBut->Disconnect("Clicked()");
	cancelBut->Disconnect("Clicked()");
	//if my understanding is good then everything else should be deleted as part of the standard process
}

void SimpleCSDialog::CloseWindow()
{
	bData->goodReturn = false;
	DeleteWindow();
}

void SimpleCSDialog::goodFinish()
{
	bData->goodReturn = true;
	DeleteWindow();
}

Bool_t SimpleCSDialog::ProcessMessage(Long_t msg, Long_t param1, Long_t param2)
{
	if( GET_MSG(msg) == kC_TEXTENTRY )
	{
		switch( GET_SUBMSG(msg) )
		{
			case  kTE_TEXTCHANGED:
				this->update(param1);
				break;
			case kTE_ENTER:
				this->update(param1);
				break;
			case kTE_TAB:
				break;
			default:
				break;
		}
	}
	/*else if ( (GET_MSG(msg) == kC_COMMAND) && (GET_SUBMSG(msg) == kCM_BUTTON) )
	{
		if( param1 == CancelButton)
		{
			bData->goodReturn = false;
			DeleteWindow();	
		}
		else if( param1 == OKButton)
		{
			bData->goodReturn = true;
			DeleteWindow();
		}
	}*/
	
	return kTRUE;
}

inline void SimpleCSDialog::update(int param)
{
	switch(param)
	{
		case SimpleCSDialog::StatesEntry:
			bData->numStates = numStatesEntry->GetIntNumber();
			break;
		case SimpleCSDialog::MinEntry:
			bData->minTheta = minThetaEntry->GetNumber();
			break;
		case SimpleCSDialog::MaxEntry:
			bData->maxTheta = maxThetaEntry->GetNumber();
			break;
		case SimpleCSDialog::SegsEntry:
			bData->thetaSegs = numSegsEntry->GetIntNumber();
			break;
		case SimpleCSDialog::PhiEntry:
			bData->phiWidth = phiWidthEntry->GetNumber();
			break;
	}
}

#endif
