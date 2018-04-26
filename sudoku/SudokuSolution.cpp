#include "StdAfx.h"
#include "SudokuSolution.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>



SudokuSolution::SudokuSolution(void)
{
	message[0]=0;
	dTime=0;
	bStarted=false;
	bGiveSudoku=false;
	level=3;
}


SudokuSolution::~SudokuSolution(void)
{
}

bool SudokuSolution::SetSudoku(int mat[])
{
	int row,col;
	memcpy(sudval,mat,TSIZE*sizeof(int));
	for(row=0;row<NSIZE;row++)
		for(col=0;col<NSIZE;col++)
			if(sudval[row][col]>0&&sudval[row][col]<=NSIZE)
				sudval[row][col]=sudval[row][col]%(NSIZE+1)+10*PLACED_FIXED;
	return true;
}

bool SudokuSolution::GetValueByRowCol(int row,int col,int* value)
{	
	if(row>=0&&row<NSIZE&&col>=0&&col<NSIZE)
	{
		*value=sudval[row][col];	
		return true;
	}
	return false;
}


bool SudokuSolution::GetValueByPos(int pos,int* value)
{	
	if(pos>=0&&pos<TSIZE)
	{
		int row = pos/NSIZE;
        int col = pos%NSIZE;
		*value=sudval[row][col];	
		return true;
	}
	return false;
}

bool SudokuSolution::SetValueByRowCol(int row,int col,int num,int type)
{
	if(type==PLACED_FIXED)
	{
		sudval[row][col]=num%(NSIZE+1);
		return true;
	}	
	else
	{
		if(!IS_PLACED_FIXED(sudval[row][col]))
		{
			if(num%(NSIZE+1)==0)
				sudval[row][col]=0;		//When deleting number no type is needed
			else
				sudval[row][col]=num%(NSIZE+1)+10*type;
			return true;
		}
	}	
	return true;
}
bool SudokuSolution::SetValueByPos(int pos,int num,int type)
{
	if(pos>=0&&pos<TSIZE)
	{
		int row = pos/NSIZE;
        int col = pos%NSIZE;
		return SetValueByRowCol(row,col,num,type);
	}
	return false;
}
void SudokuSolution::FixGiveSudoku()
{
	int row,col;
	for(row=0;row<NSIZE;row++)
		for(col=0;col<NSIZE;col++)
			if(sudval[row][col]%(NSIZE+1)>0&&sudval[row][col]%(NSIZE+1)<=NSIZE) 
				sudval[row][col]=sudval[row][col]%(NSIZE+1)+PLACED_FIXED*10;
}

bool SudokuSolution::IsSodukuSolved(int mat[])
{
	int count,i,flagrow,flagcol,flagblock,pos;
	for(count=0;count<NSIZE;count++)
	{
		flagrow=0;
		flagcol=0;
		flagblock=0;
		for(i=0;i<NSIZE;i++)
		{
			//Check if placed by hints or its blank
			if((mat[count*NSIZE+i]%(NSIZE+1))==0||IS_PLACED_BY_HINTS(mat[count*NSIZE+i]))
			{
				return false;
			}

			//Check for row			//row=count,col=i
			pos=count*NSIZE+i;
			if((mat[pos]%(NSIZE+1))!=0)
				flagrow|=1<<((mat[pos]%(NSIZE+1))-1);

			//Check for col			//col=count,row=i	
			pos=i*NSIZE+count;
			if((mat[pos]%(NSIZE+1))!=0)
				flagcol|=1<<((mat[pos]%(NSIZE+1))-1);

			//Check for block			count=block
			//col=(block/KSIZE)*KSIZE+i%KSIZE;
			//row=(block%KSIZE)*KSIZE+i/KSIZE;
			pos=((count%KSIZE)*KSIZE+i/KSIZE)*NSIZE+(count/KSIZE)*KSIZE+i%KSIZE;
			if((mat[pos]%(NSIZE+1))!=0)
				flagblock|=1<<((mat[pos]%(NSIZE+1))-1);

		}
		if(flagrow!=0x1FF||flagcol!=0x1FF||flagblock!=0x1FF)
			return false;
	}
	return true;
}

