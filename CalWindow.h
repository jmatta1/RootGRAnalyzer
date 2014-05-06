#ifndef CAL_WINDOW_H
#define CAL_WINDOW_H

class CalibrationWindow
{
	RQ_OBJECT("CalibrationWindow")
public:
	CalibrationWindow(const TGWindow* parent, UInt_t width, UInt_t height);
	~CalibrationWindow();
	
private:
	//main window
	TGMainFrame *mainWindow;
};

#endif
