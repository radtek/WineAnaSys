// MyListCtrl1.cpp : implementation file
//

#include "stdafx.h"
#include "MyListCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CExArray
/**************************************
*	�������ƣ�Add(CELLVALUE& cell)
*	�������ͣ�void
*	����˵����CELLVALUE& cell:CELLVALUE�Ľṹ�������
*	�������ܣ����CELLVALUE�ṹ��CExArray���У������ڴ��С�ᶯ̬�����Ӻͼ�С
**************************************/
template <class T>
void CExArray<T>::Add(T& cell)
{
	T* pCell=GetAt(cell.dwRow,cell.dwCol);
	if(pCell!=NULL)
	{
		memcpy(pCell,&cell,sizeof(T));
		return;
	}
	//////////////////////////////
	if(m_dwCurCount>=m_dwSize)
	{
		T *pTmp1,*pTmp2;
		pTmp1=m_pVal;
		m_dwSize+=10;
		pTmp2=new T[m_dwSize];
		memcpy(pTmp2,m_pVal,sizeof(T)*m_dwCurCount);
		m_pVal=pTmp2;
		delete[] pTmp1;
	}
	////////////////////////////
	memcpy(&m_pVal[m_dwCurCount],&cell,sizeof(T));
	m_dwCurCount++;
}

template <class T>
void CExArray<T>::DelAll()
{
	m_dwCurCount=0;
	if(m_pVal!=NULL)
	{
		delete[] m_pVal;
		m_pVal=NULL;
	}
	m_pVal=new T[10];
	m_dwSize=10;
}

template<class T>
void CExArray<T>::DelAt(DWORD dwRow,DWORD dwCol)
{
	for(int i=0;(DWORD)i<m_dwCurCount;i++)
	{
		if(m_pVal[i].dwCol==dwCol && m_pVal[i].dwRow==dwRow)
		{
			for(int j=i;(DWORD)j<m_dwCurCount-1;j++)
			{
				m_pVal[j]=m_pVal[j+1];
			}
			m_dwCurCount--;
			break;
		}
	}
	//��̬��С�ڴ�
	if(m_dwCurCount<=(m_dwSize-10))
	{
		T *pTmp1,*pTmp2;
		pTmp1=m_pVal;
		m_dwSize-=10;
		pTmp2=new T[m_dwSize];
		memcpy(pTmp2,m_pVal,sizeof(T)*m_dwCurCount);
		m_pVal=pTmp2;
		delete[] pTmp1;		
	}
}

template<class T>
BOOL CExArray<T>::GetAt(DWORD dwRow,DWORD dwCol,T& cell)
{
	for(int i=0;(DWORD)i<m_dwCurCount;i++)
	{
		if(m_pVal[i].dwCol==dwCol && m_pVal[i].dwRow==dwRow)
		{
			cell=m_pVal[i];
			return TRUE;
		}
	}
	return FALSE;
}

template<class T>
BOOL CExArray<T>::GetAt(DWORD dwIndex,T& cell)
{
	if(dwIndex>=0 && dwIndex<m_dwCurCount)
	{
		cell=m_pVal[dwIndex];
		return TRUE;
	}
	return FALSE;
}

template<class T>
T* CExArray<T>::GetAt(DWORD dwIndex)
{
	if(dwIndex>=0 && dwIndex<m_dwCurCount)
	{
		return (m_pVal+dwIndex);
	}
	return NULL;
}

template<class T>
T* CExArray<T>::GetAt(DWORD dwRow, DWORD dwCol)
{
	for(int i=0;(DWORD)i<m_dwCurCount;i++)
	{
		if(m_pVal[i].dwCol==dwCol && m_pVal[i].dwRow==dwRow)
		{
			return (m_pVal+i);
		}
	}
	return NULL;
}

template<class T>
int CExArray<T>::GetCount()
{
	return m_dwCurCount;
}

/////////////////////////////////////////////////////////////////////////////
// CMyListCtrl

CMyListCtrl::CMyListCtrl()
{
	m_pComboKillFocus=NULL;
	m_pItemDblClk=NULL;
	//////////////////
	m_pCV=NULL;
	m_pCombo=NULL;
	m_pEdit=NULL;
	m_pFont=NULL;
	m_nRowHeight=0;
	m_pCmbFont=NULL;
	m_pItemFont=NULL;
	m_nFont=0;
	m_nFontSize=0;
	/////////////////////
	m_bLineSelectBkColor=TRUE;
	m_bLineSelectedMode=FALSE;
	m_bLineSelected=FALSE;
	m_nCurSelected=-1;
	m_nCurSelCol=-1;
	m_nPrevSelected=-1;
	/////////////////////
	m_nCurItemChange=0;
	m_bColSelectedMode=TRUE;
}

CMyListCtrl::~CMyListCtrl()
{
	/////////////////////////
	if(m_pFont!=NULL)
	{
		delete m_pFont;
		m_pFont=NULL;
	}
	/////////////////////////
	if(m_pCmbFont!=NULL)
	{
		delete m_pCmbFont;
		m_pCmbFont=NULL;
	}
	/////////////////////////
	if(m_pCombo!=NULL)
	{
		delete m_pCombo;
		m_pCombo=NULL;
	}
	/////////////////////////
	if(m_pEdit!=NULL)
	{
		delete m_pEdit;
		m_pEdit=NULL;
	}
	/////////////////////////
	if(m_pItemFont!=NULL)
	{
		delete[] m_pItemFont;
		m_pItemFont=NULL;
	}
	/////////////////////////
}


BEGIN_MESSAGE_MAP(CMyListCtrl, CListCtrl)
	//{{AFX_MSG_MAP(CMyListCtrl)
	ON_EN_SETFOCUS(IDC_EDT_CTRL, OnSetfocusEdit)
	ON_EN_KILLFOCUS(IDC_EDT_CTRL, OnKillfocusEdit)
	ON_CBN_SETFOCUS(IDC_CMB_CTRL, OnSetfocusCombo)
	ON_CBN_KILLFOCUS(IDC_CMB_CTRL, OnKillfocusCombo)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_CTLCOLOR()
	ON_WM_KEYDOWN()
	ON_NOTIFY_REFLECT(LVN_COLUMNCLICK, OnColumnclick)
	ON_WM_MEASUREITEM()
	ON_WM_MEASUREITEM_REFLECT()
	//}}AFX_MSG_MAP
	//ON_NOTIFY_REFLECT(NM_CUSTOMDRAW, OnNMCustomdraw)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMyListCtrl message handlers

void CMyListCtrl::PreSubclassWindow() 
{
	// TODO: Add your specialized code here and/or call the base class
	// TODO: �ڴ����ר�ô����/����û���
	ModifyStyle(0,LVS_OWNERDRAWFIXED);

	CListCtrl::PreSubclassWindow();
	this->SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

	CHeaderCtrl *pHeader = GetHeaderCtrl();
	m_Head.SubclassWindow(pHeader->GetSafeHwnd());

	CListCtrl::PreSubclassWindow();
}

void CMyListCtrl::OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

	CListCtrl::OnMeasureItem(nIDCtl, lpMeasureItemStruct);
}
void CMyListCtrl::MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct)
{
	if (m_nRowHeight>0)
	{
		lpMeasureItemStruct->itemHeight = m_nRowHeight;
	}
}

/**************************************************************
*	�������ƣ�	Attach(DWORD dwItem, DWORD dwSubItem, void* pData, unsigned char cDataType,\
*						 unsigned char cCtrlType, int nComboStr, CString* pComboStr)
*	�������ͣ�	void
*	����˵����	DWORD dwItem,DWORD dwSubItem:�б���к�,�кš�
*				void* pData:�ڴ��ַ�����������Ԫ���һ���ڴ�
*				unsigned char cDataTyoe:��������(int-2�ֽ�,char,uchar,long-4�ֽ�,float,double)
*				unsigned char cCtrlTye:�ؼ�����(CTRL_TYPE_EDIT,CTRL_TYPE_COMBO)
*				int nComboStr,CString* pComboStr:CComboBox�ؼ��������б�ĸ���,�����б��ַ���ָ��
*	�������ܣ�	���б��еĵ�Ԫ�����һ���ؼ���һ���ڴ��ַ
**************************************************************/
void CMyListCtrl::Attach(DWORD dwItem, DWORD dwSubItem, void* pData, unsigned char cDataType,\
						 unsigned char cCtrlType, int nComboStr, CString* pComboStr)
{
	CELLVALUE cv;	
	memset(&cv,0,sizeof(CELLVALUE));
	cv.cCtrlType=cCtrlType;
	cv.cDataType=cDataType;
	cv.dwCol=dwSubItem;
	cv.dwRow=dwItem;
	cv.pValue=pData;
	cv.nComboStr=nComboStr;
	cv.pComboStr=pComboStr;
	//Init
	cv.nStrMaxLen=256;
	strcpy(cv.strPrevItem,"!@#$%^&*");
	m_val.Add(cv);
	/////////////////////////
}