bool SudokuSolution::IsSodukuSolved()
{
	return IsSodukuSolved((int*)sudval);
}

//output:
//	-1	Wrong Result
//	0	Yet Not Filled up
//  1	Solved Correctly
int SudokuSolution::CheckSudoku(void)
{
	int count,i,flagrow,flagcol,flagblock,pos,iRet=1;
	for(flagrow=0;flagrow<NSIZE;flagrow++)
	{
		for(flagcol=0;flagcol<NSIZE;flagcol++)
		{
			//Check if placed by hints or its blank
			if((sudval[flagrow][flagcol]%(NSIZE+1))==0||IS_PLACED_BY_HINTS(sudval[flagrow][flagcol]))
				iRet=0;
			//Check if Previously it is set as error
			if(IS_PLACED_ERROR(sudval[flagrow][flagcol]))
				sudval[flagrow][flagcol]=sudval[flagrow][flagcol]-PLACED_ERROR*10;

		}
	}
	for(count=0;count<NSIZE;count++)
	{
		flagrow=0;
		flagcol=0;
		flagblock=0;
		for(i=0;i<NSIZE;i++)
		{

			//Check for row			//row=count,col=i
			pos=count*NSIZE+i;
			if((sudval[count][i]%(NSIZE+1))!=0)
			{
				if(flagrow&(1<<((sudval[count][i]%(NSIZE+1))-1)))
				{
					//Current one is fixed..so prev one is error
					if(IS_PLACED_FIXED(sudval[count][i]))
					{
						int j;
						for(j=0;j<i;j++)
						{
							if(sudval[count][i]%(NSIZE+1)==sudval[count][j]%(NSIZE+1)&&IS_PLACED_ERROR(sudval[count][j]))
							{	
								sudval[count][j]+=PLACED_ERROR*10;
								break;
							}
						}
					}
					else if(!IS_PLACED_ERROR(sudval[count][i]))
						sudval[count][i]+=PLACED_ERROR*10;
				}
				else
					flagrow|=1<<((sudval[count][i]%(NSIZE+1))-1);
			}

			//Check for col			//col=count,row=i	
			pos=i*NSIZE+count;
			if((sudval[i][count]%(NSIZE+1))!=0)
			{
				if(flagcol&(1<<((sudval[i][count]%(NSIZE+1))-1)))
				{
					if(IS_PLACED_FIXED(sudval[i][count]))
					{
						int j;
						for(j=0;j<i;j++)
						{
							if(sudval[i][count]%(NSIZE+1)==sudval[j][count]%(NSIZE+1)&&!IS_PLACED_ERROR(sudval[j][count]))
							{	
								sudval[j][count]+=PLACED_ERROR*10;
								break;
							}
						}
					}
					else if(!IS_PLACED_ERROR(sudval[i][count]))
						sudval[i][count]+=PLACED_ERROR*10;
				}
				else
					flagcol|=1<<((sudval[i][count]%(NSIZE+1))-1);
			}

			//Check for block			count=block
			//col=(block/KSIZE)*KSIZE+i%KSIZE;
			//row=(block%KSIZE)*KSIZE+i/KSIZE;
			int col=(count/KSIZE)*KSIZE+i%KSIZE;
			int row=(count%KSIZE)*KSIZE+i/KSIZE;
			//pos=((count%KSIZE)*KSIZE+i/KSIZE)*NSIZE+(count/KSIZE)*KSIZE+i%KSIZE;
			if((sudval[row][col]%(NSIZE+1))!=0)
			{
				if(flagblock&(1<<((sudval[row][col]%(NSIZE+1))-1)))
				{
					if(IS_PLACED_FIXED(sudval[row][col]))
					{
						int j;
						for(j=0;j<i;j++)
						{
							int col1=(count/KSIZE)*KSIZE+j%KSIZE;
							int row1=(count%KSIZE)*KSIZE+j/KSIZE;
							if(sudval[row][col]%(NSIZE+1)==sudval[row1][col1]%(NSIZE+1))
							{	
								sudval[row1][col1]+=PLACED_ERROR*10;
								break;
							}
						}
					}
					else if(!IS_PLACED_ERROR(sudval[row][col]))
						sudval[row][col]+=PLACED_ERROR*10;
				}
				else
					flagblock|=1<<((sudval[row][col]%(NSIZE+1))-1);
			}

		}
		if(flagrow!=0x1FF||flagcol!=0x1FF||flagblock!=0x1FF)
			iRet=iRet?-1:0;
	}
	return iRet;
}

