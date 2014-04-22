#include"MainWindow.h"

MainWindow* GUI=NULL;
void makeGUI()
{
	if(GUI==NULL)
	{
		GUI = new MainWindow(gClient->GetRoot(),0,0);
	}
	else
	{
		cout<<"GUI already exists, or it was closed with the x buttons"<<endl;
		cout<<"if it was closed via the x buttons, issue the command \"GUI=NULL\""<<endl;
		cout<<"(no quotation marks) then run this function again"<<endl;
	}
}

void killGUI()
{
	if(GUI!=NULL)
	{
		delete GUI;
		GUI=NULL;
	}
	else
	{
		cout<<"No GUI to kill"<<endl;
	}
}

