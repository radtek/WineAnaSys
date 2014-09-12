// PCAResettingDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "DataServerCenter.h"
#include "PCAResettingDlg.h"
#include "afxdialogex.h"


// CPCAResettingDlg �Ի���

IMPLEMENT_DYNAMIC(CPCAResettingDlg, CDialogEx)

CPCAResettingDlg::CPCAResettingDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CPCAResettingDlg::IDD, pParent)
	, m_RadioPCAShow(0)
{

}

CPCAResettingDlg::~CPCAResettingDlg()
{
}

void CPCAResettingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Radio(pDX, IDC_RADIO_RESET_2PCA, m_RadioPCAShow);
	DDX_Control(pDX, IDC_COMBO_RESET_XAXISPC, m_ComboXaxisPC);
	DDX_Control(pDX, IDC_COMBO_RESET_YAXISPC, m_ComboYaxisPC);
	DDX_Control(pDX, IDC_COMBO_RESET_PCNUM, m_ComboPCNum);
}


BEGIN_MESSAGE_MAP(CPCAResettingDlg, CDialogEx)
	ON_BN_CLICKED(IDC_RADIO_RESET_2PCA, &CPCAResettingDlg::OnBnClickedRadioReset2pca)
	ON_BN_CLICKED(IDC_RADIO_RESET_MULTIPCA, &CPCAResettingDlg::OnBnClickedRadioResetMultipca)
	ON_BN_CLICKED(IDOK, &CPCAResettingDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CPCAResettingDlg ��Ϣ�������


void CPCAResettingDlg::OnBnClickedRadioReset2pca()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_RadioPCAShow = 0;
	m_ComboXaxisPC.EnableWindow(TRUE);
	m_ComboYaxisPC.EnableWindow(TRUE);
	m_ComboPCNum.EnableWindow(FALSE);
}


void CPCAResettingDlg::OnBnClickedRadioResetMultipca()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_RadioPCAShow = 1;
	m_ComboXaxisPC.EnableWindow(FALSE);
	m_ComboYaxisPC.EnableWindow(FALSE);
	m_ComboPCNum.EnableWindow(TRUE);
}


void CPCAResettingDlg::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if(m_RadioPCAShow == 0)
	{
		CString temp;
		if(m_ComboXaxisPC.GetCurSel() == CB_ERR)
		{
			MessageBox(L"��ѡ��x���PC��",L"PCA��������",MB_ICONWARNING);
			return;
		}
		m_ComboXaxisPC.GetLBText(m_ComboXaxisPC.GetCurSel(),temp);
		temp = temp.Right(temp.GetLength() - temp.Find(L"C")-1);
		xPC =_ttoi(temp);//m_ComboXaxisPC.GetCurSel() + 1;//Format(L"PC%d",);
		if(m_ComboYaxisPC.GetCurSel() == CB_ERR)
		{
			MessageBox(L"��ѡ��y���PC��",L"PCA��������",MB_ICONWARNING);
			return;
		}
		m_ComboYaxisPC.GetLBText(m_ComboYaxisPC.GetCurSel(),temp);
		temp = temp.Right(temp.GetLength() - temp.Find(L"C")-1);
		yPC = _ttoi(temp); //m_ComboYaxisPC.GetCurSel() + 1;//.Format(L"PC%d",);
		PCNum = 2;
	}
	else
	{
		CString temp;
		if(m_ComboPCNum.GetCurSel() == CB_ERR)
		{
			MessageBox(L"��ѡ��PC����Ŀ��",L"PCA��������",MB_ICONWARNING);
			return;
		}
		m_ComboPCNum.GetLBText(m_ComboPCNum.GetCurSel(),temp);
		temp = temp.Right(temp.GetLength() - temp.Find(L"C")-1);
		PCNum = _ttoi(temp);//m_ComboPCNum.GetCurSel() + 1;
	}
	CDialogEx::OnOK();
}


BOOL CPCAResettingDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	for(int i = 0;i<TotalPCNum;++i)
	{
		CString pc = L"PC";
		CString Num;
		Num.Format(L"%d",i+1);
		m_ComboXaxisPC.AddString(pc+Num);
		m_ComboYaxisPC.AddString(pc+Num);
		m_ComboPCNum.AddString(Num);
	}
	OnBnClickedRadioReset2pca();
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}
