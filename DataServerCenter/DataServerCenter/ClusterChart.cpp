
// PCAChart.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "DataServerCenter.h"
#include "PCAChart.h"
#include "Chart.h"
#include "SpectrumCompareDlg.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
// CClusterChart
using std::set;
using std::find;
using std::vector;
using std::map;
using std::multimap;
IMPLEMENT_DYNAMIC(CClusterChart, CWnd)

CClusterChart::CClusterChart()
{
	m_bDrawReverseX = false;  //��ͼ����
	m_bDrawReverseY = false;  //��ͼ����

	m_bShowMenum = true;
	m_bAllCurveSameBase = false;

	m_bHideUnselectCurve = false;
	m_bShowDataFlowMouse = true;  //ȡ����ʱ�Ƿ������ݸ��������ʾ
	m_bDrawLegend=true;    //�Ƿ����ͼ��

	m_crBackColor  =RGB(255,255,255);// RGB(192,192,192);//; //RGB();


	m_crGridColor = RGB(105,105,105); //RGB(120,120,120);
	m_crFrameColor =RGB(0,0,0); //RGB(255,255,255);
	m_crPlotAreaBackColor = RGB(255,255,255);
	m_RefSpecPCAColor = RGB(255,0,0);

	m_FakeColor = RGB(0,0,255);
	m_AnaColor = RGB(255,0,0);

	m_brushBack.CreateSolidBrush(m_crBackColor) ;
	m_brushGrid.CreateSolidBrush(m_crGridColor) ;

	m_brushPlotAreaBack.CreateSolidBrush(m_crPlotAreaBackColor) ;

	//m_CurveMap.RemoveAll();

	m_ptMaxPos.InitPoint(0.0,0.0);
	m_ptMinPos.InitPoint(0.0,0.0);
	m_ptZoomOffset.InitPoint(0.0,0.0);
	m_ptMoveOffset.InitPoint(0.0,0.0);

	m_rectClient.SetRectEmpty();
	m_rectPlot.SetRectEmpty();

	m_ptMargin.x = 30;
	m_ptMargin.y = 30;

	m_dVerticalFactor = 1.0;
	m_dHorizontalFactor = 1.0;
	m_iControlMode = CURVE_SELECT;
	m_bLeftButtonDown = false;
	m_iVerticalControlMode = false; //Ĭ����ˮƽ����ģʽ(ָ���ֲ���ģʽ���ƶ������Ź���)
	m_recMouseSelect.SetRectEmpty();
	m_pCurSelCurveObj = NULL;

	m_CurveNameMaxSize=0;//pDC->GetTextExtent(L"�پƿ�ͼ��");

	m_LegendLineLenth=30;

	m_SelectedCurveColor = RGB(255,0,0);  //ѡ��������ɫ

	m_ArrayCurveColor.Add(RGB(0,0,255));
	m_ArrayCurveColor.Add(RGB(128,0,128));
	m_ArrayCurveColor.Add(RGB(255,140,0));
	m_ArrayCurveColor.Add(RGB(0,255,0));
	m_ArrayCurveColor.Add(RGB(255,255,0));
	m_ArrayCurveColor.Add(RGB(0,0,0));

	m_FeatrueNum=0;
}

CClusterChart::~CClusterChart()
{

	CCurveObj * pCurveObject;
	CMapToCurveObj::iterator iter = m_FakeSpecCluster.begin();
	while(iter!= m_FakeSpecCluster.end())
	{
		pCurveObject = iter->second;
		pCurveObject->DestroyCurve();
		pCurveObject = NULL;
		++iter;
	}
	m_FakeSpecCluster.clear();

	iter = m_AnaSpecCluster.begin();
	while(iter!= m_AnaSpecCluster.end())
	{
		pCurveObject = iter->second;
		pCurveObject->DestroyCurve();
		pCurveObject = NULL;
		++iter;
	}
	m_AnaSpecCluster.clear();
}

BEGIN_MESSAGE_MAP(CClusterChart, CWnd)
	ON_WM_SIZE()
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEWHEEL()
	ON_WM_RBUTTONDOWN()
	ON_WM_CONTEXTMENU()
	ON_WM_LBUTTONDBLCLK()
	//}}AFX_MSG_MAP
	ON_UPDATE_COMMAND_UI_RANGE(9990,10015,OnUpdateMenuState)
	ON_WM_SETCURSOR()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CClusterChart message handlers

//##ModelId=48ACE0A6036B
BOOL CClusterChart::Create(DWORD dwStyle, const RECT &rect, CWnd *pParentWnd, UINT nID)
{
	BOOL result ;
	static CString className = AfxRegisterWndClass(CS_HREDRAW | CS_VREDRAW) ;

	result = CWnd::CreateEx(WS_THICKFRAME, 
		className, NULL, dwStyle, 
		rect.left, rect.top,rect.right-rect.left, rect.bottom-rect.top,
		pParentWnd->GetSafeHwnd(), (HMENU)nID) ;

	if (result != 0)
	{
		this->SetMargin();	
		InvalidateCtrl();
		this->DrawBackGnd();
		//this->DrawGrid();
	}
	return result ;
}
//##ModelId=48A14589009E
void CClusterChart::SetShowLegend(bool ShowLegend/*= true*/)
{
	m_bDrawLegend=ShowLegend;
	InvalidateChart();
}

BOOL CClusterChart::InvalidateCtrl()   //
{
	CClientDC dc(this) ;  


	m_dcPlot.DeleteDC();
	m_dcPlot.CreateCompatibleDC(&dc) ;

	m_bitmapPlot.DeleteObject();
	m_bitmapPlot.CreateCompatibleBitmap(&dc, m_nClientWidth, m_nClientHeight) ;
	m_pbitmapOldPlot = m_dcPlot.SelectObject(&m_bitmapPlot) ;

	this->InvalidateChart();
	return TRUE;
}

//##ModelId=48A1458801E4
void CClusterChart::OnSize(UINT nType, int cx, int cy) 
{
	CWnd::OnSize(nType, cx, cy);
	GetClientRect(m_rectClient) ;

	m_nClientHeight = m_rectClient.Height();
	m_nClientWidth = m_rectClient.Width();

	this->SetMargin(m_ptMargin.x,m_ptMargin.y);	//�����趨�߿�
	this->InvalidateCtrl();
}

//##ModelId=48A145880203
void CClusterChart::OnPaint() 
{
	CPaintDC dc(this); // device context for painting

	CDC memDC ;
	CBitmap memBitmap ;
	CBitmap* oldBitmap ; // bitmap originally found in CMemDC


	memDC.CreateCompatibleDC(&dc) ;
	memBitmap.CreateCompatibleBitmap(&dc, m_nClientWidth, m_nClientHeight) ;
	oldBitmap = (CBitmap *)memDC.SelectObject(&memBitmap) ;

	//CRect rc;
	//::GetClientRect(this->m_hWnd,&rc);

	//memDC.FillSolidRect(0,0,rc.Width(),rc.Height(),m_crBackColor); 

	if (memDC.GetSafeHdc() != NULL)
	{
		//memDC.BitBlt(0,0,m_nClientWidth, m_nClientHeight,&m_dcGrid, 0, 0, SRCCOPY);//��������������
		memDC.BitBlt(0, 0, m_nClientWidth, m_nClientHeight,&m_dcPlot, 0,0,SRCPAINT);  //���һ������SRCPAINTָ������DC���ݽ���OR����,����������
		switch(m_iControlMode)
		{
		case CURVE_GET_DATA:
			{	
				this->DrawGetDataCross(&memDC);
				break;
			}
		case CURVE_MOVE:
			{
				if(this->m_bLeftButtonDown)
				{
					if(m_sCurrentControlCurve.IsEmpty())
					{

					}
				}
				m_recMouseSelect.TopLeft() = m_recMouseSelect.BottomRight();
				break;

			}
		case CURVE_ZOOM_OUT:
		case CURVE_ZOOM_IN:
			{
				this->DrawZoomRect(&memDC,m_recMouseSelect);
				break;
			}
		case CURVE_SELECT:
			{
				//CPoint point;
				//GetCursorPos(&point);
				//this->ScreenToClient(&point);
				//this->DrawSelectFocus(&memDC,point);
				break;
			}
		case CURVE_FEATURE_SELECT:
			{
				//������������ĳ��ܽ�
			}
		case CURVE_PEAKAREA_CAL:
			{

			}
		case CURVE_LABEL:
			{	
				this->DrawGetDataCross(&memDC);
				break;
			}
		}
		dc.BitBlt(0,0,m_nClientWidth, m_nClientHeight,&memDC,0,0,SRCCOPY); //�����ڴ�DC����Ļ
	}
	memDC.SelectObject(oldBitmap) ;
	memBitmap.DeleteObject(); 
	memDC.DeleteDC(); 
}
//##ModelId=48A145890129

