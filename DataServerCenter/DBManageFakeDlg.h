#pragma once


// CDBManageFakeDlg �Ի���

class CDBManageFakeDlg : public CPropertySheet
{
	DECLARE_DYNAMIC(CDBManageFakeDlg)

public:
	CDBManageFakeDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDBManageFakeDlg();

// �Ի�������
	enum { IDD = IDD_DIALOG_DATABASEMANAGE_FAKE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CRect m_rect;
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
};
