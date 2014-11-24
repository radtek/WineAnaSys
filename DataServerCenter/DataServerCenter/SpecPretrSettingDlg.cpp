// SpecPretrSettingDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "DataServerCenter.h"
#include "SpecPretrSettingDlg.h"
#include "afxdialogex.h"


// CSpecPretrSettingDlg �Ի���

IMPLEMENT_DYNAMIC(CSpecPretrSettingDlg, CDialogEx)

CSpecPretrSettingDlg::CSpecPretrSettingDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CSpecPretrSettingDlg::IDD, pParent)
	, m_RadioDenoise(0)
	, m_RadioCorrelation(0)
	, m_AnaPts(0)
{
	m_wName = L"";//С������
	m_DecLevel = 0; //�ֽ����
	m_SmoothPt = 0; //ƽ������
	m_DerivativeOrder = 0; //�����״�
	m_IsDenoiseChecked = 0; //���븴ѡ���Ƿ�ѡ��
	m_IsCorrChecked = 0;   //У����ѡ���Ƿ�ѡ��
	//m_RadioDenoise = 0;  //����
	//m_RadioCorrelation = 0;//У��	
	m_SmoothPtsIndex = -1; //ƽ����������
	m_Wname1Index = -1; //С����1����
	m_Wname2Index = -1;  //С����2����
	m_DecLevelIndex = -1; //�ֽ��������
	m_DerivativeOrderIndex = -1; //������������
	m_TTppeIndex = -1;  //��ֵѡ�񷽷�����
	m_TScalIndex = -1;   //��ֵ����������
	m_TSORHIndex = -1;   //��ֵʹ�÷�ʽ
	m_TTppe = L"";
	m_TScal = L"";
	m_TSORH = L"";
}

CSpecPretrSettingDlg::~CSpecPretrSettingDlg()
{
}

void CSpecPretrSettingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Radio(pDX, IDC_RADIO_SMOOTH, m_RadioDenoise);
	DDX_Radio(pDX, IDC_RADIO_MSC, m_RadioCorrelation);
	DDX_Control(pDX, IDC_COMBO_SMOOTHPOINTS, m_ComboSmoothPts);
	DDX_Control(pDX, IDC_COMBO_WNAME, m_ComboWname1);
	DDX_Control(pDX, IDC_COMBO_WNAME2, m_ComboWname2);
	DDX_Control(pDX, IDC_COMBO_DECLEVEL, m_ComboDecLevel);
	DDX_Control(pDX, IDC_COMBO_DERIVATIVEORDER, m_ComboDerivativeOrder);
	DDX_Control(pDX, IDC_CHECK_DENOISE, m_CheckBtnDenoise);
	DDX_Control(pDX, IDC_CHECK_SPECCORRELATION, m_CheckBtnSpecCorr);
	DDX_Control(pDX, IDC_RADIO_SMOOTH, m_RadioBtnSmooth);	
	DDX_Control(pDX, IDC_RADIO_WAVELETDENOISE, m_RadioBtnDenoise);	
	DDX_Control(pDX, IDC_RADIO_MSC, m_RadioBtnMSC);
	DDX_Control(pDX, IDC_RADIO_DERIVATIVE, m_RadioBtnDerivative);
	DDX_Control(pDX, IDC_COMBO_TTYPE, m_ComboTTppe);
	DDX_Control(pDX, IDC_COMBO_TSCAL, m_ComboTScal);
	DDX_Control(pDX, IDC_COMBO_SORH, m_ComboSORH);
	DDX_Radio(pDX, IDC_RADIO_ALLPTS, m_AnaPts);
}


