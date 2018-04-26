// sudoku.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "sudoku.h"
#include "sudokusolution.h"
#include <Commdlg.h>

#define KSIZE 3
#define NSIZE 9

#define MAX_LOADSTRING 100
#define SUBBLOCKSIZE 50

#define IDT_HINTS_TIMER 1
#define IDT_TIME_TIMER 2
#define ID_BUTTON_DONE 34056

#define MESSAGEBOX_MESSAGE	-1
#define MESSAGEBOX_TIME		-2


//			Numbering Format
//		0,  1 ,................,8
//		9,  10,...............,17
//		18, 19,...............,26
//				--
//				--
//				--		
//		72 ,73,74,.............,80




// Global Variables:
HINSTANCE hInst;								// current instance
TCHAR szTitle[MAX_LOADSTRING];					// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];			// the main window class name

HBITMAP hBit;

int SelectedPos=-1;

int iBlockNum1[NSIZE*NSIZE]={0,0,0, 8,4,0, 0,0,0, 
							1,0,0, 0,0,3, 0,0,6,
							0,0,0, 0,0,0, 9,0,2,

							0,0,4, 0,8,0, 0,3,0,
							0,0,8, 0,6,0, 1,0,0, 
							0,1,0, 0,3,0, 2,0,0,

							4,7,9, 0,0,0, 0,0,0,
							6,0,0, 9,0,7, 0,0,0,
							0,0,0, 0,2,4, 0,0,7};

int iBlockNum2[NSIZE*NSIZE]={0,2,0, 0,0,7, 5,0,4, 
							4,0,0, 2,9,0, 0,1,0,
							6,0,7, 0,0,4, 0,0,9,

							3,0,5, 0,2,0, 0,0,0,
							7,1,0, 0,4,0, 8,0,6, 
							0,0,0, 0,5,6, 2,7,0,

							5,7,0, 4,0,0, 0,9,8,
							0,0,1, 5,0,3, 0,6,0,
							0,8,0, 9,0,0, 4,0,0};
int iBlockNum3[NSIZE*NSIZE]={0,0,0, 0,0,0, 0,0,0, 
							0,0,0, 0,0,3, 0,8,5,
							0,0,1, 0,2,0, 0,0,0,

							0,0,0, 5,0,7, 0,0,0,
							0,0,4, 0,0,0, 1,0,0, 
							0,9,0, 0,0,0, 0,0,0,

							5,0,0, 0,0,0, 0,7,3,
							0,0,2, 0,1,0, 0,0,0,
							0,0,0, 0,4,0, 0,0,9};

int iBlockNum4[NSIZE*NSIZE]={8,0,4, 9,0,0, 7,1,6, 
							9,0,5, 1,0,7, 0,0,5,
							0,0,1, 0,2,0, 0,0,0,

							0,0,0, 5,0,7, 0,0,0,
							0,0,4, 0,0,0, 1,0,0, 
							0,9,0, 0,0,0, 0,0,0,

							5,0,0, 0,0,0, 0,7,3,
							0,0,2, 0,1,0, 0,0,0,
							0,0,0, 0,4,0, 0,0,9};



SudokuSolution current=SudokuSolution();

// Forward declarations of functions included in this code module:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);
void InvalidateSubblock(HWND hWnd,int iNum);

bool OpenGameProblem(HWND hDlg);
bool SaveGameProblem(HWND hDlg,bool type);


int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

 	// TODO: Place code here.
	MSG msg;
	HACCEL hAccelTable;

	// Initialize global strings
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_SUDOKU, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(ID_ACCELERATOR));

	// Main message loop:
	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int) msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
//  COMMENTS:
//
//    This function and its usage are only necessary if you want this code
//    to be compatible with Win32 systems prior to the 'RegisterClassEx'
//    function that was added to Windows 95. It is important to call this function
//    so that the application will get 'well formed' small icons associated
//    with it.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_SUDOKU));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_SUDOKU);
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	HWND hWnd;
	hInst = hInstance; // Store instance handle in our global variable
	
	hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPED| WS_CAPTION |WS_SYSMENU|WS_DLGFRAME|WS_MINIMIZEBOX,CW_USEDEFAULT, 0,SUBBLOCKSIZE*NSIZE+26, SUBBLOCKSIZE*NSIZE+98, NULL, NULL, hInstance, NULL);
	hBit=(HBITMAP)LoadImage(hInst,MAKEINTRESOURCE(IDB_SUDOKU),IMAGE_BITMAP, 0, 0, LR_COPYFROMRESOURCE);

	if (!hWnd)
	{
      return FALSE;
	}
	
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

   return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND	- process the application menu
