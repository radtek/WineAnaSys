#pragma once


// CInputSpecDatasetInfoDlg �Ի���

class CInputSpecDatasetInfoDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CInputSpecDatasetInfoDlg)

public:
	CInputSpecDatasetInfoDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CInputSpecDatasetInfoDlg();

// �Ի�������
	enum { IDD = IDD_DIALOG_INPUTSPECDATASETINFO };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CString m_WineName;
	CString m_AlcoholContent;
	CString m_Flavour;
	CString m_Brand;
	afx_msg void OnBnClickedOk();
};
