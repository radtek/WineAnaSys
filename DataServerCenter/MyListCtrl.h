#if !defined(AFX_MYLISTCTRL1_H__6F14775C_CA2F_464E_84E6_D400E924309E__INCLUDED_)
#define AFX_MYLISTCTRL1_H__6F14775C_CA2F_464E_84E6_D400E924309E__INCLUDED_

#include "MyHeadCtrl.h"	// Added by ClassView
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MyListCtrl1.h : header file
//
#define CELL_MAXLEN 256

#define IDC_EDT_CTRL 5101
#define IDC_CMB_CTRL 5102
#define IDC_CHECK_CTRL 5103
#define IDC_BUTTON_CTRL 5104


#define CTRL_TYPE_EDIT	111
#define CTRL_TYPE_COMBO	112
#define CTRL_TYPE_CHECK	113

#define DATA_TYPE_CHAR		100 //(1�ֽ�)
#define DATA_TYPE_INT		101	//(2�ֽ�) WORD == unsigned short
#define DATA_TYPE_FLOAT		102 //(4�ֽ�)
#define DATA_TYPE_STRING	103
#define DATA_TYPE_UCHAR		104 //(1�ֽ�)
#define DATA_TYPE_LONG		105	//(4�ֽ�) DWORD == unsigned long
#define DATA_TYPE_DOUBLE	106 //(8�ֽ�)


typedef void(*COMBOKILLFOCUS) (CDialog* pDlg, int nCol);
typedef void(*ITEMDBLCLK) (int nItem,int nSubItem,void* pVoid);
/////////////////////////////////////////////////////////////////////////////
// CMyListCtrl window
typedef struct _CELLVALUE
{
	DWORD dwRow;//
	DWORD dwCol;//
	UINT nIDResource;//picture
	unsigned char cCtrlType;//111-IDC_EDIT,112-IDC_COMBO
	unsigned char cDataType;//100-char,101-int,102-float,103-char*
	WORD nStrMaxLen;
	char strPrevItem[CELL_MAXLEN];//���ITEMǰһ����ֵ
	unsigned char bItemChange;
	void* pValue;	//�洢���Ƕ�Ӧֵ��ָ��!
	unsigned char nComboStr;
	CString* pComboStr;
}CELLVALUE;

typedef struct _stColor
{
	int nRow;
	int nCol;
	COLORREF rgb;
}stColor;

typedef struct _stFont
{
	_stFont(void){font.CreateFont(12,0,0,0,FW_NORMAL,0,0,0,DEFAULT_CHARSET,OUT_DEFAULT_PRECIS,\
		CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,DEFAULT_PITCH|FF_SWISS,"����");};
	~_stFont(){font.DeleteObject();};
	int nRow;
	int nCol;
	CFont font;
}stFont;

typedef struct _CELLSTYLE
{
	DWORD dwRow;//
	DWORD dwCol;//
	COLORREF rgbBk;
	COLORREF rgbText;
	char cFormat;//0-����룬1-�м���룬2-�Ҷ���
	//����
	short nFontSize;
	bool bFontBold;
	bool bFontItalic;
	bool bFontUnderLine;
	char strFontName[32];
}CELLSTYLE;
/*********************************************
* CExArray��չ��:����������CArray
* �ô�������CELLVALUE�ṹָ��
* ���ܣ���ӡ�ɾ������ȡ�ṹ��CELLVALUE��ָ��
**********************************************/
template<class T>
class CExArray
{
public:
	CExArray(int nSize=10){m_dwSize=10;m_dwCurCount=0;m_pVal=NULL;m_pVal=new T[nSize];};
	virtual ~CExArray(){if(m_pVal!=NULL){delete[] m_pVal;m_pVal=NULL;}};
	//////////////////////
public:
	void Add(T& cell);
	BOOL GetAt(DWORD dwRow,DWORD dwCol,T& cell);
	BOOL GetAt(DWORD dwIndex,T& cell);
	T* GetAt(DWORD dwRow,DWORD dwCol);
	T* GetAt(DWORD dwIndex);
	void DelAt(DWORD dwRow,DWORD dwCOl);
	void DelAll(void);
	int  GetCount(void);
private:
	T *m_pVal;
	DWORD m_dwSize;
	DWORD m_dwCurCount;
};

/***************************************************************************************
* �����ƣ�	CMyListCtrl ��public CListCtrl
* ��˵����	CMyListCtrl���Ǽ̳���CListCtrl�࣬��CListCtrl������ػ�������¹���
* ���ܣ�	���������б���иߴ�С  
*			���������б�ͷ���иߴ�С
*			�������õ�Ԫ����ı���ɫ��������ɫ������
*			����������ͷÿһ�е��ı���ɫ��������ɫ������
*			����ͨ�����˫��ĳ����Ԫ����ʾ�ؼ���CEdit��CComboBox���Ըõ�Ԫ���������޸�
*			�����ٵ�Ԫ������ʾλͼ��Դ
***************************************************************************************/
class CMyListCtrl : public CListCtrl
{
// Construction
public:
	CMyListCtrl();

// Attributes
public:
// Operations
public:
	//0-char,1-int,2-float,3-char* ; 0-IDC_EDIT,1-IDC_CMB,ע�⣺pData,pComboStrֻ�Ǵ洢����ָ��!
	void Attach(DWORD dwItem,DWORD dwSubItem,void* pData,unsigned char cDataType=0,unsigned char cCtrlType=0,\
		 int nComboStr=0,CString* pComboStr=NULL);