/*******************************************
*	�������ƣ�	GetItemChangeNum()
*	�������ͣ�	int
*	�������ܣ�	����CListCtrl�б������ݱ仯�ĵ�Ԫ������
*******************************************/
int CMyListCtrl::GetItemChangeNum()
{
	int num=0;
	CELLVALUE* pcv;
	for(int i=0;i<m_val.GetCount();i++)
	{
		pcv=m_val.GetAt(i);
		if(pcv==NULL)
			continue;
		if(pcv->bItemChange)
			num++;
	}
	return num;
}

/*******************************************
*	�������ƣ�	GetFirstItemChange(int& row, int& col)
*	�������ͣ�	bool
*	����˵����	int& row,int& col:�к�,�к�
*	�������ܣ�	��ȡ��һ��ֵ�ı䵥Ԫ����кź��к�,����з���TRUE,���򷵻�FALSE
*******************************************/
bool CMyListCtrl::GetFirstItemChange(int& row, int& col)
{
	m_nCurItemChange=0;
	bool bRet=TRUE;
	bRet=GetNextItemChange(row,col);
	return bRet;
}

/*******************************************
*	�������ƣ�	GetNextItemChange(int& row, int& col)
*	�������ͣ�	bool
*	����˵����	int& row,int& col:�к�,�к�
*	�������ܣ�	��ȡ��һ��ֵ�ı䵥Ԫ����кź��к�,����з���TRUE,���򷵻�FALSE
*******************************************/
bool CMyListCtrl::GetNextItemChange(int& row, int& col)
{
	CELLVALUE* pcv;
	int num=m_val.GetCount();
	int i;
	for(i=m_nCurItemChange;i<num;i++)
	{
		pcv=m_val.GetAt(i);
		if(pcv==NULL)
			continue;
		if(pcv->bItemChange)
		{
			row=pcv->dwRow;
			col=pcv->dwCol;
			break;
		}
	}
	if(i>=num)
	{
		m_nCurItemChange=0;
		return FALSE;
	}
	m_nCurItemChange=i+1;
	return TRUE;
}

//CEdit�ؼ���Ӧ��SetFocus�¼���Ϣ
void CMyListCtrl::OnSetfocusEdit()
{
	CString str=this->GetItemText(m_pCV->dwRow,m_pCV->dwCol);
	m_pEdit->SetWindowText(str);
	m_pEdit->SetSel(0,-1);
}

//CEdit�ؼ���Ӧ��KillFocus�¼���Ϣ
void CMyListCtrl::OnKillfocusEdit()
{
	CString str;
	if(m_pEdit==NULL)
		return;

	m_pEdit->GetWindowText(str);
	SetItemText(m_pCV->dwRow,m_pCV->dwCol,str);
	Update(m_pCV->dwRow,m_pCV->dwCol,str);
	////////////////////////////////////////////////////////
	if(m_pEdit!=NULL)
	{
		delete m_pEdit;
		m_pEdit=NULL;
	}

}

//CComboBox�ؼ���Ӧ��SetFocus�¼���Ϣ
void CMyListCtrl::OnSetfocusCombo()
{
	CString str=this->GetItemText(m_pCV->dwRow,m_pCV->dwCol);
	m_pCombo->SetWindowText(str);
	m_pCombo->SetEditSel(0,-1);
}

//CComboBox�ؼ���Ӧ��KillFocus�¼���Ϣ
void CMyListCtrl::OnKillfocusCombo()
{
	CString str;
	if(m_pCombo==NULL)
		return;

	m_pCombo->GetWindowText(str);
	SetItemText(m_pCV->dwRow,m_pCV->dwCol,str);
	Update(m_pCV->dwRow,m_pCV->dwCol,str);
	////////////////////////////////////////////
	if(m_pCombo!=NULL)
	{
		delete m_pCombo;
		m_pCombo=NULL;
	}
	////////////////////////////
	//Use Callback Fuction
	if(m_pComboKillFocus!=NULL)
		m_pComboKillFocus(m_pDlg,m_pCV->dwCol);
}