BEGIN_MESSAGE_MAP(CSpecPretrSettingDlg, CDialogEx)
	ON_BN_CLICKED(IDC_RADIO_SMOOTH, &CSpecPretrSettingDlg::OnBnClickedRadioSmooth)
	ON_BN_CLICKED(IDC_RADIO_WAVELETDENOISE, &CSpecPretrSettingDlg::OnBnClickedRadioWaveletdenoise)
	ON_BN_CLICKED(IDC_RADIO_MSC, &CSpecPretrSettingDlg::OnBnClickedRadioMsc)
	ON_BN_CLICKED(IDC_RADIO_DERIVATIVE, &CSpecPretrSettingDlg::OnBnClickedRadioDerivative)
	ON_BN_CLICKED(IDC_CHECK_DENOISE, &CSpecPretrSettingDlg::OnBnClickedCheckDenoise)
	//ON_BN_CLICKED(IDC_CHECK2, &CSpecPretrSettingDlg::OnBnClickedCheck2)
	ON_BN_CLICKED(IDC_CHECK_SPECCORRELATION, &CSpecPretrSettingDlg::OnBnClickedCheckSpeccorrelation)
	ON_BN_CLICKED(IDOK, &CSpecPretrSettingDlg::OnBnClickedOk)
	ON_CBN_SELCHANGE(IDC_COMBO_WNAME, &CSpecPretrSettingDlg::OnCbnSelchangeComboWname)
	ON_NOTIFY(NM_THEMECHANGED, IDC_COMBO_WNAME, &CSpecPretrSettingDlg::OnNMThemeChangedComboWname)
	ON_BN_CLICKED(IDC_RADIO_ALLPTS, &CSpecPretrSettingDlg::OnBnClickedRadioAllpts)
	ON_BN_CLICKED(IDC_RADIO_FEATUREPTS, &CSpecPretrSettingDlg::OnBnClickedRadioFeaturepts)
END_MESSAGE_MAP()


// CSpecPretrSettingDlg ��Ϣ�������


void CSpecPretrSettingDlg::OnBnClickedRadioSmooth()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_RadioDenoise = 0;
	m_RadioBtnSmooth.SetCheck(1);
	m_ComboSmoothPts.EnableWindow(1);
	m_ComboWname1.EnableWindow(0);
	m_ComboWname2.EnableWindow(0);
	m_ComboDecLevel.EnableWindow(0);
	m_ComboTTppe.EnableWindow(0);
	m_ComboTScal.EnableWindow(0);
	m_ComboSORH.EnableWindow(0);
}


void CSpecPretrSettingDlg::OnBnClickedRadioWaveletdenoise()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_RadioDenoise = 1;
	m_RadioBtnDenoise.SetCheck(1);
	m_ComboSmoothPts.EnableWindow(0);
	m_ComboWname1.EnableWindow(1);
	m_ComboWname2.EnableWindow(1);
	m_ComboDecLevel.EnableWindow(1);
	m_ComboTTppe.EnableWindow(1);
	m_ComboTScal.EnableWindow(1);
	m_ComboSORH.EnableWindow(1);
}


void CSpecPretrSettingDlg::OnBnClickedRadioMsc()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_RadioBtnMSC.SetCheck(1);
	m_RadioCorrelation = 0;
	m_ComboDerivativeOrder.EnableWindow(0);
}


void CSpecPretrSettingDlg::OnBnClickedRadioDerivative()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_RadioBtnDerivative.SetCheck(1);
	m_RadioCorrelation = 1;
	m_ComboDerivativeOrder.EnableWindow(1);
}


void CSpecPretrSettingDlg::OnBnClickedCheckDenoise()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	m_IsDenoiseChecked = m_CheckBtnDenoise.GetCheck();
	if(0 == m_CheckBtnDenoise.GetCheck())
	{
	    m_RadioBtnSmooth.EnableWindow(0);
	    m_RadioBtnDenoise.EnableWindow(0);
		m_ComboSmoothPts.EnableWindow(0);
		m_ComboWname1.EnableWindow(0);
		m_ComboWname2.EnableWindow(0);
		m_ComboDecLevel.EnableWindow(0);
		m_ComboTTppe.EnableWindow(0);
		m_ComboTScal.EnableWindow(0);
		m_ComboSORH.EnableWindow(0);
	}
	else
	{
	    m_RadioBtnSmooth.EnableWindow(1);
	    m_RadioBtnDenoise.EnableWindow(1);
		OnBnClickedRadioSmooth();
	}

}