	//����ĳһ����Ԫ��������д���ַ���
	void SetStrMaxLen(int nRow,int nCol,int nMaxLen=256);

	//��ȡItemValue Change��Num
	int GetItemChangeNum(void);

	//�ڵ���GetNextItemChange֮ǰ�ȵ����������һ�μȿ�����
	bool GetFirstItemChange(int& row,int& col);

	//��ȡItemValue Change���кź��к�
	bool GetNextItemChange(int& row,int& col);
	/////////////////////////////////////////////////////////////
	// ���ñ�ͷ�߶�
	void SetHeadHeight(float Height);

	// Gradient - ����ϵ�������屳����,���ý�����Ϊ0
	void SetHeadBKColor(COLORREF rgb,int Gradient,int col=-1);
	/////////////////////////////////////////////////////////////

	//��������ɫ
	void SetRowColor(int row,COLORREF color);

	//ȡ�����õ�����ɫ
	void UnSetRowColor(int row);

	//��������ɫ
	void SetColColor(int col,COLORREF color); 

	//���õ�Ԫ����ɫ
	void SetItemColor(int row,int col,COLORREF color);

	//�������ı���ɫ
	void SetRowTextColor(int row,COLORREF color);

	//ȡ�����õ����ı���ɫ
	void UnSetRowTextColor(int row);

	//�������ı���ɫ
	void SetColTextColor(int col,COLORREF color);

	//���õ�Ԫ���ı���ɫ
	void SetItemTextColor(int row,int col,COLORREF color);

	//�����и�
	void SetRowHeigt(int nHeight);

	//����ͷ������
	void SetHeadFont(int nSize,BOOL bBold,BOOL bItalic,BOOL bUnderLine,LPCTSTR pszName="����");

	//����ͷ��������ɫ
	void SetHeadTextColor(COLORREF color,int col=-1);

	//////////////////////////////////////////////////////////////
	//���������б������
	void SetFont(int nSize,BOOL bBold,BOOL bItalic,BOOL bUnderLine,LPCTSTR pszName="����");

	//����һ�е�����
	void SetColFont(int col,int nSize,BOOL bBold,BOOL bItalic,BOOL bUnderLine,LPCTSTR pszName="����");

	//����ĳһ����Ԫ���е�����
	void SetItemFont(int row,int col,int nSize,BOOL bBold,BOOL bItalic,BOOL bUnderLine,LPCTSTR pszName="����");

	//������ͷĳһ�е�����
	void SetHeadColFont(int col,int nFontSize,BOOL Bold,BOOL Italic,BOOL UnderLine,LPCTSTR pszFontName="����");

	///////////////////////////////////////////////////////////////

	//����ĳһ�б�����ɫ
	int FindColColor(int col ,COLORREF &color);

	//����ĳһ�б�����ɫ
	int FindRowColor(int row ,COLORREF &color);

	//����ĳһ����Ԫ�񱳾���ɫ
	int FindItemColor(int col,int row,COLORREF &color);

	//����ĳһ���ı���ɫ
	int FindRowTextColor(int row,COLORREF &color);

	//����ĳһ���ı���ɫ
	int FindColTextColor(int col,COLORREF &color);

	//����ĳһ��Ԫ���ı���ɫ
	int FindItemTextColor(int col,int row,COLORREF &color);

	//����ĳһ�е�����
	int FindColFont(int col);

	//����ĳһ����Ԫ�������
	int FindItemFont(int row,int col);
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMyListCtrl)
	protected:
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