//CListCtrl�ؼ���DrawItem��Ϣ
void CMyListCtrl::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{

	// TODO:  ������Ĵ����Ի���ָ����
	TCHAR lpBuffer[CELL_MAXLEN];
	int nRow=lpDrawItemStruct->itemID;
	
	LV_ITEM lvi;

	LV_COLUMN lvc, lvcprev ;
	::ZeroMemory(&lvc, sizeof(lvc));
	::ZeroMemory(&lvcprev, sizeof(lvcprev));
	lvc.mask = LVCF_WIDTH | LVCF_FMT;
	lvcprev.mask = LVCF_WIDTH | LVCF_FMT;

	CDC* pDC;
	pDC = CDC::FromHandle(lpDrawItemStruct->hDC);
	
	CRect rtClient;
	GetClientRect(&rtClient);
	//////////////////////////////////////////////////////////////////////////
	for ( int nCol=0; GetColumn(nCol, &lvc); nCol++)
	{
		HEADCELL* pHead=m_Head.FindHeadCell(nCol);
		CELLVALUE* pCell=m_val.GetAt(nRow,nCol);
		if ( nCol > 0 ) 
		{
			// Get Previous Column Width in order to move the next display item
			GetColumn(nCol-1, &lvcprev) ;
			lpDrawItemStruct->rcItem.left += lvcprev.cx ;
			lpDrawItemStruct->rcItem.right += lpDrawItemStruct->rcItem.left; 
		}

		CRect rcItem;
		if (!GetSubItemRect(lpDrawItemStruct->itemID,nCol,LVIR_LABEL,rcItem))   
			continue;   

		::ZeroMemory(&lvi, sizeof(lvi));
		lvi.iItem = lpDrawItemStruct->itemID;
		lvi.mask = LVIF_TEXT | LVIF_PARAM;
		lvi.iSubItem = nCol;
		lvi.pszText = lpBuffer;
		lvi.cchTextMax = sizeof(lpBuffer);
		VERIFY(GetItem(&lvi));
		CRect rcTemp;
		rcTemp = rcItem;

		if (nCol==0)
		{
			rcTemp.left -=2;
		}

		//����ɫ
		CRect rcBk,rct;
		GetCellRect(nRow,nCol,rct);
		rcBk=rct;
		//////////////////////////////////////////////////////////////////////////
		if(nRow==0)	rcBk.top-=1;
		else		rcBk.top+=1;
		rcBk.bottom-=1;
		//////////////////////////////////////////////////////////////////////////
		COLORREF color;
		color = GetBkColor();
		pDC->FillSolidRect(rcBk,color);
		if(color==RGB(212,208,200)){
			rct.top-=1;
			rct.right+=1;
			pDC->DrawEdge(&rct,BDR_RAISEDINNER | EDGE_RAISED ,BF_RECT);
		}

		//��
		if(FindColColor(nCol,color)>-1)
		{
			pDC->FillSolidRect(rcBk,color);
			if(color==RGB(212,208,200)){
				rct.top-=1;
				rct.right+=1;
				pDC->DrawEdge(&rct,BDR_RAISEDINNER | EDGE_RAISED ,BF_RECT);
			}
		}
		//��
		if(FindRowColor(nRow,color)>-1)
		{
			pDC->FillSolidRect(rcBk,color);
			if(color==RGB(212,208,200)){
				rct.top-=1;
				rct.right+=1;
				pDC->DrawEdge(&rct,BDR_RAISEDINNER | EDGE_RAISED ,BF_RECT);
			}
		}
		//��Ԫ��
		if(FindItemColor(nCol,nRow,color)>-1)
		{
			pDC->FillSolidRect(rcBk,color);
			if(color==RGB(212,208,200)){
				rct.top-=1;
				rct.right+=1;
				pDC->DrawEdge(&rct,BDR_RAISEDINNER | EDGE_RAISED ,BF_RECT);
			}
		}
		//////////////////////////////////////////////////////////////////////////
		COLORREF colorBack=color;
		//////////////////////////////////////////////////////////////////////////

		UINT   uFormat = DT_CENTER;

		if (pHead && pHead->cFormat==0)
		{
			uFormat = DT_LEFT;
		}
		else if (pHead && pHead->cFormat==1)
		{
			uFormat = DT_CENTER;
		}
		else if (pHead && pHead->cFormat==2)
		{
			uFormat = DT_RIGHT;
		}
		TEXTMETRIC metric;
		pDC->GetTextMetrics(&metric);
		int ofst;
		ofst = rcItem.Height() - metric.tmHeight;
		rcItem.OffsetRect(0,ofst/2);
		//�ı���ɫ!
		pDC->SetTextColor(RGB(0,0,0));

		//��
		if(FindColTextColor(nCol,color)>-1)
		{
			pDC->SetTextColor(color);
		}
		//��
		if(FindRowTextColor(nRow,color)>-1)
		{
			pDC->SetTextColor(color);
		}
		//��Ԫ��
		if(FindItemTextColor(nCol,nRow,color)>-1)
		{
			pDC->SetTextColor(color);
		}
		////////////////////////////////////////////////////////////////
		//ѡ��״̬!
		//if ( lpDrawItemStruct->itemState & ODS_SELECTED )//ѡ��״̬!
		////////////////////////////////////////////////////////////////
		//ѡ��һ��
		if(m_bColSelectedMode)
		{
			if(nCol==m_nColSelected)
			{
				this->GetSubItemRect(nRow,nCol,LVIR_BOUNDS,rct);
				pDC->FillSolidRect(&rct,RGB(234,236,245));

				if(nRow==0)
				{
					pDC->FillSolidRect(rct.left+2,rct.top+1,rct.Width()-3,2,RGB(10,36,106));
					pDC->FillSolidRect(rct.left+2,rct.top+1,2,rct.Height()+1,RGB(10,36,106));
					pDC->FillSolidRect(rct.right-3,rct.top+1,2,rct.Height()+1,RGB(10,36,106));
				}
				else if(nRow==GetItemCount()-1)
				{
					pDC->FillSolidRect(rct.left+2,rct.bottom-3,rct.Width()-3,2,RGB(10,36,106));
					pDC->FillSolidRect(rct.left+2,rct.top+1,2,rct.Height()-2,RGB(10,36,106));
					pDC->FillSolidRect(rct.right-3,rct.top+1,2,rct.Height()-2,RGB(10,36,106));
				}
				else
				{
					pDC->FillSolidRect(rct.left+2,rct.top,2,rct.Height()+1,RGB(10,36,106));
					pDC->FillSolidRect(rct.right-3,rct.top,2,rct.Height()+1,RGB(10,36,106));
				}
			}
		}
		//��ѡ��
		if ( m_nCurSelected!=-1 && m_nCurSelected==nRow )
		{
			//ѡ��һ��
			if(m_bLineSelected)
			{
				CPen pen,*pOld;
				pen.CreatePen(PS_SOLID,2,RGB(10,36,106));
				CBrush *pBrush=CBrush::FromHandle((HBRUSH)GetStockObject(NULL_BRUSH));//�����ջ�ˢ����ֹ�ڵ�����;
				CBrush *pOldBrush;

				//����ÿ����Ԫ���Ʊ�����Ȼ���ٻ�һ��͸��������һ�о��ο�
				if(m_bLineSelectBkColor)
				{
					if(colorBack!=RGB(212,208,200) || nCol != 0){
						this->GetSubItemRect(nRow,nCol,LVIR_BOUNDS,rct);
						pDC->FillSolidRect(&rct,RGB(234,236,245));
					}
				}

				pOld=pDC->SelectObject(&pen);
				pOldBrush=pDC->SelectObject(pBrush);

				CRect rct;
				this->GetItemRect(nRow,&rct,LVIR_BOUNDS);
				FindColColor(0,colorBack);
				if(colorBack==RGB(212,208,200)){
					GetCellRect(nRow,0,rcBk);
					rct.left+=rcBk.Width();
				}
				//////////////////////////////////////////////////
				rct.top+=1;
				rct.left+=2;
				rct.bottom-=1;
				rct.right-=1;
				pDC->Rectangle(&rct);
				pDC->SetTextColor(RGB(0,0,255));
				pDC->SelectObject(pOldBrush);
				pDC->SelectObject(pOld);
				pen.DeleteObject();
			}
			//ѡ��ĳһ����Ԫ
			else
			{
				if(m_nCurSelCol!=-1 && m_nCurSelCol==nCol)
				{
					CPen pen,*pOld;
					pen.CreatePen(PS_SOLID,2,RGB(10,36,106));
					CBrush *pBrush=CBrush::FromHandle((HBRUSH)GetStockObject(NULL_BRUSH));//�����ջ�ˢ����ֹ�ڵ�����
					CBrush *pOldBrush;

					pOld=pDC->SelectObject(&pen);
					pOldBrush=pDC->SelectObject(pBrush);

					CRect rct;
					rct=rcTemp;
					rct.top+=1;
					rct.left+=2;
					rct.bottom-=1;
					rct.right-=1;
					pDC->Rectangle(&rct);
			
					pDC->SelectObject(pOldBrush);
					pDC->SelectObject(pOld);
					pen.DeleteObject();
				}
			}
		}
		/////////////////////////////////////////////////////////////////
		//��λͼ!
		CBitmap bmp,*pOldBmp;
		CDC memDC;
		memDC.CreateCompatibleDC(pDC);
		if(pCell)
		{
			CRect rect;
			GetCellRect(pCell->dwRow,pCell->dwCol,rect);
			if(pCell->nIDResource>0 && bmp.LoadBitmap(pCell->nIDResource))
			{
				BITMAP bm;
				bmp.GetBitmap(&bm);
				pOldBmp=memDC.SelectObject(&bmp);
				int offH=rect.Height()-bm.bmHeight;
				int offW=rect.Width()-bm.bmWidth;
				if(offH>0)
				{
					rect.top+=offH/2;
					rect.bottom-=(offH-offH/2);
				}
				if(offW>0)
				{
					rect.left+=offW/2;
					rect.right-=(offW-offW/2);
				}
				//��ʾ
				pDC->StretchBlt(rect.left,rect.top,rect.Width(),rect.Height(),&memDC,0,0,\
					bm.bmWidth,bm.bmHeight,SRCCOPY);
				//Clean
				memDC.SelectObject(pOldBmp);
				bmp.DeleteObject();
			}
		}
		//Clean
		memDC.DeleteDC();
		///////////////////////////////////////////////
		//�ı�����
		CFont * pOldFont=NULL; 
		if(m_pFont!=NULL)
			pOldFont = pDC->SelectObject(m_pFont);

		int nFontID=FindColFont(nCol);
		if(nFontID>=0)
		{
			pDC->SelectObject(pOldFont);
			pOldFont=pDC->SelectObject(&m_pItemFont[nFontID].font);
		}

		nFontID=FindItemFont(nRow,nCol);
		if(nFontID>=0)
		{
			pDC->SelectObject(pOldFont);
			pOldFont=pDC->SelectObject(&m_pItemFont[nFontID].font);
		}
		//����ı�
		pDC->DrawText(lpBuffer,&rcItem,uFormat);
		//�ָ�����
		if(pOldFont!=NULL)
			pDC->SelectObject(pOldFont);
		///////////////////////////////////////////////
		//����
		if(pCell)
		{
			Update(nRow,nCol,lpBuffer);
		}
		///////////////////////////////////////////////
		//�ؼ�
	}
}

/**********************************
*	�������ƣ�	SetHeadHeight(float Height)
*	�������ͣ�	void
*	����˵����	float Height:��ͷ�ĸ߶�ֵ,���ñ�����ֵ(1.5����2����)
*	�������ܣ�	���ñ�ͷ�߶ȵı���
**********************************/
void CMyListCtrl::SetHeadHeight(float Height)
{
	m_Head.m_fHeight = Height;
}

