#pragma once
#define KSIZE 3
#define NSIZE 9
#define TSIZE 81

//Backtracing solution type
#define FIRST_SOLUTION			1		//For geting first solution
#define NEXT_SOLUTION			2		//For geting next solution(mat is containing last solution)

//write Type
#define WRITE_SOLUTION          1		//Computer will modify original matrix
#define DONTWRITEINSOLUTION		2		//Computer will not modify original matrix

//solution type
#define COMMAN_SOLUTION			1		//Computer will consider user numbers
#define WITHOUTUSER_SOLUTION	2		//Computer will not consider user numbers
#define DONTUSEBACKTRACING		4		//Computer will not use backtracing for solving

//Number type
#define PLACED_FIXED			1
#define PLACED_BY_USER			2
#define PLACED_BY_COMMAN		4
#define PLACED_BY_BACKTRACE		8
#define PLACED_HINTS			16
#define PLACED_ERROR			32

#define IS_PLACED_FIXED(x)			((x/10)&PLACED_FIXED?1:0)
#define IS_PLACED_BY_USER(x)		((x/10)&PLACED_BY_USER?1:0)
#define IS_PLACED_BY_COMMAN(x)		((x/10)&PLACED_BY_COMMAN?1:0)
#define IS_PLACED_BY_BACKTRACE(x)	((x/10)&PLACED_BY_BACKTRACE?1:0)
#define IS_PLACED_BY_HINTS(x)		((x/10)&PLACED_HINTS?1:0)
#define IS_PLACED_ERROR(x)			((x/10)&PLACED_ERROR?1:0)



class runnable
{
public:
    virtual ~runnable() {}
    static DWORD WINAPI run_thread(LPVOID args)
    {
        runnable *prunnable = static_cast<runnable*>(args);
        return prunnable->Start_Generate();
    }
protected:
    virtual DWORD Start_Generate() = 0; //derived class must implement this!
};



class SudokuSolution:  public runnable
{
private:
	HWND hWnd;	//Only needed to send WM_SODUKUGENERATED message
	int sudval[NSIZE][NSIZE];//num    +10*bin[1(PLACED_BY_COM_MAN) 1(PLACED_BY_USER) 1(PLACED_FIXED)]
	char message[50];
	DWORD dTime;
	bool bStarted;
	bool bGiveSudoku;
	int level;

	int SolveSudokuByBackTracing(int mat[], int type);
	

	int SetNextValComManMethod(int mat[],int type,int *hint);

	bool IsSodukuSolved(int mat[]);
	int SolveSudoku(int mat[],int type);
	int IsSodukuSolvable(int mat[],int type);

	bool IsNumPlacementPossible(int mat[],int num,int select);
	
	
	
	
public:
	SudokuSolution(void);
	~SudokuSolution(void);


	int SolveSudokuByBackTracing(int type);

	bool GenerateSoduku(HWND hwnd);
	DWORD Start_Generate(void);

	bool SetValueByPos(int pos,int num,int type);
	bool SetValueByRowCol(int row,int col,int num,int type);
	bool SetSudoku(int mat[]);

	bool GetValueByPos(int pos,int* value);
	bool GetValueByRowCol(int row,int col,int* value);

	int SolveSudoku(int type);
	int IsSodukuSolvable(int type);
	bool IsSodukuSolved(void);
	int CheckSudoku(void);
	
	int FindHint(void);
	int IsHint(void);

	void SetLevel(int lvl)
	{
			level=lvl;
	}
	void GiveSudoku(bool status)
	{
		bGiveSudoku=status;
		if(status)
			memset(sudval,0,TSIZE*sizeof(int));
	}

	bool IsGiveSudoku()
	{
		return bGiveSudoku;
	}
	void FixGiveSudoku();

	char *GetPaintMessage(void)
	{
		return message;
	}
	void SetPaintMessage(char *msg)
	{
		strcpy_s(message,msg);
	}

	DWORD GetTimerValue()
	{
		return dTime;
	}
	bool IsTimerStarted()
	{
		return bStarted;
	}
	void StartTimer(bool status)
	{
		if(status)
		{
			dTime=0;
			bStarted=true;
		}
		else
			bStarted=false;
	}
	void IncrementTimer(void)
	{
		dTime++;
	}

};