void CClusterChart::DrawLegend()
{
	DrawLegend(m_dcPlot,m_rectPlot);
}

void CClusterChart::DrawLegend(CDC &m_dcPlot,CRect m_rectPlot)
{
	m_dcPlot.SetTextAlign (TA_LEFT|TA_TOP);//��ʾ���ַ����������
	//COLORREF LegendColor;

	CSize CurveNameSize;

	int i=0;

	m_dcPlot.SetBkColor (m_crPlotAreaBackColor) ;

	//���
	m_dcPlot.FillRect(CRect(m_rectPlot.right+10 ,m_rectPlot.top,m_rectPlot.right+63+m_CurveNameMaxSize.cx,m_rectPlot.top+(m_CurveNameMaxSize.cy+5)*2),&m_brushPlotAreaBack);
	int r = 5;

    //���ο�ͼ��ͼ��
	CPen* oldPen;
	CPen LegendPen1(PS_SOLID,2,m_FakeColor);

	oldPen = m_dcPlot.SelectObject(&LegendPen1);

	m_dcPlot.Ellipse(m_rectPlot.right+10,m_rectPlot.top+i*(CurveNameSize.cy+5)+5+3,
		m_rectPlot.right+20,m_rectPlot.top+i*(CurveNameSize.cy+5)+5+10+3);

	m_dcPlot.SelectObject(&oldPen);
	m_dcPlot.SetTextColor(m_FakeColor);
	CurveNameSize = m_dcPlot.GetTextExtent(L"�پƿ�ͼ��");

	m_dcPlot.SelectObject(&oldPen);
	m_dcPlot.SetTextColor(m_FakeColor);
	m_dcPlot.TextOut(m_rectPlot.right+23,m_rectPlot.top+i*(CurveNameSize.cy+5)+5,L"�پƿ�ͼ��");
	++i;

	//LegendPen.DeleteObject();

	CPen LegendPen2(PS_SOLID,2,m_AnaColor);

	oldPen = m_dcPlot.SelectObject(&LegendPen2);
	m_dcPlot.Ellipse(m_rectPlot.right+10,m_rectPlot.top+i*(CurveNameSize.cy+5)+5+3,
		m_rectPlot.right+20,m_rectPlot.top+i*(CurveNameSize.cy+5)+5+10+3);

	m_dcPlot.SelectObject(&oldPen);
	m_dcPlot.SetTextColor(m_AnaColor);

	CurveNameSize = m_dcPlot.GetTextExtent(L"������ͼ��");

	m_dcPlot.SelectObject(&oldPen);
	m_dcPlot.SetTextColor(m_AnaColor);
	m_dcPlot.TextOut(m_rectPlot.right+23,m_rectPlot.top+i*(CurveNameSize.cy+5)+5,L"������ͼ��");

	m_dcPlot.SelectObject(&oldPen);
	InvalidateRect(m_rectClient);
}

void CClusterChart::DrawBackGnd()
{
	DrawBackGnd(m_dcPlot,m_rectPlot);
}

void CClusterChart::DrawBackGnd(CDC &m_dcPlot,CRect m_rectPlot)
{
	m_dcPlot.FillRect(m_rectPlot, &m_brushPlotAreaBack);
}

void CClusterChart::DrawUnPlotArea()
{
	DrawUnPlotArea(m_dcPlot,m_rectPlot,m_rectClient);
}

void CClusterChart::DrawUnPlotArea(CDC &m_dcPlot,CRect m_rectPlot,CRect m_rectClient)
{
	bool bNoCurveSelect = true;
	CString str;
	int i=0,j=0,k=0;
	COLORREF textColor = m_crGridColor;
	COLORREF girdColor = m_crGridColor;
	COLORREF frameColor = m_crFrameColor;
	CSize textSize;
	CPen* oldPen;
	CCurveObj* curveObject = NULL;

	m_dcPlot.FillRect(CRect(m_rectClient.left, m_rectClient.top, m_rectClient.right, m_rectPlot.top), &m_brushBack);
	m_dcPlot.FillRect(CRect(m_rectPlot.right, m_rectPlot.top, m_rectClient.right, m_rectClient.bottom), &m_brushBack);
	m_dcPlot.FillRect(CRect(m_rectClient.left, m_rectPlot.bottom, m_rectPlot.right, m_rectClient.bottom),&m_brushBack);
	m_dcPlot.FillRect(CRect(m_rectClient.left, m_rectPlot.top, m_rectPlot.left, m_rectPlot.bottom), &m_brushBack);

	m_dcPlot.SetTextColor (textColor);
	m_dcPlot.SetBkColor (m_crBackColor) ;


	CPen frameSolidPen(PS_SOLID,1,frameColor);
	oldPen = m_dcPlot.SelectObject(&frameSolidPen);


	//���������

	m_dcPlot.MoveTo(m_rectPlot.TopLeft());
	m_dcPlot.LineTo(m_rectPlot.left,m_rectPlot.bottom);
	m_dcPlot.LineTo(m_rectPlot.right,m_rectPlot.bottom);
	m_dcPlot.LineTo(m_rectPlot.right,m_rectPlot.top);
	m_dcPlot.LineTo(m_rectPlot.TopLeft());
	m_dcPlot.SelectObject(oldPen) ;

	//ȡ�����귶Χ��׼����������̶�
	this->GetMaxDataRect();

	CFloatPoint point = m_ptZoomOffset ;

	m_dVerticalFactor = m_rectPlot.Height()/(point.m_PosY+0.000000001);
	m_dHorizontalFactor = m_rectPlot.Width()/(point.m_PosX+0.000000001);

	//����������̶���,10�ȷ�,�߳�10����
	int _iPreciseX = 2;
	int _iPreciseY = 3;
	if(m_pCurSelCurveObj)//������ʾ����,��Ҫ��X��
	{
		_iPreciseX = this->FloatDataPrecise(m_pCurSelCurveObj->GetTimeBaseFactor());
		_iPreciseY = this->FloatDataPrecise(m_ptMaxPos.m_PosY);
		if(_iPreciseY>4)
			_iPreciseY = 4;
	}

	int nVerScaleLines = CluserNum+2;  
	
	double nScaleHeightY = point.m_PosY/11;
	
	double dGridHeight = m_rectPlot.Height()/nVerScaleLines;
	
	double tempY = 0.0;	
	double tempY2=0.0;

	CPen girdSolidPen(PS_SOLID, 1, girdColor);
	oldPen = m_dcPlot.SelectObject(&girdSolidPen);

	m_dcPlot.SetTextAlign(TA_LEFT|TA_TOP) ;


	int maxX = (int)m_ptMaxPos.m_PosX;
	int maxY = (int)m_ptMaxPos.m_PosY;
	int minX = (int)m_ptMinPos.m_PosX;
	int minY = (int)m_ptMinPos.m_PosY;

	//��Y��
	for(int i = maxY;i>=minY;--i)
	{
		str.Format(L"%d",i); 
		textSize = m_dcPlot.GetTextExtent(str); 
		//����ˮƽ����С���
		tempY = (double)m_rectPlot.top + (double)m_rectPlot.Height()*(m_ptMaxPos.m_PosY - (double)i)/(m_ptMaxPos.m_PosY-m_ptMinPos.m_PosY);	

		m_dcPlot.MoveTo(m_rectPlot.left,(int)tempY);
		m_dcPlot.LineTo(m_rectPlot.left+6,(int)tempY);

		m_dcPlot.SetTextAlign (TA_RIGHT|TA_TOP) ;
		m_dcPlot.TextOut (m_rectPlot.left-textSize.cy/4, (int)tempY-textSize.cy/2, str) ;
	}

	int nHorScaleLines = (m_FakeSpecCluster.size()+m_AnaSpecCluster.size())/10 + 1;  

	int nHorScale = 10;
	double dGridWidth = m_rectPlot.Width()/nHorScaleLines;
	int tempX = 0;	
	
	//��X��
	for(int i= minX;i<=maxX;i+=10)
	{
		tempX = m_rectPlot.left + (double)m_rectPlot.Width()*((double)i - m_ptMinPos.m_PosX)/(m_ptMaxPos.m_PosX - m_ptMinPos.m_PosX);	

		m_dcPlot.MoveTo(tempX,m_rectPlot.bottom);
		m_dcPlot.LineTo(tempX,m_rectPlot.bottom+6);

		str.Format (L"%d", i) ;
		textSize = m_dcPlot.GetTextExtent(str); 
		m_dcPlot.TextOut (tempX+textSize.cx/2,m_rectPlot.bottom+5, str);	

		/*if(j<nHorScaleLines)
		{
			for(k =0;k<nHorScale;k++)  //����С�ı�ȳ�
			{
				for(i = m_rectPlot.bottom-2; i<m_rectPlot.bottom; i++)
				{
					m_dcPlot.SetPixel(tempX+(dGridWidth/nHorScale)*k,i,frameColor) ;
				}
			}
		}*/
	}
	CString XUnit;
	CString YUnit = L"������(���0��ʾ����û�о۵���һ��)";

	XUnit.Format(L"ͼ�ױ��(1-%dΪ�پ���ͼ��%d-%dΪ���з�����ͼ��)",m_FakeSpecCluster.size(),m_FakeSpecCluster.size()+1,m_FakeSpecCluster.size()+m_AnaSpecCluster.size());

	CSize szYUnit = m_dcPlot.GetTextExtent(YUnit);
	CSize szXUnit = m_dcPlot.GetTextExtent(XUnit);

	LOGFONT lf;                                         //��������ṹ
	lf.lfWeight=0;                                      //�������=10
	lf.lfHeight=0;	                                    //����߶�(��ת���������)=56
	lf.lfWidth=0;                                       //������(��ת�������߶�)=20
	lf.lfUnderline=FALSE;                               //���»���
	lf.lfStrikeOut=FALSE;                               //��ɾ����
	lf.lfItalic=FALSE;                                  //��б��
	lf.lfEscapement=900;                                //������ʾ�Ƕ�=90��
	lf.lfCharSet=DEFAULT_CHARSET;                       //ʹ��ȱʡ�ַ���
	CFont myLogFont;                                    //�����������
	myLogFont.CreateFontIndirect(&lf);                  //�����߼�����
	CFont *pOldFont=m_dcPlot.SelectObject(&myLogFont);  //ѡ���豸������

	m_dcPlot.TextOut(2,m_rectClient.Height()/2-szYUnit.cx/2,YUnit);

	myLogFont.DeleteObject();
	lf.lfEscapement = 0;
	myLogFont.CreateFontIndirect(&lf);                  //�����߼�����
	m_dcPlot.SelectObject(&myLogFont);                  //ѡ���豸������

	m_dcPlot.TextOut(m_rectClient.Width()/2+szXUnit.cx/2,m_rectClient.Height()-20,XUnit);

	m_dcPlot.SelectObject(pOldFont);                    //��myFont���豸�����з���
	myLogFont.DeleteObject();

	m_ptZoomOffset.InitPoint(-10000000.0,-10000000.0);  //�˴�����������ʼ��,
	m_ptMoveOffset.InitPoint(-10000000.0,-10000000.0);
	InvalidateRect(m_rectClient);
}
//##ModelId=48A14588034B
void CClusterChart::InvalidateChart()
{
	if( m_dcPlot.GetSafeHdc()==NULL )
		return ;
	CString _curveName;
	CCurveObj* curveObject = NULL;
	//������
	DrawBackGnd();
	//���پƹ��׾�����
	CMapToCurveObj::iterator iter = m_FakeSpecCluster.begin();

	while(iter!=m_FakeSpecCluster.end())
	{
		iter->second->SetCurveColor(m_FakeColor);
		iter->second->m_cPlotRect = m_rectPlot;
		iter->second->DrawPoint(&m_dcPlot,m_rectPlot,CIRCLE);//���Ƶ�
		++iter;
	}
	//���������׾�����
	iter = m_AnaSpecCluster.begin();
	while(iter!=m_AnaSpecCluster.end())
	{
		iter->second->SetCurveColor(m_AnaColor);
		iter->second->m_cPlotRect = m_rectPlot;
		iter->second->DrawPoint(&m_dcPlot,m_rectPlot,CIRCLE);//���Ƶ�
		++iter;
	}
	DrawUnPlotArea();
	//��ͼ��
	if(m_bDrawLegend)
	{
		DrawLegend();
	}
	InvalidateRect(m_rectClient);
}