/**********************************
*	�������ƣ�	SetColColor(int col,COLORREF color)
*	�������ͣ�	void
*	����˵����	int col,COLORREF color:�к�,��ɫֵ
*	�������ܣ�	�����б�ĳһ�еı�����ɫ
**********************************/
void CMyListCtrl::SetColColor(int col,COLORREF color)
{
	COLORREF cr;
	stColor Color;
	Color.nCol=col;
	Color.rgb=color;
	int nRet=FindColColor(col,cr);
	if(nRet>=0)
	{
		if(color==cr)
			return;
		m_ptrListCol.RemoveAt(nRet,1);
	}
	m_ptrListCol.Add(Color);
}

/**********************************
*	�������ƣ�	SetRowColor(int row,COLORREF color)
*	�������ͣ�	void
*	����˵����	int row,COLORREF color:�к�,��ɫֵ
*	�������ܣ�	�����б�ĳһ�еı�����ɫ
**********************************/
void CMyListCtrl::SetRowColor(int row,COLORREF color)
{
	COLORREF cr;
	stColor Color;		
	Color.nRow=row;
	Color.rgb=color;
	int nRet=FindRowColor(row,cr);
	if(nRet>=0)
	{
		if(color==cr)
			return;
		m_ptrListRow.RemoveAt(nRet,1);
	}	
	m_ptrListRow.Add(Color);
	////////////////////////////
	CRect rect;
	GetItemRect(row,&rect,LVIR_BOUNDS);
	this->InvalidateRect(&rect);
}

/**********************************
*	�������ƣ�	UnSetRowColor(int row)
*	�������ͣ�	void
*	����˵����	int row:�к�
*	�������ܣ�	ȡ�����õ�����ɫ
**********************************/
void CMyListCtrl::UnSetRowColor(int row)
{
	COLORREF cr;
	int nRet=FindRowColor(row,cr);
	if(nRet>=0)
		m_ptrListRow.RemoveAt(nRet,1);
	////////////////////////////
	CRect rect;
	GetItemRect(row,&rect,LVIR_BOUNDS);
	this->InvalidateRect(&rect);
}

/**********************************
*	�������ƣ�	SetItemColor(int row,int col,COLORREF color)
*	�������ͣ�	void
*	����˵����	int row,int col,COLORREF color:�к�,�к�,��ɫֵ
*	�������ܣ�	�����б�ĳһ��Ԫ��ı�����ɫ
**********************************/
void CMyListCtrl::SetItemColor(int row,int col,COLORREF color)
{
	COLORREF cr;
	stColor Color;
	Color.nCol=col;
	Color.nRow=row;
	Color.rgb=color;
	int nRet=FindItemColor(col,row,cr);
	if(nRet>=0)
	{
		if(cr==color)
			return;
		m_ptrListItem.RemoveAt(nRet,1);
	}
	m_ptrListItem.Add(Color);
}

/**********************************
*	�������ƣ�	SetRowHeigt(int nHeight)
*	�������ͣ�	void
*	����˵����	int nHeight:�߶�ֵ
*	�������ܣ�	�����б���и�
**********************************/
void CMyListCtrl::SetRowHeigt(int nHeight)
{
	m_nRowHeight = nHeight;

	CRect rcWin;
	GetWindowRect(&rcWin);
	WINDOWPOS wp;
	wp.hwnd = m_hWnd;
	wp.cx = rcWin.Width();
	wp.cy = rcWin.Height();
	wp.flags = SWP_NOACTIVATE | SWP_NOMOVE | SWP_NOOWNERZORDER | SWP_NOZORDER;
	SendMessage(WM_WINDOWPOSCHANGED, 0, (LPARAM)&wp);
}

/**********************************
*	�������ƣ�	SetHeadFont(int nSize,bool bBold,bool bItalic,bool bUnderLine,LPCTSTR pszName)
*	�������ͣ�	void
*	����˵����	int nSize,bool bBold,bool bItalic,bool bUnderLine,LPCTSTR pszName:�����С,�Ƿ����,�Ƿ�б��,
*				�Ƿ����»���,��������(��:����)
*	�������ܣ�	�����б�ͷ���е�����
**********************************/
void CMyListCtrl::SetHeadFont(int nSize,BOOL bBold,BOOL bItalic,BOOL bUnderLine,LPCTSTR pszName)
{
	if(m_Head.m_pFont!=NULL)
	{
		delete m_Head.m_pFont;
		m_Head.m_pFont=NULL;
	}
	m_Head.m_pFont=new CFont();
	m_Head.m_pFont->CreateFont(-nSize,0,0,0,bBold?FW_BOLD:FW_NORMAL,bItalic,bUnderLine,0,DEFAULT_CHARSET,\
		OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,DEFAULT_PITCH|FF_SWISS,pszName);

}

/**********************************
*	�������ƣ�	SetHeadTextColor(COLORREF color, int col)
*	�������ͣ�	void
*	����˵����	COLORREF color, int col:��ɫֵ,�к�
*	�������ܣ�	�����б�ͷĳһ�е��ı���ɫ
**********************************/
void CMyListCtrl::SetHeadTextColor(COLORREF color, int col)
{
	if(col==-1)
	{
		for(int i=0;i<m_Head.GetItemCount();i++)
			m_Head.m_HeadCell[i].rgbText=color;
	}
	else
	{
		HEADCELL* pHead=m_Head.FindHeadCell(col);
		if(pHead)
		{
			pHead->rgbText=color;
		}
	}
}

/**********************************
*	�������ƣ�	SetColFont(int col,int nSize,BOOL bBold,BOOL bItalic,BOOL bUnderLine,LPCTSTR pszName)
*	�������ͣ�	void
*	����˵����	int col,int nSize,BOOL bBold,BOOL bItalic,BOOL bUnderLine,LPCTSTR pszName:�к�,�����С,
*				�Ƿ����,�Ƿ�б��,�Ƿ����»���,��������(��:����)
*	�������ܣ�	�����б�ĳһ�е�����
**********************************/
void CMyListCtrl::SetColFont(int col,int nSize,BOOL bBold,BOOL bItalic,BOOL bUnderLine,LPCTSTR pszName)
{
	for(int i=0;i<m_nFont;i++)
	{
		if(m_pItemFont[i].nCol==col)
		{
			m_pItemFont[i].font.DeleteObject();
			m_pItemFont[i].font.CreateFont(-nSize,0,0,0,bBold?FW_BOLD:FW_NORMAL,bItalic,bUnderLine,0,DEFAULT_CHARSET,\
		OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,DEFAULT_PITCH|FF_SWISS,pszName);
			return;
		}
	}
	///////////////////////////////////////////
	if(m_nFont>=m_nFontSize)
	{
		stFont *pTmp1,*pTmp2;
		pTmp1=m_pItemFont;
		m_nFontSize+=10;
		pTmp2=new stFont[m_nFontSize];
		if(m_pItemFont!=NULL)
			memcpy(pTmp2,m_pItemFont,sizeof(stFont)*m_nFont);
		m_pItemFont=pTmp2;
		if(m_pItemFont!=NULL)
			delete[] pTmp1;
	}
	m_pItemFont[m_nFont].nCol=col;
	m_pItemFont[m_nFont].nRow=-1;
	if(m_pItemFont!=NULL)
		m_pItemFont[m_nFont].font.DeleteObject();
	m_pItemFont[m_nFont].font.CreateFont(-nSize,0,0,0,bBold?FW_BOLD:FW_NORMAL,bItalic,bUnderLine,0,DEFAULT_CHARSET,\
		OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,DEFAULT_PITCH|FF_SWISS,pszName);
	//////////////////////////////
	m_nFont++;
}

