#include<iostream>
#include<fstream>
#include<string>
#include<sstream>
using std::istringstream;
using std::string;
using std::ofstream;
using std::ifstream;
using std::getline;
using std::cout;
using std::cin;
using std::endl;
#include"cubicspline.h"

int main(int argc, char* argv[])
{
	float thStep = 0.0001;//ie 100 ng/cm2 chunks for the numerical integrals
	
	float minEn;
	float maxEn;
	cout<<"Enter the Minimum energy expected (MeV)"<<endl;
	cin>>minEn;
	minEn = minEn-10.0;
	cout<<"Enter the Beam Energy (MeV)"<<endl;
	cin>>maxEn;
	maxEn = maxEn + 10.0;
	float de = ((maxEn-minEn)/999.0);
	ofstream enOutput;
	enOutput.open("./enList.txt", std::fstream::out);
	int minBnd = (1000.0*(minEn));
	int maxBnd = (1000.0*(maxEn));
	int delta = (1000.0*de);
	for(int i = minBnd; i<=maxBnd; i+=delta)
	{
		enOutput<<i<<endl;
	}
	enOutput.close();
	cout<<"Energy list written to: enList.txt"<<endl;
	cout<<"Please give the file name of the file with the SRIM output"<<endl;
	string fileName;
	cin>>fileName;
	
	//parse the SRIM file
	ifstream input;
	input.open(fileName.c_str(),std::fstream::in);
	string line;
	//discard the first 4 lines
	for(int i=0; i<4; ++i)
	{
		getline(input, line);
	}
	//arrays for holding energies and de/dx
	float* enArr = new float[1000];
	float* deArr = new float[1000];
	//read the next 1000 lines for the energy losses
	for(int i=0; i<1000; ++i)
	{
		getline(input, line);
		istringstream conv;
		//get the energy
		int ind = line.find(' ');
		string energy = line.substr(0,ind+1);
		string tempLine = line.substr(ind+2);
		conv.str(energy);
		conv>>enArr[i];
		conv.clear();
		enArr[i]/=1000.0;
		//get the electronic energy loss
		ind = line.find(' ');
		string elELoss = tempLine.substr(0,ind+1);
		string nELoss = tempLine.substr(ind+1);
		conv.str(elELoss);
		float temp;
		conv>>temp;
		conv.clear();
		//get the nuclear energy loss
		conv.str(nELoss);
		float temp2;
		conv>>temp2;
		conv.clear();
		//set the energy loss equal to the sum of the electronic and nuclear components
		deArr[i] = (temp+temp2);
	}
	input.close();
	CubicSpline eLoss(enArr,deArr,1000,0.0,0.0,BothNatural);
	
	//setup and do the integrals
	float minTh;
	float maxTh;
	cout<<"Please give the thickness of the thinnest target in mg/cm^2"<<endl;
	cin>>minTh;
	minTh = minTh/4.0;
	cout<<"Please give the thickness of the thickest target in mg/cm^2"<<endl;
	cin>>maxTh;
	maxTh = maxTh*4.0;
	
	float dt = ((maxTh-minTh)/999.0);
	
	float* thArray = new float[1000];
	
	float* finalEns = new float[1000000];
	float* initEns = new float[1000000];
	
	//the grids will have 1000 points in the thickness direction, and 1000 points in the energy direction
	//first calculate the initial energy vs thickness grid (with the final energies as grid points)
	{
	cout<<"Calculating initial to final energy grid.\nAcross the thickness range: "<<minTh<<"mg/cm2 to "<<maxTh<<"mg/cm^2"<<endl;
	float th = minTh;
	thArray[0]=th;
	cout<<"Currently at: Thickness = "<<thArray[0]<<" mg/cm2"<<endl;
	for(int j=0; j<1000; ++j)
	{
		int index = j;
		float en = enArr[j];
		float thickness=0.0;
		float deltaEn=0.0;
		while( (thickness<th) )
		{
			float dedx = eLoss((en-deltaEn));
			float enLost = dedx*thStep;
			deltaEn += enLost;
			thickness += thStep;
		}
		finalEns[index]=(en-deltaEn);
	}
	for(int i=1; i<1000; ++i)//loop across starting energies
	{
		th = (minTh + (i*dt));
		thArray[i]=th;
		if((i%4)==0){
		cout<<"Currently at: Thickness = "<<thArray[i]<<" mg/cm2"<<endl;}
		for(int j=0; j<1000; ++j)
		{
			int index = i*1000+j;
			int index2 = (index-1000);
			float en = finalEns[index2];
			float thickness=thArray[i-1];
			float deltaEn=0.0;
			while( (thickness<th) )
			{
				float dedx = eLoss((en-deltaEn));
				float enLost = dedx*thStep;
				deltaEn += enLost;
				thickness += thStep;
			}
			finalEns[index]=(en-deltaEn);
		}
	}
	}
	//now write out the initial energy binary grid file
	cout<<"Give the filename for the file that has initial energy vs thickness"<<endl;
	cin>>fileName;
	
	ofstream output;
	output.open(fileName.c_str(), std::fstream::out | std::fstream::binary);
	int sideLen = 1000;
	//first write the number of points in the x direction
	output.write(reinterpret_cast<char*>(&sideLen),sizeof(int));
	//then write the number of points in the y direction
	output.write(reinterpret_cast<char*>(&sideLen),sizeof(int));
	//then write the array of x values
	output.write(reinterpret_cast<char*>(thArray),1000*sizeof(float));
	//then write the array of y values
	output.write(reinterpret_cast<char*>(enArr),1000*sizeof(float));
	//then write the grid of output values
	output.write(reinterpret_cast<char*>(finalEns),1000000*sizeof(float));
	
	output.close();
	
	//now calculate the final energy vs thickness grid (with the initial energies as grid points)
	{
	cout<<"Calculating final to initial energy grid.\nAcross the thickness range: "<<minTh<<"mg/cm2 to "<<maxTh<<"mg/cm^2"<<endl;
	float th = thArray[0];
	cout<<"Currently at: Thickness = "<<thArray[0]<<" mg/cm2"<<endl;
	for(int j=0; j<1000; ++j)
	{
		int index = j;
		float en = enArr[j];
		double thickness=0.0;
		double deltaEn=0.0;
		while( (thickness<th) )
		{
			double dedx = eLoss((en+deltaEn));
			double enLost = dedx*thStep;
			deltaEn += enLost;
			thickness += thStep;
		}
		initEns[index]=(en+deltaEn);
	}
	for(int i=1; i<1000; ++i)//loop across starting energies
	{
		th = thArray[i];
		if((i%4)==0){
		cout<<"Currently at: Thickness = "<<thArray[i]<<" mg/cm2"<<endl;}
		for(int j=0; j<1000; ++j)
		{
			int index = i*1000+j;
			int index2 = (index-1000);
			float en = finalEns[index2];
			float thickness=thArray[i-1];
			double deltaEn=0.0;
			while( (thickness<th) )
			{
				double dedx = eLoss((en+deltaEn));
				double enLost = dedx*thStep;
				deltaEn += enLost;
				thickness += thStep;
			}
			initEns[index]=(en+deltaEn);
		}
	}
	}
	
	//now write out the final energy binary grid file
	cout<<"Give the filename for the file that has final energy vs thickness"<<endl;
	cin>>fileName;
	
	output.open(fileName.c_str(), std::fstream::out | std::fstream::binary);
	//first write the number of points in the x direction
	output.write(reinterpret_cast<char*>(&sideLen),sizeof(int));
	//then write the number of points in the y direction
	output.write(reinterpret_cast<char*>(&sideLen),sizeof(int));
	//then write the array of x values
	output.write(reinterpret_cast<char*>(thArray),1000*sizeof(float));
	//then write the array of y values
	output.write(reinterpret_cast<char*>(enArr),1000*sizeof(float));
	//then write the grid of output values
	output.write(reinterpret_cast<char*>(initEns),1000000*sizeof(float));
	for(int i=0; i<1000; ++i)
	{
		cout<<thArray[i]<<"  "<<enArr[i]<<"  "<<finalEns[i]<<endl;
	
	}
	output.close();
	
	delete[] enArr;
	delete[] deArr;
	delete[] thArray;
	delete[] finalEns;
	delete[] initEns;
	
	return 0;
}
