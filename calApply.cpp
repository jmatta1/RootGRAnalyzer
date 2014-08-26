#include<iostream>
#include<sstream>
#include<fstream>
#include<iostream>
using std::string;
using std::ostringstream;
using std::istringstream;
using std::ofstream;
using std::ifstream;
using std::cout;
using std::cin;
using std::endl;
using std::flush;
using std::ios_base;

#include<TString.h>
#include<TFile.h>
#include<TTree.h>
#include<TMath.h>

#include"guiSupport.h"
#include"bicubicinterp.h"

TFile* frFile = NULL;

string makeFirstOrderShapeTreeName(int runN)
{
	ostringstream bgNamer;
	bgNamer<<"run"<<runN<<"_tree_shape1";
	string temp = bgNamer.str();
	return temp;
}

TTree* retrieveFirstOrderShapeTree(int runN)
{
	string histName = makeFirstOrderShapeTreeName(runN);
	return reinterpret_cast<TTree*>(frFile->Get(histName.c_str()));
}

TTree* testFirstOrderShapeTree(int runN)
{
	TTree* temp = retrieveFirstOrderShapeTree(runN);
	if( temp==NULL )
	{
		cout<<"Missing a first order shape tree, you might have the wrong friend file loaded"<<endl;
		return NULL;
	}
	else
	{
		return temp;
	}
}

string makeExTreeName(int runN)
{
	ostringstream bgNamer;
	bgNamer<<"run"<<runN<<"_Ex_tree";
	string temp = bgNamer.str();
	return temp;
}

TTree* retrieveExTree(int runN)
{
	string histName = makeExTreeName(runN);
	return reinterpret_cast<TTree*>(frFile->Get(histName.c_str()));
}

TTree* testExTree(int runN)
{
	TTree* temp = retrieveExTree(runN);
	if( temp==NULL )
	{
		cout<<"Missing an excitation energy tree, you might have the wrong friend file loaded"<<endl;
		return NULL;
	}
	else
	{
		return temp;
	}
}

void parseCalFileLine(const string& line, CalDataLine& tempData)
{
	istringstream conv;
	//read the cal run number
	int ind = line.find(',');
	string temp = line.substr(0,ind);
	string tempLine = line.substr(ind+1);
	conv.str(temp);
	conv>>tempData.calRunNum;
	conv.clear();
	//read the a
	ind = tempLine.find(',');
	temp = tempLine.substr(0,ind);
	tempLine = tempLine.substr(ind+1);
	conv.str(temp);
	conv>>tempData.a;
	conv.clear();
	//read the b
	ind = tempLine.find(',');
	temp = tempLine.substr(0,ind);
	tempLine = tempLine.substr(ind+1);
	conv.str(temp);
	conv>>tempData.b;
	conv.clear();
	//read the c
	ind = tempLine.find(',');
	temp = tempLine.substr(0,ind);
	tempLine = tempLine.substr(ind+1);
	conv.str(temp);
	conv>>tempData.c;
	conv.clear();
	//read the cal D1
	ind = tempLine.find(',');
	temp = tempLine.substr(0,ind);
	tempLine = tempLine.substr(ind+1);
	conv.str(temp);
	conv>>tempData.calD1;
	conv.clear();
	//read the raw run number
	ind = tempLine.find(',');
	temp = tempLine.substr(0,ind);
	tempLine = tempLine.substr(ind+1);
	conv.str(temp);
	conv>>tempData.rawRunNum;
	conv.clear();
	//read the raw D1
	ind = tempLine.find(',');
	temp = tempLine.substr(0,ind);
	tempLine = tempLine.substr(ind+1);
	conv.str(temp);
	conv>>tempData.rawD1;
	conv.clear();
}

