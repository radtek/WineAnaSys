#pragma once


// CSpecNoiseDlg 对话框

class CSpecNoiseDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CSpecNoiseDlg)

public:
	CSpecNoiseDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CSpecNoiseDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_NOISE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CString m_NoiseInfo;
};