void CClusterChart::PrintChart(CDC* pDC, CPrintInfo* pInfo)//�ṩ�Ĵ�ӡ�ӿ�
{
	CDC MemDC; //���ȶ���һ����ʾ�豸����
	CBitmap MemBitmap;//����һ��λͼ����

	//���������Ļ��ʾ���ݵ��ڴ���ʾ�豸
	MemDC.CreateCompatibleDC(NULL);

	//����һ������Ļ��ʾ���ݵ�λͼ
	MemBitmap.CreateCompatibleBitmap(pDC,m_rectClient.Width(),m_rectClient.Height());

	//��λͼѡ�뵽�ڴ���ʾ�豸��
	//ֻ��ѡ����λͼ���ڴ���ʾ�豸���еط���ͼ������ָ����λͼ��
	CBitmap *pOldBit=MemDC.SelectObject(&MemBitmap);

	//���ñ���ɫ��λͼ����ɾ�����ɫ��Ϊ����
	MemDC.FillSolidRect(0,0,m_rectClient.Width(),m_rectClient.Height(),RGB(255,255,255));

	CString _curveName;
	CCurveObj* curveObject = NULL;

	//������
	DrawBackGnd(MemDC,m_rectClient);
	//������
	CMapToCurveObj::iterator iter = m_FakeSpecCluster.begin();
	while(iter!=m_FakeSpecCluster.end())
	{
		iter->second->SetCurveColor(m_FakeColor);
		iter->second->m_cPlotRect = m_rectPlot;
		iter->second->DrawPoint(&MemDC,m_rectPlot,CIRCLE);//���Ƶ�
		++iter;
	}
	//���������׾�����
	iter = m_AnaSpecCluster.begin();
	while(iter!=m_AnaSpecCluster.end())
	{
		iter->second->SetCurveColor(m_AnaColor);
		iter->second->m_cPlotRect = m_rectPlot;
		iter->second->DrawPoint(&MemDC,m_rectPlot,CIRCLE);//���Ƶ�
		++iter;
	}
	DrawUnPlotArea(MemDC,m_rectPlot,m_rectClient);
	//��ͼ��
	if(m_bDrawLegend)
	{
		DrawLegend(MemDC,m_rectPlot);
	}

	pDC->StretchBlt(0,0,pInfo->m_rectDraw.Width(),pInfo->m_rectDraw.Height(),&MemDC,0,0,m_rectClient.Width(),m_rectClient.Height(),SRCCOPY);

	//��ͼ��ɺ������
	MemBitmap.DeleteObject();
	MemDC.DeleteDC();

}
//��ָ��index�����߶�������������
BOOL CClusterChart::AppendPoint(CString curveName,double Posx,double Posy,BOOL Fake)
{
	CCurveObj* curveObject = NULL;
	CMapToCurveObj::iterator iter_map;
	
	if(Fake)
	{
		iter_map= m_FakeSpecCluster.find(curveName);
		if( iter_map != m_FakeSpecCluster.end())
		{
			CFloatPoint point;
			point.InitPoint(Posx,Posy);
			curveObject = iter_map->second;
			curveObject->AppendNewPoint(point);
			return TRUE;
		}
	}
	else
	{
		iter_map = m_AnaSpecCluster.find(curveName);
		if( iter_map != m_AnaSpecCluster.end())
		{
			CFloatPoint point;
			point.InitPoint(Posx,Posy);
			curveObject = iter_map->second;
			curveObject->AppendNewPoint(point);
			return TRUE;
		}
	}
	return FALSE;
}

//�����Ƿ��������߹���һ������ϵ,Ĭ����ÿһ���������Լ�������ϵ�л�ͼ��trueΪ�������߹���һ������ϵ

void CClusterChart::OnSize(int cx,int cy)
{
	if(this->GetSafeHwnd())
	{
		this->SetWindowPos(NULL,0,0,cx,cy,SWP_SHOWWINDOW);  //���ô˺���������WM_SIZE��Ϣ
	}
}

