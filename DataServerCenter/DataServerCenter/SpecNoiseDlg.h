#pragma once


// CSpecNoiseDlg �Ի���

class CSpecNoiseDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CSpecNoiseDlg)

public:
	CSpecNoiseDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CSpecNoiseDlg();

// �Ի�������
	enum { IDD = IDD_DIALOG_NOISE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CString m_NoiseInfo;
};
