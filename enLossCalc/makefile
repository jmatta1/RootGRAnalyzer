#This make file compiles the energy loss interpolation grid calculator

ROOTLIBS=$(shell root-config --nonew --libs) -L$(shell root-config --nonew --libdir) -lMathMore
ROOTINCS=-I$(shell root-config --nonew --incdir)

target:debug

.PHONY: debug
debug: FLAGS=-g

.PHONY: opt0
opt0: FLAGS=

.PHONY: opt1
opt1: FLAGS=-O1

.PHONY: opt2
opt2: FLAGS=-O2

.PHONY: opt3
opt3: FLAGS=-O3

.PHONY: dopt1
dopt1: FLAGS=-O1 -g

.PHONY: dopt2
dopt2: FLAGS=-O2 -g

.PHONY: dopt3
dopt3: FLAGS=-O3 -g

.PHONY: clean
clean:
	rm -rf obj_debug
	rm -rf obj_opt0
	rm -rf obj_opt1
	rm -rf obj_opt2
	rm -rf obj_opt3
	rm -rf obj_dopt1
	rm -rf obj_dopt2
	rm -rf obj_dopt3

.PHONY: cleanall
cleanall: clean
	rm -f enLossCalc

debug opt0 opt1 opt2 opt3 dopt1 dopt2 dopt3:
	mkdir -p obj_$@
	$(MAKE) OUTDIR=obj_$@ FLAGS="$(FLAGS) $(BASE_FLAGS)" obj_$@/enLossCalc
	cp obj_$@/enLossCalc ./enLossCalc

$(OUTDIR)/enLossCalc: $(OUTDIR)/main.o
	g++ $(FLAGS) $^ -o $@ $(ROOTLIBS)

$(OUTDIR)/main.o: ./main.cpp
	g++ $(FLAGS) -c $< -o $@ $(ROOTLIBS) $(ROOTINCS)


