// SocketSend.cpp : ����Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "SocketSend.h"
#include "function.h"

#define MAX_LOADSTRING 100

#define IDB_BUTTON 1025
#define IDB_BUTTON_CONNECT 1026
#define IDB_BUTTON_SEND 1027
#define IDB_BUTTON_CLOSE 1028

HWND editHwnd = NULL;//�ı���ľ��
HWND buttonHwnd = NULL;//��ť�ľ��
HWND buttonConnect = NULL;//���Ӱ�ť�ľ��
HWND buttonSend = NULL;//���Ͱ�ť�ľ��
HWND buttonClose = NULL;//�رհ�ť�ľ��

int connectStatus = FALSE;
SOCKET clientSocket = INVALID_SOCKET;

// ȫ�ֱ���:
HINSTANCE hInst;								// ��ǰʵ��
TCHAR szTitle[MAX_LOADSTRING];					// �������ı�
TCHAR szWindowClass[MAX_LOADSTRING];			// ����������
//add by wxjia
TCHAR szButton[MAX_LOADSTRING];

// �˴���ģ���а����ĺ�����ǰ������:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

 	// TODO: �ڴ˷��ô��롣
	MSG msg;
	HACCEL hAccelTable;

	// ��ʼ��ȫ���ַ���
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_SOCKETSEND, szWindowClass, MAX_LOADSTRING);
	LoadString(hInstance, IDS_BUTTON_TITLE, szButton, MAX_LOADSTRING);//by wxjia
	MyRegisterClass(hInstance);

	// ִ��Ӧ�ó����ʼ��:
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_SOCKETSEND));

	// ����Ϣѭ��:
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
//  ����: MyRegisterClass()
//
//  Ŀ��: ע�ᴰ���ࡣ
//
//  ע��:
//
//    ����ϣ��
//    �˴��������ӵ� Windows 95 �еġ�RegisterClassEx��
//    ����֮ǰ�� Win32 ϵͳ����ʱ������Ҫ�˺��������÷������ô˺���ʮ����Ҫ��
//    ����Ӧ�ó���Ϳ��Ի�ù�����
//    ����ʽ��ȷ�ġ�Сͼ�ꡣ
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
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_SOCKETSEND));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_SOCKETSEND);
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

//
//   ����: InitInstance(HINSTANCE, int)
//
//   Ŀ��: ����ʵ�����������������
//
//   ע��:
//
//        �ڴ˺����У�������ȫ�ֱ����б���ʵ�������
//        ��������ʾ�����򴰿ڡ�
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   HWND hWnd;

   hInst = hInstance; // ��ʵ������洢��ȫ�ֱ�����

   hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      400, 100, 400, 600, NULL, NULL, hInstance, NULL);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   //��ʼ������
   //����һ�����߳� �ڷ������Ի�������ʾʱ��
   SetTimer(hWnd,0,1000,MyTimerProc);

   return TRUE;
}

//
//  ����: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  Ŀ��: ���������ڵ���Ϣ��
//
//  WM_COMMAND	- ����Ӧ�ó���˵�
//  WM_PAINT	- ����������
//  WM_DESTROY	- �����˳���Ϣ������
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;

	switch (message)
	{
	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// �����˵�ѡ��:
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		case IDB_BUTTON:
			
			break;
		case IDB_BUTTON_CONNECT:
			if(connectStatus != TRUE)//��δ���ӷ���������������
			{
				//Windows Sockets ��̬��ĳ�ʼ��
				InitDll(hWnd);
				//����SOCKET
				InitSocket(clientSocket, hWnd);
				//���ӷ�����
				connectServer(hWnd);
			}
			if(connectStatus != TRUE)
			{
				mySetWindowText("���ӷ�����ʧ��");
				break;
			}
			break;
		case IDB_BUTTON_SEND:
		{
			//���úͻ�ȡ�ı����������
			TCHAR editTextBuffer[MAX_NUM_BUF];
			memset(editTextBuffer,0,MAX_NUM_BUF);
			GetWindowText(editHwnd, editTextBuffer, MAX_NUM_BUF);
			int ret = sendLine(clientSocket, editTextBuffer, hWnd);
			if (SOCKET_ERROR == ret)
			{
				mySetWindowText("����ʧ��");
				break;
			}
			break;
		}
		case IDB_BUTTON_CLOSE:
		{
			sendLine(clientSocket, "close", hWnd);
			int ret = closeService(clientSocket);
			if (FALSE == ret)
			{
				mySetWindowText("client has stopped");
				break;
			}else if(SOCKET_ERROR == ret)
			{
				mySetWindowText("client closesocket fail");
				break;
			}else if (TRUE == ret)
			{
				mySetWindowText("�ͻ��˹ر�socket�ɹ�");
				break;
			} 
			mySetWindowText("�������رղ��ɹ���������ôʧ�ܵ���Ҳ��֪��");
			break;
		}
			
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;

	case WM_LBUTTONDOWN:
		{
			//���úͻ�ȡ�ı����������
			TCHAR editTextBuffer[MAX_NUM_BUF];
			GetWindowText(editHwnd, editTextBuffer, MAX_NUM_BUF);
			HDC hdc = GetDC(hWnd);
			TCHAR newTextStr[MAX_NUM_BUF];
			wsprintf(newTextStr,TEXT("%s - %s"),editTextBuffer,"jiaweixi");
			SetWindowText(editHwnd, newTextStr);
		
			break;
		}
		
	case WM_CREATE:
		{
			editHwnd = CreateWindow("edit", NULL,
				WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL | ES_MULTILINE ,
				30, 100, 300, 100, hWnd, (HMENU)1, ((LPCREATESTRUCT) lParam) -> hInstance, NULL);

			buttonHwnd = CreateWindow("button", TEXT("���ӷ���"),
				WS_CHILD | WS_VISIBLE | WS_BORDER  ,
				30, 470, 90, 30, hWnd, (HMENU)IDB_BUTTON_CONNECT, ((LPCREATESTRUCT) lParam) -> hInstance, NULL);


			buttonConnect = CreateWindow("button", TEXT("��������"),
				WS_CHILD | WS_VISIBLE | WS_BORDER  ,
				130, 470, 90, 30, hWnd, (HMENU)IDB_BUTTON_SEND, ((LPCREATESTRUCT) lParam) -> hInstance, NULL);


			buttonSend = CreateWindow("button", TEXT("�Ͽ�����"),
				WS_CHILD | WS_VISIBLE | WS_BORDER  ,
				230, 470, 90, 30, hWnd, (HMENU)IDB_BUTTON_CLOSE, ((LPCREATESTRUCT) lParam) -> hInstance, NULL);


			buttonClose = CreateWindow("button", TEXT("jiaweixi"),
				WS_CHILD | WS_VISIBLE | WS_BORDER  ,
				30, 300, 100, 50, hWnd, (HMENU)IDB_BUTTON, ((LPCREATESTRUCT) lParam) -> hInstance, NULL);


			break;
		}
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		// TODO: �ڴ����������ͼ����...
		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// �����ڡ������Ϣ��������
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