void parseRunFileLine(const string& line, RunData& tempData)
{
	istringstream conv;
	//read the run number
	int ind = line.find(',');
	string temp = line.substr(0,ind);
	string tempLine = line.substr(ind+1);
	conv.str(temp);
	conv>>tempData.runNumber;
	conv.clear();
	//read the angle
	ind = tempLine.find(',');
	temp = tempLine.substr(0,ind);
	tempLine = tempLine.substr(ind+1);
	conv.str(temp);
	conv>>tempData.angle;
	conv.clear();
	//read the beam energy 
	ind = tempLine.find(',');
	temp = tempLine.substr(0,ind);
	tempLine = tempLine.substr(ind+1);
	conv.str(temp);
	conv>>tempData.beamEn;
	conv.clear();
	//read the Charge State
	ind = tempLine.find(',');
	temp = tempLine.substr(0,ind);
	tempLine = tempLine.substr(ind+1);
	conv.str(temp);
	conv>>tempData.chargeState;
	conv.clear();
	//read the projectile mass
	ind = tempLine.find(',');
	temp = tempLine.substr(0,ind);
	tempLine = tempLine.substr(ind+1);
	conv.str(temp);
	conv>>tempData.projMass;
	conv.clear();
	//read the target mass
	ind = tempLine.find(',');
	temp = tempLine.substr(0,ind);
	tempLine = tempLine.substr(ind+1);
	conv.str(temp);
	conv>>tempData.targetMass;
	conv.clear();
	//read the target thickness
	ind = tempLine.find(',');
	temp = tempLine.substr(0,ind);
	tempLine = tempLine.substr(ind+1);
	conv.str(temp);
	conv>>tempData.thickness;
	conv.clear();
	//read the beam integral
	ind = tempLine.find(',');
	temp = tempLine.substr(0,ind);
	tempLine = tempLine.substr(ind+1);
	conv.str(temp);
	conv>>tempData.beamInt;
	conv.clear();
	//read the BI range
	ind = tempLine.find(',');
	temp = tempLine.substr(0,ind);
	tempLine = tempLine.substr(ind+1);
	conv.str(temp);
	conv>>tempData.biRange;
	conv.clear();
	//read the GR request
	ind = tempLine.find(',');
	temp = tempLine.substr(0,ind);
	tempLine = tempLine.substr(ind+1);
	conv.str(temp);
	conv>>tempData.grRequest;
	conv.clear();
	//read the GR accept
	ind = tempLine.find(',');
	temp = tempLine.substr(0,ind);
	tempLine = tempLine.substr(ind+1);
	conv.str(temp);
	conv>>tempData.grAccept;
	conv.clear();
	//read the vdc effeciency
	ind = tempLine.find(',');
	temp = tempLine.substr(0,ind);
	tempLine = tempLine.substr(ind+1);
	conv.str(temp);
	conv>>tempData.vdcEff;
	conv.clear();
}

RunData* readRunData(int &size)
{
	size=0;
	//get the file name
	cout<<"What is the run data file?"<<endl;
	string fName;
	cin>>fName;
	
	ifstream input;
	input.open(fName.c_str());
	
	//count the number of lines
	string line;
	while (getline(input, line))
	{
		if(line.length()>4)//to handle blank lines at the end of a file
		{
			++size;
		}
		else
		{
			break;
		}
	}
	//jump back to the beginning
	input.clear();
	input.seekg(0,ios_base::beg);
	//allocate the runs array
	RunData* runs = new RunData[size];
	//now read line by line to get the run data
	getline(input, line);
	int count = 0;
	while(!input.eof() && count<size)
	{
		parseRunFileLine(line, (runs[count]) );
		getline(input, line);
		++count;
	}
	cout<<"\nRun data has been loaded"<<endl;
	return runs;
}

