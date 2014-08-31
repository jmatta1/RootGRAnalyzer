
LIBS=$(shell root-config --nonew --libs) -L$(shell root-config --nonew --libdir)
INCS=-I$(shell root-config --nonew --incdir)

calApply: calApply.cpp bicubicinterp.h guiSupport.h
	g++ -O3 calApply.cpp $(INCS) $(LIBS) -o calApply

clean:
	-rm calApply