void CSpecPretrSettingDlg::OnBnClickedCheckSpeccorrelation()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	m_IsCorrChecked = m_CheckBtnSpecCorr.GetCheck();

	if(0 == m_CheckBtnSpecCorr.GetCheck())
	{
		m_RadioBtnMSC.EnableWindow(0);
		m_RadioBtnDerivative.EnableWindow(0);
		m_ComboDerivativeOrder.EnableWindow(0);
	}
	else
	{
		m_RadioBtnMSC.EnableWindow(1);
		m_RadioBtnDerivative.EnableWindow(1);
		OnBnClickedRadioMsc();
	}
}


BOOL CSpecPretrSettingDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	for(int i =0;i<5;++i)
	{
		CString szptNum;
		szptNum.Format(L"%d",2*i+3);
		m_ComboSmoothPts.AddString(szptNum);
	}
	
	m_ComboWname1.AddString(L"haar");
	m_ComboWname1.AddString(L"db");
	m_ComboWname1.AddString(L"sym");
	
	//�ֽ����
	for(int i =0;i<9;++i)
	{
		CString szDeclevel;
		szDeclevel.Format(L"%d",i+1);
		m_ComboDecLevel.AddString(szDeclevel);
	}


	m_ComboDerivativeOrder.AddString(L"1");
	m_ComboDerivativeOrder.AddString(L"2");
	
	m_ComboTTppe.AddString(L"rigrsure");
	m_ComboTTppe.AddString(L"heursure");
	m_ComboTTppe.AddString(L"sqtwolog");
	m_ComboTTppe.AddString(L"minimaxi");

	m_ComboTScal.AddString(L"one");
	m_ComboTScal.AddString(L"sln");
	m_ComboTScal.AddString(L"mln");

	m_ComboSORH.AddString(L"Ӳ��ֵ");
	m_ComboSORH.AddString(L"����ֵ");

	//���ó�ʼ״̬
	m_CheckBtnDenoise.EnableWindow(1);
	m_CheckBtnSpecCorr.EnableWindow(1);

	m_CheckBtnDenoise.SetCheck(m_IsDenoiseChecked);
	m_CheckBtnSpecCorr.SetCheck(m_IsCorrChecked);

	if(m_IsDenoiseChecked)
	{
		
		m_RadioBtnSmooth.EnableWindow(1);
		m_RadioBtnDenoise.EnableWindow(1);
		if(0 == m_RadioDenoise) //�����ƽ��
		{
			m_ComboSmoothPts.EnableWindow(1);
			m_ComboWname1.EnableWindow(0);
			m_ComboWname2.EnableWindow(0);
			m_ComboDecLevel.EnableWindow(0);

			if(-1!=m_SmoothPtsIndex)
			{
				m_ComboSmoothPts.SetCurSel(m_SmoothPtsIndex);
			}
		}
		else  //�����С������
		{
			m_ComboSmoothPts.EnableWindow(0);
			m_ComboWname1.EnableWindow(1);		
			m_ComboDecLevel.EnableWindow(1);
			m_ComboTTppe.EnableWindow(1);
			m_ComboTScal.EnableWindow(1);
			if(-1!=m_Wname1Index)
			{
				m_ComboWname1.SetCurSel(m_Wname1Index);
			}

			if(m_Wname1Index == 1) //���ѡ��dbС��
			{
				m_ComboWname2.EnableWindow(1);
				for(int i =0;i<5;++i)
				{
					CString szNum;
					szNum.Format(L"%d",i+1);
					m_ComboWname2.AddString(szNum);
				}
			}
			else if(m_Wname1Index == 2) //���ѡ��symС��
			{
				m_ComboWname2.EnableWindow(1);
				for(int i =1;i<4;++i)
				{
					CString szNum;
					szNum.Format(L"%d",i+1);
					m_ComboWname2.AddString(szNum);
				}			
			}
			else
			{
				m_ComboWname2.EnableWindow(0);
			}
			if(-1!=m_Wname2Index)
			{
				m_ComboWname2.SetCurSel(m_Wname2Index);
			}

			if(-1!=m_DecLevelIndex)
			{
				m_ComboDecLevel.SetCurSel(m_DecLevelIndex);
			}
			if(-1!=m_TTppeIndex)
			{
				m_ComboTTppe.SetCurSel(m_TTppeIndex);
			}
			if(-1!=m_TScalIndex)
			{
				m_ComboTScal.SetCurSel(m_TScalIndex);
			}
			if(-1 != m_TSORHIndex )
			{
				m_ComboSORH.SetCurSel(m_TSORHIndex);
			}
		}
	}
	else
	{
		m_RadioBtnSmooth.EnableWindow(0);
		m_RadioBtnDenoise.EnableWindow(0);
		m_ComboSmoothPts.EnableWindow(0);
		m_ComboWname1.EnableWindow(0);
		m_ComboWname2.EnableWindow(0);
		m_ComboDecLevel.EnableWindow(0);
		m_ComboTTppe.EnableWindow(0);
		m_ComboTScal.EnableWindow(0);
		m_ComboSORH.EnableWindow(0);
	}

	if(m_IsCorrChecked)
	{
		m_RadioBtnMSC.EnableWindow(1);
		m_RadioBtnDerivative.EnableWindow(1);
		if(0 == m_RadioCorrelation)
		{
			m_ComboDerivativeOrder.EnableWindow(0);
		}
		else
		{
			m_ComboDerivativeOrder.EnableWindow(1);
		}
	}
	else
	{
		m_RadioBtnMSC.EnableWindow(0);
		m_RadioBtnDerivative.EnableWindow(0);
		m_ComboDerivativeOrder.EnableWindow(0);
	}
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}


