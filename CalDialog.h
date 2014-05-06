#ifndef CAL_DIALOG_H
#define CAL_DIALOG_H

class CalibrationDialog
{
	RQ_OBJECT("CalibrationDialog")
public:
	CalibrationDialog(const TGWindow* parent, UInt_t width, UInt_t height);
	~CalibrationDialog();
	
private:
	//main window
	TGMainFrame *mainWindow;
};

#endif