//���ö�̬ʱ������Y������������,�����ڶ�̬��ͼ֮ǰ�趨�ô˲���
void CClusterChart::SetRealTimeBaseCurveDataRange(double lower,double upper)  
{
	CFloatPoint point(lower,upper);
	CCurveObj::SetRealTimeBaseCurveDataRange(point);
}
//�����µ����߶���
//##ModelId=48A14588031D
CCurveObj& CClusterChart::CreateNewPoint(CString curveName,bool timeBaseFlag ,bool Fake) //����һ���µĵ�
{
	CCurveObj* curveObject = NULL;
	if((m_CurveNameMaxSize.cx == 0)&&(m_CurveNameMaxSize.cy == 0))
	{
		CDC *pDC = GetDC();
		m_CurveNameMaxSize=pDC->GetTextExtent(L"�پƿ�ͼ��");
		ReleaseDC(pDC);
	}

	if(Fake)
	{
		//����������
		if(m_FakeSpecCluster.find(curveName) == m_FakeSpecCluster.end())
		{
			curveObject = new CCurveObj(m_rectPlot);
			if(curveObject)
			{
				curveObject->SetTimeBaseFlag(timeBaseFlag);
				m_FakeSpecCluster.insert(make_pair(curveName,curveObject));

			}
		}
	}
	else
	{
		//����������
		if(m_AnaSpecCluster.find(curveName) == m_AnaSpecCluster.end())
		{
			curveObject = new CCurveObj(m_rectPlot);
			if(curveObject)
			{
				curveObject->SetTimeBaseFlag(timeBaseFlag);
				m_AnaSpecCluster.insert(make_pair(curveName,curveObject));

			}
		}

	}
	return *curveObject;
}
//ɾ��ѡ������
bool CClusterChart::DeleteCurveSelected()
{
	return true;
}
//##ModelId=48A14588033C
BOOL CClusterChart::DestroyMapCurve(CMapStringToOb &m_CurveMap)
{
	//BOOL rc = FALSE;
	CCurveObj* curveObject = NULL;
	CString curveName;

	POSITION pos = m_CurveMap.GetStartPosition();	
	while(pos)
	{
		m_CurveMap.GetNextAssoc(pos,curveName, (CObject*&)curveObject);
		if(curveObject)
		{
			curveObject->DestroyCurve();
			delete curveObject;
			curveObject = NULL;
			m_CurveMap.RemoveKey(curveName);
		}
	}   
	return TRUE;
}
BOOL CClusterChart::DestroyMapCurve(CMultiMapToCurveObj &RefPCATotalMap)
{
	CMultiMapToCurveObj::iterator iter = RefPCATotalMap.begin();
	CCurveObj* curveObject = NULL;
	while(iter != RefPCATotalMap.end())
	{
		curveObject = iter->second;
		curveObject->DestroyCurve();
		//delete [] pArray;
		curveObject = NULL;
		++iter;
	}
	RefPCATotalMap.clear();
	return TRUE;
}
BOOL CClusterChart::DestroyArrayCurve(CMultiMapToFloatPtArray &RefPCATotalArray)
{
	CMultiMapToFloatPtArray::iterator iter = RefPCATotalArray.begin();
	CFloatPoint* pArray = NULL;
	while(iter != RefPCATotalArray.end())
	{
		pArray = iter->second;
		delete [] pArray;
		pArray = NULL;
		++iter;
	}
	RefPCATotalArray.clear();
	return TRUE;
}
BOOL CClusterChart::DestroyCurve(CString curveName/* = _T("\0")*/)
{
	BOOL rc = FALSE;
	CCurveObj* curveObject = NULL;

	CMapToCurveObj::iterator iter_map = m_FakeSpecCluster.begin();
	while(iter_map!=m_FakeSpecCluster.end())
	{
		curveObject = iter_map->second;
		if(curveObject)
		{
			curveObject->DestroyCurve();
			delete curveObject;
			curveObject = NULL;
		}
		++iter_map;
	}
	m_FakeSpecCluster.clear();

    iter_map = m_AnaSpecCluster.begin();
	while(iter_map!=m_AnaSpecCluster.end())
	{
		curveObject = iter_map->second;
		if(curveObject)
		{
			curveObject->DestroyCurve();
			delete curveObject;
			curveObject = NULL;
		}
		++iter_map;
	}
	m_AnaSpecCluster.clear();
	m_ptMaxPos.InitPoint(0.0,0.0);
	m_ptMinPos.InitPoint(0.0,0.0);
	m_ptZoomOffset.InitPoint(0.0,0.0);
	m_ptMoveOffset.InitPoint(0.0,0.0);
	m_CurveNameMaxSize=0;
	this->SetMargin();	
	InvalidateCtrl();
	this->InvalidateChart();
	return rc;
}
//##ModelId=48A145890010
void CClusterChart::SetCurveColor(CString curveName,COLORREF color/* = RGB(255,0,0)*/,int lineWidth/*=1*/)
{
}
void CClusterChart::SelectCurve(CString curveName/* = _T("\0")*/)
{
}

bool CClusterChart::AddSelectedCurve(CString curveName)
{
	return 1;
}


void CClusterChart::SetMargin(int xMargin/*=30*/,int yMargin/* = 20*/)
{
	if(xMargin>0 && yMargin >0 )
	{
		//�����޸���ͼ����С
		if(m_bDrawLegend&&m_CurveNameMaxSize.cx>0)
		{
			m_rectPlot = m_rectClient;
			m_ptMargin = CPoint(xMargin,yMargin);
			m_rectPlot.DeflateRect(m_ptMargin.x*2+m_CurveNameMaxSize.cx/2,m_ptMargin.y);  
			m_rectPlot.OffsetRect(m_ptMargin.x-m_CurveNameMaxSize.cx/2,-m_ptMargin.y/2);//�����Ļ�ͼ����
		}
		else
		{
			m_rectPlot = m_rectClient;
			m_ptMargin = CPoint(xMargin,yMargin);
			m_rectPlot.DeflateRect(m_ptMargin.x*2,m_ptMargin.y);  
			m_rectPlot.OffsetRect(m_ptMargin.x,-m_ptMargin.y/2);//�����Ļ�ͼ����
		}
		this->InvalidateChart();
	}
}
//���ߵ������ԣ���ѡ�е�����������
//##ModelId=48A145890001
bool CClusterChart::HitTest(CPoint point)
{
	return 1;
}
//##ModelId=48A14589002E
void CClusterChart::MoveCurve(CString curveName /*= _T("\0")*/,int moveType /*= MOVE_RESET*/,double percent /*= 5*/)
{
	if(m_sCurrentControlCurve != curveName)
		m_sCurrentControlCurve = curveName;
	if(curveName.IsEmpty())  //�������߶������ƶ�����
	{
		CString _curveName;
		CCurveObj* curveObject = NULL;

		CMapToCurveObj::iterator iter1 = m_FakeSpecCluster.begin();
		while(iter1!= m_FakeSpecCluster.end())
		{
			curveObject = iter1->second;
			if(curveObject)
			{
				switch(moveType)
				{
				case CURVE_MOVE_UP:
					{
						curveObject->Move(0,percent);
						break;
					}
				case CURVE_MOVE_DOWN:
					{
						curveObject->Move(0,-percent);
						break;
					}
				case CURVE_MOVE_LEFT:
					{
						curveObject->Move(percent,0);
						break;
					}
				case CURVE_MOVE_RIGHT:
					{
						curveObject->Move(-percent,0);
						break;
					}
				case CURVE_RESET:
					{
						curveObject->ResetCurve(m_ptMaxPosConst,m_ptMinPosConst);
						break;
					}
				default:
					break;
				}
			}
			++iter1;
		}
		CMapToCurveObj::iterator iter2 = m_AnaSpecCluster.begin();
		while(iter2!= m_AnaSpecCluster.end())
		{
			curveObject = iter2->second;
			if(curveObject)
			{
				switch(moveType)
				{
				case CURVE_MOVE_UP:
					{
						curveObject->Move(0,percent);
						break;
					}
				case CURVE_MOVE_DOWN:
					{
						curveObject->Move(0,-percent);
						break;
					}
				case CURVE_MOVE_LEFT:
					{
						curveObject->Move(percent,0);
						break;
					}
				case CURVE_MOVE_RIGHT:
					{
						curveObject->Move(-percent,0);
						break;
					}
				case CURVE_RESET:
					{
						curveObject->ResetCurve(m_ptMaxPosConst,m_ptMinPosConst);
						break;
					}
				default:
					break;
				}
			}
			++iter2;
		}
	}
	this->InvalidateChart();
}
//��������
void CClusterChart::ZoomCurve(CString curveName /*= _T("\0")*/,int zoomType /*= ZOOM_RESET*/,double percent /*= 5*/)
{
}
void CClusterChart::IndividualCurveMove(CString curveName,int moveType,double percent)
{
}
void CClusterChart::IndividualCurveZoom(CString curveName,int zoomType,double percent)
{
}
void CClusterChart::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	this->SetFocus();
	if(m_rectPlot.PtInRect(point))
	{
		if(!m_bLeftButtonDown)
		{
			m_recMouseSelect.TopLeft() = point;
			m_Line.m_Begin=point;


			CMapToCurveObj::iterator iter = m_FakeSpecCluster.begin();
			CString _curveName;
			CCurveObj* curveObject = iter->second;

			if(curveObject)
			{
				m_BeginPoint = curveObject->Position2Data(m_Line.m_Begin,m_Line.m_Begin); //����ʼ������ͼ����

				m_bLeftButtonDown = true;
			}
		}
	}
	CWnd::OnLButtonDown(nFlags, point);
}

