
LIBS=$(shell root-config --nonew --libs) -L$(shell root-config --nonew --libdir)
INCS=-I$(shell root-config --nonew --incdir)
all:calApply corApply

calApply: calApply.cpp bicubicinterp.h guiSupport.h
	g++ -O3 calApply.cpp $(INCS) $(LIBS) -o calApply

corApply: corApply.cpp guiSupport.h
	g++ -O3 corApply.cpp $(INCS) $(LIBS) -o corApply

clean:
	-rm calApply
	-rm corApply
