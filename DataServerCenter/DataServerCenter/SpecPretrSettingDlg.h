#pragma once
#include "afxwin.h"


// CSpecPretrSettingDlg �Ի���

class CSpecPretrSettingDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CSpecPretrSettingDlg)

public:
	CSpecPretrSettingDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CSpecPretrSettingDlg();

// �Ի�������
	enum { IDD = IDD_DIALOG_PRETREATMENTSETTING };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	bool m_IsDenoiseChecked; //���븴ѡ���Ƿ�ѡ��
	bool m_IsCorrChecked;   //У����ѡ���Ƿ�ѡ��
	int m_RadioDenoise;  //����
	int m_RadioCorrelation;//У��	
	int m_SmoothPtsIndex; //ƽ����������
	int m_Wname1Index; //С����1����
	int m_Wname2Index;  //С����2����
	int m_DecLevelIndex; //�ֽ��������
	int m_TTppeIndex;  //��ֵѡ�񷽷�����
	int m_TScalIndex; //��ֵ����������
	int m_TSORHIndex; //��ֵʹ�÷�ʽ
	int m_DerivativeOrderIndex; //������������
	int m_AnaPts;    //�����ĵ�
public:
	int m_SmoothPt; //ƽ������
	CString m_wName;//С������
	int m_DecLevel; //�ֽ����
	int m_DerivativeOrder; //�����״�
	CString m_TTppe;
	CString m_TScal;
	CString m_TSORH;
public:
	afx_msg void OnBnClickedRadioSmooth();
	afx_msg void OnBnClickedRadioWaveletdenoise();
	afx_msg void OnBnClickedRadioMsc();
	afx_msg void OnBnClickedRadioDerivative();
	CComboBox m_ComboSmoothPts;
	CComboBox m_ComboWname1;
	CComboBox m_ComboWname2;
	CComboBox m_ComboDecLevel;
	CComboBox m_ComboDerivativeOrder;
	CComboBox m_ComboTTppe;
	CComboBox m_ComboTScal;
	CComboBox m_ComboSORH;
	afx_msg void OnBnClickedCheckDenoise();
	CButton m_CheckBtnDenoise;
//	afx_msg void OnBnClickedCheck2();
	afx_msg void OnBnClickedCheckSpeccorrelation();
	CButton m_CheckBtnSpecCorr;

	CButton m_RadioBtnSmooth;
	CButton m_RadioBtnDenoise;
	CButton m_RadioBtnMSC;
	CButton m_RadioBtnDerivative;
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	afx_msg void OnCbnSelchangeComboWname();
	afx_msg void OnNMThemeChangedComboWname(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedRadioAllpts();
	afx_msg void OnBnClickedRadioFeaturepts();
};
