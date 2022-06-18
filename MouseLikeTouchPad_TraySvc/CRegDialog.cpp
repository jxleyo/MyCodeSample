// CRegDialog.cpp: 实现文件
//

#include "pch.h"
#include "MouseLikeTouchPad_TraySvc.h"
#include "CRegDialog.h"
#include "afxdialogex.h"


// CRegDialog 对话框

IMPLEMENT_DYNAMIC(CRegDialog, CDialogEx)

CRegDialog::CRegDialog(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_REG, pParent)
{

}

CRegDialog::~CRegDialog()
{
}

void CRegDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CRegDialog, CDialogEx)

END_MESSAGE_MAP()


// CRegDialog 消息处理程序