//##ModelId=48A145880233
void CClusterChart::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	if(m_rectPlot.PtInRect(point))
	{
		//����Ҫ������µĲ���m_iControlMode == CURVE_MOVE |||| m_iControlMode == CURVE_SELECT 
		//||m_iControlMode == CURVE_FEATURE_SELECT ||m_iControlMode == CURVE_PEAKAREA_CAL 
		if( m_iControlMode == CURVE_GET_DATA || m_iControlMode == CURVE_LABEL||m_iControlMode == CURVE_FEATURE_SELECT 
			||m_iControlMode == CURVE_PEAKAREA_CAL  )
		{
			switch(m_iControlMode)
			{
			case CURVE_GET_DATA:
				{
					m_ptCrossCenter = point;
					break;
				}
			case CURVE_FEATURE_SELECT:
				{
					m_Line.m_End = point;
					break;
				}
			case CURVE_PEAKAREA_CAL:
				{
					m_Line.m_End = point;
					CMapToCurveObj::iterator iter = m_FakeSpecCluster.begin();
					CCurveObj* curveObject = iter->second;
					CFloatPoint _PointEnd;
					if(curveObject)
					{
						m_EndPoint = curveObject->Position2Data(m_Line.m_End,m_Line.m_End);  //������������ͼ����
					}
					break;
				}
			case CURVE_LABEL:
				{
					m_ptCrossCenter = point;
					break;
				}
			}
			this->InvalidateRect(m_rectPlot);
		}
		//��Ҫ������µĲ���
		else if(m_bLeftButtonDown &&(m_iControlMode == CURVE_MOVE || m_iControlMode == CURVE_SELECT 
			|| m_iControlMode == CURVE_ZOOM_OUT|| m_iControlMode == CURVE_ZOOM_IN))
		{
			switch(m_iControlMode)
			{

			case CURVE_SELECT:
				{
					break;
				}
			case CURVE_MOVE:
				{	
					m_recMouseSelect.BottomRight() = point;
					if(m_sCurrentControlCurve.IsEmpty())
					{

						CString curveName;
						CCurveObj* curveObject = NULL;
						CMapToCurveObj::iterator iter1 = m_FakeSpecCluster.begin();
						while(iter1!= m_FakeSpecCluster.end())
						{
							curveObject = iter1->second;
							if(curveObject)
							{
								this->IndividualCurveControl(curveObject,m_iControlMode,m_recMouseSelect);
							}
							++iter1;
						}
						CMapToCurveObj::iterator iter = m_AnaSpecCluster.begin();
						while(iter!= m_AnaSpecCluster.end())
						{
							curveObject = iter->second;
							if(curveObject)
							{
								this->IndividualCurveControl(curveObject,m_iControlMode,m_recMouseSelect);
							}
							++iter;
						}
					}
					m_recMouseSelect.TopLeft() = m_recMouseSelect.BottomRight();
					break;
				}		
			case CURVE_ZOOM_OUT:
				{
					m_recMouseSelect.BottomRight() = point;
					OnPaint();
					break;
				}
			case CURVE_ZOOM_IN:
				{
					m_recMouseSelect.BottomRight() = point;
					break;
				}
			}
			this->InvalidateRect(m_rectPlot);
		}
	}
	CWnd::OnMouseMove(nFlags, point);
}

void CClusterChart::OnLButtonUp(UINT nFlags, CPoint point) 
{
	if(m_rectPlot.PtInRect(point) )
	{
		if(m_bLeftButtonDown)
		{
			m_recMouseSelect.BottomRight() = point;

			m_Line.m_End = point;

			CString curveName;
			CCurveObj* curveObject = NULL;

			CMapToCurveObj::iterator iter1 = m_FakeSpecCluster.begin();
			curveObject = iter1->second;

			if(curveObject)
			{
				curveObject->Position2Data(point);
			}

			if(m_sCurrentControlCurve.IsEmpty())
			{
				CString curveName;
				CCurveObj* curveObject = NULL;
				CMapToCurveObj::iterator iter1 = m_FakeSpecCluster.begin();
				while(iter1!=m_FakeSpecCluster.end())
				{
					curveObject = iter1->second;
					if(curveObject)
					{
						this->IndividualCurveControl(curveObject,m_iControlMode,m_recMouseSelect);
					}
					++iter1;
				}
				CMapToCurveObj::iterator iter = m_AnaSpecCluster.begin();
				while(iter!= m_AnaSpecCluster.end())
				{
					curveObject = iter->second;
					if(curveObject)
					{
						this->IndividualCurveControl(curveObject,m_iControlMode,m_recMouseSelect);
					}
					++iter;
				}
			}

			if(m_iControlMode == CURVE_LABEL)
			{
				m_SetLabelPoint.insert(point);

			}

			this->InvalidateChart();
			m_recMouseSelect.SetRectEmpty();
		}	
	}
	m_bLeftButtonDown = false;
	CWnd::OnLButtonUp(nFlags, point);
}


BOOL CClusterChart::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt) 
{
	CString curveName;
	CCurveObj* curveObject = NULL;
	CMapToCurveObj::iterator iter1 = m_FakeSpecCluster.begin();
	while(iter1!=m_FakeSpecCluster.end())
	{
		curveObject = iter1->second;
		if(curveObject)
		{
			IndividualCurveWheelContol(curveObject,m_iControlMode,m_iVerticalControlMode,zDelta,pt);
		}
		++iter1;
	}
	CMapToCurveObj::iterator iter = m_AnaSpecCluster.begin();
	while(iter!= m_AnaSpecCluster.end())
	{
		curveObject = iter->second;
		if(curveObject)
		{
			IndividualCurveWheelContol(curveObject,m_iControlMode,m_iVerticalControlMode,zDelta,pt);
		}
		++iter;
	}

	this->InvalidateChart();
	return CWnd::OnMouseWheel(nFlags, zDelta, pt);
}

void CClusterChart::IndividualCurveWheelContol(CCurveObj* curveObj/* = NULL*/,int controlMode/* = CURVE_MOVE*/,bool verticalControl/* = false*/,int zDelta/* = 0*/,CPoint point)
{
	if(curveObj == NULL)
		return ;
	switch(controlMode)
	{
	case CURVE_SELECT:
	case CURVE_MOVE:
		{
			if(!verticalControl)  //��ֱ�ƶ�
			{
				if(zDelta>0)
					curveObj->Move(2,0);
				else
					curveObj->Move(-2,0);
			}
			else//ˮƽ�ƶ�
			{
				if(zDelta>0)
					curveObj->Move(0,2);
				else
					curveObj->Move(0,-2);
			}
			break;
		}
	case CURVE_ZOOM_OUT:
	case CURVE_ZOOM_IN:
		{
			if(!verticalControl)  //��ֱ����
			{
				if(zDelta>0)
					curveObj->ZoomIn(2,0);
				else
					curveObj->ZoomIn(-2,0);
			}
			else  //ˮƽ����
			{
				if(zDelta>0)
					curveObj->ZoomIn(0,2);
				else
					curveObj->ZoomIn(0,-2);
			}
			break;
		}
	case CURVE_GET_DATA:
		{
			CPoint pt = point;
			CRect rect = m_rectPlot;
			this->ClientToScreen(&rect);
			if(rect.PtInRect(pt))
			{
				if(verticalControl)
				{
					if(zDelta>0)
						pt.Offset(0,1);
					else
						pt.Offset(0,-1);
				}
				else
				{
					if(zDelta>0)
						pt.Offset(1,0);
					else
						pt.Offset(-1,0);
				}
				SetCursorPos(pt.x,pt.y);
			}
			break;
		}
	}
}