//  WM_PAINT	- Paint the main window
//  WM_DESTROY	- post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hdc;
	RECT rt;
	int irslt;

	switch (message)
	{
	case WM_CREATE:
		current.SetSudoku(iBlockNum3);
		current.SetPaintMessage("Ready");
		SetTimer(hWnd,IDT_TIME_TIMER,1000,(TIMERPROC) NULL);
		current.StartTimer(true);
		break;

	//it is called by the soduku solution class
	case WM_SUDOKUGENERATED:
		current.SetPaintMessage("New Sudoku Generated");
		InvalidateRect(hWnd,NULL,FALSE);
		SetTimer(hWnd,IDT_TIME_TIMER,1000,(TIMERPROC) NULL);
		current.StartTimer(true);
		break;
	case WM_COMMAND:
		// Parse the menu selections:
		switch (LOWORD(wParam))
		{
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case ID_FILE_GIVE:	
			KillTimer(hWnd,IDT_TIME_TIMER);
			CreateWindow( L"button", L"Done", WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON, 
                         SUBBLOCKSIZE*8-18 ,SUBBLOCKSIZE*NSIZE+12 , SUBBLOCKSIZE+25, 
                         SUBBLOCKSIZE-22, hWnd,(HMENU) ID_BUTTON_DONE,  hInst, NULL );
			current.SetPaintMessage("Give Soduku Values");
			//InvalidateSubblock(hWnd,MESSAGEBOX_MESSAGE);
			current.GiveSudoku(true);

			//Disable Menus
			EnableMenuItem(GetMenu(hWnd), ID_TOOLS_SOLVE,MF_GRAYED);
			EnableMenuItem(GetMenu(hWnd), ID_TOOLS_SOLVEFROMSTART  ,MF_GRAYED);
			EnableMenuItem(GetMenu(hWnd), ID_TOOLS_CHECKSUDOKU,MF_GRAYED);
			EnableMenuItem(GetMenu(hWnd), ID_TOOLS_HINTS,MF_GRAYED);
			EnableMenuItem(GetMenu(hWnd), ID_FILE_GIVE,MF_GRAYED);
			
			InvalidateRect(hWnd,NULL,FALSE);
			break;
		case ID_BUTTON_DONE:
			//Soduku is totally given

			//Enable Menus
			EnableMenuItem(GetMenu(hWnd), ID_TOOLS_SOLVE,MF_ENABLED);
			EnableMenuItem(GetMenu(hWnd), ID_TOOLS_SOLVEFROMSTART  ,MF_ENABLED);
			EnableMenuItem(GetMenu(hWnd), ID_TOOLS_CHECKSUDOKU,MF_ENABLED);
			EnableMenuItem(GetMenu(hWnd), ID_TOOLS_HINTS,MF_ENABLED);
			EnableMenuItem(GetMenu(hWnd), ID_FILE_GIVE,MF_ENABLED);
			DestroyWindow(GetDlgItem(hWnd,ID_BUTTON_DONE));
			current.GiveSudoku(false);

			current.FixGiveSudoku();
			current.SetPaintMessage("Ready");
			InvalidateRect(hWnd,NULL,FALSE);
			SetTimer(hWnd,IDT_TIME_TIMER,1000,(TIMERPROC) NULL);
			current.StartTimer(true);
			break;
		case ID_AUTOGENERATE_EASY:	
			if(current.IsGiveSudoku())
			{
				//Enable Menus
				EnableMenuItem(GetMenu(hWnd), ID_TOOLS_SOLVE,MF_ENABLED);
				EnableMenuItem(GetMenu(hWnd), ID_TOOLS_SOLVEFROMSTART  ,MF_ENABLED);
				EnableMenuItem(GetMenu(hWnd), ID_TOOLS_CHECKSUDOKU,MF_ENABLED);
				EnableMenuItem(GetMenu(hWnd), ID_TOOLS_HINTS,MF_ENABLED);
				EnableMenuItem(GetMenu(hWnd), ID_FILE_GIVE,MF_ENABLED);
				DestroyWindow(GetDlgItem(hWnd,ID_BUTTON_DONE));
				current.GiveSudoku(false);
			}
			else
				KillTimer(hWnd,IDT_TIME_TIMER);
			current.SetLevel(3);
			current.GenerateSoduku(hWnd);
			current.SetPaintMessage("Wait Generating Sudoku...");
			InvalidateSubblock(hWnd,MESSAGEBOX_MESSAGE);
			break;
		case ID_AUTOGENERATE_MEDIUM:	
			if(current.IsGiveSudoku())
			{
				//Enable Menus
				EnableMenuItem(GetMenu(hWnd), ID_TOOLS_SOLVE,MF_ENABLED);
				EnableMenuItem(GetMenu(hWnd), ID_TOOLS_SOLVEFROMSTART  ,MF_ENABLED);
				EnableMenuItem(GetMenu(hWnd), ID_TOOLS_CHECKSUDOKU,MF_ENABLED);
				EnableMenuItem(GetMenu(hWnd), ID_TOOLS_HINTS,MF_ENABLED);
				EnableMenuItem(GetMenu(hWnd), ID_FILE_GIVE,MF_ENABLED);
				DestroyWindow(GetDlgItem(hWnd,ID_BUTTON_DONE));
				current.GiveSudoku(false);
			}
			else
				KillTimer(hWnd,IDT_TIME_TIMER);
			current.SetLevel(2);
			current.GenerateSoduku(hWnd);
			current.SetPaintMessage("Wait Generating Sudoku...");
			InvalidateSubblock(hWnd,MESSAGEBOX_MESSAGE);
			break;
		case ID_AUTOGENERATE_HARD:	
			if(current.IsGiveSudoku())
			{
				//Enable Menus
				EnableMenuItem(GetMenu(hWnd), ID_TOOLS_SOLVE,MF_ENABLED);
				EnableMenuItem(GetMenu(hWnd), ID_TOOLS_SOLVEFROMSTART  ,MF_ENABLED);
				EnableMenuItem(GetMenu(hWnd), ID_TOOLS_CHECKSUDOKU,MF_ENABLED);
				EnableMenuItem(GetMenu(hWnd), ID_TOOLS_HINTS,MF_ENABLED);
				EnableMenuItem(GetMenu(hWnd), ID_FILE_GIVE,MF_ENABLED);
				DestroyWindow(GetDlgItem(hWnd,ID_BUTTON_DONE));
				current.GiveSudoku(false);
			}
			else
				KillTimer(hWnd,IDT_TIME_TIMER);
			current.SetLevel(1);
			current.GenerateSoduku(hWnd);
			current.SetPaintMessage("Wait Generating Sudoku...");
			InvalidateSubblock(hWnd,MESSAGEBOX_MESSAGE);
			break;
		case ID_AUTOGENERATE_HARDEST:	
			if(current.IsGiveSudoku())
			{
				//Enable Menus
				EnableMenuItem(GetMenu(hWnd), ID_TOOLS_SOLVE,MF_ENABLED);
				EnableMenuItem(GetMenu(hWnd), ID_TOOLS_SOLVEFROMSTART  ,MF_ENABLED);
				EnableMenuItem(GetMenu(hWnd), ID_TOOLS_CHECKSUDOKU,MF_ENABLED);
				EnableMenuItem(GetMenu(hWnd), ID_TOOLS_HINTS,MF_ENABLED);
				EnableMenuItem(GetMenu(hWnd), ID_FILE_GIVE,MF_ENABLED);
				DestroyWindow(GetDlgItem(hWnd,ID_BUTTON_DONE));
				current.GiveSudoku(false);
			}
			else
				KillTimer(hWnd,IDT_TIME_TIMER);
			current.SetLevel(0);
			current.GenerateSoduku(hWnd);
			current.SetPaintMessage("Wait Generating Sudoku...");
			InvalidateSubblock(hWnd,MESSAGEBOX_MESSAGE);
			break;

		case ID_TOOLS_SOLVE:
			current.StartTimer(false);
			irslt=current.SolveSudoku(COMMAN_SOLUTION);
			while(true)
			{
				if(irslt==0)
				{
					current.SetPaintMessage("Solved:Successfully Uniquely");
					InvalidateRect(hWnd,NULL,FALSE);
				}
				else if(irslt==1)
				{
					current.SetPaintMessage("Solved:Many Solution");
					InvalidateRect(hWnd,NULL,FALSE);
					if(MessageBox(hWnd,L"Want to get next solution",L"Many Solution",MB_YESNO)==6)
					{
						current.SolveSudokuByBackTracing(NEXT_SOLUTION);
						continue;
					}
					else
						break;

				}
				else
				{
					current.SetPaintMessage("Error:Can't Be Solved");
					InvalidateRect(hWnd,NULL,FALSE);
				}
			}
			break;
		case ID_TOOLS_SOLVEFROMSTART:
			current.StartTimer(false);
			irslt=current.SolveSudoku(WITHOUTUSER_SOLUTION);
			if(irslt==0)
			{
				current.SetPaintMessage("Solved Uniquely");
				InvalidateRect(hWnd,NULL,FALSE);
			}
			else if(irslt==1)
			{
				current.SetPaintMessage("Solved not uniquely");
				InvalidateRect(hWnd,NULL,FALSE);
			}
			else
			{
				current.SetPaintMessage("Cannot Be Solved");
				InvalidateRect(hWnd,NULL,FALSE);
			}
			break;
		case ID_TOOLS_CHECKSUDOKU:
			irslt=current.CheckSudoku();
			if(irslt==0)
			{
				current.SetPaintMessage("Yet Not Filled up");
				InvalidateSubblock(hWnd,MESSAGEBOX_MESSAGE);
			}
			else if(irslt==1)
			{
				current.SetPaintMessage("Solved Correctly");
				InvalidateRect(hWnd,NULL,FALSE);
				current.StartTimer(false);
			}
			else
			{
				current.SetPaintMessage("Wrong Result");
				InvalidateRect(hWnd,NULL,FALSE);
			}
			break;
		case ID_TOOLS_HINTS:
			irslt=current.FindHint();
			if(irslt==0)
			{
				current.SetPaintMessage("Already Solved");
				InvalidateSubblock(hWnd,MESSAGEBOX_MESSAGE);
			}
			else if(irslt>0)
			{
				current.SetPaintMessage("Hint got successfully");
				InvalidateSubblock(hWnd,irslt/10);
				InvalidateSubblock(hWnd,MESSAGEBOX_MESSAGE);
 
				SetTimer(hWnd,IDT_HINTS_TIMER,5000,(TIMERPROC) NULL);

			}
			else if(irslt==-2)
			{
				current.SetPaintMessage("Errors are there");
				InvalidateSubblock(hWnd,MESSAGEBOX_MESSAGE);
			}
			else
			{
				current.SetPaintMessage("Hint unavailable");
				InvalidateSubblock(hWnd,MESSAGEBOX_MESSAGE);
			}
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	case WM_TIMER:
		switch (wParam) 
		{ 
		case IDT_HINTS_TIMER: 
			KillTimer(hWnd, IDT_HINTS_TIMER); 
			// Remove the hints(if any)
			irslt=current.IsHint();
			if(irslt>0)
			{
				current.SetValueByPos(irslt/10,0,PLACED_BY_USER);
				InvalidateSubblock(hWnd,irslt/10);
				current.SetPaintMessage("Ready");
				InvalidateSubblock(hWnd,MESSAGEBOX_MESSAGE);
			}
			else
				InvalidateRect(hWnd,NULL,FALSE);
			break;
		case IDT_TIME_TIMER:
			if(current.IsTimerStarted())
			{
				current.IncrementTimer();
				InvalidateSubblock(hWnd,MESSAGEBOX_TIME);
			}
			// process the 1sec timer 
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		} 
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		// TODO: Add any drawing code here...
		
		if(hdc)
		{
			//Show Messagebox
			RoundRect(hdc,7,SUBBLOCKSIZE*NSIZE+12,SUBBLOCKSIZE*8-20, SUBBLOCKSIZE*NSIZE+40 ,10,10);
			RoundRect(hdc,SUBBLOCKSIZE*8-18,SUBBLOCKSIZE*NSIZE+12,SUBBLOCKSIZE*NSIZE+7, SUBBLOCKSIZE*NSIZE+40 ,10,10);
			char* messg=current.GetPaintMessage();
		
			SetTextColor(hdc,RGB(100,0,0));
			LOGFONT lf={0};
			lf.lfHeight=18;
			lf.lfWidth=7;
			lf.lfWeight=FW_MEDIUM;
			wcscpy_s(lf.lfFaceName,_T("Times New Roman"));
			HFONT hf=CreateFontIndirect(&lf);
			SelectObject(hdc,hf);
			TextOutA(hdc,18,SUBBLOCKSIZE*NSIZE+18,messg,strlen(messg));
			
			char mesg[10];
			DWORD dtime=current.GetTimerValue();
			sprintf_s(mesg,"%2d:%02d:%02d",dtime/3600,(dtime/60)%60,dtime%60);
			SetTextColor(hdc,RGB(255,0,0));
			TextOutA(hdc,SUBBLOCKSIZE*8-8,SUBBLOCKSIZE*NSIZE+18,mesg,strlen(mesg));
			DeleteObject(hf);
		}
		
		//If image is loaded
		if(hBit&&hdc)
		{
			HDC hMem=CreateCompatibleDC(hdc);
			SelectObject(hMem,hBit);
			//HBRUSH hRed=CreateSolidBrush(RGB(200,0,0));
			
			BITMAP bm;
			GetObject(hBit, sizeof(bm), &bm);			
			SetStretchBltMode(hdc,COLORONCOLOR);
			StretchBlt(hdc,7,7,SUBBLOCKSIZE*NSIZE,SUBBLOCKSIZE*NSIZE,hMem,0,0,bm.bmWidth,bm.bmHeight,SRCCOPY);
			DeleteDC(hMem);

			
			RECT rt={7,7,7+NSIZE*SUBBLOCKSIZE,7+NSIZE*SUBBLOCKSIZE};
			DrawEdge(hdc,&rt,EDGE_BUMP,BF_RECT);


			LOGFONT lf={0};
			lf.lfHeight=30;
			lf.lfWidth=15;
			lf.lfWeight=FW_SEMIBOLD;
			wcscpy_s(lf.lfFaceName,_T("Times New Roman"));
			HFONT hf=CreateFontIndirect(&lf);
			SelectObject(hdc,hf);
			//SetBkColor(hdc,GetSysColor(COLOR_WINDOW));
			int index,num;
			SetBkMode(hdc,TRANSPARENT);
			for(index=0;index<NSIZE*NSIZE;index++)
			{		
				current.GetValueByPos(index,&num);
				rt.left=20+SUBBLOCKSIZE*(index%9);
				rt.top=20+SUBBLOCKSIZE*((index/9)%9);
				rt.right=SUBBLOCKSIZE*(index%9+1)-12;
				rt.bottom=SUBBLOCKSIZE*((index/9)%9+1)-8;


				if(num!=0)
				{
					WCHAR str[3];
					wsprintf(str,_T("%d"),num%10);

					// Send some text out
					if(IS_PLACED_FIXED(num))
						SetTextColor(hdc,RGB(100,20,50)); //FIXED
					else if(IS_PLACED_BY_COMMAN(num)||IS_PLACED_BY_BACKTRACE(num))	//COMMAN
						SetTextColor(hdc,RGB(255,0,100)); //ERROR
					else if(IS_PLACED_ERROR(num))
						SetTextColor(hdc,RGB(255,0,0)); //ERROR
					else if(IS_PLACED_BY_HINTS(num))
						SetTextColor(hdc,RGB(250,100,50));//HINT
					else 
						SetTextColor(hdc,RGB(0,50,255)); //USER
					TextOut(hdc,rt.left+2,rt.top-2,str,1);
				}
			}
			DeleteObject(hf);			
		}	
		EndPaint(hWnd, &ps);
		break;
	case WM_MOUSEMOVE:
		if(GET_X_LPARAM(lParam)>7&&GET_X_LPARAM(lParam)<(7+SUBBLOCKSIZE*NSIZE)&&GET_Y_LPARAM(lParam)>7&&GET_Y_LPARAM(lParam)<(7+SUBBLOCKSIZE*NSIZE))
		{
			int xpress=(GET_X_LPARAM(lParam)-7)/SUBBLOCKSIZE,ypress=(GET_Y_LPARAM(lParam)-7)/SUBBLOCKSIZE;
			if(SelectedPos!=xpress+ypress*NSIZE)
			{
				InvalidateSubblock(hWnd,SelectedPos);
				SelectedPos=xpress+ypress*NSIZE;
			}
			hdc=GetDC(hWnd);
			rt.left=(SelectedPos%NSIZE)*SUBBLOCKSIZE+7;
			rt.top=(SelectedPos/NSIZE)*SUBBLOCKSIZE+7;
			rt.right=(SelectedPos%NSIZE)*SUBBLOCKSIZE+SUBBLOCKSIZE+6;
			rt.bottom=(SelectedPos/NSIZE)*SUBBLOCKSIZE+SUBBLOCKSIZE+6;
			DrawFocusRect(hdc,&rt);
			ReleaseDC(hWnd,hdc);
		}
		else if(SelectedPos!=-1)
		{
			InvalidateSubblock(hWnd,SelectedPos);
			SelectedPos=-1;
		}
		break;
	case WM_CHAR:
		//Erasing Message

		if(wParam>='0'&&wParam<='9'&&SelectedPos!=-1)
		{
			current.SetValueByPos(SelectedPos,wParam-'0',PLACED_BY_USER);
			InvalidateSubblock(hWnd,SelectedPos);
			if(!current.IsGiveSudoku())
			{
				if(current.IsSodukuSolved())
				{
					//current.SetPaintMessage("Solved Successfully");
					current.StartTimer(false);
					if(MessageBox(hWnd,_T("Congratulations-You have successfully finished this game.\nDo you want to play another game?\nPress \"Yes\" to play again or \"No\" to Exit from this application"),_T("You Won"),MB_YESNO)==IDYES)
					{
						KillTimer(hWnd,IDT_TIME_TIMER);
						current.GenerateSoduku(hWnd);
						current.SetPaintMessage("Wait Generating Sudoku...");
						InvalidateSubblock(hWnd,MESSAGEBOX_MESSAGE);
					}
					else
						SendMessage(hWnd,WM_DESTROY,0,0);
				}
				else
				{
					if(!current.IsTimerStarted())
						current.StartTimer(true);
					current.SetPaintMessage("Ready");
				}
			}
			InvalidateSubblock(hWnd,MESSAGEBOX_MESSAGE);
				
		}
		break;
	case WM_MOUSEWHEEL:
		if(SelectedPos!=-1)
		{
			int num;
			current.GetValueByPos(SelectedPos,&num);
			if(IS_PLACED_FIXED(num))
				break;
			else if(IS_PLACED_BY_HINTS(num))
				num=0;
			num=(NSIZE+1+num+GET_WHEEL_DELTA_WPARAM(wParam)/120)%(NSIZE+1);
			current.SetValueByPos(SelectedPos,num,PLACED_BY_USER);
			InvalidateSubblock(hWnd,SelectedPos);
			if(!current.IsGiveSudoku())
			{
				if(current.IsSodukuSolved())
				{
					//current.SetPaintMessage("Solved Successfully");
					current.StartTimer(false);
					if(MessageBox(hWnd,_T("Congratulations-You have successfully finished this game.\nDo you want to play another game?\nPress \"Yes\" to play again or \"No\" to Exit from this application"),_T("You Won"),MB_YESNO)==IDYES)
					{
						KillTimer(hWnd,IDT_TIME_TIMER);
						current.GenerateSoduku(hWnd);
						current.SetPaintMessage("Wait Generating Sudoku...");
						InvalidateSubblock(hWnd,MESSAGEBOX_MESSAGE);
					}
					else
						SendMessage(hWnd,WM_DESTROY,0,0);
				}
				else
				{
					if(!current.IsTimerStarted())
						current.StartTimer(true);
					current.SetPaintMessage("Ready");
				}
			}
			InvalidateSubblock(hWnd,MESSAGEBOX_MESSAGE);
		}
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}




void InvalidateSubblock(HWND hWnd,int iNum)
{
	RECT rt;
	if(iNum==MESSAGEBOX_MESSAGE)	//For messagebox message
	{
		rt.left=7;
		rt.top=SUBBLOCKSIZE*NSIZE+12;
		rt.right=SUBBLOCKSIZE*8-20;
		rt.bottom=SUBBLOCKSIZE*NSIZE+48;
	}
	else if(iNum==MESSAGEBOX_TIME)
	{
		rt.left=SUBBLOCKSIZE*8-12;
		rt.top=SUBBLOCKSIZE*NSIZE+18;
		rt.right=SUBBLOCKSIZE*NSIZE+7;
		rt.bottom=SUBBLOCKSIZE*NSIZE+48;
	}
	else
	{
		rt.left=(iNum%NSIZE)*SUBBLOCKSIZE+7;
		rt.top=(iNum/NSIZE)*SUBBLOCKSIZE+7;
		rt.right=(iNum%NSIZE)*SUBBLOCKSIZE+SUBBLOCKSIZE+6;
		rt.bottom=(iNum/NSIZE)*SUBBLOCKSIZE+SUBBLOCKSIZE+6;
	}
	InvalidateRect(hWnd,&rt,FALSE);
}



