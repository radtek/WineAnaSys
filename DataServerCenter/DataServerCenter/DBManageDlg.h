#pragma once
#include "DBManageSheet.h"

// CDBManageDlg �Ի���

class CDBManageDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CDBManageDlg)

public:
	CDBManageDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDBManageDlg();

	CDBManageSheet m_CDBManageSheet;
	CDBManagePageTrue m_DBManagePageTrue;
    CDBManagePageFake m_DBManagePageFake;
	int SelectPage;

	CWnd * pwnd; //����ָ�򡰸����Ի���
// �Ի�������
	enum { IDD = IDD_DIALOG_DATABASEMANAGE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnClose();
	afx_msg void OnPaint();
};
