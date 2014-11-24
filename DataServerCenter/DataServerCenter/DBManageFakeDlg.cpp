// DBManageFakeDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "DataServerCenter.h"
#include "DBManageFakeDlg.h"
#include "afxdialogex.h"


// CDBManageFakeDlg 对话框

IMPLEMENT_DYNAMIC(CDBManageFakeDlg, CDialogEx)

CDBManageFakeDlg::CDBManageFakeDlg(CWnd* pParent /*=NULL*/)
	: CPropertyPage(CDBManageFakeDlg::IDD, pParent)
{

}

CDBManageFakeDlg::~CDBManageFakeDlg()
{
}

void CDBManageFakeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDBManageFakeDlg, CDialogEx)
	ON_WM_SIZE()
END_MESSAGE_MAP()


// CDBManageFakeDlg 消息处理程序


BOOL CDBManageFakeDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	GetClientRect(&m_rect);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CDBManageFakeDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
	/*CWnd *pWnd[25]; 
	pWnd[0] = GetDlgItem(IDC_BUTTON_OPENDATAPACKAGE_FAKE);
	pWnd[1] = GetDlgItem(IDC_BUTTON_CLEARSPECTOADD_FAKE);
	pWnd[2] = GetDlgItem(IDC_BUTTON_ADDTOLIB_FAKE);
	pWnd[3] = GetDlgItem(IDC_LIST_TOADD_FAKE);
	pWnd[4] = GetDlgItem(IDC_STATIC_PICTUREOFSPECTOADD_FAKE);
	pWnd[5] = GetDlgItem(IDC_BUTTON_LAST_FAKE);
	pWnd[6] = GetDlgItem(IDC_BUTTON_NEXT_FAKE);
	pWnd[7] = GetDlgItem(IDC_STATIC1FAKE);
	pWnd[8] = GetDlgItem(IDC_COMBO_FIELD1_FAKE);
	pWnd[9] = GetDlgItem(IDC_EDIT_KEY1_FAKE);
	pWnd[10] = GetDlgItem(IDC_COMBO_ANDOR1_FAKE);
	pWnd[11] = GetDlgItem(IDC_COMBO_FIELD2_FAKE);
	pWnd[12] = GetDlgItem(IDC_EDIT_KEY2_FAKE);
	pWnd[13] = GetDlgItem(IDC_COMBO_ANDOR2_FAKE);
	pWnd[14] = GetDlgItem(IDC_COMBO_FIELD3_FAKE);
	pWnd[15] = GetDlgItem(IDC_EDIT_KEY3_FAKE);
	pWnd[16] = GetDlgItem(IDC_BUTTON_SEARCHWITHKEYWORDS_FAKE);
	pWnd[17] = GetDlgItem(IDC_BUTTON_SHOWALL_FAKE);
	pWnd[18] = GetDlgItem(IDC_BUTTON_DELETE_FAKE);
	pWnd[19] = GetDlgItem(IDC_BUTTON_UPDATEMODEL_FAKE);
	pWnd[20] = GetDlgItem(IDC_LIST_SPECLIB_FAKE);
	pWnd[21] = GetDlgItem(IDC_STATIC_PICTUREINLIB_FAKE);
	pWnd[22] = GetDlgItem(IDC_BUTTON_LASTLIB_FAKE);
	pWnd[23] = GetDlgItem(IDC_BUTTON_NEXTLIB_FAKE);
	pWnd[24] = GetDlgItem(IDC_STATIC2FAKE);
	
	//pWnd[7] = GetDlgItem(IDC_BUTTON_BACKFROMOFFLINE);
	//pWnd[7]=GetDlgItem(IDC_STATIC_LOGIN_TITLE);*/

	for(int i=0;i<25;i++)
	{
		//获取控件句柄 
		if(pWnd[i])//判断是否为空，因为对话框创建时会调用此函数，而当时控件还未创建 
		{ 
			CRect rect; //获取控件变化前大小 
			pWnd[i]->GetWindowRect(&rect); 
			ScreenToClient(&rect);//将控件大小转换为在对话框中的区域坐标 
			//cx/m_rect.Width()为对话框在横向的变化比例 
			rect.left=rect.left*cx/m_rect.Width();//////调整控件大小 
			rect.right=rect.right*cx/m_rect.Width(); 
			rect.top=rect.top*cy/m_rect.Height(); 
			rect.bottom=rect.bottom*cy/m_rect.Height(); 
			pWnd[i]->MoveWindow(rect);//设置控件大小 
		}
	}

	GetClientRect(&m_rect);//将变化后的对话框大小设为旧大小 */
}