void CClusterChart::IndividualCurveControl(CCurveObj* curveObj/* = NULL*/,int controlMode /*= CURVE_MOVE*/,CRect controlRect)
{	
	if(curveObj == NULL)
		return ;

	switch(controlMode)
	{
	case CURVE_SELECT:
		{
			break;
		}
	case CURVE_MOVE:
		{
			curveObj->MouseSelectMove(controlRect);  //ues mouse to CURVE_MOVE the curve
			break;
		}
	case CURVE_ZOOM_IN:
		{
			if(controlRect.TopLeft() == controlRect.BottomRight()) //����ǵ������,��Ŵ�̶�������
			{
				controlRect.InflateRect(150,150);
			}
			curveObj->MouseSelectZoomIn(controlRect); //use mouse to ZoomIn the curve
			break;
		}
	case CURVE_ZOOM_OUT:
		{
			if(controlRect.TopLeft() == controlRect.BottomRight()) //����ǵ������,����С�̶�������
			{
				curveObj->ZoomOut(5,5);//
			}
			else
			{
				curveObj->ZoomOut((double)abs(controlRect.Width())/(double)abs(m_rectPlot.Width())*100.0,(double)abs(controlRect.Height())/(double)abs(m_rectPlot.Height())*100.0);
			}
			break;
		}
	}
	Invalidate();
}
//##ModelId=48A14589005E
BOOL CClusterChart::GetMaxDataRect(CString curveName/*=_T("\0")*/)  //�������������������������ֵ
{	
	BOOL rc = FALSE;
	CCurveObj* curveObject = NULL;

	if(curveName.IsEmpty())  //
	{
		CString _curveName;
		CMapToCurveObj::iterator iter_map = m_FakeSpecCluster.begin();
		if(iter_map!= m_FakeSpecCluster.end())
		{
			curveObject = iter_map->second;
			if(curveObject)
			{
				curveObject->GetDrawCurvePar(m_ptMaxPos,m_ptMinPos,m_ptMoveOffset,m_ptZoomOffset);

				//����ǽ������ţ���ֱ�ӷ���
				if(m_iControlMode == CURVE_ZOOM_IN ||m_iControlMode == CURVE_ZOOM_IN_V||
					m_iControlMode == CURVE_ZOOM_IN_H||m_iControlMode == CURVE_ZOOM_OUT||
					m_iControlMode == CURVE_ZOOM_OUT_V||m_iControlMode == CURVE_ZOOM_OUT_H)
				{

				}
				else
				{

				}
			}
		}
		else
		{

		}
	}
	else
	{

	}
	return rc;
}
int CClusterChart::ReCalculateAllCurveMaxMin()
{
	CCurveObj* curveObject = NULL;

	m_ptMaxPos.InitPoint(-10000000000.0,-10000000000.0);//��ǰ����
	m_ptMinPos.InitPoint(10000000000.0,10000000000.0);//��ǰ��С��

	CFloatPoint MaxPos;
	CFloatPoint MinPos;

	CDC *pDc=GetDC();
	CSize CurveNameSize;

	//�Ա�ͼ��PCA
	CString _curveName;
	CMapToCurveObj::iterator iter = m_FakeSpecCluster.begin();
	while(iter!=m_FakeSpecCluster.end())
	{
		curveObject = iter->second;
		if(curveObject)
		{
			curveObject->GetDrawCurvePar(MaxPos,MinPos,m_ptMoveOffset,m_ptZoomOffset);
			this->CalculateMaxPoint(m_ptMaxPos,&MaxPos);
			this->CalculateMinPoint(m_ptMinPos,&MinPos);

			//��������ͼ����϶
			CurveNameSize = pDc->GetTextExtent(_curveName);
			m_CurveNameMaxSize.cy=m_CurveNameMaxSize.cy>CurveNameSize.cy?m_CurveNameMaxSize.cy:CurveNameSize.cy;
		}
		++iter;
	}
	//�ο�ͼ��
    iter = m_AnaSpecCluster.begin();
	while(iter!=m_AnaSpecCluster.end())
	{
		curveObject = iter->second;
		if(curveObject)
		{
			curveObject->GetDrawCurvePar(MaxPos,MinPos,m_ptMoveOffset,m_ptZoomOffset);
			this->CalculateMaxPoint(m_ptMaxPos,&MaxPos);
			this->CalculateMinPoint(m_ptMinPos,&MinPos);

			//��������ͼ����϶
			CurveNameSize = pDc->GetTextExtent(_curveName);
			m_CurveNameMaxSize.cy=m_CurveNameMaxSize.cy>CurveNameSize.cy?m_CurveNameMaxSize.cy:CurveNameSize.cy;
		}
		++iter;
	}

	m_ptMaxPos.m_PosX += 10;
	m_ptMaxPos.m_PosY += 1;

	m_ptMinPos.m_PosX = 0;
	m_ptMinPos.m_PosY = -1;
	//�Ա�ͼ��PCA
	m_ptMaxPosConst = m_ptMaxPos;
	m_ptMinPosConst = m_ptMinPos;

	this->ResetAllCurveMaxMin(m_ptMaxPos,m_ptMinPos);
	return 0;
}
int CClusterChart::ResetAllCurveMaxMin(CFloatPoint MaxPos,CFloatPoint MinPos)
{
	CCurveObj* curveObject = NULL;

	CMapToCurveObj::iterator iter = m_AnaSpecCluster.begin();
	while(iter!=m_AnaSpecCluster.end())
	{
		curveObject = iter->second;
		if(curveObject)
		{
			curveObject->SetCurveMaxMinPar(MaxPos,MinPos);
		}
		++iter;
	}
	//�ο�ͼ��
	iter = m_FakeSpecCluster.begin();
	while(iter!=m_FakeSpecCluster.end())
	{
		curveObject = iter->second;
		if(curveObject)
		{
			curveObject->SetCurveMaxMinPar(MaxPos,MinPos);
		}
		++iter;
	}
	return  0;
}

void CClusterChart::CalculateMaxPoint(CFloatPoint& pointDes,const CFloatPoint* PointSource)
{
	if(PointSource->m_PosX > pointDes.m_PosX)
		pointDes.m_PosX = PointSource->m_PosX;
	if(PointSource->m_PosY > pointDes.m_PosY)
		pointDes.m_PosY = PointSource->m_PosY;
}

void CClusterChart::CalculateMinPoint(CFloatPoint& pointDes,const CFloatPoint* PointSource)
{
	if(PointSource->m_PosX < pointDes.m_PosX)
		pointDes.m_PosX = PointSource->m_PosX;
	if(PointSource->m_PosY < pointDes.m_PosY)
		pointDes.m_PosY = PointSource->m_PosY;
}

void CClusterChart::OnRButtonDown(UINT nFlags, CPoint point) 
{
	this->SetFocus();  //��ý���
	CWnd::OnRButtonDown(nFlags, point);
}

void CClusterChart::OnContextMenu(CWnd* pWnd, CPoint point) 
{
}

BOOL CClusterChart::OnCommand(WPARAM wParam, LPARAM lParam) 
{
	int menuID = LOWORD(wParam);
	switch(menuID)
	{
	case 9998://�ڶ�������ϵ�ڹ�������ϵ���л�
		{	
			m_bAllCurveSameBase = !m_bAllCurveSameBase;
			this->InvalidateChart();
			break;
		}
	case 9999:  //ȡ����ģʽ
		{
			m_iControlMode = CURVE_GET_DATA;
			break;
		}
	case 10000: //reset the individual curve
		{	
			this->MoveCurve(m_sCurrentControlCurve,CURVE_RESET);
			this->SetFocus();
			break;
		}
	case 10007:  //��λ��������
		{
			m_sCurrentControlCurve.Empty();
			m_pCurSelCurveObj = NULL;
			this->MoveCurve(m_sCurrentControlCurve,CURVE_RESET);
			this->SetFocus();
			break;
		}
	case 10008:  //ѡ��������
		{
			m_iControlMode = CURVE_SELECT;
			::SendMessage(m_pParent->m_hWnd,WM_CHANGECHARTMODE,m_iControlMode,0);
			break;
		}
	case 10001:  //select all curve
		{
			m_iControlMode = CURVE_SELECT;
			this->InvalidateChart();
			break;
		}
	case 10006:  //�ƶ����ߣ���ͨ����꣬������������ֿ������ߵ��ƶ�
		{ 
			m_iControlMode = CURVE_MOVE;
			::SendMessage(m_pParent->m_hWnd,WM_CHANGECHARTMODE,m_iControlMode,0);
			break;
		}
	case 10002: //���߷Ŵ�
		{
			m_iControlMode = CURVE_ZOOM_IN;
			::SendMessage(m_pParent->m_hWnd,WM_CHANGECHARTMODE,m_iControlMode,0);
			break;
		}
	case 10003: //������С
		{
			m_iControlMode = CURVE_ZOOM_OUT;
			::SendMessage(m_pParent->m_hWnd,WM_CHANGECHARTMODE,m_iControlMode,0);
			break;
		}
	case 10004: //ˮƽ����ģʽ
		{
			m_iVerticalControlMode = false;
			break;
		}
	case 10005:  //��ֱ����ģʽ
		{
			m_iVerticalControlMode = true;
			break;
		}
	case 10009:  //x���ͼ����
		{
			m_bDrawReverseX = !m_bDrawReverseX;
			CCurveObj::SetCurveDirction(m_bDrawReverseX,m_bDrawReverseY);
			this->InvalidateChart();
			break;
		}
	case 10010:  //Y��ͼ����
		{  
			m_bDrawReverseY = !m_bDrawReverseY;
			CCurveObj::SetCurveDirction(m_bDrawReverseX,m_bDrawReverseY);
			this->InvalidateChart();
			break;
		}
	case 10011:  //����ѡ��
		{
			m_iControlMode = CURVE_FEATURE_SELECT;
			break;
		}

	case 10014:  //
		{
			m_iControlMode = CURVE_PEAKAREA_CAL;
			break;
		}
	case 10015:  //
		{
			m_iControlMode = CURVE_LABEL;
			break;
		}
	}
	return CWnd::OnCommand(wParam, lParam);
}

void CClusterChart::OnUpdateMenuState(CCmdUI* pCmdUI)//���²˵��ϵ�״̬��ʾ
{
}