void CSpecPretrSettingDlg::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if(m_IsDenoiseChecked)//���Ҫ���н���
	{
		if(0 == m_RadioDenoise)//����ƽ��
		{		
			if(m_ComboSmoothPts.GetCurSel() == CB_ERR)
			{
				MessageBox(L"��ѡ��ƽ���ĵ�����",L"Ԥ��������",MB_ICONERROR);
				return ;
			}
			m_SmoothPtsIndex = m_ComboSmoothPts.GetCurSel();

			CString szSmoothPt;
			m_ComboSmoothPts.GetLBText(m_ComboSmoothPts.GetCurSel(),szSmoothPt);
			m_SmoothPt = _wtoi(szSmoothPt); //ƽ������
		}
		else //����С��ȥ��
		{
			
			if(m_ComboWname1.GetCurSel() == CB_ERR)
			{
				MessageBox(L"��ѡ����ȷ��С����",L"Ԥ��������",MB_ICONERROR);
				return ;
			}

			if(m_ComboWname1.GetCurSel() !=0&&m_ComboWname2.GetCurSel() == CB_ERR)
			{
				MessageBox(L"��ѡ����ȷ��С����",L"Ԥ��������",MB_ICONERROR);
				return ;
			}




			m_Wname1Index = m_ComboWname1.GetCurSel();
			m_Wname2Index = m_ComboWname2.GetCurSel();	

			if(m_ComboDecLevel.GetCurSel() == CB_ERR)
			{
				MessageBox(L"��ѡ��С���ֽ�Ĳ�����",L"Ԥ��������",MB_ICONERROR);
				return ;
			}
			m_DecLevelIndex = m_ComboDecLevel.GetCurSel();

			CString wName1,wName2;
			m_ComboWname1.GetLBText(m_ComboWname1.GetCurSel(),wName1);

			if(m_Wname2Index!=CB_ERR)
				m_ComboWname2.GetLBText(m_ComboWname2.GetCurSel(),wName2);
			m_wName = wName1+wName2;

			CString szDecLevel;
			m_ComboDecLevel.GetLBText(m_ComboDecLevel.GetCurSel(),szDecLevel);
			m_DecLevel = _wtoi(szDecLevel); 


			if(m_ComboTTppe.GetCurSel() == CB_ERR)
			{
				MessageBox(L"��ѡ����ֵѡ�񷽷���",L"Ԥ��������",MB_ICONERROR);
				return ;
			}
			if(m_ComboTScal.GetCurSel() == CB_ERR)
			{
				MessageBox(L"��ѡ����ֵ��������",L"Ԥ��������",MB_ICONERROR);
				return ;
			}
			if(m_ComboSORH.GetCurSel() == CB_ERR)
			{
				MessageBox(L"��ѡ����ֵʹ�÷�ʽ��",L"Ԥ��������",MB_ICONERROR);
				return ;
			}

			m_TTppeIndex = m_ComboTTppe.GetCurSel();  
			m_TScalIndex = m_ComboTScal.GetCurSel(); 
			m_TSORHIndex = m_ComboSORH.GetCurSel();

			m_ComboTTppe.GetLBText(m_TTppeIndex,m_TTppe);
			m_ComboTScal.GetLBText(m_TScalIndex,m_TScal);

			CString szsorh;
			m_ComboSORH.GetLBText(m_TSORHIndex,szsorh);
			if(szsorh == L"Ӳ��ֵ")
				m_TSORH = L"h";
			else
				m_TSORH = L"s";
		}
	}
	if(m_IsCorrChecked)
	{
		if(1 == m_RadioCorrelation)
		{	
			if(m_ComboDerivativeOrder.GetCurSel() == CB_ERR)
			{
				MessageBox(L"��ѡ�����״Σ�",L"Ԥ��������",MB_ICONERROR);
				return ;
			}
			m_DerivativeOrderIndex = m_ComboDerivativeOrder.GetCurSel();

			CString szDerivativeOrder;
			m_ComboDerivativeOrder.GetLBText(m_ComboDerivativeOrder.GetCurSel(),szDerivativeOrder);
			m_DerivativeOrder = _wtoi(szDerivativeOrder); //ƽ������
		}
	}
	CDialogEx::OnOK();
}


