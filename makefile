
ROOTLIBS=$(shell root-config --nonew --libs) -L$(shell root-config --nonew --libdir)
ROOTINCS=-I$(shell root-config --nonew --incdir)

calApply: calApply.cpp bicubicinterp.h guiSupport.h
	g++ calApply.cpp $(ROOTINCS) $(ROOTLIBS) -o calApply
