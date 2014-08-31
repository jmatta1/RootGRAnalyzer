const char *csvDataType[] = { "Run Data",      "*.csv",
                               0,               0 };

const char *combRootFileType[] = { "Root File",     "*_merge.root",
                                    0,               0 };

const char *auxRootFileType[] = { "Aux Root File",     "*_aux.root",
                                   0,                   0 };

const char *frRootFileType[] = { "Friend Root File",     "*_frnd.root",
                                  0,                      0 };

const char *interpDataType[] = { "Interp Data",      "*.igf",
                               0,               0 };

const char *calDataType[] = { "Calibration Data",      "*.csv",
                               0,               0 };

const char *initMessage="Welcome to the Notre Dame Giant Resonance Analysis GUI";
const char *calInitMessage="Welcome to the Notre Dame Focal Plane Calibration GUI";

const char *calColNames[] = { "Ex", "P_fp", "Calib. Xfp", "Assigned Cent.", "Calib. P_fp",
											"P_fp Resid", "Fit Selection", "", ""};

const char *calFuncs[] = { "pol1", "pol2" };

struct RunData
{
	int runNumber;
	float angle;
	float beamEn;
	int chargeState;
	float projMass;
	float targetMass;
	float thickness;
	int beamInt;
	int biRange;
	int grRequest;
	int grAccept;
	float vdcEff;
};

struct FitData
{
	double height;
	double centroid;
	double width;
	bool isAssign;
	FitData& operator=(FitData& rhs){height=rhs.height; centroid=rhs.centroid; width=rhs.width; isAssign=rhs.isAssign; return *this;}
};

struct StateFit
{
	int ftInd;
	bool isSet;
	StateFit& operator=(StateFit& rhs){isSet=rhs.isSet; ftInd=rhs.ftInd; return *this;}
};

struct StateData
{
	double en;
	double scatEn;
	double fpMom;
	//bool isAssigned;
	StateData& operator=(StateData& rhs){en=rhs.en; scatEn=rhs.scatEn; fpMom=rhs.fpMom; return *this;}
};

struct CalDataLine
{
	int calRunNum;
	int rawRunNum;
	float a;
	float b;
	float c;
	float calD1;
	float rawD1;
};

struct BinData
{
	BinData():runNum(-1), numBins(0), edges(NULL){}
	~BinData(){if(edges!=NULL){delete edges;}}
	int runNum;
	int numBins;
	float* edges;
};

inline void swapDoubles(double& first, double& second)
{
	double swap = first;
	first = second;
	second = swap;
}

//in place partitioning function
int partitionDoubles(double* arr, int size)
{
	//first set things up so the midpoint is the pivot
	//first grab a median element by selecting the median of 3
	int midInd = (size/2);
	int lastInd = (size-1);
	//sort the first middle and last cell in the array
	if(arr[0] > arr[lastInd])
	{
		swapDoubles(arr[0],arr[lastInd]);
	}
	if(arr[midInd] > arr[lastInd])
	{//now see if the middle should be at the end
		swapDoubles(arr[midInd],arr[lastInd]);
	}
	if(arr[midInd] < arr[0])
	{//or see if the middle should be at the beginning
		swapDoubles(arr[midInd],arr[0]);
	}
	//now partition the array 
	int pivot = midInd;
	int last = (size-1);
	double pivotVal = arr[pivot];
	swapDoubles(arr[pivot],arr[last]);
	int tempInd = 0;
	for(int i = 0; i<last; ++i)
	{
		if( arr[i] <= pivotVal )
		{
			swapDoubles(arr[i], arr[tempInd]);
			++tempInd;
		}
	}
	swapDoubles(arr[tempInd], arr[last]);
	return tempInd;
}

void sortDoubles(double* arr, int size)
{
	//if size > 10 use quicksort divide and conquer
	if(size>10)
	{
		//partition the array and get the pivot point
		int pivotInd = partitionDoubles(arr, size);
		//now do the recursion
		//first recurr on the lower half of the array
		sortDoubles(arr,pivotInd);
		//sortDoubles recurr on the upper half of the array
		sortDoubles(arr+pivotInd,(size-pivotInd+1));
	}
	else
	{//otherwise comb sort
		const float shrink = 1.3;
		int gap = size;
		bool swapped = false;
		while(gap>1 || swapped)
		{
			if (gap>1)
			{
				gap = (int)(((float)gap)/shrink);
			}
			
			int i=0;
			int offsetI = gap;
			swapped=false;
			for(; offsetI<size; ++i)
			{
				if(arr[i]>arr[offsetI])
				{
					swapDoubles(arr[i],arr[offsetI]);
					swapped=true;
				}
				++offsetI;
			}
		}
	}
	
}
