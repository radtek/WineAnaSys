#pragma once
#include "afxcmn.h"


// CPredictedRecordDlg �Ի���

class CPredictedRecordDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CPredictedRecordDlg)

public:
	CPredictedRecordDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CPredictedRecordDlg();

// �Ի�������
	enum { IDD = IDD_DIALOG_PREDICTEDRECORD };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_ListPredictedRecord;
	virtual BOOL OnInitDialog();
};
