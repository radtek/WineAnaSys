#pragma once


// CDetailCompareDlg �Ի���

class CDetailCompareDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CDetailCompareDlg)

public:
	CDetailCompareDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDetailCompareDlg();

// �Ի�������
	enum { IDD = IDD_DIALOG_DETAILCOMPARE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
};
