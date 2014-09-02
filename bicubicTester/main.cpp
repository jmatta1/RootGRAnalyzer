#include<iostream>
#include<fstream>
#include<string>
using std::cout;
using std::cin;
using std::endl;
using std::ifstream;
using std::string;

#include"bicubicinterp.h"

int main(int argc, char* argv[])
{
	//read in the file of x and y values
	cout<<"What is the F2I igf file you wish to test?"<<endl;
	string fileName;
	cin>>fileName;
	ifstream inFile;
	inFile.open(fileName.c_str(),std::fstream::in | std::fstream::binary);
	if(!inFile.is_open())
	{
		cout<<"Invalid file name or path, exitting"<<endl;
		//return fail code
		return 1;
	}
	//load the interpolation grid file
	int* sizes = new int[2];
	inFile.read(reinterpret_cast<char*>(sizes),2*sizeof(int));//technically we do not really need these since both are always 1000
	float* xVal = new float[sizes[0]];
	float* yVal = new float[sizes[1]];
	float* zVal = new float[sizes[0]*sizes[1]];
	inFile.read(reinterpret_cast<char*>(xVal),sizeof(float)*sizes[0]);
	cout<<"Min thickness is: "<<xVal[0]<<" Max thickness is: "<<xVal[sizes[0]-1]<<endl;
	inFile.read(reinterpret_cast<char*>(yVal),sizeof(float)*sizes[1]);
	cout<<"Min energy is: "<<yVal[0]<<" Max energy is: "<<yVal[sizes[1]-1]<<endl;
	inFile.read(reinterpret_cast<char*>(zVal),sizeof(float)*sizes[0]*sizes[1]);
	inFile.close();
	
	//xpoints, ypoints, num points, first deriv at pt 1, first deriv at pt 2, type of boundary conditions
	BiCubicInterpolation interp(xVal, sizes[0], yVal, sizes[1], zVal);
	cout<<"\nDone constructing the interpolator"<<endl;
	cout<<"Starting testing loop, use \"ctrl+c\" to exit"<<endl;

	//loop to test values from the user
	//return 0;
	char ans='y';
	while(true)
	{
		float x;
		float y;
		cout<<"Thickness (mg/cm^2)?"<<endl;
		cin>>x;
		cout<<"Final Energy (MeV)?"<<endl;
		cin>>y;
		
		cout<<"\n At Th = "<<x<<",  En = "<<y<<" The Initial Energy Was: "<<interp(x,y)<<"\n"<<endl;
	}
	
	//delete the arrays
	delete[] sizes;
	delete[] xVal;
	delete[] yVal;
	delete[] zVal;
	//return success code
	return 0;
}