//type:
//	COMMAN_SOLUTION				//Computer will consider user numbers
//	WITHOUTUSER_SOLUTION		//Computer will not consider user numbers
//	DONTUSEBACKTRACING			//Computer will not use backtracing for solving
//output:
//	-1	cannot be solved
//	0	can be solved uniquely
//  1	can be solved by backtracing/many solution
int SudokuSolution::IsSodukuSolvable(int type)
{
	int mat[TSIZE];
	memcpy(mat,sudval,TSIZE*sizeof(int));
	return SolveSudoku(mat,type);
}

//type:
//	COMMAN_SOLUTION				//Computer will consider user numbers
//	WITHOUTUSER_SOLUTION		//Computer will not consider user numbers
//	DONTUSEBACKTRACING			//Computer will not use backtracing for solving
//output:
//	-1	cannot be solved
//	0	can be solved uniquely
//  1	can be solved by backtracing/many solution
int SudokuSolution::IsSodukuSolvable(int mat[],int type)
{
	return SolveSudoku(mat,type);
}


//type:
//	COMMAN_SOLUTION				//Computer will consider user numbers
//	WITHOUTUSER_SOLUTION		//Computer will not consider user numbers
//	DONTUSEBACKTRACING			//Computer will not use backtracing for solving
//output:
//	-1	cannot be solved
//	0	can be solved uniquely
//  1	can be solved by backtracing/many solution
int SudokuSolution::SolveSudoku(int mat[],int type)
{
	int iResult=0;

	int row,col;
	for(row=0;row<NSIZE;row++)
	{
		for(col=0;col<NSIZE;col++)
		{
			if(IS_PLACED_BY_USER(mat[row*NSIZE+col])&&(type&WITHOUTUSER_SOLUTION))
				mat[row*NSIZE+col]=0;
			if(IS_PLACED_BY_HINTS(mat[row*NSIZE+col]))
				mat[row*NSIZE+col]=0;

		}
	}
	while(iResult==0)
	{
		iResult=SetNextValComManMethod(mat,WRITE_SOLUTION,NULL);
		if(iResult==1)
		{
			//Solution is completed
			if(IsSodukuSolved(mat))
			{
				//strcpy_s(message,"Solved Successfully");
				return 0;
			}
			//strcpy_s(message,"Cannot be solved");
			return -1;	
		}
		else if(iResult==-2)
			return -1;
		//if(iResult==-1)
		//{
		//	break;
		//}
	}	
	//strcpy_s(message,"More than one solution.");					//Manual Method failed
	if(type&DONTUSEBACKTRACING)
		return -1;
	return SolveSudokuByBackTracing(mat,FIRST_SOLUTION);
}

//type:
//	COMMAN_SOLUTION				//Computer will consider user numbers
//	WITHOUTUSER_SOLUTION		//Computer will not consider user numbers
//	DONTUSEBACKTRACING			//Computer will not use backtracing for solving
//output:
//	-1	cannot be solved
//	0	solved uniquely
//  1	solved by backtracing/many solution

int SudokuSolution::SolveSudoku(int type)
{
	return SolveSudoku((int*)sudval,type);
}


//input: 
//type = FIRST_SOLUTION for geting first solution
//type = NEXT_SOLUTION for geting first solution

