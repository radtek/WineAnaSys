#pragma once


// CShowInfoDlg �Ի���

class CShowInfoDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CShowInfoDlg)

public:
	CShowInfoDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CShowInfoDlg();

	// �Ի�������
	enum { IDD = IDD_DIALOG_SHOWINFO };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CString m_StaticInfoShow;
};