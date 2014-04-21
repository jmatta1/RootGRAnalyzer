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

#endif
