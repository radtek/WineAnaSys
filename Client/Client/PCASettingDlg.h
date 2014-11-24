#pragma once
#include "afxwin.h"


// CPCASettingDlg 对话框

class CPCASettingDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CPCASettingDlg)

public:
	CPCASettingDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CPCASettingDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_PCAANASETTING };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	int m_RadioPCnum;   //选择显示的PC数

	CComboBox m_ComboXaxisPC; //x轴显示的PC
	int xPC;

	CComboBox m_ComboYaxisPC; //y轴显示的PC
	int yPC;

	CComboBox m_ComboPCNum; //显示的PC数
	int PCNum;

	int m_RadioSpecToPCA;

	int m_InitPCNum; //对话框初始化时可选的最大显示pc个数
	CComboBox m_ComboRefSpecType;   //参考图谱类型
	CString m_WineName;
	CString m_BatchNo;

public:
	afx_msg void OnBnClickedRadio2pca();
	afx_msg void OnBnClickedRadioMultipca();
	virtual BOOL OnInitDialog();	
	afx_msg void OnBnClickedRadioPlotedspec();
	afx_msg void OnBnClickedRadioAllspec();	
	afx_msg void OnBnClickedOk();
};

