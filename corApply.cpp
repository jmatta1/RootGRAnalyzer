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

#include"guiSupport.h"

TFile* frFile = NULL;

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

string makeExCorrTreeName(int runN)
{
	ostringstream bgNamer;
	bgNamer<<"run"<<runN<<"_Excorr_tree";
	string temp = bgNamer.str();
	return temp;
}

TTree* retrieveExCorrTree(int runN)
{
	string histName = makeExCorrTreeName(runN);
	return reinterpret_cast<TTree*>(frFile->Get(histName.c_str()));
}

TTree* testExCorrTree(int runN)
{
	TTree* temp = retrieveExCorrTree(runN);
	if( temp==NULL )
	{
		cout<<"Missing a corrected excitation energy tree, you might have the wrong friend file loaded"<<endl;
		return NULL;
	}
	else
	{
		return temp;
	}
}

void parseCorrFileLine(const string& line, CorrFunc& tempData)
{
	istringstream conv;
	//read the cal run number
	int ind = line.find(',');
	string temp = line.substr(0,ind);
	string tempLine = line.substr(ind+1);
	conv.str(temp);
	conv>>tempData.runNum;
	conv.clear();
	//read each parameter
	for(int i=0; i<numParams; ++i)
	{
		ind = tempLine.find(',');
		temp = tempLine.substr(0,ind);
		tempLine = tempLine.substr(ind+1);
		conv.str(temp);
		conv>>(tempData.params[i]);
		//cout<<tempData.params[i]<<endl;
		conv.clear();
	}
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

void applyEnergyCorr(RunData* runs, int numRuns)
{
	
	cout<<"Please give the file with the run numbers and their associated correction functions."<<endl;
	string corrFileName;
	cin>>corrFileName;
	
	ifstream inFile;
	inFile.open(corrFileName.c_str());
	
	//read the cal file line by line, as we read each line, process that run
	//count the number of lines
	string line;
	int numCors = 0;
	while (getline(inFile, line))
	{
		if(line.length()>4)//to handle blank lines at the end of a file
		{
			++numCors;
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
	CorrFunc corrDat;
	while(!inFile.eof() && count<numRuns)
	{
		parseCorrFileLine(line, corrDat );
		getline(inFile, line);
		++count;
		int runInd=-1;
		//now hunt through the list of runs to find the run this cal line corresponds to
		for(int i=0; i<numRuns; ++i)
		{
			if(runs[i].runNumber == corrDat.runNum)
			{
				runInd = i;
				break;
			}
		}
		if(runInd == -1)
		{
			cout<<"Could not find run: "<<corrDat.runNum<<" in the run data file"<<endl;
			return;
		}
		//cout<<"At point 1"<<endl;
		TTree* test = retrieveExCorrTree(runs[runInd].runNumber);
		string treeName =makeExCorrTreeName(runs[runInd].runNumber);
		if(test!=NULL)
		{
			delete test;
			test=NULL;
			ostringstream nameBuilder;
			nameBuilder<<treeName<<";*";
			string toBeDeleted = nameBuilder.str();
			frFile->Delete(toBeDeleted.c_str());
			cout<<"Deleted original corrected excitation friend tree so we can build a replacement"<<endl;
		}
		TTree* exTree = new TTree(treeName.c_str(),treeName.c_str());
		//set up variables whose addresses will be used in reading individual events
		float thCorr;
		float exOld;
		//cout<<"At point 2"<<endl;
		TTree* orig = testExTree( runs[runInd].runNumber );
		orig->SetBranchAddress("Ex",&exOld);
		orig->SetBranchAddress("Thcorr",&thCorr);
		//set up the variable whose address will be used in writing the individual events
		float exNew;
		exTree->Branch("Excorr",&exNew,"Excorr/F");
		//cout<<"At point 3"<<endl;
		Long64_t numEnts = (orig->GetEntries());
		for(Long64_t j = 0; j<numEnts; ++j)
		{
			orig->GetEntry(j);
			
			exNew = (exOld + corrDat.eval(exOld, thCorr));
			
			if( (j%50000) == 0)
			{
				cout<<"Event number: "<<j<<endl;
			}
			exTree->Fill();
		}
		frFile->cd();
		string oldTreeName = makeExTreeName(runs[runInd].runNumber);
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
	applyEnergyCorr(runs, numRuns);
	delete runs;
}