/**********************************
*	�������ƣ�	SetItemFont(int row,int col,int nSize,BOOL bBold,BOOL bItalic,BOOL bUnderLine,LPCTSTR pszName)
*	�������ͣ�	void
*	����˵����	int row,int col,int nSize,BOOL bBold,BOOL bItalic,BOOL bUnderLine,LPCTSTR pszName:�к�,�к�
*				�����С,�Ƿ����,�Ƿ�б��,�Ƿ����»���,��������(��:����)
*	�������ܣ�	�����б�ĳһ��Ԫ�������
**********************************/
void CMyListCtrl::SetItemFont(int row,int col,int nSize,BOOL bBold,BOOL bItalic,BOOL bUnderLine,LPCTSTR pszName)
{
	for(int i=0;i<m_nFont;i++)
	{
		if(m_pItemFont[i].nCol==col && m_pItemFont[i].nRow==row)
		{
			m_pItemFont[i].font.DeleteObject();
			m_pItemFont[i].font.CreateFont(-nSize,0,0,0,bBold?FW_BOLD:FW_NORMAL,bItalic,bUnderLine,0,\
				DEFAULT_CHARSET,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,\
				DEFAULT_PITCH|FF_SWISS,pszName);
			return;
		}
	}
	///////////////////////////////////////////
	if(m_nFont>=m_nFontSize)
	{
		stFont *pTmp1,*pTmp2;
		pTmp1=m_pItemFont;
		m_nFontSize+=10;
		pTmp2=new stFont[m_nFontSize];
		if(m_pItemFont!=NULL)
			memcpy(pTmp2,m_pItemFont,sizeof(stFont)*m_nFont);
		m_pItemFont=pTmp2;
		if(m_pItemFont!=NULL)
			delete[] pTmp1;
	}
	m_pItemFont[m_nFont].nCol=col;
	m_pItemFont[m_nFont].nRow=row;
	if(m_pItemFont!=NULL)
		m_pItemFont[m_nFont].font.DeleteObject();
	m_pItemFont[m_nFont].font.CreateFont(-nSize,0,0,0,bBold?FW_BOLD:FW_NORMAL,bItalic,bUnderLine,0,DEFAULT_CHARSET,\
		OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,DEFAULT_PITCH|FF_SWISS,pszName);
	//////////////////////////////
	m_nFont++;

}

/**********************************
*	�������ƣ�	SetFont(int nSize,BOOL bBold,BOOL bItalic,BOOL bUnderLine,LPCTSTR pszName)
*	�������ͣ�	void
*	����˵����	int nSize,BOOL bBold,BOOL bItalic,BOOL bUnderLine,LPCTSTR pszName:
*				�����С,�Ƿ����,�Ƿ�б��,�Ƿ����»���,��������(��:����)
*	�������ܣ�	�����б����е�����
**********************************/
void CMyListCtrl::SetFont(int nSize,BOOL bBold,BOOL bItalic,BOOL bUnderLine,LPCTSTR pszName)
{
	/////////////////////////////////////////////////////////
	if(m_pFont!=NULL)
	{
		delete m_pFont;
		m_pFont=NULL;
	}
	m_pFont=new CFont();
	m_pFont->CreateFont(-nSize,0,0,0,bBold?FW_BOLD:FW_NORMAL,bItalic,bUnderLine,0,DEFAULT_CHARSET,\
		OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,DEFAULT_PITCH|FF_SWISS,pszName);
	//////////////////////////////
	if(m_pCmbFont!=NULL)
	{
		delete m_pCmbFont;
		m_pCmbFont=NULL;
	}
	m_pCmbFont=new CFont();
	m_pCmbFont->CreateFont(-nSize+2,0,0,0,bBold?FW_BOLD:FW_NORMAL,bItalic,bUnderLine,0,DEFAULT_CHARSET,\
		OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,DEFAULT_PITCH|FF_SWISS,pszName);
}

/**********************************
*	�������ƣ�	SetColTextColor(int col,COLORREF color)
*	�������ͣ�	void
*	����˵����	int col,COLORREF color:�к�,��ɫֵ
*	�������ܣ�	�����б�ĳһ�е��ı���ɫ
**********************************/
void CMyListCtrl::SetColTextColor(int col,COLORREF color)
{
	COLORREF cr;
	stColor Color;
	Color.nCol=col;
	Color.rgb=color;
	int nRet=FindColTextColor(col,cr);
	if(nRet>=0)
	{
		if(color==cr)
			return;
		m_colTextColor.RemoveAt(nRet,1);
	}
	m_colTextColor.Add(Color);
}

/**********************************
*	�������ƣ�	SetRowTextColor(int row,COLORREF color)
*	�������ͣ�	void
*	����˵����	int row,COLORREF color:�к�,��ɫֵ
*	�������ܣ�	�����б�ĳһ�е��ı���ɫ
**********************************/
void CMyListCtrl::SetRowTextColor(int row,COLORREF color)
{
	COLORREF cr;
	stColor Color;
	Color.nRow=row;
	Color.rgb=color;
	int nRet=FindRowTextColor(row,cr);
	if(nRet>=0)
	{
		if(color==cr)
			return;
		m_rowTextColor.RemoveAt(nRet,1);
	}
	m_rowTextColor.Add(Color);
	//////////////////////////////////
	CRect rect;
	GetItemRect(row,&rect,LVIR_BOUNDS);
	this->InvalidateRect(&rect);
}

/**********************************
*	�������ƣ�	UnSetRowTextColor(int row)
*	�������ͣ�	void
*	����˵����	int row:�к�
*	�������ܣ�	ȡ�������б�ĳһ�е��ı���ɫ
**********************************/
void CMyListCtrl::UnSetRowTextColor(int row)
{
	COLORREF cr;
	int nRet=FindRowTextColor(row,cr);
	if(nRet>=0)
		m_rowTextColor.RemoveAt(nRet,1);
	//////////////////////////////////
	CRect rect;
	GetItemRect(row,&rect,LVIR_BOUNDS);
	this->InvalidateRect(&rect);
}

/**********************************
*	�������ƣ�	SetItemTextColor(int row,int col,COLORREF color)
*	�������ͣ�	void
*	����˵����	int row,int col,COLORREF color:�к�,�к�,��ɫֵ
*	�������ܣ�	�����б�ĳһ��Ԫ����ı���ɫ
**********************************/
void CMyListCtrl::SetItemTextColor(int row,int col,COLORREF color)
{
	COLORREF cr;
	stColor Color;
	Color.nCol=col;
	Color.nRow=row;
	Color.rgb=color;
	int nRet=FindItemTextColor(col,row,cr);
	if(nRet>=0)
	{
		if(color==cr)
			return;
		m_ItemTextColor.RemoveAt(nRet,1);
	}
	m_ItemTextColor.Add(Color);
}

int CMyListCtrl::InsertColumn(int col, LPCTSTR pszHead, UINT Format, int nHeadWidth)
{
	HEADCELL hc;
	switch(Format)
	{
	case DT_LEFT:
		hc.cFormat=0;
		break;
	case DT_CENTER:
		hc.cFormat=1;
		break;
	case DT_RIGHT:
		hc.cFormat=2;
		break;
	default:
		hc.cFormat=1;
		break;
	}
	hc.nCol=col;
	hc.rgbBack=RGB(212,208,200);
	hc.rgbText=RGB(0,0,0);
	hc.strHead.Format("%s",pszHead);
	m_Head.m_HeadCell.Add(hc);

	return CListCtrl::InsertColumn(col,pszHead,Format,nHeadWidth);
}

void CMyListCtrl::InsertColumn(int num,CString* pHead,UINT Format,int* pHeadWidth)
{
	int nWidth=0;
	CRect rect;
	int nW = 0;
	this->GetClientRect(&rect);
	if(pHeadWidth == NULL)
		nW = rect.Width()/num;

	for (int i=0;i<num;i++)
	{
		if(pHeadWidth)
		{
			InsertColumn(i,pHead[i],Format,pHeadWidth[i]);
			nWidth+=pHeadWidth[i];
		}
		else
		{
			InsertColumn(i,pHead[i],Format,nW);
			nWidth+=nW;
		}
	}
	if(rect.Width()>nWidth) InsertColumn(num,"",Format,rect.Width()-nWidth);
}

void CMyListCtrl::SetHeadBKColor(COLORREF rgb, int Gradient, int col)
{
	m_Head.m_nGradient=Gradient;
	if(col==-1)
	{
		for(int i=0;i<m_Head.m_HeadCell.GetSize();i++)
		{
			m_Head.m_HeadCell[i].rgbBack=rgb;
		}
	}
	else
	{
		HEADCELL* pHead=m_Head.FindHeadCell(col);
		if(pHead)
		{
			pHead->rgbBack=rgb;
		}
	}
}