void CSpecPretrSettingDlg::OnCbnSelchangeComboWname()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_Wname1Index = m_ComboWname1.GetCurSel();
	if(m_Wname1Index == 1) //���ѡ��dbС��
	{
		m_ComboWname2.EnableWindow(1);
		for (int i = m_ComboWname2.GetCount() - 1; i >= 0; i--)
		{
			m_ComboWname2.DeleteString(i);
		}
		for(int i =0;i<5;++i)
		{
			CString szNum;
			szNum.Format(L"%d",i+1);
			m_ComboWname2.AddString(szNum);
		}
	}
	else if(m_Wname1Index == 2) //���ѡ��symС��
	{
		m_ComboWname2.EnableWindow(1);
		for (int i = m_ComboWname2.GetCount()-1;i>=0;i--)
		{
			m_ComboWname2.DeleteString(i);
		}
		for(int i =1;i<4;++i)
		{
			CString szNum;
			szNum.Format(L"%d",i+1);
			m_ComboWname2.AddString(szNum);
		}
	}
	else
	{
		for (int i = m_ComboWname2.GetCount()-1;i>=0;i--)
		{
			m_ComboWname2.DeleteString(i);
		}
		m_ComboWname2.EnableWindow(0);
	}
}


void CSpecPretrSettingDlg::OnNMThemeChangedComboWname(NMHDR *pNMHDR, LRESULT *pResult)
{
	// �ù���Ҫ��ʹ�� Windows XP ����߰汾��
	// ���� _WIN32_WINNT ���� >= 0x0501��
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	*pResult = 0;



}


void CSpecPretrSettingDlg::OnBnClickedRadioAllpts()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_AnaPts = 0;
}


void CSpecPretrSettingDlg::OnBnClickedRadioFeaturepts()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_AnaPts = 1;
}
