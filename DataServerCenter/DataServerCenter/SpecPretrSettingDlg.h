#pragma once
#include "afxwin.h"


// CSpecPretrSettingDlg 对话框

class CSpecPretrSettingDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CSpecPretrSettingDlg)

public:
	CSpecPretrSettingDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CSpecPretrSettingDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_PRETREATMENTSETTING };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	bool m_IsDenoiseChecked; //降噪复选框是否选中
	bool m_IsCorrChecked;   //校正复选框是否选中
	int m_RadioDenoise;  //降噪
	int m_RadioCorrelation;//校正	
	int m_SmoothPtsIndex; //平滑点数索引
	int m_Wname1Index; //小波名1索引
	int m_Wname2Index;  //小波名2索引
	int m_DecLevelIndex; //分解层数索引
	int m_TTppeIndex;  //阈值选择方法索引
	int m_TScalIndex; //阈值处理方法索引
	int m_TSORHIndex; //阈值使用方式
	int m_DerivativeOrderIndex; //导数阶数索引
	int m_AnaPts;    //分析的点
public:
	int m_SmoothPt; //平滑点数
	CString m_wName;//小波名称
	int m_DecLevel; //分解层数
	int m_DerivativeOrder; //导数阶次
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
