#pragma once
#include "shockwaveflash.h"


// CLoginingDlg �Ի���

class CLoginingDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CLoginingDlg)

public:
	CLoginingDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CLoginingDlg();

// �Ի�������
	enum { IDD = IDD_DIALOG_LOGINING };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CShockwaveflash m_shockWaveFlash;
protected:
	afx_msg LRESULT OnPlaylogininganimation(WPARAM wParam, LPARAM lParam);
};
