// DBManageFakeDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "DataServerCenter.h"
#include "DBManageFakeDlg.h"
#include "afxdialogex.h"


// CDBManageFakeDlg �Ի���

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


// CDBManageFakeDlg ��Ϣ�������


BOOL CDBManageFakeDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	GetClientRect(&m_rect);
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}


void CDBManageFakeDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO: �ڴ˴������Ϣ����������
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
		//��ȡ�ؼ���� 
		if(pWnd[i])//�ж��Ƿ�Ϊ�գ���Ϊ�Ի��򴴽�ʱ����ô˺���������ʱ�ؼ���δ���� 
		{ 
			CRect rect; //��ȡ�ؼ��仯ǰ��С 
			pWnd[i]->GetWindowRect(&rect); 
			ScreenToClient(&rect);//���ؼ���Сת��Ϊ�ڶԻ����е��������� 
			//cx/m_rect.Width()Ϊ�Ի����ں���ı仯���� 
			rect.left=rect.left*cx/m_rect.Width();//////�����ؼ���С 
			rect.right=rect.right*cx/m_rect.Width(); 
			rect.top=rect.top*cy/m_rect.Height(); 
			rect.bottom=rect.bottom*cy/m_rect.Height(); 
			pWnd[i]->MoveWindow(rect);//���ÿؼ���С 
		}
	}

	GetClientRect(&m_rect);//���仯��ĶԻ����С��Ϊ�ɴ�С */
}
