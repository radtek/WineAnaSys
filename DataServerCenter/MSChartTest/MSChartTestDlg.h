
// MSChartTestDlg.h : ͷ�ļ�
//

#pragma once
#include "mschart.h"

// CMSChartTestDlg �Ի���
class CMSChartTestDlg : public CDialogEx
{
// ����
public:
	CMSChartTestDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_MSCHARTTEST_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CMSChart m_Chart;
	void InitChart();
	//InitChart();
	void DrawChart();
};
