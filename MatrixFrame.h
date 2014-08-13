#ifndef MATRIX_FRAME_H
#define MATRIX_FRAME_H

#include<TGCanvas.h>
#include<TGFrame.h>
#include<TGLayout.h>

class MatrixFrame
{
RQ_OBJECT("MatrixFrame")
public:
	MatrixFrame(const TGWindow *p, int rows, int cols);
	virtual ~MatrixFrame(){delete frame;}
	
	TGFrame* getFrame() const {return frame;}
	
	void setCanvas(TGCanvas *can){ canvas = can; }
	void handleMouseWheel(Event_t *event);

private:
	TGCompositeFrame *frame;
	TGCanvas* canvas;

};

MatrixFrame::MatrixFrame(const TGWindow *p, int rows, int cols)
{
	frame = new TGCompositeFrame(p, 10, 10);
	
	frame->Connect("ProcessedEvent(Event_t*)","MatrixFrame", this, "handleMouseWheel(Event_t*)");
	
	canvas = NULL;
	
	frame->SetLayoutManager(new TGMatrixLayout(frame, rows, cols, 3, 3));
	
	gVirtualX->GrabButton(frame->GetId(), kAnyButton, kAnyModifier,
			kButtonPressMask | kButtonReleaseMask| kPointerMotionMask, kNone, kNone);
	
}

void MatrixFrame::handleMouseWheel(Event_t* event)
{
	if(event->fType != kButtonPress && event->fType != kButtonRelease){return;}
	
	if(canvas == NULL){return;}
	
	float temp = ((float)canvas->GetViewPort()->GetHeight());
	temp*=temp;
	temp/=((float)canvas->GetContainer()->GetHeight());
	Int_t page = ((int)temp);
	if(event->fCode == kButton4)
	{//scroll up
		Int_t newPos = canvas->GetVsbPosition() - page;
		if(newPos < 0){newPos = 0;}
		canvas->SetVsbPosition(newPos);
	}
	if(event->fCode == kButton5)
	{//scroll down
		Int_t newPos = canvas->GetVsbPosition() + page;
		canvas->SetVsbPosition(newPos);
	}
}

#endif