BOOL CClusterChart::PreTranslateMessage(MSG* pMsg) 
{
	switch(pMsg->message)
	{
	case WM_KEYDOWN:
		{
			CPoint point = pMsg->pt;
			CRect rect = m_rectPlot;
			this->ClientToScreen(&rect);

			switch(pMsg->wParam)
			{
			case VK_LEFT:
				{
					if(m_iControlMode == CURVE_GET_DATA)
					{
						if(rect.PtInRect(point))
						{
							point.Offset(-1,0);
							SetCursorPos(point.x,point.y);
						}
					}
					else
					{
						this->MoveCurve(m_sCurrentControlCurve,CURVE_MOVE_LEFT,0.1);
					}
					return TRUE;
				}
			case VK_RIGHT:
				{
					if(m_iControlMode == CURVE_GET_DATA)
					{
						if(rect.PtInRect(point))
						{
							point.Offset(1,0);
							SetCursorPos(point.x,point.y);
						}
					}
					else
					{
						this->MoveCurve(m_sCurrentControlCurve,CURVE_MOVE_RIGHT,0.1);
					}
					return TRUE;
				}
			case VK_UP:
				{
					if(m_iControlMode == CURVE_GET_DATA)
					{
						if(rect.PtInRect(point))
						{
							point.Offset(0,-1);
							SetCursorPos(point.x,point.y);
						}
					}
					else
					{
						this->MoveCurve(m_sCurrentControlCurve,CURVE_MOVE_UP,0.1);
					}
					return TRUE;
				}
			case VK_DOWN:
				{
					if(m_iControlMode == CURVE_GET_DATA)
					{
						if(rect.PtInRect(point))
						{
							point.Offset(0,1);
							SetCursorPos(point.x,point.y);
						}
					}
					else
					{
						this->MoveCurve(m_sCurrentControlCurve,CURVE_MOVE_DOWN,0.1);
					}
					return TRUE;
				}
			case VK_F5:
				{
					this->MoveCurve(m_sCurrentControlCurve,CURVE_RESET);
					return TRUE;
				}
			}
			break;
		}
	case WM_KEYUP:
		{
			this->SetFocus();
			break;
		}
	}	
	return CWnd::PreTranslateMessage(pMsg);
}

void CClusterChart::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	ResetCurve();
	CWnd::OnLButtonDblClk(nFlags, point);
}

LRESULT CClusterChart::DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	return CWnd::DefWindowProc(message, wParam, lParam);
}

int CClusterChart::FloatDataPrecise(double f)
{
	char buf[32];
	int dot=0;
	int i=0;

	sprintf(buf, "%lf", f);

	i = strlen(buf) - 1;
	while(buf[i] == '0')
	{
		i--;
	}

	dot = 0;
	while(buf[dot] != '.')
	{
		dot++;
	}

	return ((i - dot) > 0)?(i - dot):0;
}

void CClusterChart::DrawSelectFocus(CDC* pDc,CPoint&  point)
{

}

void CClusterChart::DrawZoomRect(CDC* pDc,CRect rect)
{
	CPen zoomRectPen(PS_DOT,1,m_crFrameColor);
	CPen* oldPen = pDc->SelectObject(&zoomRectPen);

	pDc->MoveTo(rect.TopLeft());
	pDc->LineTo(rect.right,rect.top);
	pDc->LineTo(rect.BottomRight());
	pDc->LineTo(rect.left,rect.bottom);
	pDc->LineTo(rect.TopLeft());

	CPoint point = rect.CenterPoint();

	pDc->MoveTo(point.x,point.y-4);
	pDc->LineTo(point.x,point.y+4);
	pDc->MoveTo(point.x-4,point.y);
	pDc->LineTo(point.x+4,point.y);

	pDc->SelectObject(oldPen);
}

void CClusterChart::DrawSelectRange(CDC *pDc,CLine Line)
{
	CPen zoomRectPen(PS_SOLID,1,RGB(0,0,0));
	CPen* oldPen = pDc->SelectObject(&zoomRectPen);

	pDc->Rectangle(Line.m_Begin.x-2,Line.m_Begin.y-2,Line.m_Begin.x+2,Line.m_Begin.y+2);
	pDc->Rectangle(Line.m_End.x-2,Line.m_End.y-2,Line.m_End.x+2,Line.m_End.y+2);

	if(m_iControlMode == CURVE_PEAKAREA_CAL)
	{
		CPoint _begin;
		CPoint _end;
		if(Line.m_Begin !=  Line.m_End)
		{
			if(Line.m_Begin.x == Line.m_End.x)
			{
				//����ֱ��
				_begin.x = Line.m_Begin.x;
				_begin.y = m_rectPlot.top;
				_end.x = Line.m_End.x;
				_end.y = m_rectPlot.bottom;
				pDc->MoveTo(_begin);
				pDc->LineTo(_end);
				pDc->SelectObject(oldPen);
				return;
			}
			//��������б��
			m_Slope = ((double)(Line.m_Begin.y-Line.m_End.y))/(Line.m_Begin.x-Line.m_End.x);

			//�����ʼ���λ��
			_begin.x = m_rectPlot.left;
			_begin.y = (LONG)(m_Slope*(_begin.x - Line.m_Begin.x)+Line.m_Begin.y);

			if(_begin.y > m_rectPlot.bottom)//˵����ʼ��Ӧ���ڵױ�
			{
				//���¼����ʼ��
				_begin.y = m_rectPlot.bottom;
				_begin.x = (LONG)((_begin.y - Line.m_Begin.y)/m_Slope+Line.m_Begin.x);
			}
			else if(_begin.y < m_rectPlot.top)  //˵����ʼ��Ӧ���ڶ���
			{
				//���¼����ʼ��
				_begin.y = m_rectPlot.top;
				_begin.x = (LONG)((_begin.y - Line.m_Begin.y)/m_Slope+Line.m_Begin.x);	
			}
			//���������λ��
			_end.x = m_rectPlot.right;
			_end.y = (LONG)(m_Slope*(_end.x - Line.m_End.x)+Line.m_End.y);

			if(_end.y > m_rectPlot.bottom)//˵����ʼ��Ӧ���ڵױ�
			{
				//���¼��������
				_end.y = m_rectPlot.bottom;
				_end.x = (LONG)((_end.y - Line.m_End.y)/m_Slope+Line.m_End.x);	

			}
			else if(_end.y < m_rectPlot.top)  //˵��������Ӧ���ڶ���
			{
				//���¼��������
				_end.y = m_rectPlot.top;
				_end.x = (LONG)((_end.y - Line.m_End.y)/m_Slope+Line.m_End.x);	
			}
		}
		else
		{
			//����ʼ��ͽ�����һ��
			_begin.x = 0;
			_begin.y = Line.m_Begin.y;
			_end.x = 0;
			_end.y = Line.m_Begin.y;
		}
		pDc->MoveTo(_begin);
		pDc->LineTo(_end);
		//������
		pDc->MoveTo(CPoint(Line.m_Begin.x,m_rectPlot.bottom));
		pDc->LineTo(CPoint(Line.m_Begin.x,m_rectPlot.top));

		pDc->MoveTo(CPoint(Line.m_End.x,m_rectPlot.bottom));
		pDc->LineTo(CPoint(Line.m_End.x,m_rectPlot.top));

	}
	else
	{
		pDc->MoveTo(Line.m_Begin);
		pDc->LineTo(Line.m_End);
	}

	pDc->SelectObject(oldPen);
}

void CClusterChart::DrawSelectRangeText(CDC* pDc)
{
}

void CClusterChart::DrawNearestPt2Cursor(CDC *pDc,CPoint _point)
{
	CPen zoomRectPen(PS_SOLID,2,RGB(0,0,255));
	CPen* oldPen = pDc->SelectObject(&zoomRectPen);

	pDc->Rectangle(_point.x-2,_point.y-2,_point.x+2,_point.y+2);
	pDc->SelectObject(oldPen);
}

void CClusterChart::DrawGetDataCross(CDC *pDc)
{
}

void CClusterChart::PreSubclassWindow() 
{
	// TODO: Add your specialized code here and/or call the base class

	CWnd::PreSubclassWindow();
}

void CClusterChart::DrawDataFocus(CDC *pDC,CPoint point,COLORREF color)
{
	CPen pen;
	pen.CreatePen(PS_SOLID, 2, color);
	CPen* pPen = pDC->SelectObject(&pen);
	int nRadios = 2;
	CBrush* pBrush = (CBrush*)pDC->SelectStockObject(NULL_BRUSH);

	pDC->Ellipse(point.x - nRadios,point.y - nRadios,point.x + nRadios,point.y + nRadios);

	pDC->SelectObject(pBrush);
	pDC->SelectObject(pPen);
}
//�����Ƕ�������ϵ���ǹ�������ϵ,ÿ����һ��״̬��������ת��Ĭ���Ƕ�������ϵ
void CClusterChart::ChangeToIndividualCoordinateMode(bool individualMode/* = true*/)
{

}

