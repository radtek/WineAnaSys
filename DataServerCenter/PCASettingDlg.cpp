// PCASettingDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "DataServerCenter.h"
#include "PCASettingDlg.h"
#include "afxdialogex.h"
#include "WineTypeIndex.h"

// CPCASettingDlg �Ի���

IMPLEMENT_DYNAMIC(CPCASettingDlg, CDialogEx)

CPCASettingDlg::CPCASettingDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CPCASettingDlg::IDD, pParent)
	, m_RadioPCnum(0)
	, m_RadioSpecToPCA(0)
{
	//xPC = L"";
	//yPC = L"";
}

CPCASettingDlg::~CPCASettingDlg()
{
}

void CPCASettingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Radio(pDX, IDC_RADIO_2PCA, m_RadioPCnum);
	DDX_Control(pDX, IDC_COMBO_XAXISPC, m_ComboXaxisPC);
	DDX_Control(pDX, IDC_COMBO_YAXISPC, m_ComboYaxisPC);
	DDX_Control(pDX, IDC_COMBO_PCNUM, m_ComboPCNum);
	DDX_Radio(pDX, IDC_RADIO_PLOTEDSPEC, m_RadioSpecToPCA);
	//DDX_Control(pDX, IDC_COMBO_REFSPECTYPE, m_ComboRefSpecType);
}


BEGIN_MESSAGE_MAP(CPCASettingDlg, CDialogEx)
	ON_BN_CLICKED(IDC_RADIO_2PCA, &CPCASettingDlg::OnBnClickedRadio2pca)
	ON_BN_CLICKED(IDC_RADIO_MULTIPCA, &CPCASettingDlg::OnBnClickedRadioMultipca)
	ON_BN_CLICKED(IDC_RADIO_PLOTEDSPEC, &CPCASettingDlg::OnBnClickedRadioPlotedspec)
	ON_BN_CLICKED(IDC_RADIO_ALLSPEC, &CPCASettingDlg::OnBnClickedRadioAllspec)
	ON_BN_CLICKED(IDOK, &CPCASettingDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CPCASettingDlg ��Ϣ�������


void CPCASettingDlg::OnBnClickedRadio2pca()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_RadioPCnum = 0;
	m_ComboXaxisPC.EnableWindow(TRUE);
	m_ComboYaxisPC.EnableWindow(TRUE);
	m_ComboPCNum.EnableWindow(FALSE);
}


void CPCASettingDlg::OnBnClickedRadioMultipca()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_RadioPCnum = 1;
	m_ComboXaxisPC.EnableWindow(FALSE);
	m_ComboYaxisPC.EnableWindow(FALSE);
	m_ComboPCNum.EnableWindow(TRUE);
}


BOOL CPCASettingDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	
	OnBnClickedRadio2pca();


	CWineTypeIndex  WineTypeIndex;
	if(!WineTypeIndex.Open())
	{
		MessageBox(L"",L"",MB_ICONERROR);
		return FALSE;
	}

	for(int i=0;i<m_InitPCNum;i++)
	{
		CString Temp;
		Temp.Format(L"PC%d",i+1);
		m_ComboXaxisPC.AddString(Temp);
	}
	

	for(int i=0;i<m_InitPCNum;i++)
	{
		CString Temp;
		Temp.Format(L"PC%d",i+1);
		m_ComboYaxisPC.AddString(Temp);
	}

	for(int i=0;i<m_InitPCNum;i++)
	{
		CString Temp;
		Temp.Format(L"%d",i+1);
		m_ComboPCNum.AddString(Temp);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}


void CPCASettingDlg::OnBnClickedRadioPlotedspec()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_RadioSpecToPCA = 0;
}


void CPCASettingDlg::OnBnClickedRadioAllspec()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_RadioSpecToPCA = 1;
}


void CPCASettingDlg::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if(m_InitPCNum == 0)
	{
		MessageBox(L"��ǰ��δ��������㣬����ʹ����������з�����",L"PCA��������",MB_ICONWARNING);
		return;
	}


	/*if(m_ComboRefSpecType.GetCurSel() == CB_ERR)
	{
		MessageBox(L"��ѡ��һ�ֲο�ͼ�����ͣ�",L"PCA��������",MB_ICONWARNING);
		return;
	}
	else
	{
		CString SpecTypeInfo;
		m_ComboRefSpecType.GetLBText(m_ComboRefSpecType.GetCurSel(),SpecTypeInfo);
		//��ȡ�׾���
		m_WineName = SpecTypeInfo.Left(SpecTypeInfo.Find(L"("));
		//��ȡ�׾�����
		m_BatchNo = SpecTypeInfo.Right(SpecTypeInfo.GetLength() - SpecTypeInfo.Find(L"(")-1);
		m_BatchNo = m_BatchNo.Left(m_BatchNo.GetLength()-1);
	}*/

	if(m_RadioPCnum == 0)
	{
		CString temp;
		if(m_ComboXaxisPC.GetCurSel() == CB_ERR)
		{
			MessageBox(L"��ѡ��x���PC��",L"PCA��������",MB_ICONWARNING);
			return;
		}
		m_ComboXaxisPC.GetLBText(m_ComboXaxisPC.GetCurSel(),temp);
		temp = temp.Right(temp.GetLength() - temp.Find(L"C")-1);
		xPC =_ttoi(temp);
		if(m_ComboYaxisPC.GetCurSel() == CB_ERR)
		{
			MessageBox(L"��ѡ��y���PC��",L"PCA��������",MB_ICONWARNING);
			return;
		}
		m_ComboYaxisPC.GetLBText(m_ComboYaxisPC.GetCurSel(),temp);
		temp = temp.Right(temp.GetLength() - temp.Find(L"C")-1);
		yPC = _ttoi(temp); 
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
