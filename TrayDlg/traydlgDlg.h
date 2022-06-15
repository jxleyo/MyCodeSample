
// traydlgDlg.h: 头文件
//

#pragma once


// CtraydlgDlg 对话框
class CtraydlgDlg : public CDialogEx
{
// 构造
public:
	CtraydlgDlg(CWnd* pParent = nullptr);	// 标准构造函数

	BOOL CtraydlgDlg::DestroyWindow();
	void CtraydlgDlg::OnSize(UINT nType, int cx, int cy);

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_TRAYDLG_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	CMenu m_Menu;
	NOTIFYICONDATA m_nid;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnTest1();
	afx_msg void OnTest2();
	LRESULT OnSystemTray(WPARAM wParam, LPARAM lParam);

	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnClose(); //响应关闭事件！
};