// �����������б�ĳһ���¼�
void CMyListCtrl::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	//ˢ��!
	CRect rect;
	//////////////////////////////////////////////////
	if(m_nCurSelected!=-1)
	{
		GetItemRect(m_nCurSelected,&rect,LVIR_BOUNDS);
		this->InvalidateRect(&rect);
	}
	//////////////////////////////////////////////////
	//�ҳ�ѡ�е�����һ��
	for(int i=0;i<this->GetItemCount();i++)
	{
		this->GetItemRect(i,&rect,LVIR_BOUNDS);
		if(rect.PtInRect(point))
		{
			//////////////////////////
			//��һ��ѡ�е��к�
			m_nPrevSelected=m_nCurSelected;
			//��ǰѡ�е��к�
			m_nCurSelected=i;
			//�ж��Ƿ���������ѡ��ģʽ
			if(m_bLineSelectedMode)
				m_bLineSelected=TRUE;
			else
			{
				////////////////////////////////////////////
				//����ѡ��ģʽʱ����Ҫ���ҳ�ѡ�е�����һ��
				GetCellRect(i,0,rect);
				if(rect.PtInRect(point))
					m_bLineSelected=TRUE;
				else
				{
					m_bLineSelected=FALSE;
					CPoint pt;
					::GetCursorPos(&pt);
					::ScreenToClient(this->m_hWnd,&pt);
					for(int j=0;j<this->GetHeaderCtrl()->GetItemCount();j++)
					{
						GetCellRect(i,j,rect);
						if(rect.PtInRect(pt))
						{
							m_nCurSelCol=j;
							break;
						}
					}
				}
			}
			//////////////////////////////
			break;
		}
	}
	
	CListCtrl::OnLButtonDown(nFlags, point);
}

// ������˫���б�ĳһ���¼�
void CMyListCtrl::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	CRect rect;
	//////////////////////////////////////////////
	for(int i=0;i<m_val.GetCount();i++)
	{
		m_pCV=m_val.GetAt(i);
		this->GetSubItemRect(m_pCV->dwRow,m_pCV->dwCol,LVIR_BOUNDS,rect);
		if(rect.PtInRect(point))
		{
			CreateCtrl(m_pCV);
			break;
		}
	}
	if(m_pItemDblClk!=NULL){
		for (int i=0;i<GetItemCount();i++)
		{
			this->GetSubItemRect(i,m_nColItem,LVIR_BOUNDS,rect);
			if(rect.PtInRect(point)){
				CString str=GetItemText(i,m_nColItem);
				m_pItemDblClk(i,m_nColItem,m_pVoid);
				break;
			}
		}
	}

	CListCtrl::OnLButtonDblClk(nFlags, point);
}

// ��ȡ�б��ĳһ��ĳһ�еľ�������
void CMyListCtrl::GetCellRect(int nItem, int nSubItem, CRect &rect)
{
	this->GetItemRect(nItem,&rect,LVIR_BOUNDS);

	LVCOLUMN column;
	column.mask=LVCF_WIDTH;
	for(int i=0;i<nSubItem;i++)
	{
		this->GetColumn(i,&column);
		rect.left+=column.cx;
	}

	this->GetColumn(nSubItem,&column);
	rect.right=rect.left+column.cx;
}

// ����pCV�еĲ��������ı�����������ؼ���
BOOL CMyListCtrl::CreateCtrl(CELLVALUE* pCV)
{
	DWORD dwStyle;
	CRect rect;
	/////////////////////////////////////////////////////////////
	if(m_pEdit!=NULL)
	{
		delete m_pEdit;
		m_pEdit=NULL;
	}
	if(m_pCombo!=NULL)
	{
		delete m_pCombo;
		m_pCombo=NULL;
	}
	if(m_pButton!=NULL)
	{
		delete m_pButton;
		m_pButton=NULL;
	}
	/////////////////////////////////////////////////////////////
	CString str=this->GetItemText(pCV->dwRow,pCV->dwCol);
	this->GetSubItemRect(pCV->dwRow,pCV->dwCol,LVIR_BOUNDS,rect);

	switch(pCV->cCtrlType)
	{
	case CTRL_TYPE_EDIT:
		{
			m_pEdit=new CEdit();
			dwStyle=WS_BORDER|WS_VISIBLE|ES_AUTOHSCROLL|ES_LEFT;
			rect.bottom-=1;
			m_pEdit->Create(dwStyle,rect,this,IDC_EDT_CTRL);
			if(m_pFont!=NULL)
				m_pEdit->SetFont(m_pFont);
			m_pEdit->SetWindowText(str);
			m_pEdit->ShowWindow(SW_SHOW);
			m_pEdit->SetFocus();
		}
		break;
	case CTRL_TYPE_COMBO:
		{
			m_pCombo=new CComboBox();
			dwStyle=WS_CHILD|CBS_DROPDOWN|WS_VISIBLE|WS_VSCROLL;
			rect.bottom+=200;
			m_pCombo->Create(dwStyle,rect,this,IDC_CMB_CTRL);
			if(m_pCmbFont!=NULL)
				m_pCombo->SetFont(m_pCmbFont);

			for(int i=0;i<pCV->nComboStr;i++)
			{
				m_pCombo->AddString(pCV->pComboStr[i]);
			}

			m_pCombo->ShowWindow(SW_SHOW);
			m_pCombo->SetFocus();
		}
		break;


	case CTRL_TYPE_CHECK:
		{
		}
		break;
	}
	if(m_pCombo!=NULL || m_pEdit!=NULL) 
		return TRUE;
	else								
		return FALSE;
}

//��������ɫ
int CMyListCtrl::FindColColor(int col,COLORREF &color)
{
	int flag = -1;
	for (int i=0;i<m_ptrListCol.GetSize();i++)
	{
		stColor Color=m_ptrListCol.GetAt(i);
		if (Color.nCol==col)
		{
			flag = i;
			color = Color.rgb;
			break;
		}
	}

	return flag;
}

//��������ɫ
int CMyListCtrl::FindRowColor(int row,COLORREF &color)
{
	int flag = -1;
	for (int i=0;i<m_ptrListRow.GetSize();i++)
	{
		stColor Color=m_ptrListRow.GetAt(i);
		if (Color.nRow==row)
		{
			flag = i;
			color = Color.rgb;
			break;
		}
	}

	return flag;
}

//����ĳһ��Ԫ��ɫ
int CMyListCtrl::FindItemColor(int col,int row,COLORREF &color)
{
	int flag = -1;
	for (int i=0;i<m_ptrListItem.GetSize();i++)
	{
		stColor Color = m_ptrListItem.GetAt(i);
		if (Color.nCol==col && Color.nRow==row)
		{
			flag = i;
			color = Color.rgb;
			break;
		}
	}

	return flag;
}

// �������ı���ɫ
int CMyListCtrl::FindColTextColor(int col,COLORREF &color)
{
	int flag = -1;
	for (int i=0;i<m_colTextColor.GetSize();i++)
	{
		stColor Color = m_colTextColor.GetAt(i);
		if (Color.nCol==col)
		{
			flag = 1;
			color = Color.rgb;
			break;
		}
	}

	return flag;
}

// �������ı���ɫ
int CMyListCtrl::FindRowTextColor(int row,COLORREF &color)
{
	int flag = -1;
	for (int i=0;i<m_rowTextColor.GetSize();i++)
	{
		stColor Color = m_rowTextColor.GetAt(i);
		if (Color.nRow==row)
		{
			flag = i;
			color = Color.rgb;
			break;
		}
	}

	return flag;
}

// ����ĳһ��Ԫ�ı���ɫ
int CMyListCtrl::FindItemTextColor(int col,int row,COLORREF &color)
{
	int flag = -1;
	for (int i=0;i<m_ItemTextColor.GetSize();i++)
	{
		stColor Color = m_ItemTextColor.GetAt(i);
		if (Color.nCol==col && Color.nRow==row)
		{
			flag = i;
			color = Color.rgb;
			break;
		}
	}

	return flag;
}

// ����������
int CMyListCtrl::FindColFont(int col)
{
	for(int i=0;i<m_nFont;i++)
	{
		if(m_pItemFont[i].nCol==col && m_pItemFont[i].nRow==-1)
		{
			return i;
		}
	}
	return -1;
}

// ����ĳһ��Ԫ����
int CMyListCtrl::FindItemFont(int row,int col)
{
	for(int i=0;i<m_nFont;i++)
	{
		if(m_pItemFont[i].nCol==col && m_pItemFont[i].nRow==row)
		{
			return i;
		}
	}
	return -1;
}

