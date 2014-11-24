#if !defined(AFX_MYHEADCTRL_H__AB8227D1_FD91_4CDA_9D85_2A40D73736A0__INCLUDED_)
#define AFX_MYHEADCTRL_H__AB8227D1_FD91_4CDA_9D85_2A40D73736A0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Afxtempl.h"

typedef struct _HEADCELL
{
	int nCol;
	CString strHead;
	COLORREF rgbBack;
	COLORREF rgbText;
	char cFormat;//0��ʾ����룬1��ʾ�м���룬2��ʾ�Ҷ���
}HEADCELL;

typedef struct _HEADFONT
{
	_HEADFONT(void){font.CreateFont(12,0,0,0,FW_NORMAL,0,0,0,DEFAULT_CHARSET,OUT_DEFAULT_PRECIS,\
		CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,DEFAULT_PITCH|FF_SWISS,"����");};
	~_HEADFONT(){font.DeleteObject();};
	int nCol;
	CFont font;
}HEADFONT;
/////////////////////////////////////////////////////////////////////////////
// CMyHeadCtrl window

class CMyHeadCtrl : public CHeaderCtrl
{
// Construction
public:
	CMyHeadCtrl();

// Attributes
public:
	CArray<HEADCELL,HEADCELL&> m_HeadCell;	//ͷ����Ԫ����Ϣ
	int m_nGradient;						// �����屳��������ϵ��
	float m_fHeight;						//��ͷ�߶ȣ����Ǳ���,
	CFont *m_pFont;							//����
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMyHeadCtrl)
	//}}AFX_VIRTUAL

// Implementation
public:
	void SetColFont(int col,int nSize,BOOL bBold,BOOL bItalic,BOOL bUnderLine,LPCTSTR pszName);
	HEADCELL* FindHeadCell(int col);
	virtual ~CMyHeadCtrl();
	
	// Generated message map functions
	LRESULT OnLayout( WPARAM wParam, LPARAM lParam );
protected:
	//{{AFX_MSG(CMyHeadCtrl)
	afx_msg void OnPaint();
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
private:
	int m_nDown , m_nUp;
	BYTE m_offset;
	HEADFONT* m_pColFont;
	int m_nFont,m_nFontSize;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MYHEADCTRL_H__AB8227D1_FD91_4CDA_9D85_2A40D73736A0__INCLUDED_)