//output:
//	-1	cannot be solved
//  1	can be solved by backtracing/many solution


int SudokuSolution::SolveSudokuByBackTracing(int type)
{
	return SolveSudokuByBackTracing((int*)sudval,type);
}

int SudokuSolution::SolveSudokuByBackTracing(int mat[], int type)
{
	int count=0,prevcount,progress=+1;
	int iMask[NSIZE*NSIZE];

	int row,col,flag=1;
	for(row=NSIZE-1;row>=0;row--)
	{
		for(col=NSIZE-1;col>=0;col--)
		{
			if(IS_PLACED_BY_COMMAN(mat[row*NSIZE+col])||IS_PLACED_FIXED(mat[row*NSIZE+col]))
				iMask[row*NSIZE+col]=mat[row*NSIZE+col];
			else 
			{
				if(flag&&(type!=FIRST_SOLUTION))
					count=row*NSIZE+col;
				flag=0;
				iMask[row*NSIZE+col]=0;
			}

		}
	}

	//memcpy(iMask,mat,sizeof(int)*NSIZE*NSIZE);
	while ( count >= 0 && count< (NSIZE*NSIZE) )
    {
        int row = count/NSIZE;
        int col = count%NSIZE;

        if (!iMask[row*NSIZE+col])
        {
            
			prevcount= mat[row*NSIZE+col]%(NSIZE+1);
			mat[row*NSIZE+col]= 0;
			int num=NSIZE+1;
			for(num = prevcount+1; num <= NSIZE; num++)
			{
				if (IsNumPlacementPossible(mat,num,row*NSIZE+col))
				{
					mat[row*NSIZE+col]= num+10*PLACED_BY_BACKTRACE;
					progress=+1;//FORWARD;
					break;
				}
			}
			if (num>NSIZE)
				progress=-1;	//BACK
        }
		count += progress;
    }
	return 1;
}

//type:
//	WRITE_SOLUTION          0		//Computer will modify original matrix
//	DONTWRITEINSOLUTION		1		//Computer will not modify original matrix
//output:
//	-2	Solution Impossible
//	-1	No hint is available
//	0	means operation successful
//  1	Solution is completed
int SudokuSolution::SetNextValComManMethod(int mat[],int type,int *hints)
{
	int temp;	
	int flagrow,flagcol,flagblock,flagcompleted=false,flagpossible=true,flagonescancompleted=false;
	for(int count=0;true;count++)
	{
		count=count%TSIZE;
		if(count==0)
		{
			if(flagcompleted)
				return 1;				//Solution is filled up
			if(!flagpossible)
				return -2;				//Previous Mistake..Solution Impossible
			if(flagonescancompleted)
				return -1;				//No hints..Solvable by backtracing
			flagcompleted=true;
			flagpossible=false;
			flagonescancompleted=true;
		}
		if(mat[count]%(NSIZE+1)==0||IS_PLACED_BY_HINTS(mat[count]))			//This is blank space to be filled
		{
			mat[count]=0;
			flagcompleted=false;
			int row=count/NSIZE,col=count%NSIZE;
			int block=row/KSIZE+KSIZE*(col/KSIZE);
			for(int num=1;num<=NSIZE;num++)
			{
				flagrow=-1;				//-1 means still nowhere "num" can be placed
				flagcol=-1;				//-2 means "num" can be placed more than one place
				flagblock=-1;			//other value means "num" can be placed on that place
				for(int i=0;i<NSIZE;i++)
				{
					//Solve in Col..number is fixed...changing row
					temp=i*NSIZE+col;
					if(IsNumPlacementPossible(mat,num,temp)&&mat[temp]==0)  
						flagrow=(flagrow==-1)?temp:-2;

					//Solve in Row..number is fixed...changing col
					temp=row*NSIZE+i;
					if(IsNumPlacementPossible(mat,num,temp)&&mat[temp]==0)  
						flagcol=(flagcol==-1)?temp:-2;

					//Solve in Block..number is fixed...changing row,col inside block
					temp=(block/KSIZE)*KSIZE+i%KSIZE + ((block%KSIZE)*KSIZE+i/KSIZE)*NSIZE;
					if(IsNumPlacementPossible(mat,num,temp)&&mat[temp]==0)  
						flagblock=(flagblock==-1)?temp:-2;
				}
				if(flagrow==-2||flagcol==-2||flagblock==-2)
					flagpossible=true;
				if(flagrow>=0)
				{
					if(!(type&DONTWRITEINSOLUTION))
						mat[flagrow]=num+10*PLACED_BY_COMMAN;
					if(hints)
						*hints=num+10*flagrow;
					return 0;
				}
				if(flagcol>=0)
				{
					
					if(!(type&DONTWRITEINSOLUTION))
						mat[flagcol]=num+10*PLACED_BY_COMMAN;
					if(hints)
						*hints=num+10*flagcol;
					return 0;
				}
				if(flagblock>=0)
				{
					
					if(!(type&DONTWRITEINSOLUTION))
						mat[flagblock]=num+10*PLACED_BY_COMMAN;
					if(hints)
						*hints=num+10*flagblock;
					return 0;
				}
			}
			
		}
	}		
	return -1;
}