// ˢ��
void CMyListCtrl::Update(int row, int col, LPCTSTR pszTxt)
{
	CString str;
	str.Format("%s",pszTxt);
	CELLVALUE* pCV=m_val.GetAt(row,col);
	if(pCV==NULL) // �ж�ָ���Ƿ�ΪNULL
		return;
	///////////////////////////////////////
	//�жϵ�Ԫ���е������Ƿ��иı�!
	if(strcmp(pCV->strPrevItem,"!@#$%^&*")==0)
	{
		memset(pCV->strPrevItem,0,CELL_MAXLEN);
		strcpy(pCV->strPrevItem,str);
	}

	if(strcmp(str,pCV->strPrevItem)!=0)
		pCV->bItemChange=1;
	else
		pCV->bItemChange=0;
	//////////////////////////////////////
	switch(pCV->cDataType)
	{
	case DATA_TYPE_CHAR://char
		{
			UpdateCharData(pCV,str);
		}
		break;
	case DATA_TYPE_INT://int(2B)
		{
			UpdateIntData(pCV,str);
		}
		break;
	case DATA_TYPE_FLOAT://float
		{
			UpdateFloatData(pCV,str);
		}
		break;
	case DATA_TYPE_STRING://char*
		{
			UpdateStringData(pCV,str);
		}
		break;
	case DATA_TYPE_UCHAR://unsigned char
		{
			UpdateUCharData(pCV,str);
		}
		break;
	case DATA_TYPE_LONG://unsigned long
		{
			UpdateLongData(pCV,str);
		}
		break;
	case DATA_TYPE_DOUBLE://2��float
		{
			UpdateDoubleData(pCV,str);
		}
		break;
	}
}

void CMyListCtrl::UpdateCharData(CELLVALUE* pcv,CString& strItem)
{
	if(pcv->pValue==NULL)
		return;
	///////////////////////
	switch(pcv->cCtrlType)
	{
	case CTRL_TYPE_EDIT:
		{
			if(strItem.GetLength()>0 && strItem.GetLength()<2)
			{
				if((strItem.GetAt(0)>='a' && strItem.GetAt(0)<='z') || (strItem.GetAt(0)>='A' && \
					strItem.GetAt(0)<='Z'))
					*((char*)pcv->pValue)=strItem.GetAt(0);
				else
					*((char*)pcv->pValue)=atoi(strItem);
			}
			if(strItem.GetLength()>1)
			{
				*((char*)pcv->pValue)=atoi(strItem);
			}
		}
		break;
	case CTRL_TYPE_CHECK:
		break;
	case CTRL_TYPE_COMBO:
		{
			char nIndex;
			if(m_pCombo!=NULL)
				nIndex=m_pCombo->GetCurSel();
			if(strItem=="")
				*((char*)pcv->pValue)=-1;
			else
			{
				if(m_pCombo!=NULL)
					*((char*)pcv->pValue)=nIndex;
				else
				{
					for(int i=0;i<pcv->nComboStr;i++)
					{
						if(strcmp(pcv->pComboStr[i],strItem)==0)
						{
							*((char*)pcv->pValue)=i;
							break;
						}
					}
				}
			}
		}
		break;
	}	
}

void CMyListCtrl::UpdateUCharData(CELLVALUE* pcv,CString& strItem)
{
	if(pcv->pValue==NULL)
		return;
	///////////////////////
	switch(pcv->cCtrlType)
	{
	case CTRL_TYPE_EDIT:
		{
			if(strItem.GetLength()>0 && strItem.GetLength()<2)
			{
				if((strItem.GetAt(0)>='a' && strItem.GetAt(0)<='z') || (strItem.GetAt(0)>='A' && \
					strItem.GetAt(0)<='Z'))
					*((unsigned char*)pcv->pValue)=strItem.GetAt(0);
				else
					*((unsigned char*)pcv->pValue)=atoi(strItem);
			}
			if(strItem.GetLength()>1)
			{
				if(pcv->pValue!=NULL)
					*((unsigned char*)pcv->pValue)=atoi(strItem);
			}			
		}
		break;
	case CTRL_TYPE_CHECK:
		break;
	case CTRL_TYPE_COMBO:
		{
			unsigned char nIndex;
			if(m_pCombo!=NULL)
				nIndex=m_pCombo->GetCurSel();
			if(strItem=="")
				*((unsigned char*)pcv->pValue)=-1;
			else
			{
				if(m_pCombo!=NULL)
					*((unsigned char*)pcv->pValue)=nIndex;
				else
				{
					for(int i=0;i<pcv->nComboStr;i++)
					{
						if(strcmp(pcv->pComboStr[i],strItem)==0)
						{
							*((unsigned char*)pcv->pValue)=i;
							break;
						}
					}
				}
			}
		}
		break;
	}
}

void CMyListCtrl::UpdateStringData(CELLVALUE* pcv,CString& strItem)
{
	if(pcv->pValue==NULL)
		return;
	////////////////////////
	switch(pcv->cCtrlType)
	{
	case CTRL_TYPE_EDIT:
		{
			if(strItem.GetLength()>pcv->nStrMaxLen)
				memcpy((char*)pcv->pValue,strItem,pcv->nStrMaxLen);
			else
				strcpy((char*)pcv->pValue,strItem);
		}
		break;
	case CTRL_TYPE_CHECK:
		break;
	case CTRL_TYPE_COMBO:
		{
			if(strItem.GetLength()>pcv->nStrMaxLen)
				memcpy(((char*)pcv->pValue),strItem,pcv->nStrMaxLen);
			else
				strcpy(((char*)pcv->pValue),strItem);
		}
		break;
	}
}

void CMyListCtrl::UpdateIntData(CELLVALUE* pcv,CString& strItem)
{
	if(pcv->pValue==NULL)
		return;
	///////////////////////
	switch(pcv->cCtrlType)
	{
	case CTRL_TYPE_EDIT:
		{
			*((WORD*)pcv->pValue)=atoi(strItem);
		}
		break;
	case CTRL_TYPE_CHECK:
		break;
	case CTRL_TYPE_COMBO:
		{
			WORD nIndex;
			if(m_pCombo!=NULL)
				nIndex=m_pCombo->GetCurSel();
			if(strItem=="")
				*((WORD*)pcv->pValue)=-1;
			else
			{
				if(m_pCombo!=NULL)
					*((WORD*)pcv->pValue)=nIndex;
				else
				{
					for(int i=0;i<pcv->nComboStr;i++)
					{
						if(strcmp(pcv->pComboStr[i],strItem)==0)
						{
							*((WORD*)pcv->pValue)=i;
							break;
						}
					}
				}
			}
		}
		break;
	}
}

void CMyListCtrl::UpdateLongData(CELLVALUE* pcv,CString& strItem)
{
	if(pcv->pValue==NULL)
		return;
	///////////////////////
	switch(pcv->cCtrlType)
	{
	case CTRL_TYPE_EDIT:
		{
			*((DWORD*)pcv->pValue)=atoi(strItem);
		}
		break;
	case CTRL_TYPE_COMBO:
		{
			DWORD nIndex;
			if(m_pCombo!=NULL)
				nIndex=m_pCombo->GetCurSel();
			if(strItem=="")
				*((DWORD*)pcv->pValue)=-1;
			else
			{
				if(m_pCombo!=NULL)
					*((DWORD*)pcv->pValue)=nIndex;
				else
				{
					for(int i=0;i<pcv->nComboStr;i++)
					{
						if(strcmp(pcv->pComboStr[i],strItem)==0)
						{
							*((DWORD*)pcv->pValue)=i;
							break;
						}
					}
				}
			}
		}
		break;
	case CTRL_TYPE_CHECK:
		break;
	}
}

void CMyListCtrl::UpdateFloatData(CELLVALUE* pcv,CString& strItem)
{
	if(pcv->pValue==NULL)
		return;
	///////////////////////
	switch(pcv->cCtrlType)
	{
	case CTRL_TYPE_EDIT:
		{
			*((float*)pcv->pValue)=(float)atof(strItem);
		}
		break;
	case CTRL_TYPE_CHECK:
		break;
	case CTRL_TYPE_COMBO:
		{
			float fIndex;
			if(m_pCombo!=NULL)
				fIndex=(float)m_pCombo->GetCurSel();
			if(strItem=="")
				*((float*)pcv->pValue)=-1.0;
			else
			{
				if(m_pCombo!=NULL)
					*((float*)pcv->pValue)=fIndex;
				else
				{
					for(int i=0;i<pcv->nComboStr;i++)
					{
						if(strcmp(pcv->pComboStr[i],strItem)==0)
						{
							*((float*)pcv->pValue)=(float)i;
							break;
						}
					}
				}
			}
		}
		break;
	}
}

