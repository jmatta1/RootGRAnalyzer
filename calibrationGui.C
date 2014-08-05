#include"CalWindow.h"

CalWindow* CALGUI=NULL;
void makeGUI()
{
	if(CALGUI==NULL)
	{
		CALGUI = new CalWindow(gClient->GetRoot(),500,400);
	}
	else
	{
		cout<<"CALGUI already exists, or it was closed with the 'x' buttons"<<endl;
		cout<<"if it was closed via the 'x' buttons, issue the command \"CALGUI=NULL\""<<endl;
		cout<<"(no quotation marks) then run this function again"<<endl;
	}
}

void killGUI()
{
	if(GUI!=NULL)
	{
		delete CALGUI;
		CALGUI=NULL;
	}
	else
	{
		cout<<"No GUI to kill"<<endl;
	}
}

