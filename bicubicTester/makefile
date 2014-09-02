
bicubicTester: main.o cubicspline.o bicubicinterp.o
	g++ -O3 main.o cubicspline.o bicubicinterp.o -o bicubicTester

main.o: main.cpp cubicspline.h
	g++ -c -O3 main.cpp -o main.o

cubicspline.o: cubicspline.cpp cubicspline.h
	g++ -c -O3 cubicspline.cpp -o cubicspline.o

bicubicinterp.o: bicubicinterp.cpp bicubicinterp.h
	g++ -c -O3 bicubicinterp.cpp -o bicubicinterp.o

clean:
	-rm -f cubicspline.o
	-rm -f bicubicinterp.o
	-rm -f main.o

cleanall: clean
	-rm -f bicubicTester
