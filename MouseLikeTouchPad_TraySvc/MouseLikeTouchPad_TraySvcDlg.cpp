
// MouseLikeTouchPad_TraySvcDlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "MouseLikeTouchPad_TraySvc.h"
#include "MouseLikeTouchPad_TraySvcDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMouseLikeTouchPadTraySvcDlg 对话框



CMouseLikeTouchPadTraySvcDlg::CMouseLikeTouchPadTraySvcDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MOUSELIKETOUCHPAD_TRAYSVC_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMouseLikeTouchPadTraySvcDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CMouseLikeTouchPadTraySvcDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_SYSCOMMAND()
	ON_COMMAND(IDM_EXIT, &CMouseLikeTouchPadTraySvcDlg::OnExit)
	ON_MESSAGE(WM_SYSTEMTRAY, OnSystemTray)
END_MESSAGE_MAP()


// CMouseLikeTouchPadTraySvcDlg 消息处理程序

BOOL CMouseLikeTouchPadTraySvcDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	AddTrayIcon();//添加托盘图标

	ShowBalloonTip(L"MouseLikeTouchPadTray_Svc", L"仿鼠标触摸板服务就绪", 1500, NIIF_INFO);

	// 加载托盘菜单资源
	m_Menu.LoadMenu(IDR_TRAYMENU);
	// 为对话框设置菜单
	this->SetMenu(&m_Menu);


	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	CString sCmdLine = AfxGetApp()->m_lpCmdLine;//只包含参数
	if (sCmdLine == L"ShowDialog") {//调用参数 //wcscmp(sCmdLine, L"ShowDialog") == 0//sCmdLine == L"ShowDialog"//sCmdLine.Compare(L"ShowDialog")==0
		//AfxMessageBox(sCmdLine);
		ShowWindow(SW_SHOWNORMAL);
	}
	else {
		//ModifyStyleEx(WS_EX_APPWINDOW, WS_EX_TOOLWINDOW);//从任务栏中去掉.
		//ShowWindow(SW_MINIMIZE);//注意此处是最小化，不是SW_HIDE
		PostMessage(WM_SYSCOMMAND, SC_CLOSE, 0);
	}
	

	// TODO: 在此添加额外的初始化代码

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CMouseLikeTouchPadTraySvcDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CMouseLikeTouchPadTraySvcDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CMouseLikeTouchPadTraySvcDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if (nID == SC_CLOSE) //SC_MINIMIZE
	{
		ShowWindow(SW_HIDE); //隐藏主窗口
		return;
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

void CMouseLikeTouchPadTraySvcDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);
	// TODO: Add your message handler code here
	if (nType == SIZE_MINIMIZED)
	{
		ShowWindow(SW_HIDE); // 当最小化时，隐藏主窗口              
	}
}

void CMouseLikeTouchPadTraySvcDlg::OnExit()
{
	//在托盘区删除图标

	Shell_NotifyIcon(NIM_DELETE, &m_nid);
	/*退出程序语句exit(0);postquitmessage(0);//onok(); oncancel();//sendmessage(wm_close, 0, 0);//exitprocess(0);//其中以exit(0)最为迅速，在实践方面*/
	PostQuitMessage(0);
	//ExitProcess(EXIT_OK);

}


//屏蔽 Esc 和 Enter 关闭窗口（由于Esc直接调用 OnCancel()，Enter 直接调用 OnOk()）
void CMouseLikeTouchPadTraySvcDlg::OnOK()
{
	return;
}
void CMouseLikeTouchPadTraySvcDlg::OnCancel()
{
	return;
}
void CMouseLikeTouchPadTraySvcDlg::OnClose()
{
	ShowWindow(SW_HIDE); //隐藏主
	// TODO:  在此添加消息处理程序代码和/或调用默认值

	//CDialogEx::OnCancel();
}

BOOL CMouseLikeTouchPadTraySvcDlg::DestroyWindow()
{
	// TODO: Add your specialized code here and/or call the base class

	// 在托盘区删除图标
	Shell_NotifyIcon(NIM_DELETE, &m_nid);
	/*退出程序语句exit(0);postquitmessage(0);//onok(); oncancel();//sendmessage(wm_close, 0, 0);//exitprocess(0);//其中以exit(0)最为迅速，在实践方面*/
	PostQuitMessage(0);
	//ExitProcess(EXIT_OK);
	return CDialog::DestroyWindow();

}


void CMouseLikeTouchPadTraySvcDlg::AddTrayIcon()
{
	//---------------------------托盘显示---------------------------------//

	m_nid.cbSize = (DWORD)sizeof(NOTIFYICONDATA);

	m_nid.hWnd = this->m_hWnd;

	m_nid.uID = IDR_MAINFRAME;

	m_nid.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;

	m_nid.uCallbackMessage = WM_SYSTEMTRAY;             // 自定义的消息名称

	m_nid.hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_MAINFRAME));

	wcscpy_s(m_nid.szTip, L"仿鼠标触摸板服务程序");                // 信息提示条

	Shell_NotifyIcon(NIM_ADD, &m_nid);                // 在托盘区添加图标
}

BOOL CMouseLikeTouchPadTraySvcDlg::ShowBalloonTip(LPCWSTR szMsg, LPCWSTR szTitle, UINT uTimeOut, DWORD dwInfoFlags)
{
	//---------------------------托盘冒泡消息---------------------------------//

	m_nid.cbSize = (DWORD)sizeof(NOTIFYICONDATA);

	m_nid.uFlags = NIF_INFO;//NIF_INFO//

	m_nid.hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_MAINFRAME));

	wcscpy_s(m_nid.szInfo, szMsg ? szMsg : L"");
	wcscpy_s(m_nid.szInfoTitle, szTitle ? szTitle : L"");

	return Shell_NotifyIcon(NIM_MODIFY, &m_nid);
}

LRESULT CMouseLikeTouchPadTraySvcDlg::OnSystemTray(WPARAM wParam, LPARAM lParam)
{
	if (wParam != IDR_MAINFRAME)
		return 1;

	switch (lParam)

	{
		case WM_LBUTTONUP: {// 左键起来时显示主窗口
			this->ShowWindow(SW_SHOWNORMAL);         // 
		}
						 break;

		case WM_RBUTTONUP: {    // 右键起来时弹出菜单
			LPPOINT lpoint = new tagPOINT;

			::GetCursorPos(lpoint);                    // 得到鼠标位置
			CMenu menu;
			menu.CreatePopupMenu();                    // 声明一个弹出式菜单

			menu.AppendMenu(MF_STRING, IDM_EXIT, L"关闭");//menu.AppendMenu(MF_STRING, WM_DESTROY, L"关闭");//

			SetForegroundWindow();//设置当失去焦点时菜单自动消失

			menu.TrackPopupMenu(TPM_LEFTALIGN | TPM_BOTTOMALIGN, lpoint->x, lpoint->y, this);

			HMENU hmenu = menu.Detach();
			menu.DestroyMenu();
			delete lpoint;
		}
						 break;
	}

	return 0;
}


//UINT WM_TASKBARCREATED = RegisterWindowMessage(L"TastBarCreated");//桌面Explorer崩溃后系统任务栏重建时发送的消息，不要更改//0
//if (message == WM_TASKBARCREATED) {
//	Shell_NotifyIcon(NIM_ADD, pNID);//重新加载托盘
//	//SendMessage(hWnd, message, wParam, lParam);
//}

