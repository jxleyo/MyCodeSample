
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
	DDX_Control(pDX, IDC_STATIC_ICONSMALL, m_Static_IconSmall);
	DDX_Control(pDX, IDC_STATIC_SKETCHIMAGE, m_Static_Sketch);
}

BEGIN_MESSAGE_MAP(CMouseLikeTouchPadTraySvcDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_SYSCOMMAND()
	ON_COMMAND(IDM_EXIT, &CMouseLikeTouchPadTraySvcDlg::OnExit)
	ON_MESSAGE(WM_SYSTEMTRAY, OnSystemTray)
	ON_COMMAND(ID_MANUAL, &CMouseLikeTouchPadTraySvcDlg::OnManual)
	ON_COMMAND(ID_VIDEOTUTOR, &CMouseLikeTouchPadTraySvcDlg::OnVideotutor)
	ON_COMMAND(ID_ABOUT, &CMouseLikeTouchPadTraySvcDlg::OnAbout)
	ON_NOTIFY(NM_CLICK, IDC_SYSLINK_WEBSITE, &CMouseLikeTouchPadTraySvcDlg::OnNMClickSyslinkWebsite)
	ON_BN_CLICKED(IDC_REGISTRY, &CMouseLikeTouchPadTraySvcDlg::OnClickedRegistry)
	ON_NOTIFY(NM_CLICK, IDC_SYSLINK_EULA, &CMouseLikeTouchPadTraySvcDlg::OnNMClickSyslinkEula)
	ON_STN_CLICKED(IDC_STATIC_SKETCHIMAGE, &CMouseLikeTouchPadTraySvcDlg::OnStnClickedStaticSketchimage)
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
		ShowImage();
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
	else if (nID == SC_MINIMIZE) //SC_MINIMIZE
		{
		    PostMessage(WM_SYSCOMMAND, SC_CLOSE, 0);
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

			// 经常使用菜单，但是如果直接加载菜单资源，调用TrackPopupMenu时 就会出现菜单显示不全的问题， 基本上解决方法就是先GetSubMenu，再TrackPopupMenu，
				//两种方法，一种就是在菜单资源里把所要加载的资源放到一个Popup属性的菜单下面，
				//另外就是程序创建一个Popup菜单，把菜单资源附加到这个Popup菜单上，再从Popup菜单中GetSubMenu(0)  取得需要的菜单

			CMenu PopMenu;
			PopMenu.CreatePopupMenu();                    // 声明一个弹出式菜单

			//CMenu TrayMenu;
			//TrayMenu.LoadMenu(IDR_TRAYMENU);//从一个资源加载菜单

			//PopMenu.AppendMenu(MF_POPUP, (UINT_PTR)TrayMenu.m_hMenu, L"Pop");
			PopMenu.AppendMenu(MF_STRING, IDM_EXIT, L"关闭");//PopMenu.AppendMenu(MF_STRING, WM_DESTROY, L"关闭");//
			//PopMenu.EnableMenuItem(IDM_EXIT, MFS_GRAYED);//使某项菜单变灰

			PopMenu.AppendMenu(MF_STRING, 0, L"当前设置");
			PopMenu.AppendMenu(MF_SEPARATOR, 0, L"");

			PopMenu.AppendMenu(MF_STRING, ID_OPMETHOD, L"操作方式（仿鼠标式触摸板）");
			PopMenu.EnableMenuItem(ID_OPMETHOD, MFS_GRAYED);//使某项菜单变灰

			PopMenu.AppendMenu(MF_STRING, ID_WHEELSTATUS, L"滚轮启用（开）");
			PopMenu.EnableMenuItem(ID_WHEELSTATUS, MFS_GRAYED);//使某项菜单变灰

			PopMenu.AppendMenu(MF_STRING, ID_WHEELMODE, L"滚轮方式（模拟鼠标）");
			PopMenu.EnableMenuItem(ID_WHEELMODE, MFS_GRAYED);//使某项菜单变灰

			PopMenu.AppendMenu(MF_STRING, ID_SENSITIVITY, L"灵敏度（高）");
			PopMenu.EnableMenuItem(ID_SENSITIVITY, MFS_GRAYED);//使某项菜单变灰

			PopMenu.AppendMenu(MF_SEPARATOR, 0, L"");//分割线

			PopMenu.AppendMenu(MF_STRING, ID_MANUAL, L"说明手册");
			PopMenu.AppendMenu(MF_STRING, ID_VIDEOTUTOR, L"视频教程");
			PopMenu.AppendMenu(MF_STRING, ID_ABOUT, L"关于。。。");

			//// 因为右键菜单是弹出式菜单，不包含主菜单栏，所以取子菜单   
			//CMenu* TrackMenu = TrayMenu.GetSubMenu(0);

			SetForegroundWindow();//设置当失去焦点时菜单自动消失
			
			// 弹出右键菜单，菜单左侧与point.x坐标值对齐 ,底部与y坐标值对齐  
			//TrackMenu->TrackPopupMenu(TPM_LEFTALIGN | TPM_BOTTOMALIGN, lpoint->x, lpoint->y, this);
			PopMenu.TrackPopupMenu(TPM_LEFTALIGN | TPM_BOTTOMALIGN, lpoint->x, lpoint->y, this);

			//销毁菜单
			HMENU hmenu = PopMenu.Detach();
			PopMenu.DestroyMenu();

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



void CMouseLikeTouchPadTraySvcDlg::OnManual()
{
	ShellExecute(NULL, L"open", L"https://github.com/jxleyo", NULL, NULL, SW_SHOWNORMAL);
}


void CMouseLikeTouchPadTraySvcDlg::OnVideotutor()
{
	ShellExecute(NULL, L"open", L"https://space.bilibili.com/409976933", NULL, NULL, SW_SHOWNORMAL);
	//ShellExecute(NULL, L"open", L"https://www.youtube.com/channel/UC3hQyN-2ZL_q7pCKoASAblQ", NULL, NULL, SW_SHOWNORMAL);
}


void CMouseLikeTouchPadTraySvcDlg::OnAbout()
{
	this->ShowWindow(SW_SHOWNORMAL);         // 
}


void CMouseLikeTouchPadTraySvcDlg::OnNMClickSyslinkWebsite(NMHDR* pNMHDR, LRESULT* pResult)
{
	ShellExecute(NULL, L"open", L"https://github.com/jxleyo", NULL, NULL, SW_SHOWNORMAL);
	*pResult = 0;
}


void CMouseLikeTouchPadTraySvcDlg::OnClickedRegistry()
{
	ShellExecute(NULL, L"open", L"https://github.com/jxleyo", NULL, NULL, SW_SHOWNORMAL);
}


void CMouseLikeTouchPadTraySvcDlg::OnNMClickSyslinkEula(NMHDR* pNMHDR, LRESULT* pResult)
{
	ShellExecute(NULL, L"open", L"https://github.com/jxleyo", NULL, NULL, SW_SHOWNORMAL);
	*pResult = 0;
}


void CMouseLikeTouchPadTraySvcDlg::OnStnClickedStaticSketchimage()
{
	//原来，需要修改static控件的属性notify 为TRUE。
	ShellExecute(NULL, L"open", L"https://space.bilibili.com/409976933", NULL, NULL, SW_SHOWNORMAL);
}



void CMouseLikeTouchPadTraySvcDlg::ShowImage()
{
	//UpdateWindow();

	CImage Image;
	if (!LoadImageFromRes(&Image, IDB_PNG_ICONSMALL, _T("PNG"))) return;
	if (Image.IsNull())
	{
		MessageBox(_T("IMAGE Res Load Err"));
		//return -1;
	}

	//CImage转为CBitmap：
	HBITMAP hbmp = (HBITMAP)(&Image)->operator HBITMAP();
	m_Static_IconSmall.ModifyStyle(0xf, SS_BITMAP | SS_CENTERIMAGE);////设置静态控件窗口风格为位图居中显示
	m_Static_IconSmall.SetBitmap(hbmp);////将图片设置到Picture控件上  

	CRect rect;//定义矩形类
	int height = Image.GetHeight();
	int width = Image.GetWidth();

	m_Static_IconSmall.GetClientRect(&rect); //获得pictrue控件所在的矩形区域
	CDC* pDc = m_Static_IconSmall.GetDC();//获得pictrue控件的Dc
	SetStretchBltMode(pDc->m_hDC, STRETCH_HALFTONE);
	Image.StretchBlt(pDc->m_hDC, rect, SRCCOPY);
	ReleaseDC(pDc);//释放picture控件的Dc


	//CBitmap bitmap;
	//BITMAP bmpInfo;
	//bitmap.GetBitmap(&bmpInfo);
	//CDC dcMemory;
	//CDC* pDC = m_Static_IconSmall.GetDC();
	//dcMemory.CreateCompatibleDC(pDC);
	//CBitmap* pOldBitmap = dcMemory.SelectObject(&bitmap);
	//CRect rect;
	//m_Static_IconSmall.GetClientRect(&rect);
	//int nX = rect.left + (rect.Width() - bmpInfo.bmWidth) / 2;
	//int nY = rect.top + (rect.Height() - bmpInfo.bmHeight) / 2;
	////pDC->BitBlt(0, 0, bmpInfo.bmWidth, bmpInfo.bmHeight, &dcMemory, 0, 0, SRCCOPY);  
	//pDC->SetStretchBltMode(COLORONCOLOR);
	//pDC->StretchBlt(0, 0, rect.Width(), rect.Height(), &dcMemory, 0, 0, bmpInfo.bmWidth, bmpInfo.bmHeight, SRCCOPY);
	//dcMemory.SelectObject(pOldBitmap);
	//ReleaseDC(pDC);


	if (!LoadImageFromRes(&Image, IDB_JPG_SKETCHCN, _T("JPG"))) return;
	if (Image.IsNull())
	{
		MessageBox(_T("IMAGE Res Load Err"));
		//return -1;
	}

	hbmp = (HBITMAP)(&Image)->operator HBITMAP();
	m_Static_Sketch.SetBitmap(hbmp);


	//CBitmap* bm;
	//bm->DeleteObject();
	//bm->Attach(hbmp);

}


BOOL CMouseLikeTouchPadTraySvcDlg::LoadImageFromRes(CImage* pImage, UINT nResID, LPCTSTR lpTyp)
{
	if (pImage == NULL)
		return false;
	pImage->Destroy();
	// 查找资源
	HRSRC hRsrc = ::FindResource(AfxGetResourceHandle(), MAKEINTRESOURCE(nResID), lpTyp);
	if (hRsrc == NULL)
		return false;
	// 加载资源
	HGLOBAL hImgData = ::LoadResource(AfxGetResourceHandle(), hRsrc);
	if (hImgData == NULL)
	{
		::FreeResource(hImgData);
		return false;
	}
	// 锁定内存中的指定资源
	LPVOID lpVoid = ::LockResource(hImgData);
	LPSTREAM pStream = NULL;
	DWORD dwSize = ::SizeofResource(AfxGetResourceHandle(), hRsrc);
	HGLOBAL hNew = ::GlobalAlloc(GHND, dwSize);
	LPBYTE lpByte = (LPBYTE)::GlobalLock(hNew);
	::memcpy(lpByte, lpVoid, dwSize);
	// 解除内存中的指定资源
	::GlobalUnlock(hNew);
	// 从指定内存创建流对象
	HRESULT ht = ::CreateStreamOnHGlobal(hNew, TRUE, &pStream);
	if (ht != S_OK)
	{
		GlobalFree(hNew);
	}
	else
	{
		// 加载图片
		pImage->Load(pStream);
		GlobalFree(hNew);
	}
	// 释放资源
	::FreeResource(hImgData);
	return true;
}

