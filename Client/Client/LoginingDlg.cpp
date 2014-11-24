// LoginingDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Client.h"
#include "LoginingDlg.h"
#include "afxdialogex.h"


// CLoginingDlg 对话框

IMPLEMENT_DYNAMIC(CLoginingDlg, CDialogEx)

CLoginingDlg::CLoginingDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CLoginingDlg::IDD, pParent)
{

}

CLoginingDlg::~CLoginingDlg()
{
	int i = 0;
}

void CLoginingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SHOCKWAVEFLASH, m_shockWaveFlash);
}


BEGIN_MESSAGE_MAP(CLoginingDlg, CDialogEx)
	ON_MESSAGE(WM_PLAYLOGININGANIMATION, &CLoginingDlg::OnPlaylogininganimation)
END_MESSAGE_MAP()


// CLoginingDlg 消息处理程序


afx_msg LRESULT CLoginingDlg::OnPlaylogininganimation(WPARAM wParam, LPARAM lParam)
{
	m_shockWaveFlash.LoadMovie(0,L"C:\\Users\\wth515812076\\Documents\\Visual Studio 2010\\Projects\\Client\\Clientprivate.swf");
	m_shockWaveFlash.Play();
	return 0;
}
