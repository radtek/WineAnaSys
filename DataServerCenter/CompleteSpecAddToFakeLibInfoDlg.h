#pragma once


// CCompleteSpecAddToFakeLibInfoDlg �Ի���

class CCompleteSpecAddToFakeLibInfoDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CCompleteSpecAddToFakeLibInfoDlg)

public:
	CCompleteSpecAddToFakeLibInfoDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CCompleteSpecAddToFakeLibInfoDlg();

// �Ի�������
	enum { IDD = IDD_DIALOG_COMPLETESPECADDTOFAKELIBINFO };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CString m_WineName;
	//CString m_TraceInfo;
	CString m_Comfrom;
	CString m_SpecIDTitle;
	CString m_Manager;
//	afx_msg void OnEnChangeEditWinenameFake();
	//afx_msg void OnEnChangeEditTraceinfoFake();
//	afx_msg void OnEnChangeEditComfromFake();
//	afx_msg void OnEnChangeEditSpecidtitleFake();
//	afx_msg void OnEnChangeEditManagerFake();
	CString m_WineNameInstruction;
	//CString m_TraceInfoInstruction;
	CString m_ComfromInstruction;
	CString m_SpecIDTitleInstruction;
	CString m_ManagerInstruction;
	afx_msg void OnEnSetfocusEditWinenameFake();
	afx_msg void OnEnSetfocusEditComfromFake();
	afx_msg void OnEnSetfocusEditSpecidtitleFake();
	afx_msg void OnEnSetfocusEditManagerFake();
	afx_msg void OnBnClickedOk();
};
