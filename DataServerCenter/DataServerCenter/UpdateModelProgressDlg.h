#pragma once


// CUpdateModelProgressDlg �Ի���

class CUpdateModelProgressDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CUpdateModelProgressDlg)

public:
	CUpdateModelProgressDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CUpdateModelProgressDlg();

// �Ի�������
	enum { IDD = IDD_DIALOG_UPDATEMODELPROGRESS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
};
