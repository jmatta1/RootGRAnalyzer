#ifndef DIALOGS_H
#define DIALOGS_H

#include<TGWindow.h>
#include<TGTransientFrame.h>

struct CSBounds
{
	int numStates;
	float minTheta;
	float deltaTheta;
	int thetaSegs;
	float phiWidth;
}

class SimpleCSDialog
{
public:
	SimpleCSDialog(const TGWindow *p, TGWindow *main, CSBounds& bndData);

};

SimpleCSDialog::SimpleCSDialog(const TGWindow *p, TGWindow *main, CSBounds& bndData)
TGTransientFrame(p, main, 10, 10, kVerticalFrame)
{
	//create a simple cs bounds dialog
	SetCleanup(kDeep
	
}

#endif