// Implementation
public:
	//����һ��,��һ��д��Intֵ
	int	 InsertItemInt(int nItem,int val);

	//��ָ����Ԫ��д��Intֵ
	BOOL SetItemInt(int nItem,int nSubItem,int val);

	//��ָ����Ԫ��д��Floatֵ
	BOOL SetItemFloat(int nItem,int nSubItem,float val);

	//��ȡָ����Ԫ���ֵת��ΪInt����
	int	 GetItemInt(int nItem,int nSubItem);
	
	//��ȡָ����Ԫ���ֵת��ΪFloat����
	float GetItemFloat(int nItem,int nSubItem);
	//////////////////////////////////////////////////////////////////////////
	//����б�
	void Clear(void);

	//����б�ͷ
	void ClearHead(void);

	//���ûص�������KillComboBox�Ļص���
	void SetCallbackFunc(COMBOKILLFOCUS pFunc,CDialog* pDlg=NULL);

	// ��Ԫ�����˫���¼�
	void SetItemDblClkFunc(ITEMDBLCLK pFunc,int nColItem,void* pParam);

	//��ȡ��ǰѡ�е���
	int GetCurSel(void);

	//��ȡ��һ��ѡ�е���
	int GetPrevSel(void);

	//���õ�ǰѡ�е���
	void SetCurSel(int nItem);

	//����ѡ��һ�е�ģʽ
	void SetColSelectMode(bool bColSelect=TRUE);

	//����ѡ��һ�е�ģʽ
	void SetLineSelectMode(bool bLineSelect=TRUE,bool bLineSelectBkColor=TRUE);

	//COMBOBOX�ؼ�Value Change ʱ���ڸ���
	void UpdateCombo(int nCol,CString* pComboStr,int nComboStr);

	//���õ�Ԫ��ͼƬ
	void SetItemImage(int row,int col,UINT nIDResource);

	//ȡ����Ԫ�е�ͼƬ
	void UnSetItemImage(int row,int col);

	//�жϵ�Ԫ�����Ƿ���ͼƬ
	BOOL IsItemImage(int row,int col);

	//���µ�Ԫ���е��ı���
	void Update(int row,int col,LPCTSTR pszTxt);

	//��ȡ��Ԫ���Rect
	void GetCellRect(int nItem,int nSubItem,CRect& rect);

	//����һ��(Format=DT_LEFT��DT_CENTER��DT_RIGHT)
	int InsertColumn(int col,LPCTSTR pszHead,UINT Format,int nHeadWidth);

	//����������
	void InsertColumn(int num,CString* pHead,UINT Format,int* pHeadWidth);

	//��Ӧ����Ϣ
	virtual ~CMyListCtrl();
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	afx_msg void OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	void MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	//˽�к���
private:
	void UpdateCharData(CELLVALUE* pcv,CString& strItem);
	void UpdateUCharData(CELLVALUE* pcv,CString& strItem);
	void UpdateStringData(CELLVALUE* pcv,CString& strItem);
	void UpdateIntData(CELLVALUE* pcv,CString& strItem);
	void UpdateLongData(CELLVALUE* pcv,CString& strItem);
	void UpdateFloatData(CELLVALUE* pcv,CString& strItem);
	void UpdateDoubleData(CELLVALUE* pcv,CString& strItem);
	// Generated message map functions
	//afx_msg void OnNMCustomdraw(NMHDR *pNMHDR, LRESULT *pResult);
protected:
	//{{AFX_MSG(CMyListCtrl)
	afx_msg void OnSetfocusEdit();
	afx_msg void OnKillfocusEdit();
	afx_msg void OnSetfocusCombo();
	afx_msg void OnKillfocusCombo();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnColumnclick(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()

	//���˽�г�Ա����
private:
	int m_nCurSelCol;//��ǰѡ�е���
	int m_nPrevSelected;//��һ��ѡ�е���
	int m_nCurSelected;//��ǰѡ�е���
	bool m_bLineSelected;
	bool m_bLineSelectedMode;//һ��ѡ��ģʽ
	int m_nColSelected;
	bool m_bColSelectedMode;//һ��ѡ��ģʽ
	bool m_bLineSelectBkColor;//��ѡ��ģʽʱ��ѡ��һ���Ƿ���Ҫ����ɫ
	//Private Function
	BOOL CreateCtrl(CELLVALUE* pCV);
	///////////////////////////////
	CMyHeadCtrl m_Head;
	CFont *m_pFont;
	///////////////////////////////
	//�ؼ�
	CEdit* m_pEdit;
	CComboBox* m_pCombo;

	//����
	CButton *m_pButton;
	////////////////////////////

	////////////////////////////
	stFont* m_pItemFont;	//����Item����
	int m_nFont,m_nFontSize;
	////////////////////////////
	CExArray<CELLVALUE> m_val;
//	CExArray<CELLSTYLE> m_stl;
	CFont *m_pCmbFont;
	/////////////////
	CArray<stColor,stColor&> m_ptrListRow;		//��������ɫ
	CArray<stColor,stColor&> m_ptrListCol;		//��������ɫ
	CArray<stColor,stColor&> m_ptrListItem;		//����Item��ɫ��
	CArray<stColor,stColor&> m_colTextColor;	//������������ɫ
	CArray<stColor,stColor&> m_rowTextColor;	//������������ɫ
	CArray<stColor,stColor&> m_ItemTextColor;	//���浥Ԫ��������ɫ
	/////////////////
	CELLVALUE* m_pCV;
	int m_nRowHeight;//�и�!
	///////////////////////
	//CALLBACK FUNCTION
	COMBOKILLFOCUS m_pComboKillFocus;
	ITEMDBLCLK m_pItemDblClk;
	//CALLBACK FUNCTION PARAMS COMBOKILLFOCUS
	CDialog* m_pDlg;
	//CALLBACK FUNCTION PARAMS ITEMDLBCLK
	int m_nColItem;
	void* m_pVoid; // ���صĲ���
	/////////////////////////////////////////////////
	//����GetNextItemChange(int& row,int& col)����
	int m_nCurItemChange;
	
};

/////////////////////////////////////////////////////////////////////////////


//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MYLISTCTRL1_H__6F14775C_CA2F_464E_84E6_D400E924309E__INCLUDED_)
