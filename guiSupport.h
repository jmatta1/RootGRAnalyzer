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

const char *initMessage="Welcome to the Notre Dame Giant Resonance Analysis GUI";
const char *calInitMessage="Welcome to the Notre Dame Focal Plane Calibration GUI";

/*
TGTextEntry** exEntry;//excitation energy holders
TGTextEntry** fpmEntry;//calculated focal plane momenta
TGTextEntry** calxfpEntry;//xfp from calculated focal plane momentum
TGTextEntry** aFitEntry;//assigned fit centroids
TGTextEntry** calfpmEntry;//focal plain momenta from calibration
TGTextEntry** difEntry;//difference between assigned fit centroid and calculated centroid
TGComboBox** fitBox;//combo boxes for holding unassigned fits
TGTextButton** assignButtons;//buttons to assign a selected fit to a state
TGTextButton** unAssignButtons;//buttons to unassign a fit from a state
*/

const char *calColNames[] = { "Ex", "P_fp", "Calib. Xfp", "Assigned Cent.", "Calib. P_fp",
											"Xfp Resid", "Fit Selection", "", ""};

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
	FitData& operator=(FitData& rhs){height=rhs.height; centroid=rhs.centroid; width=rhs.width; return *this;}
};

struct StateFit
{
	double fpMom;
	double centroid;
	StateFit& operator=(StateFit& rhs){fpMom=rhs.fpMom; centroid=rhs.centroid; return *this;}
};

struct StateData
{
	int index;
	double en;
	double scatEn;
	double fpMom;
	//bool isAssigned;
	StateData& operator=(StateData& rhs){index=rhs.index; en=rhs.en; scatEn=rhs.scatEn; fpMom=rhs.fpMom; return *this;}
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