bool SudokuSolution::IsNumPlacementPossible(int matrix[],int num,int select)
{
	int i,col=select%NSIZE,row=select/NSIZE,x,y;
	int block=row/KSIZE+KSIZE*(col/KSIZE);
	for(i=0;i<NSIZE;i++)
	{
		if((matrix[row*NSIZE+i]%(NSIZE+1))==num)
			return false;
		if((matrix[i*NSIZE+col]%(NSIZE+1))==num)
			return false;
		x=(block/KSIZE)*KSIZE+i%KSIZE;
		y=(block%KSIZE)*KSIZE+i/KSIZE;
		if((matrix[x+y*NSIZE]%(NSIZE+1))==num)
			return false;
	}
	return true;
}

//output: 
//	-1			No hints available
//	num+10*pos	Hints are available

int SudokuSolution::IsHint(void)
{
	int row,col,pos;
	for(row=0;row<NSIZE;row++)
	{
		for(col=0;col<NSIZE;col++)
		{
			if(IS_PLACED_BY_HINTS(sudval[row][col]))
			{
				pos=row*NSIZE+col;
				return(10*pos+sudval[row][col]%(NSIZE+1));
			}

		}
	}
	return -1;
}
//output: 
//	-2			Solution impossible/Errors are there
//	-1			No hints available
//	num+10*pos	Hints are available
//	0			Solution completed
int SudokuSolution::FindHint()
{
	int hint;
	int irslt=SetNextValComManMethod((int*)sudval,DONTWRITEINSOLUTION,&hint);
	if(irslt==1)
	{
		//solution filledup
		if(IsSodukuSolved())
			return 0;
	}
	else if(irslt==0)
	{
		//Hints found/operation successful
		if(SetValueByPos(hint/10,hint%(NSIZE+1),PLACED_HINTS))
		{
			return hint;
		}
	}
	else if(irslt==-2)
		return -2;
	//No Hints Found
	return -1;

}
void Swap(int* a,int* b)
{
	int temp=*a;
	*a=*b;
	*b=temp;
}
DWORD SudokuSolution::Start_Generate(void)
{
	int sample[NSIZE*NSIZE]={1,2,9,	6,3,7,	5,8,4,
							4,3,8,	2,9,5,	6,1,7,
							6,5,7,	8,1,4,	3,2,9,

							3,6,5,	7,2,8,	9,4,1,
							7,1,2,	3,4,9,	8,5,6,
							8,9,4,	1,5,6,	2,7,3,

							5,7,3,	4,6,2,	1,9,8,
							9,4,1,	5,8,3,	7,6,2,
							2,8,6,	9,7,1,	4,3,5};
	int change_row[]={0,1,2,3,4,5,6,7,8};
	int change_col[]={0,1,2,3,4,5,6,7,8};
	int changerowflag,changecolflag,flag,tempmat[NSIZE*NSIZE]={0};
	short shift;

	//Generate new solved space
	srand((unsigned)time(NULL));
	changerowflag=rand();
	srand((unsigned)changerowflag);
	changerowflag|=(rand()<<15);
	srand(changerowflag);
	changecolflag=rand();
	srand((unsigned)changecolflag);
	changecolflag|=(rand()<<15);
	for(shift=0;shift<32;shift+=6)
	{
		flag=(changerowflag>>shift)&0x3f;
		if(flag&0x01)
			Swap(&change_row[0],&change_row[1]);
		if(flag&0x02)
			Swap(&change_row[0],&change_row[2]);
		if(flag&0x04)
			Swap(&change_row[1],&change_row[2]);
		if(flag&0x08)
		{
			Swap(&change_row[0],&change_row[3]);
			Swap(&change_row[1],&change_row[4]);
			Swap(&change_row[2],&change_row[5]);
		}
		if(flag&0x10)
		{
			Swap(&change_row[0],&change_row[6]);
			Swap(&change_row[1],&change_row[7]);
			Swap(&change_row[2],&change_row[8]);
		}
		if(flag&0x20)
		{
			Swap(&change_row[3],&change_row[6]);
			Swap(&change_row[4],&change_row[7]);
			Swap(&change_row[5],&change_row[8]);
		}

		flag=(changecolflag>>shift)&0x3f;
		if(flag&0x01)
			Swap(&change_col[0],&change_col[1]);
		if(flag&0x02)
			Swap(&change_col[0],&change_col[2]);
		if(flag&0x04)
			Swap(&change_col[1],&change_col[2]);
		if(flag&0x08)
		{
			Swap(&change_col[0],&change_col[3]);
			Swap(&change_col[1],&change_col[4]);
			Swap(&change_col[2],&change_col[5]);
		}
		if(flag&0x10)
		{
			Swap(&change_col[0],&change_col[6]);
			Swap(&change_col[1],&change_col[7]);
			Swap(&change_col[2],&change_col[8]);
		}
		if(flag&0x20)
		{
			Swap(&change_col[3],&change_col[6]);
			Swap(&change_col[4],&change_col[7]);
			Swap(&change_col[5],&change_col[8]);
		}
	}
	for(changerowflag=0;changerowflag<NSIZE;changerowflag++)
	{
		for(changecolflag=0;changecolflag<NSIZE;changecolflag++)
		{
			tempmat[changerowflag*NSIZE+changecolflag]=sample[change_row[changerowflag]*NSIZE+change_col[changecolflag]];
			
			//This is for next use of sample for removiing redundancy
			sample[change_row[changerowflag]*NSIZE+change_col[changecolflag]]=change_row[changerowflag]*NSIZE+change_col[changecolflag];
		}
	}
	int mat[TSIZE];
	memcpy(mat,tempmat,TSIZE*sizeof(int));
	//Remove Redundancy
	//"sample" is the colection of unchecked position and "changerowflag" is the number of unchecked position
	changerowflag=TSIZE;

	int totalcheck=level*15+rand()%5;	
	if(level==0)
		totalcheck=0;

	while(true)
	{
		
		if(changerowflag<=totalcheck)
			break;
		changecolflag=rand()%changerowflag;
		flag=sample[changecolflag];
		sample[changecolflag]=sample[changerowflag-1];

		tempmat[flag]=0;
		if(IsSodukuSolvable(tempmat,COMMAN_SOLUTION|DONTUSEBACKTRACING)==0)
			mat[flag]=0;
		memcpy(tempmat,mat,TSIZE*sizeof(int));
		changerowflag--;
	}
	SetSudoku(mat);
	SendMessage(hWnd,WM_SUDOKUGENERATED,0,0);
	return 1;
}

bool SudokuSolution::GenerateSoduku(HWND hwnd)
{
	hWnd=hwnd;
	CreateThread(NULL, 0, &runnable::run_thread, this, 0, NULL);
	return true;
}

