#include"abCorWindow.h"

AberationCorrectionWindow* ACGUI=NULL;
void makeGUI()
{
	if(ACGUI==NULL)
	{
		ACGUI = new AberationCorrectionWindow(gClient->GetRoot(),500,400);
	}
	else
	{
		cout<<"ACGUI already exists, or it was closed with the 'x' buttons"<<endl;
		cout<<"if it was closed via the 'x' buttons, issue the command \"ACGUI=NULL\""<<endl;
		cout<<"(no quotation marks) then run this function again"<<endl;
	}
}

void killGUI()
{
	if(ACGUI!=NULL)
	{
		delete ACGUI;
		ACGUI=NULL;
	}
	else
	{
		cout<<"No GUI to kill"<<endl;
	}
}