void applyEnergyCal(RunData* runs, int numRuns)
{
	
	cout<<"Please give the file with the calibration polynomials, equivalent runs, and field strengths"<<endl;
	string calFileName;
	cin>>calFileName;
	
	cout<<"Please give the interpolation file with final energy as an input and initial energy as an output"<<endl;
	string interpName;
	cin>>interpName;
	
	//load the interpolation grid file
	int* sizesBuff = new int[2];
	float* xVal = new float[1000];
	float* yVal = new float[1000];
	float* zVal = new float[1000000];
	
	ifstream inFile;
	inFile.open(interpName.c_str(),std::fstream::in | std::fstream::binary);
	inFile.read(reinterpret_cast<char*>(sizesBuff),2*sizeof(int));//technically we do not really need these since both are always 1000
	inFile.read(reinterpret_cast<char*>(xVal),sizeof(float)*1000);
	inFile.read(reinterpret_cast<char*>(yVal),sizeof(float)*1000);
	inFile.read(reinterpret_cast<char*>(zVal),sizeof(float)*1000000);
	inFile.close();
	
	BiCubicInterpolation* interpFVI = new BiCubicInterpolation(xVal, 1000, yVal, 1000, zVal);
	cout<<"\nFinal vs Initial Interp Data is now Read In"<<endl;
	
	interpName="";
	
	cout<<"Please give the interpolation file with initial energy as an input and final energy as an output"<<endl;
	cin>>interpName;
	
	//load the interpolation grid file
	inFile.open(interpName.c_str(),std::fstream::in | std::fstream::binary);
	inFile.read(reinterpret_cast<char*>(sizesBuff),2*sizeof(int));//technically we do not really need these since both are always 1000
	inFile.read(reinterpret_cast<char*>(xVal),sizeof(float)*1000);
	inFile.read(reinterpret_cast<char*>(yVal),sizeof(float)*1000);
	inFile.read(reinterpret_cast<char*>(zVal),sizeof(float)*1000000);
	inFile.close();
	
	BiCubicInterpolation* interpIVF = new BiCubicInterpolation(xVal, 1000, yVal, 1000, zVal);
	cout<<"\nInitial vs Final Interp Data is now Read In"<<endl;
	
	delete[] sizesBuff;
	delete[] xVal;
	delete[] yVal;
	delete[] zVal;
	
	inFile.open(calFileName.c_str());
	
	//read the cal file line by line, as we read each line, process that run
	//count the number of lines
	string line;
	int numCals = 0;
	while (getline(inFile, line))
	{
		if(line.length()>4)//to handle blank lines at the end of a file
		{
			++numCals;
		}
		else
		{
			break;
		}
	}
	//jump back to the beginning
	inFile.clear();
	inFile.seekg(0,ios_base::beg);
	//now read line by line to get the run data
	getline(inFile, line);
	int count = 0;
	CalDataLine calDat;
	while(!inFile.eof() && count<numRuns)
	{
		parseCalFileLine(line, calDat );
		getline(inFile, line);
		++count;
		int runInd=-1;
		//now hunt through the list of runs to find the run this cal line corresponds to
		for(int i=0; i<numRuns; ++i)
		{
			if(runs[i].runNumber == calDat.rawRunNum)
			{
				runInd = i;
				break;
			}
		}
		if(runInd == -1)
		{
			delete interpIVF;
			delete interpFVI;
			cout<<"Cal run: "<<calDat.calRunNum<<" For raw run: "<<calDat.rawRunNum<<" could not find the raw run in run data"<<endl;
			return;
		}
		//cout<<"At point 1"<<endl;
		string treeName = makeExTreeName(runs[runInd].runNumber);
		TTree* exTree = new TTree(treeName.c_str(),treeName.c_str());
		//put the polynomial parameters for this run into quick variables
		float a = calDat.a;
		float b = calDat.b;
		float c = calDat.c;
		//set up variables whose addresses will be used in reading individual events
		float thcorr;
		float xfp;
		//cout<<"At point 2"<<endl;
		TTree* orig = testFirstOrderShapeTree( runs[runInd].runNumber );
		orig->SetBranchAddress("Xfp",&xfp);
		orig->SetBranchAddress("Thcorr",&thcorr);
		//set up the variable whose address will be used in writing the individual events
		float ex;
		exTree->Branch("Ex",&ex,"Ex/F");
		//cout<<"At point 3"<<endl;
		Long64_t numEnts = (orig->GetEntries());
		//calculate/get kinematics factors
		float amuToMev = 931.4948236727373;
		//GR angle setting
		float centAngle = runs[runInd].angle;
		//projectile mass
		float pM = runs[runInd].projMass*amuToMev;
		//target mass
		float tM = runs[runInd].targetMass*amuToMev;
		//half thickness
		float halfThick = runs[runInd].thickness/2.0;
		//cout<<"At point 4"<<endl;
		//beam energy
		float bE = runs[runInd].beamEn;
		//projectile energy
		float pE = pM + interpIVF->getVal(halfThick, bE);
		//constant term
		float constTerm = 2.0*pM*pM+tM*tM+2.0*tM*pE;
		//cout<<"At point 5"<<endl;
		//partial term 1
		float partTerm1 = 2.0*tM;
		//cout<<"At point 6"<<endl;
		//partial term 2
		float partTerm2 = 2.0*pE;
		//cout<<"At point 7"<<endl;
		//partial term 3
		float partTerm3 = 2.0*TMath::Sqrt(pE*pE-pM*pM);
		//cout<<"At point 8"<<endl;
		//field strenght ratio
		float ratio = (calDat.rawD1/calDat.calD1);
		//cout<<"bE: "<<bE<<" pE "<<pE<<endl;
		cout<<"Preparing to calibrate run"<<runs[runInd].runNumber<<"\n";
		cout<<"There are: "<<numEnts<<" events to process."<<endl;
		//fill the new tree
		for(Long64_t j = 0; j<numEnts; ++j)
		{
			orig->GetEntry(j);
			//cosine of the corrected and offset angle
			float radians = (thcorr+centAngle)*3.1415926/180.0;
			float costh = TMath::Cos(radians);
			//thickness of target traversed by scattered ion
			float thTrav = halfThick/costh;
			//focal plane momentum
			float fpMom = ratio*(a + b*xfp + c*xfp*xfp);
			//focal plane total En
			float fpEn = TMath::Sqrt(fpMom*fpMom+pM*pM);
			//focal plane KE
			float fpKe = fpEn-pM;
			//cout<<"cos(theta) "<<costh<<" thcorr: "<<thcorr<<" centAngle: "<<centAngle<<endl;
			//cout<<"thTrav: "<<thTrav<<" fpKe: "<<fpKe<<endl;
			//scattered KE
			float scKe = interpFVI->getVal(thTrav, fpKe);
			//scattered total energy
			float scEn = scKe + pM;
			//scattered momentum
			float scMom = TMath::Sqrt(scEn*scEn-pM*pM);
			//term under square root
			float sqrtTerm = (constTerm - partTerm1*scEn - partTerm2*scEn + partTerm3*scMom*costh);
			//finally, the excitation energy
			ex = TMath::Sqrt(sqrtTerm) - tM;
			/*cout<<"Angle: "<<radians*180.0/3.1415926<<" thickness: "<<thTrav<<" Xfp: "<<xfp<<" fp Momentum: "<<fpMom<<endl;
			cout<<"fp Kinetic En: "<<fpKe<<" scattered Kinetic En: "<<scKe<<" excitation energy: "<<ex<<endl;
			if(j==20)
			{
				return;
			}*/
			if( (j%50000) == 0)
			{
				cout<<"Event number: "<<j<<endl;
			}
			exTree->Fill();
		}
		frFile->cd();
		string oldTreeName = makeFirstOrderShapeTreeName(runs[runInd].runNumber);
		string frFilePath = frFile->GetName();
		int index = frFilePath.find_last_of('/');
		string frFileName = "./";
		frFileName.append( frFilePath.substr(index+1) );
		exTree->AddFriend( oldTreeName.c_str(), frFileName.c_str() );
		exTree->FlushBaskets();
		exTree->Write(treeName.c_str(),TObject::kOverwrite);
		TFile* file = exTree->GetCurrentFile();
		file->Flush();
		delete exTree;
	}
	
	delete interpIVF;
	delete interpFVI;
}

int main()
{
	//get run data
	int numRuns = 0;
	RunData* runs = readRunData(numRuns);
	cout<<"What is the name of the friend tree file?"<<endl;
	string frTreeName;
	cin>>frTreeName;
	frFile = TFile::Open(frTreeName.c_str(),"UPDATE");
	applyEnergyCal(runs, numRuns);
	delete runs;
}


