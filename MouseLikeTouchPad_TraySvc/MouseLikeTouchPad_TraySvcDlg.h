
// MouseLikeTouchPad_TraySvcDlg.h: 头文件
//

#pragma once


// CMouseLikeTouchPadTraySvcDlg 对话框
class CMouseLikeTouchPadTraySvcDlg : public CDialogEx
{
// 构造
public:
	CMouseLikeTouchPadTraySvcDlg(CWnd* pParent = nullptr);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MOUSELIKETOUCHPAD_TRAYSVC_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	CMenu m_Menu;
	NOTIFYICONDATA m_nid;

	// TraySvc标签页
	CTabCtrl m_TablCtrl;

	CDialogEx m_TablDialog_About;
	CDialogEx m_TablDialog_Setting;
	CDialogEx m_TablDialog_Reg;

	INT mCurTab;
	

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();

	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	DECLARE_MESSAGE_MAP()

public:

	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnClose(); //响应关闭事件！

	BOOL DestroyWindow();
	void OnSize(UINT nType, int cx, int cy);

	afx_msg void OnExit();
	afx_msg void AddTrayIcon();
	BOOL ShowBalloonTip(LPCWSTR szMsg, LPCWSTR szTitle, UINT uTimeOut, DWORD dwInfoFlags);
	LRESULT OnSystemTray(WPARAM wParam, LPARAM lParam);


	afx_msg void OnSelchangeTab(NMHDR* pNMHDR, LRESULT* pResult);
};