void CClusterChart::ChangeToGetDataMode()  //�ö����л���ȡ����ģʽ������ģʽ��������
{
	WPARAM controlCode = MAKELONG(9999,0);
	this->PostMessage(WM_COMMAND,controlCode,0);
}
void CClusterChart::ResetCurve(bool allCurve/* = false*/)  //��λ���أ����ѡ����һ�����ߣ���λ�������ߣ����򣬽���λ���е�����
{
	WPARAM controlCode = 0x0;
	if(allCurve)
	{
		controlCode = MAKELONG(10007,0);
	}
	else
	{
		controlCode = MAKELONG(10000,0);
	}
	this->PostMessage(WM_COMMAND,controlCode,0);
}
void CClusterChart::ChangeToSelectMode(bool allCurve/* = false*/)  //�л���ѡ������ģʽ
{
	WPARAM controlCode = 0x0;
	if(allCurve)
	{
		controlCode = MAKELONG(10001,0);
	}
	else
	{
		controlCode = MAKELONG(10008,0);
	}
	this->PostMessage(WM_COMMAND,controlCode,0);
}
void CClusterChart::ChangeToMoveMode()  //�л����ƶ�����ģʽ
{
	WPARAM controlCode = MAKELONG(10006,0);
	this->PostMessage(WM_COMMAND,controlCode,0);
}
void CClusterChart::ChangeToZoomInMode() //
{
	WPARAM controlCode = MAKELONG(10002,0);
	this->PostMessage(WM_COMMAND,controlCode,0);
}
void CClusterChart::ChangeToZoomOutMode()
{
	WPARAM controlCode = MAKELONG(10003,0);
	this->PostMessage(WM_COMMAND,controlCode,0);
}
void CClusterChart::ChangeToVerticalMode()
{
	WPARAM controlCode = MAKELONG(10005,0);
	this->PostMessage(WM_COMMAND,controlCode,0);
}
void CClusterChart::ChageToHorizontalMode()
{
	WPARAM controlCode = MAKELONG(10004,0);
	this->PostMessage(WM_COMMAND,controlCode,0);
}
void CClusterChart::ChangeToPeakAreaCalMode()
{
	WPARAM controlCode = MAKELONG(10014,0);
	this->PostMessage(WM_COMMAND,controlCode,0);
}
void CClusterChart::ChangeToLableMode()
{
	WPARAM controlCode = MAKELONG(10015,0);
	this->PostMessage(WM_COMMAND,controlCode,0);
}

void CClusterChart::ChageDrawDirectionX()
{
	WPARAM controlCode = MAKELONG(10009,0);
	this->PostMessage(WM_COMMAND,controlCode,0);
}
void CClusterChart::ChageDrawDirectionY()
{
	WPARAM controlCode = MAKELONG(10010,0);
	this->PostMessage(WM_COMMAND,controlCode,0);
}
void CClusterChart::HideUnselectCurve(bool hide/* = false*/)
{
	m_bHideUnselectCurve = hide;
	InvalidateChart();
}
void CClusterChart::DataShowFollowCursor(bool follow/* = false*/)
{
	m_bShowDataFlowMouse = follow;
}
void CClusterChart::SetBackgroundColor(COLORREF backColor)
{

	m_brushBack.DeleteObject() ;
	m_crBackColor = backColor;
	m_brushBack.CreateSolidBrush(m_crBackColor) ;

	InvalidateChart();
}
void CClusterChart::SetGridLineColor(COLORREF gridLienColor)
{
	m_crGridColor = gridLienColor;
	InvalidateChart();
}
void CClusterChart::SavePlotAsBmp(CString filename)
{

	CDC dc; 
	dc.CreateDC((LPCTSTR)"DISPLAY",NULL,NULL,NULL);

	CDC memDC ;
	CBitmap memBitmap ;
	CBitmap* oldBitmap ; // bitmap originally found in CMemDC


	memDC.CreateCompatibleDC(&dc) ;
	memBitmap.CreateCompatibleBitmap(&dc, m_nClientWidth, m_nClientHeight) ;
	oldBitmap = (CBitmap *)memDC.SelectObject(&memBitmap) ;

	if (memDC.GetSafeHdc() != NULL)
	{
		//memDC.BitBlt(0,0,m_nClientWidth, m_nClientHeight,&m_dcGrid, 0, 0, SRCCOPY);//��������������
		memDC.BitBlt(0, 0, m_nClientWidth, m_nClientHeight,&m_dcPlot, 0,0,SRCPAINT);  //���һ������SRCPAINTָ������DC���ݽ���OR����,����������
		memDC.SelectObject(oldBitmap);
	}

	BITMAP btm;  //��ȡbmp�ļ���Ϣ
	memBitmap.GetBitmap(&btm); 

	//�����ڴ�
	DWORD size=btm.bmWidthBytes*btm.bmHeight; 
	LPSTR lpData=(LPSTR)GlobalAlloc(GPTR,size); 
	if(lpData != NULL) 
	{ 
		//���bmpͷ�ļ���Ϣ
		BITMAPINFOHEADER bih; 
		bih.biBitCount=btm.bmBitsPixel; 
		bih.biClrImportant=0; 
		bih.biClrUsed=0; 
		bih.biCompression=0; 
		bih.biHeight=btm.bmHeight; 
		bih.biPlanes=1; 
		bih.biSize=sizeof(BITMAPINFOHEADER); 
		bih.biSizeImage=size; 
		bih.biWidth=btm.bmWidth; 
		bih.biXPelsPerMeter=0; 
		bih.biYPelsPerMeter=0; 
		GetDIBits(dc.m_hDC,memBitmap,0,bih.biHeight,lpData,(BITMAPINFO*)&bih,DIB_RGB_COLORS); //��bm�л�ȡbmp����

		BITMAPFILEHEADER bfh; //bmp�ļ�ͷ����
		bfh.bfReserved1=bfh.bfReserved2=0; 
		bfh.bfType=((WORD)('M'<<8)|'B'); 
		bfh.bfSize=sizeof(bfh)+size; 
		bfh.bfOffBits=sizeof(bfh); 

		CFile bf; 
		if(bf.Open(filename,CFile::modeCreate|CFile::modeWrite)) 
		{ 
			bf.Write(&bfh,sizeof(BITMAPFILEHEADER)); 
			bf.Write(&bih,sizeof(BITMAPINFOHEADER)); //Ҫ�Ѵ���Ĵ����Ϊ����. 
			bf.Write(lpData,size); 
			bf.Close(); 
		} 
		::GlobalFree(lpData); 
	} 
	dc.DeleteDC(); 
}
void CClusterChart::SaveData(CString filePath)
{
}

int CClusterChart::SmoothCurve(CCurveDataSetArray& tempArray,CString _curveName,int SmoothMethod,int N)
{
	return 0;
}

int CClusterChart::ShowSelectedNoise()
{
	return 0;
}
int CClusterChart::ShowSelectedMean()
{
	return 0;
}

void CClusterChart::ShowSelectedPeakArea() //��������
{
}

BOOL CClusterChart::CurveExist(const CString &_curveName)   //�жϸ����Ƶ������Ƿ����
{
	return 0;
}

BOOL CClusterChart::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: �ڴ����ר�ô����/����û���
	if (cs.lpszClass == NULL)
		cs.lpszClass = AfxRegisterWndClass(CS_DBLCLKS);
	return CWnd::PreCreateWindow(cs);
}


BOOL CClusterChart::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	CPoint _cursorPoint;
	GetCursorPos(&_cursorPoint);
	ScreenToClient(&_cursorPoint);

	if(m_rectPlot.PtInRect(_cursorPoint))
	{
		switch(m_iControlMode)
		{

		case CURVE_MOVE:
			{
				::SetCursor(AfxGetApp()->LoadCursor(IDC_CURSORMOVE));
				return TRUE;
			}
		case CURVE_LABEL:
			{
				::SetCursor(AfxGetApp()->LoadCursor(IDC_CURSOR_LABEL));
				return TRUE;
			}
		case CURVE_ZOOM_OUT:
			{
				::SetCursor(AfxGetApp()->LoadCursor(IDC_CURSOR_ZOOMOUT));
				return TRUE;
			}
		case CURVE_ZOOM_IN:
			{
				::SetCursor(AfxGetApp()->LoadCursor(IDC_CURSOR_ZOOMIN));
				return TRUE;
			}
		}
	}
	else
	{
		::SetCursor(AfxGetApp()->LoadStandardCursor(IDC_ARROW));
	}

	return CWnd::OnSetCursor(pWnd, nHitTest, message);
}

void CClusterChart::RemoveAllLabel()
{
	m_SetLabelPoint.clear();
	this->InvalidateChart();
}