void CMyListCtrl::UpdateDoubleData(CELLVALUE* pcv,CString& strItem)
{
	if(pcv->pValue==NULL)
		return;
	//////////////////////
	switch(pcv->cCtrlType)
	{
	case CTRL_TYPE_EDIT:
		{
			*((double*)pcv->pValue)=atof(strItem);
		}
		break;
	case CTRL_TYPE_COMBO:
		{
			double fIndex;
			if(m_pCombo!=NULL)
				fIndex=(double)m_pCombo->GetCurSel();
			if(strItem=="")
				*((double*)pcv->pValue)=-1.0;
			else
			{
				if(m_pCombo!=NULL)
					*((double*)pcv->pValue)=fIndex;
				else
				{
					for(int i=0;i<pcv->nComboStr;i++)
					{
						if(strcmp(pcv->pComboStr[i],strItem)==0)
						{
							*((double*)pcv->pValue)=(double)i;
							break;
						}
					}
				}
			}
		}
		break;
	case CTRL_TYPE_CHECK:
		break;
	}
}

HBRUSH CMyListCtrl::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CListCtrl::OnCtlColor(pDC, pWnd, nCtlColor);
	
	// TODO: Change any attributes of the DC here
	if(pWnd->GetDlgCtrlID()==CTRL_TYPE_CHECK)
	{
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(RGB(0,0,0));
		pDC->SetBkColor(RGB(255,255,255));
		hbr=(HBRUSH)GetStockObject(WHITE_BRUSH);
	}
	// TODO: Return a different brush if the default is not desired
	return hbr;
}

void CMyListCtrl::SetHeadColFont(int col,int nFontSize, BOOL Bold, BOOL Italic, BOOL UnderLine, LPCTSTR pszFontName)
{
	m_Head.SetColFont(col,nFontSize,Bold,Italic,UnderLine,pszFontName);
}

void CMyListCtrl::SetItemImage(int row, int col, UINT nIDResource)
{
	CELLVALUE* pCell;
	CELLVALUE cell;
	memset(&cell,0,sizeof(CELLVALUE));
	pCell=m_val.GetAt(row,col);
	if(pCell)
	{
		pCell->nIDResource=nIDResource;
	}
	else
	{
		cell.dwRow=row;
		cell.dwCol=col;
		cell.nIDResource=nIDResource;
		m_val.Add(cell);
	}
	////////////////////////////
	//ˢ��
	CRect rect;
	GetCellRect(row,col,rect);
	this->InvalidateRect(&rect,FALSE);
}

BOOL CMyListCtrl::IsItemImage(int row,int col)
{
	CELLVALUE* pCell=m_val.GetAt(row,col);
	if(pCell)
	{
		if(pCell->nIDResource>0)
			return TRUE;
	}
	return FALSE;
}

void CMyListCtrl::UnSetItemImage(int row, int col)
{
	CELLVALUE *pCell=m_val.GetAt(row,col);
	if(pCell)
	{
		pCell->nIDResource=0;
	}
	////////////////////////////
	//ˢ��
	CRect rect;
	GetItemRect(row,&rect,LVIR_BOUNDS);
	this->InvalidateRect(&rect,FALSE);
}

void CMyListCtrl::UpdateCombo(int nCol,CString *pComboStr, int nComboStr)
{
	for(int i=0;i<m_val.GetCount();i++)
	{
		if(m_val.GetAt(0,nCol)->cCtrlType==CTRL_TYPE_COMBO)
		{
			m_val.GetAt(0,nCol)->nComboStr=nComboStr;
			m_val.GetAt(0,nCol)->pComboStr=pComboStr;
		}
	}
}

void CMyListCtrl::SetLineSelectMode(bool bLineSelect, bool bLineSelectBkColor)
{
	m_bLineSelectedMode=bLineSelect;
	m_bLineSelected=TRUE;
	m_bLineSelectBkColor=bLineSelectBkColor;
}

void CMyListCtrl::SetColSelectMode(bool bColSelect)
{
	m_bColSelectedMode=bColSelect;
}

void CMyListCtrl::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	// TODO: Add your message handler code here and/or call default
	CRect rect;
	//////////////////////////////////////////////////
	if(m_nCurSelected!=-1)
	{
		GetItemRect(m_nCurSelected,&rect,LVIR_BOUNDS);
		this->InvalidateRect(&rect);
	}
	//////////////////////////////////////////////////
	switch(nChar)
	{
	case 40://��
		m_nCurSelected++;
		if(m_nCurSelected>=this->GetItemCount())
			m_nCurSelected--;
		break;
	case 38://��
		m_nCurSelected--;
		if(m_nCurSelected<0)
			m_nCurSelected++;
		break;
	case 39://��
		m_nCurSelCol++;
		if(m_nCurSelCol>=this->GetHeaderCtrl()->GetItemCount())
			m_nCurSelCol--;
		break;
	case 37://��
		m_nCurSelCol--;
		if(m_nCurSelCol<0)
			m_nCurSelCol++;
		break;
	}
	this->GetItemRect(m_nCurSelected,&rect,LVIR_BOUNDS);
	this->InvalidateRect(&rect);

	CListCtrl::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CMyListCtrl::SetCurSel(int nItem)
{
	CRect rect;
	//////////////////////////////////////////////////
	if(m_nCurSelected!=-1)
	{
		m_bLineSelected=FALSE;
		GetItemRect(m_nCurSelected,&rect,LVIR_BOUNDS);
		this->InvalidateRect(&rect);
	}
	m_nCurSelected=nItem;
	m_bLineSelected=TRUE;
	this->GetItemRect(m_nCurSelected,&rect,LVIR_BOUNDS);
	this->InvalidateRect(&rect);

	if(nItem!=-1)
	{
		/////////////////////////////
		//������ָ������
		CSize size;
		size.cx=0;
		int nVerPos=this->GetScrollPos(SB_VERT)+1;
		if(nVerPos==nItem)
			return;
		else if(nVerPos<nItem)
			size.cy=(nItem-nVerPos)*rect.Height();
		else if(nVerPos>nItem)
			size.cy=(nItem-nVerPos)*rect.Height();

		this->Scroll(size);
	}
}

int CMyListCtrl::GetCurSel()
{
	return m_nCurSelected;
}

int CMyListCtrl::GetPrevSel()
{
	return m_nPrevSelected;
}

void CMyListCtrl::SetCallbackFunc(COMBOKILLFOCUS pFunc,CDialog* pDlg)
{
	m_pComboKillFocus=pFunc;
	m_pDlg=pDlg;
}

void CMyListCtrl::SetItemDblClkFunc(ITEMDBLCLK pFunc,int nColItem,void* pParam)
{
	m_pItemDblClk=pFunc;
	m_nColItem=nColItem;
	m_pVoid=pParam;
}

void CMyListCtrl::SetStrMaxLen(int nRow, int nCol, int nMaxLen)
{
	CELLVALUE* pcv=m_val.GetAt(nRow,nCol);
	pcv->nStrMaxLen=nMaxLen;
}

// CListCtrl����ͷ�����¼�
void CMyListCtrl::OnColumnclick(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here

	*pResult = 0;
}

void CMyListCtrl::Clear()
{
	DeleteAllItems();
	/////////////////////////
	m_val.DelAll();
}

void CMyListCtrl::ClearHead(void)
{
	int headCount = m_Head.GetItemCount();
	for (int i=0;i<headCount;i++)
	{
		DeleteColumn(0);
	}
	m_Head.m_HeadCell.RemoveAll();
}

int	 CMyListCtrl::InsertItemInt(int nItem,int val)
{
	CString str="";
	if(val<0) str.Format("-%d",abs(val));
	else str.Format("%d",val);
	return InsertItem(nItem,str);
}

BOOL CMyListCtrl::SetItemInt(int nItem,int nSubItem,int val)
{
	CString str="";
	if(val<0) str.Format("-%d",abs(val));
	else str.Format("%d",val);
	return SetItemText(nItem,nSubItem,str);
}

BOOL CMyListCtrl::SetItemFloat(int nItem,int nSubItem,float val)
{
	CString str="";
	if(val<0.0) str.Format("-%.4f",(val*-1.0));
	else str.Format("%.4f",val);
	return SetItemText(nItem,nSubItem,str);
}

int	 CMyListCtrl::GetItemInt(int nItem,int nSubItem)
{
	CString str=GetItemText(nItem,nSubItem);
	return (atoi(str));
}

float CMyListCtrl::GetItemFloat(int nItem,int nSubItem)
{
	CString str=GetItemText(nItem,nSubItem);
	return ((float)atof(str));	
}
