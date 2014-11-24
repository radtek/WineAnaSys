// PCAChart.cpp : 实现文件
#include "stdafx.h"
//#include "DataServerCenter.h"
#include "PCAChart.h"
#include "PCADlg.h"
#include "Chart.h"
#include "SpectrumCompareDlg.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
// CPCAChart
using std::set;
using std::find;
using std::vector;
using std::map;
using std::multimap;
IMPLEMENT_DYNAMIC(CPCAChart, CWnd)

CPCAChart::CPCAChart()
{
	m_bDrawReverseX = false;  //绘图方向
	m_bDrawReverseY = false;  //绘图方向

	m_bShowMenum = true;
	m_bAllCurveSameBase = false;

	m_bHideUnselectCurve = false;
	m_bShowDataFlowMouse = true;  //取数据时是否让数据跟随鼠标显示
	m_bDrawLegend=true;    //是否绘制图例

	m_crBackColor  =RGB(255,255,255);// RGB(192,192,192);//; //RGB();


	m_crGridColor = RGB(105,105,105); //RGB(120,120,120);
	m_crFrameColor =RGB(0,0,0); //RGB(255,255,255);
	m_crPlotAreaBackColor = RGB(255,255,255);
	m_RefSpecPCAColor = RGB(255,0,0);


	m_brushBack.CreateSolidBrush(m_crBackColor) ;
	m_brushGrid.CreateSolidBrush(m_crGridColor) ;

	m_brushPlotAreaBack.CreateSolidBrush(m_crPlotAreaBackColor) ;

	m_CurveMap.clear();
	m_RefCurveMap.clear();

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
	m_iVerticalControlMode = false; //默认是水平控制模式(指管轮操作模式的移动即缩放功能)
	m_recMouseSelect.SetRectEmpty();
	m_pCurSelCurveObj = NULL;
	m_CurveNameMaxSize=0;
	m_LegendLineLenth=30;

	m_SelectedCurveColor = RGB(255,0,0);  //选中曲线颜色

	m_ArrayCurveColor.Add(RGB(0,0,255));
	m_ArrayCurveColor.Add(RGB(128,0,128));
	m_ArrayCurveColor.Add(RGB(255,140,0));
	m_ArrayCurveColor.Add(RGB(0,255,0));
	m_ArrayCurveColor.Add(RGB(255,215,0));
	m_ArrayCurveColor.Add(RGB(0,0,0));

	m_FeatrueNum=0;
}

CPCAChart::~CPCAChart()
{
	CCurveObj * pCurveObject = NULL;
	CMultiMapToCurveObj::iterator iter1 = m_CurveMap.begin();
	while(iter1!= m_CurveMap.end())
	{
		pCurveObject = iter1->second;
		pCurveObject->DestroyCurve();
		pCurveObject = NULL;
		++iter1;
	}
	m_CurveMap.clear();
	CMultiMapToCurveObj::iterator iter = m_RefCurveMap.begin();
	while(iter!= m_RefCurveMap.end())
	{
		pCurveObject = iter->second;
		pCurveObject->DestroyCurve();
		pCurveObject = NULL;
		++iter;
	}
	m_RefCurveMap.clear();
}

BEGIN_MESSAGE_MAP(CPCAChart, CWnd)
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
// CPCAChart message handlers

//##ModelId=48ACE0A6036B
BOOL CPCAChart::Create(DWORD dwStyle, const RECT &rect, CWnd *pParentWnd, UINT nID)
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
		this->DrawGrid();
	}
	return result ;
}
//##ModelId=48A14589009E
void CPCAChart::SetShowLegend(bool ShowLegend/*= true*/)
{
	m_bDrawLegend=ShowLegend;
	InvalidateChart();
}

BOOL CPCAChart::InvalidateCtrl()   //
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
void CPCAChart::OnSize(UINT nType, int cx, int cy) 
{
	CWnd::OnSize(nType, cx, cy);
	GetClientRect(m_rectClient) ;

	m_nClientHeight = m_rectClient.Height();
	m_nClientWidth = m_rectClient.Width();

	this->SetMargin(m_ptMargin.x,m_ptMargin.y);	//重新设定边框
	this->InvalidateCtrl();
}

//##ModelId=48A145880203
void CPCAChart::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	CDC memDC ;
	CBitmap memBitmap ;
	CBitmap* oldBitmap ; // bitmap originally found in CMemDC
	memDC.CreateCompatibleDC(&dc) ;
	memBitmap.CreateCompatibleBitmap(&dc, m_nClientWidth, m_nClientHeight) ;
	oldBitmap = (CBitmap *)memDC.SelectObject(&memBitmap) ;
	if (memDC.GetSafeHdc() != NULL)
	{
		memDC.BitBlt(0, 0, m_nClientWidth, m_nClientHeight,&m_dcPlot, 0,0,SRCPAINT);  //最后一个参数SRCPAINT指定两个DC内容进行OR运算,即叠加运算
		switch(m_iControlMode)
		{
		case CURVE_GET_DATA:
			{	
				break;
			}
		case CURVE_MOVE:
			{
				if(this->m_bLeftButtonDown)
				{
					if(m_sCurrentControlCurve.IsEmpty())
					{

						CString curveName;
						CCurveObj* curveObject = NULL;
						CMultiMapToCurveObj::iterator iter1 = m_CurveMap.begin();
						while(iter1!= m_CurveMap.end())
						{
							curveObject = iter1->second;
							if(curveObject)
							{
								this->IndividualCurveControl(curveObject,m_iControlMode,m_recMouseSelect);
							}
							++iter1;
						}


						iter1 = m_RefCurveMap.begin();
						while(iter1!= m_RefCurveMap.end())
						{
							curveObject = iter1->second;
							if(curveObject)
							{
								this->IndividualCurveControl(curveObject,m_iControlMode,m_recMouseSelect);
							}
							++iter1;
						}

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
				break;
			}
		case CURVE_LABEL:
			{	
				break;
			}
		}
		dc.BitBlt(0,0,m_nClientWidth, m_nClientHeight,&memDC,0,0,SRCCOPY); //拷贝内存DC到屏幕
	}
	memDC.SelectObject(oldBitmap) ;
	memBitmap.DeleteObject(); 
	memDC.DeleteDC(); 
}


void CPCAChart::DrawLegend()
{
	DrawLegend(m_dcPlot,m_rectPlot);
}

void CPCAChart::DrawLegend(CDC &m_dcPlot,CRect m_rectPlot)
{
	m_dcPlot.SetTextAlign (TA_LEFT|TA_TOP);//显示文字方向从左向右
	COLORREF LegendColor;
	CSize CurveNameSize;
	CString _curveName;
	int i=0;
	CCurveObj* curveObject = NULL;

	m_dcPlot.SetBkColor (m_crPlotAreaBackColor) ;
	//填充
	int ntotalCurve=m_CurveMap.size();
	m_dcPlot.FillRect(CRect(m_rectPlot.right+10 ,m_rectPlot.top,m_rectPlot.right+63+m_CurveNameMaxSize.cx,m_rectPlot.top+(m_CurveNameMaxSize.cy+5)*ntotalCurve),&m_brushPlotAreaBack);
	int r = 5;
    //画参考图谱图例
	CPen* oldPen = NULL;
	CPen LegendPen(PS_SOLID,2,m_RefSpecPCAColor);
	//得到曲线名称字符串尺寸
	CurveNameSize=m_dcPlot.GetTextExtent(RefSpecType);
	CurveNameSize = m_CurveNameMaxSize;
	if(PCANum == 2)
	{
		std::vector<CString>::iterator iterRefName = m_RefName.begin();
		while(iterRefName!=m_RefName.end())
		{
			LegendColor=m_RefColor[*iterRefName];//curveObject->GetCurveColor();
			CPen LegendPen(PS_SOLID,2,LegendColor);
			oldPen = m_dcPlot.SelectObject(&LegendPen);
			//得到曲线名称字符串尺寸
			CurveNameSize=m_dcPlot.GetTextExtent(*iterRefName);
			m_dcPlot.Ellipse(m_rectPlot.right+10,m_rectPlot.top+i*(CurveNameSize.cy+5)+(CurveNameSize.cy+5)/2+5-r,
				m_rectPlot.right+10+2*r,m_rectPlot.top+i*(CurveNameSize.cy+5)+(CurveNameSize.cy+5)/2+5+r);
			m_dcPlot.SelectObject(oldPen);
			m_dcPlot.SetTextColor(LegendColor);
			m_dcPlot.TextOut(m_rectPlot.right+63,m_rectPlot.top+i*(CurveNameSize.cy+5)+5,m_RefName[i]);
			++i;			
			++iterRefName;
		}
		//画项目光谱图例
		CPen LegendPen(PS_SOLID,2,RGB(255,0,0));
		m_dcPlot.SelectObject(&LegendPen);

		m_dcPlot.Rectangle(m_rectPlot.right+10,m_rectPlot.top+i*(CurveNameSize.cy+5)+(CurveNameSize.cy+5)/2+5-r,
			m_rectPlot.right+10+2*r,m_rectPlot.top+i*(CurveNameSize.cy+5)+(CurveNameSize.cy+5)/2+5+r);

		m_dcPlot.SelectObject(oldPen);
		m_dcPlot.SetTextColor(RGB(255,0,0));
		m_dcPlot.TextOut(m_rectPlot.right+63,m_rectPlot.top+i*(CurveNameSize.cy+5)+5,L"项目光谱");
		++i;
	}
	else
	{
		std::vector<CString>::iterator iterRefName = m_RefName.begin();
		while(iterRefName!=m_RefName.end())
		{
			CPen* oldPen;
			LegendColor=m_RefColor[*iterRefName];
			CPen LegendPen(PS_SOLID,2,LegendColor);
			oldPen = m_dcPlot.SelectObject(&LegendPen);
			//得到曲线名称字符串尺寸
			CurveNameSize=m_dcPlot.GetTextExtent(*iterRefName);
			m_dcPlot.MoveTo(m_rectPlot.right+10,m_rectPlot.top+i*(CurveNameSize.cy+5)+(CurveNameSize.cy+5)/2+5);
			m_dcPlot.LineTo(m_rectPlot.right+60,m_rectPlot.top+i*(CurveNameSize.cy+5)+(CurveNameSize.cy+5)/2+5);

			m_dcPlot.SelectObject(oldPen);
			m_dcPlot.SetTextColor(m_RefSpecPCAColor);
			m_dcPlot.TextOut(m_rectPlot.right+63,m_rectPlot.top+i*(CurveNameSize.cy+5)+5,m_RefName[i]);
			++i;	
			++iterRefName;
		}
		//画非参考图谱图例
		CPen LegendPen(PS_SOLID,2,RGB(255,0,0));
		m_dcPlot.SelectObject(&LegendPen);
		m_dcPlot.MoveTo(m_rectPlot.right+10,m_rectPlot.top+i*(CurveNameSize.cy+5)+(CurveNameSize.cy+5)/2+5);
		m_dcPlot.LineTo(m_rectPlot.right+60,m_rectPlot.top+i*(CurveNameSize.cy+5)+(CurveNameSize.cy+5)/2+5);	
		m_dcPlot.SetTextColor(RGB(255,0,0));
		m_dcPlot.TextOut(m_rectPlot.right+63,m_rectPlot.top+i*(CurveNameSize.cy+5)+5,L"项目光谱");
		++i;
	}
	m_dcPlot.SelectObject(oldPen);
	InvalidateRect(m_rectClient);
}

void CPCAChart::DrawBackGnd()
{
	DrawBackGnd(m_dcPlot,m_rectPlot);
}

void CPCAChart::DrawBackGnd(CDC &m_dcPlot,CRect m_rectPlot)
{
	m_dcPlot.FillRect(m_rectPlot, &m_brushPlotAreaBack);
}

void CPCAChart::DrawGrid()
{
	DrawGrid(m_dcPlot,m_rectPlot,m_rectClient);
}

void CPCAChart::DrawGrid(CDC &m_dcPlot,CRect m_rectPlot,CRect m_rectClient)
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
	CString curveName;

	m_dcPlot.FillRect(CRect(m_rectClient.left, m_rectClient.top, m_rectClient.right, m_rectPlot.top), &m_brushBack);
	m_dcPlot.FillRect(CRect(m_rectPlot.right, m_rectPlot.top, m_rectClient.right, m_rectClient.bottom), &m_brushBack);
	m_dcPlot.FillRect(CRect(m_rectClient.left, m_rectPlot.bottom, m_rectPlot.right, m_rectClient.bottom),&m_brushBack);
	m_dcPlot.FillRect(CRect(m_rectClient.left, m_rectPlot.top, m_rectPlot.left, m_rectPlot.bottom), &m_brushBack);

	m_dcPlot.SetTextColor (textColor);
	m_dcPlot.SetBkColor (m_crBackColor) ;

	CPen frameSolidPen(PS_SOLID,1,frameColor);
	oldPen = m_dcPlot.SelectObject(&frameSolidPen);
	//绘制坐标框
	m_dcPlot.MoveTo(m_rectPlot.TopLeft());
	m_dcPlot.LineTo(m_rectPlot.left,m_rectPlot.bottom);
	m_dcPlot.LineTo(m_rectPlot.right,m_rectPlot.bottom);
	m_dcPlot.LineTo(m_rectPlot.right,m_rectPlot.top);
	m_dcPlot.LineTo(m_rectPlot.TopLeft());
	m_dcPlot.SelectObject(oldPen) ;

	if(curveName.IsEmpty() && !m_bAllCurveSameBase)
	{
		bNoCurveSelect = false;
	}
	else
	{
		bNoCurveSelect = false;
	}

	//取出坐标范围，准备绘制坐标刻度
	this->GetMaxDataRect(curveName);

	CFloatPoint point = m_ptZoomOffset ;

	m_dVerticalFactor = m_rectPlot.Height()/(point.m_PosY+0.000000001);
	m_dHorizontalFactor = m_rectPlot.Width()/(point.m_PosX+0.000000001);

	//绘制纵坐标刻度线,10等分,线长10像素
	int _iPreciseX = 2;
	int _iPreciseY = 3;
	if(m_pCurSelCurveObj)//计算显示精度,主要是X轴
	{
		_iPreciseX = this->FloatDataPrecise(m_pCurSelCurveObj->GetTimeBaseFactor());
		_iPreciseY = this->FloatDataPrecise(m_ptMaxPos.m_PosY);
		if(_iPreciseY>4)
			_iPreciseY = 4;
	}

	int nVerScaleLines = 10;  
	double nScaleHeightY = point.m_PosY/nVerScaleLines;
	double dGridHeight = m_rectPlot.Height()/nVerScaleLines;
	double tempY = 0.0;	
	double tempY2=0.0;

	CPen girdSolidPen(PS_SOLID, 1, girdColor);
	oldPen = m_dcPlot.SelectObject(&girdSolidPen);

	m_dcPlot.SetTextAlign (TA_LEFT|TA_TOP) ;

	for(j=0;j<=nVerScaleLines;j++)
	{
		tempY2 = (m_bDrawReverseY)?nScaleHeightY*(nVerScaleLines-j):nScaleHeightY*j;  //曲线绘制方向改变时坐标也要相应的改变

		tempY = m_ptZoomOffset.m_PosY - tempY2 + m_ptMinPos.m_PosY - m_ptMoveOffset.m_PosY ;  //求出刻度值坐标
		if((double)abs(tempY)<90000000.0)
			str.Format (L"%.*lf", _iPreciseY, tempY) ;
		else
		{
			str.Format(L"%.*e",_iPreciseY,tempY);
		}
		textSize = m_dcPlot.GetTextExtent(str); 
		//绘制水平方向网格线
		tempY = m_rectPlot.top + (long)(dGridHeight*j);	
		for (i=m_rectPlot.left; i<m_rectPlot.right && (j!=nVerScaleLines) && (j!=0); i+=2)	 
		{
			m_dcPlot.SetPixel (i+2,tempY,girdColor) ;
		}	
		if(!bNoCurveSelect)
		{
			m_dcPlot.SetTextAlign (TA_RIGHT|TA_TOP) ;
			m_dcPlot.TextOut (m_rectPlot.left-textSize.cy/4, tempY-textSize.cy/2, str) ;
		}

		if(j<nVerScaleLines)
		{
			for(k =0;k<nVerScaleLines;k++)  //绘制小的标度尺
			{
				for(i = m_rectPlot.left; i<m_rectPlot.left+2; i++)
				{
					m_dcPlot.SetPixel (i+1,tempY+(dGridHeight/nVerScaleLines)*k,frameColor) ;
				}
			}
		}
	}
	if(2 == PCANum)
	{
		//绘制横坐标刻度线,10等分,线长10像素
		int nHorScaleLines = 10;  
		double nScaleWidthX = point.m_PosX/nHorScaleLines;
		double dGridWidth = m_rectPlot.Width()/nHorScaleLines;
		double tempX = 0.0;	
		double tempX2;

		for(j=0;j<=nHorScaleLines;j++)
		{
			tempX2 = (m_bDrawReverseX)?nScaleWidthX*j:nScaleWidthX*(nHorScaleLines-j);  //曲线绘制方向改变时坐标也要相应的改变
			tempX = m_ptZoomOffset.m_PosX - tempX2 + m_ptMinPos.m_PosX - m_ptMoveOffset.m_PosX ;  //求出刻度值坐标
			str.Format (L"%.*lf",_iPreciseX, tempX) ;
			textSize = m_dcPlot.GetTextExtent(str); 
			//绘制垂直方向网格线 
			tempX = m_rectPlot.left + (long)(dGridWidth*j);	
			for (i=m_rectPlot.top+2; i<m_rectPlot.bottom; i+=2)	 
			{
				m_dcPlot.SetPixel(tempX,i,girdColor) ;
			}
			//绘制标度
			if(!bNoCurveSelect)
			{
				m_dcPlot.TextOut (tempX+textSize.cx/2,m_rectPlot.bottom+5, str);	
			}

			if(j<nHorScaleLines)
			{
				for(k =0;k<nHorScaleLines;k++)  //绘制小的标度尺
				{
					for(i = m_rectPlot.bottom-2; i<m_rectPlot.bottom; i++)
					{
						m_dcPlot.SetPixel(tempX+(dGridWidth/nHorScaleLines)*k,i,frameColor) ;
					}
				}
			}
		}
		m_dcPlot.SelectObject (oldPen) ;
	}
	else
	{
		//绘制横坐标刻度线,PCANum - 1等分,线长10像素
		int nHorScaleLines = PCANum - 1;  
		double nScaleWidthX = point.m_PosX/nHorScaleLines;
		double dGridWidth = m_rectPlot.Width()/nHorScaleLines;
		double tempX = 0.0;	
		//double tempX2;
		CMultiMapToCurveObj::iterator iter = m_RefCurveMap.begin();

		for(j=0;j<=nHorScaleLines;++j)
		{
			str.Format (L"PC%d",j+1) ;
			textSize = m_dcPlot.GetTextExtent(str); 
			//绘制垂直方向网格线
			CFloatPoint tempFloat= iter->second->GetPoint(j);
			CPoint scrPoint;
			CPoint tempPoint = iter->second->CFloat2Point(tempFloat,this->m_rectPlot);
			tempX = m_rectPlot.left + (long)(dGridWidth*j);
			for (i=m_rectPlot.top+2; i<m_rectPlot.bottom; i+=2)	 
			{
				m_dcPlot.SetPixel(tempPoint.x,i,girdColor) ;
			}
			//绘制x轴PC标识
			if(!bNoCurveSelect)
			{
				m_dcPlot.TextOut (tempPoint.x+textSize.cx/2,m_rectPlot.bottom+5, str);	
			}
			if(j<nHorScaleLines)
			{
				for(k =0;k<nHorScaleLines;k++)  //绘制小的标度尺
				{
					for(i = m_rectPlot.bottom-2; i<m_rectPlot.bottom; i++)
					{
						m_dcPlot.SetPixel(tempPoint.x+(dGridWidth/nHorScaleLines)*k,i,frameColor) ;
					}
				}
			}
		}
		m_dcPlot.SelectObject (oldPen) ;
	}
	m_ptZoomOffset.InitPoint(0.0,0.0);
	m_ptMoveOffset.InitPoint(0.0,0.0);
	InvalidateRect(m_rectClient);
}
void CPCAChart::DrawUnPlotArea()
{
	DrawUnPlotArea(m_dcPlot,m_rectPlot,m_rectClient);
}
void CPCAChart::DrawUnPlotArea(CDC &m_dcPlot,CRect m_rectPlot,CRect m_rectClient)
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
	CString curveName;
	m_dcPlot.FillRect(CRect(m_rectClient.left, m_rectClient.top, m_rectClient.right, m_rectPlot.top), &m_brushBack);
	m_dcPlot.FillRect(CRect(m_rectPlot.right, m_rectPlot.top, m_rectClient.right, m_rectClient.bottom), &m_brushBack);
	m_dcPlot.FillRect(CRect(m_rectClient.left, m_rectPlot.bottom, m_rectPlot.right, m_rectClient.bottom),&m_brushBack);
	m_dcPlot.FillRect(CRect(m_rectClient.left, m_rectPlot.top, m_rectPlot.left, m_rectPlot.bottom), &m_brushBack);

	m_dcPlot.SetTextColor (textColor);
	m_dcPlot.SetBkColor (m_crBackColor) ;


	CPen frameSolidPen(PS_SOLID,1,frameColor);
	oldPen = m_dcPlot.SelectObject(&frameSolidPen);
	//绘制坐标框
	m_dcPlot.MoveTo(m_rectPlot.TopLeft());
	m_dcPlot.LineTo(m_rectPlot.left,m_rectPlot.bottom);
	m_dcPlot.LineTo(m_rectPlot.right,m_rectPlot.bottom);
	m_dcPlot.LineTo(m_rectPlot.right,m_rectPlot.top);
	m_dcPlot.LineTo(m_rectPlot.TopLeft());
	m_dcPlot.SelectObject(oldPen) ;

	if(curveName.IsEmpty() && !m_bAllCurveSameBase)
	{
		bNoCurveSelect = false;
	}
	else
	{
		bNoCurveSelect = false;
	}

	//取出坐标范围，准备绘制坐标刻度
	this->GetMaxDataRect(curveName);
	

	CFloatPoint point = m_ptZoomOffset ;

	m_dVerticalFactor = m_rectPlot.Height()/(point.m_PosY+0.000000001);
	m_dHorizontalFactor = m_rectPlot.Width()/(point.m_PosX+0.000000001);

	//绘制纵坐标刻度线,10等分,线长10像素
	int _iPreciseX = 2;
	int _iPreciseY = 3;
	if(m_pCurSelCurveObj)//计算显示精度,主要是X轴
	{
		_iPreciseX = this->FloatDataPrecise(m_pCurSelCurveObj->GetTimeBaseFactor());
		_iPreciseY = this->FloatDataPrecise(m_ptMaxPos.m_PosY);
		if(_iPreciseY>4)
			_iPreciseY = 4;
	}

	int nVerScaleLines = 10;  
	double nScaleHeightY = point.m_PosY/nVerScaleLines;
	double dGridHeight = m_rectPlot.Height()/nVerScaleLines;
	double tempY = 0.0;	
	double tempY2=0.0;

	CPen girdSolidPen(PS_SOLID, 1, girdColor);
	oldPen = m_dcPlot.SelectObject(&girdSolidPen);

	int rValue=GetRValue(girdColor);
	int gValue=GetGValue(girdColor);
	int bValue=GetBValue(girdColor);
	m_dcPlot.SetTextAlign(TA_LEFT|TA_TOP) ;
	for(j=0;j<=nVerScaleLines;j++)
	{
		tempY2 = (m_bDrawReverseY)?nScaleHeightY*(nVerScaleLines-j):nScaleHeightY*j;  //曲线绘制方向改变时坐标也要相应的改变

		tempY = m_ptZoomOffset.m_PosY - tempY2 + m_ptMinPos.m_PosY - m_ptMoveOffset.m_PosY ;  //求出刻度值坐标
		if((double)abs(tempY)<90000000.0)
			str.Format (L"%.*lf", _iPreciseY, tempY) ;
		else
		{
			str.Format(L"%.*e",_iPreciseY,tempY);
		}
		textSize = m_dcPlot.GetTextExtent(str); 
		//绘制水平方向网格线
		tempY = m_rectPlot.top + (long)(dGridHeight*j);	
		if(!bNoCurveSelect)
		{
			m_dcPlot.SetTextAlign (TA_RIGHT|TA_TOP) ;
			m_dcPlot.TextOut (m_rectPlot.left-textSize.cy/4, tempY-textSize.cy/2, str) ;
		}

		if(j<nVerScaleLines)
		{
			for(k =0;k<nVerScaleLines;k++)  //绘制小的标度尺
			{
				for(i = m_rectPlot.left; i<m_rectPlot.left+2; i++)
				{
					m_dcPlot.SetPixel (i+1,tempY+(dGridHeight/nVerScaleLines)*k,frameColor) ;
				}

			}
		}
	}

	//绘制横坐标刻度线,10等分,线长10像素
	CString xLegand;
	CString yLegand;
	if(PCANum == 2)
	{

		int nHorScaleLines = 10;  
		double nScaleWidthX = point.m_PosX/nHorScaleLines;
		double dGridWidth = m_rectPlot.Width()/nHorScaleLines;
		double tempX = 0.0;	
		double tempX2;

		for(j=0;j<=nHorScaleLines;j++)
		{
			tempX2 = (m_bDrawReverseX)?nScaleWidthX*j:nScaleWidthX*(nHorScaleLines-j);  //曲线绘制方向改变时坐标也要相应的改变

			tempX = m_ptZoomOffset.m_PosX - tempX2 + m_ptMinPos.m_PosX - m_ptMoveOffset.m_PosX ;  //求出刻度值坐标
			str.Format (L"%.*lf",_iPreciseX, tempX) ;
			textSize = m_dcPlot.GetTextExtent(str); 
			//绘制垂直方向网格线
			tempX = m_rectPlot.left + (long)(dGridWidth*j);	
			//绘制标度
			if(!bNoCurveSelect)
			{
				m_dcPlot.TextOut (tempX+textSize.cx/2,m_rectPlot.bottom+5, str);	
			}

			if(j<nHorScaleLines)
			{
				for(k =0;k<nHorScaleLines;k++)  //绘制小的标度尺
				{
					for(i = m_rectPlot.bottom-2; i<m_rectPlot.bottom; i++)
					{
						m_dcPlot.SetPixel(tempX+(dGridWidth/nHorScaleLines)*k,i,frameColor) ;
					}
				}
			}
		}

		CString XUnit;
		XUnit.Format(L"PC%d", XPC);
		CString YUnit;
		YUnit.Format(L"PC%d", YPC);

		CSize szXUnit = m_dcPlot.GetTextExtent(XUnit);
		CSize szYUnit = m_dcPlot.GetTextExtent(YUnit);

		LOGFONT lf; //定义字体结构
		lf.lfWeight=0; //字体磅数=10
		lf.lfHeight=0;	 //字体高度(旋转后的字体宽度)=56
		lf.lfWidth=0;   //字体宽度(旋转后的字体高度)=20
		lf.lfUnderline=FALSE; //无下划线
		lf.lfStrikeOut=FALSE; //无删除线
		lf.lfItalic=FALSE; //非斜体
		lf.lfEscapement=900; //字体显示角度=90°
		lf.lfCharSet=DEFAULT_CHARSET; //使用缺省字符集
		//wcscpy(lf.lfFaceName,L"@system"); //字体名=@system
		CFont myLogFont; //定义字体对象
		myLogFont.CreateFontIndirect(&lf); //创建逻辑字体
		CFont *pOldFont=m_dcPlot.SelectObject(&myLogFont);//选入设备描述表

		m_dcPlot.TextOut(2,m_rectClient.Height()/2-szYUnit.cx/2,YUnit);
		myLogFont.DeleteObject();
		lf.lfEscapement = 0;
		myLogFont.CreateFontIndirect(&lf); //创建逻辑字体
		m_dcPlot.SelectObject(&myLogFont);//选入设备描述表

		m_dcPlot.TextOut(m_rectClient.Width()/2+szXUnit.cx/2,m_rectClient.Height()-20,XUnit);
		m_dcPlot.SelectObject(pOldFont); //将myFont从设备环境中分离
		myLogFont.DeleteObject();
		m_dcPlot.SelectObject(oldPen) ;
	}
	else
	{
		//绘制横坐标刻度线,PCANum - 1等分,线长10像素
		int nHorScaleLines = PCANum - 1;  
		double nScaleWidthX = point.m_PosX/nHorScaleLines;
		double dGridWidth = m_rectPlot.Width()/nHorScaleLines;
		double tempX = 0.0;	
		CMultiMapToCurveObj::iterator iter = m_RefCurveMap.begin();

		for(j=0;j<=nHorScaleLines;++j)
		{
			str.Format (L"PC%d",j+1) ;
			textSize = m_dcPlot.GetTextExtent(str); 
			//绘制垂直方向网格线
			CFloatPoint tempFloat= iter->second->GetPoint(j);
			CPoint scrPoint;
			CPoint tempPoint = iter->second->CFloat2Point(tempFloat,this->m_rectPlot);

			tempX = m_rectPlot.left + (long)(dGridWidth*j);	
			for (i=m_rectPlot.top+2; i<m_rectPlot.bottom && (j!=nHorScaleLines) && (j!=0); i+=2)	 
			{
				m_dcPlot.SetPixel(tempPoint.x,i,girdColor) ;
			}
			//绘制x轴PC标识
			if(!bNoCurveSelect)
			{
				m_dcPlot.TextOut (tempPoint.x+textSize.cx/2,m_rectPlot.bottom+5, str);
			}

			if(j<nHorScaleLines)
			{
				for(k =0;k<nHorScaleLines;k++)  //绘制小的标度尺
				{
					for(i = m_rectPlot.bottom-2; i<m_rectPlot.bottom; i++)
					{
						m_dcPlot.SetPixel(tempPoint.x+(dGridWidth/nHorScaleLines)*k,i,frameColor) ;
					}
				}
			}
		}
		m_dcPlot.SelectObject (oldPen) ;
	}
	m_ptZoomOffset.InitPoint(-10000000.0,-10000000.0);  //此处必须这样初始化,
	m_ptMoveOffset.InitPoint(-10000000.0,-10000000.0);
	InvalidateRect(m_rectClient);
}


void CPCAChart::InvalidateChart()
{
	if( m_dcPlot.GetSafeHdc()==NULL )//m_dcGrid.GetSafeHdc()==NULL ||
		return ;
	CString _curveName;
	CCurveObj* curveObject = NULL;

	//画背景
	DrawBackGnd();
	//画网格
	this->DrawGrid();

	CMultiMapToCurveObj::iterator iter1 = m_CurveMap.begin();	
	while(iter1!=m_CurveMap.end())
	{
		//m_CurveMap.GetNextAssoc(pos,_curveName, (CObject*&)curveObject);
		curveObject = iter1->second;
		curveObject->m_cPlotRect = m_rectPlot;
		if(curveObject)
		{
			curveObject->SetCurveColor(RGB(255,0,0));
			if(2 == PCANum)
				curveObject->DrawPoint(&m_dcPlot,m_rectPlot,SQUARE);//绘制点
			else
				curveObject->DrawCurve(&m_dcPlot,m_rectPlot);//绘制曲线
		}
		++iter1;
	}

	//画参考图谱PC
	CMultiMapToCurveObj::iterator iter = m_RefCurveMap.begin();

	while(iter!=m_RefCurveMap.end())
	{
		std::vector<CString>::iterator itername = m_RefName.begin();
		int Index = 0;
		while(itername!=m_RefName.end())
		{
			if(*itername!=iter->first)
			{
				++itername;
				++Index;
			}
			else
			{
				break;
			}
			
		}
		if(iter->second)
		{	
			iter->second->SetCurveColor(m_RefColor[*itername]);
			iter->second->m_cPlotRect = m_rectPlot;
			if(2 == PCANum)
				iter->second->DrawPoint(&m_dcPlot,m_rectPlot,CIRCLE);//绘制点
			else
				iter->second->DrawCurve(&m_dcPlot,m_rectPlot);//绘制曲线
		}
		++iter;
	}

	DrawUnPlotArea();
	//画图例
	if(m_bDrawLegend)
	{
		DrawLegend();
	}
}

void CPCAChart::PrintChart(CDC* pDC, CPrintInfo* pInfo)//提供的打印接口
{
	CString _curveName;
	CCurveObj* curveObject = NULL;

	CDC MemDC; //首先定义一个显示设备对象
	CBitmap MemBitmap;//定义一个位图对象

	//随后建立与屏幕显示兼容的内存显示设备
	MemDC.CreateCompatibleDC(NULL);
	//这时还不能绘图，因为没有地方画 ^_^
	//下面建立一个与屏幕显示兼容的位图，至于位图的大小嘛，可以用窗口的大小
	MemBitmap.CreateCompatibleBitmap(pDC,m_rectClient.Width(),m_rectClient.Height());

	//将位图选入到内存显示设备中
	CBitmap *pOldBit=MemDC.SelectObject(&MemBitmap);

	//先用背景色将位图清除干净，白色作为背景
	MemDC.FillSolidRect(0,0,m_rectClient.Width(),m_rectClient.Height(),RGB(255,255,255));
	//绘图
	//画背景
	DrawBackGnd(MemDC,m_rectClient); 
	//画网格
	DrawGrid(MemDC,m_rectPlot,m_rectClient);

	CMultiMapToCurveObj::iterator iter1 = m_CurveMap.begin();	
	while(iter1!=m_CurveMap.end())
	{
		//m_CurveMap.GetNextAssoc(pos,_curveName, (CObject*&)curveObject);
		curveObject = iter1->second;
		curveObject->m_cPlotRect = m_rectPlot;
		if(curveObject)
		{
			curveObject->SetCurveColor(RGB(255,0,0));
			if(2 == PCANum)
				curveObject->DrawPoint(&MemDC,m_rectPlot,SQUARE);//绘制点
			else
				curveObject->DrawCurve(&MemDC,m_rectPlot);//绘制曲线
		}
		++iter1;
	}

	//画参考图谱PC
	CMultiMapToCurveObj::iterator iter = m_RefCurveMap.begin();

	while(iter!=m_RefCurveMap.end())
	{

		std::vector<CString>::iterator itername = m_RefName.begin();
		int Index = 0;
		while(itername!=m_RefName.end())
		{
			if(*itername!=iter->first)
			{
				++itername;
				++Index;
			}
			else
			{
				break;
			}

		}
		if(iter->second)
		{	
			iter->second->SetCurveColor(m_RefColor[*itername]);
			iter->second->m_cPlotRect = m_rectPlot;
			if(2 == PCANum)
				iter->second->DrawPoint(&MemDC,m_rectPlot,CIRCLE);//绘制点
			else
				iter->second->DrawCurve(&MemDC,m_rectPlot);//绘制曲线
		}
		++iter;
	}

	DrawUnPlotArea(MemDC,m_rectPlot,m_rectClient);
	//画图例
	if(m_bDrawLegend)
	{
		DrawLegend(MemDC,m_rectPlot);
	}
	pDC->StretchBlt(0,0,pInfo->m_rectDraw.Width(),pInfo->m_rectDraw.Height(),&MemDC,0,0,m_rectClient.Width(),m_rectClient.Height(),SRCCOPY);
	//绘图完成后的清理
	MemBitmap.DeleteObject();
	MemDC.DeleteDC();
}

//控制是否所有曲线共用一个坐标系,默认是每一条曲线在自己的坐标系中绘图。true为所有曲线共用一个坐标系
void CPCAChart::SetAllCurveDrawMode(bool onePlot/*=false*/)
{

}

void CPCAChart::OnSize(int cx,int cy)
{
	if(this->GetSafeHwnd())
	{
		this->SetWindowPos(NULL,0,0,cx,cy,SWP_SHOWWINDOW);  //调用此函数将激发WM_SIZE消息
	}
}
void CPCAChart::SetTimeBaseCurveProperty(CString curveName/*=_T("")*/,double timeFactor/*=1*/)
{

}
//设置动态时间曲线Y轴数据上下限,必须在动态绘图之前设定好此参数
void CPCAChart::SetRealTimeBaseCurveDataRange(double lower,double upper)  
{
	CFloatPoint point(lower,upper);
	CCurveObj::SetRealTimeBaseCurveDataRange(point);
}

CFloatPoint * CPCAChart::CreateNewPCAArray(CString curveName,size_t PCNum ,bool REF)
{
	CFloatPoint* pArray = NULL;

	//全体PCA
	//若为参考图谱PCA
	if(m_PCATotalArray.size()==0 &&(m_RefPCATotalArray.size() == 0))
	{
		CDC* pDc=GetDC();
		CSize CurveNameSize;
		CurveNameSize = pDc->GetTextExtent(curveName);
		
		m_CurveNameMaxSize=CurveNameSize;
		ReleaseDC(pDc);
	}
	if(REF)
	{
		pArray = new CFloatPoint[PCNum];
		if(m_RefName.size() == 0)
		{	
			COLORREF tmpColor = m_ArrayCurveColor[m_RefName.size()%m_ArrayCurveColor.GetCount()];
			m_RefColor[curveName] = tmpColor;
			m_RefName.insert(m_RefName.end(),curveName);
		}
		else
		{
			if(m_RefName[m_RefName.size() - 1] !=curveName)
			{			
				COLORREF tmpColor = m_ArrayCurveColor[m_RefName.size()%m_ArrayCurveColor.GetCount()];
				m_RefColor[curveName] = tmpColor;
				m_RefName.insert(m_RefName.end(),curveName);
			}
		}
		if(pArray)
		{
			m_RefPCATotalArray.insert(CMultiMapToFloatPtArray::value_type(curveName,pArray));
		}
		CDC* pDc=GetDC();
		CSize CurveNameSize;
		CurveNameSize = pDc->GetTextExtent(curveName);
		m_CurveNameMaxSize.cx=m_CurveNameMaxSize.cx>CurveNameSize.cx?m_CurveNameMaxSize.cx:CurveNameSize.cx;
		m_CurveNameMaxSize.cy=m_CurveNameMaxSize.cy>CurveNameSize.cy?m_CurveNameMaxSize.cy:CurveNameSize.cy;
		ReleaseDC(pDc);
		//}
	}
	else
	{
		//若已有曲线
		//if(m_PCATotalArray.find(curveName)==m_PCATotalArray.end())
		//{
			//将新曲线设为选中曲线
		pArray = new CFloatPoint[PCNum];
		//this->SetCurveSelected(curveName);
		if(pArray)
		{
			//curveObject->SetTimeBaseFlag(timeBaseFlag);
			//curveObject->SetCurveColor(m_SelectedCurveColor,2);
			m_PCATotalArray.insert(CMultiMapToFloatPtArray::value_type(curveName,pArray));
		}
		CDC* pDc=GetDC();
		CSize CurveNameSize;
		CurveNameSize = pDc->GetTextExtent(curveName);
		m_CurveNameMaxSize.cx=m_CurveNameMaxSize.cx>CurveNameSize.cx?m_CurveNameMaxSize.cx:CurveNameSize.cx;
		m_CurveNameMaxSize.cy=m_CurveNameMaxSize.cy>CurveNameSize.cy?m_CurveNameMaxSize.cy:CurveNameSize.cy;
		ReleaseDC(pDc);
		//}
	}
	if(m_bDrawLegend)
	{
		SetMargin();
	}
	return pArray;
}


void CPCAChart::SetShowParameter(int PCNum,int xPC,int yPC)
{
	PCANum = PCNum;   //显示的PCA个数
	//CString RefSpecType;  //参考光谱类别名
	//仅适用于只显示2个PC成分
	if(2 == PCANum)
	{
		XPC = xPC;       //x轴PC
		YPC = yPC;       //y轴PC
	}	
}
void CPCAChart::AdjustTotal2Show()
{
	if((m_PCATotalArray.size() == 0)&&(m_RefPCATotalArray.size() == 0))
	{
		m_CurveNameMaxSize=0;
	}
	if(2 == PCANum)
	{
		//用于显示的PCA曲线或点
		//先处理参考图谱PCA
		//清空	
		this->DestroyMapCurve(m_RefCurveMap);
		CMultiMapToFloatPtArray::iterator iterRef = m_RefPCATotalArray.begin();		
		while(iterRef!= m_RefPCATotalArray.end())
		{
			CCurveObj *curveObject = NULL;
			CString curveName;
			curveName = iterRef->first;
			curveObject = new CCurveObj(m_rectPlot);
			if(curveObject)
			{
				curveObject->SetTimeBaseFlag(0);
				m_RefCurveMap.insert(CMultiMapToCurveObj::value_type(curveName,curveObject));
			}
			CFloatPoint temp;
			temp.m_PosX = iterRef->second[(XPC-1)].m_PosY;
			temp.m_PosY = iterRef->second[(YPC-1)].m_PosY;

			curveObject->AppendNewPoint(temp);

			CDC* pDc=GetDC();
			CSize CurveNameSize;
			CurveNameSize = pDc->GetTextExtent(curveName);
			m_CurveNameMaxSize.cx=m_CurveNameMaxSize.cx>CurveNameSize.cx?m_CurveNameMaxSize.cx:CurveNameSize.cx;
			m_CurveNameMaxSize.cy=m_CurveNameMaxSize.cy>CurveNameSize.cy?m_CurveNameMaxSize.cy:CurveNameSize.cy;
			ReleaseDC(pDc);
			++iterRef;
		}
		//处理对比图谱PCA
		this->DestroyMapCurve(m_CurveMap);
		CCurveObj* curveObject = NULL;
		CString curveName;

		CMultiMapToFloatPtArray::iterator iter=  m_PCATotalArray.begin();
		while(iter!= m_PCATotalArray.end())
		{
			//m_PCATotalMap.GetNextAssoc(pos,curveName, (CObject*&)curveObject);
			CCurveObj* CurveObject =NULL;
			CurveObject = new CCurveObj(m_rectPlot);
			if(CurveObject)
			{
				CurveObject->SetTimeBaseFlag(0);
				m_CurveMap.insert(make_pair(iter->first,CurveObject));
			}
			CFloatPoint temp;
			temp.m_PosX = iter->second[(XPC-1)].m_PosY;
			temp.m_PosY = iter->second[(YPC-1)].m_PosY;

			CurveObject->AppendNewPoint(temp,1);

			CDC* pDc=GetDC();
			CSize CurveNameSize;
			CurveNameSize = pDc->GetTextExtent(curveName);
			m_CurveNameMaxSize.cx=m_CurveNameMaxSize.cx>CurveNameSize.cx?m_CurveNameMaxSize.cx:CurveNameSize.cx;
			m_CurveNameMaxSize.cy=m_CurveNameMaxSize.cy>CurveNameSize.cy?m_CurveNameMaxSize.cy:CurveNameSize.cy;
			ReleaseDC(pDc);
			++iter;
		}  
	}
	else
	{
		//用于显示的PCA曲线或点
		//先处理参考图谱PCA
		//清空	
		this->DestroyMapCurve(m_RefCurveMap);
		CMultiMapToFloatPtArray::iterator iterRef = m_RefPCATotalArray.begin();		
		while(iterRef!= m_RefPCATotalArray.end())
		{
			CCurveObj *curveObject = NULL;
			CString curveName;
			curveName = iterRef->first;
			curveObject = new CCurveObj(m_rectPlot);
			if(curveObject)
			{
				curveObject->SetTimeBaseFlag(0);
				m_RefCurveMap.insert(CMultiMapToCurveObj::value_type(iterRef->first,curveObject));
			}

			for(int i = 0; i<PCANum;++i)
			{
				CFloatPoint temp;
				temp.m_PosX = i+1;
				temp.m_PosY = iterRef->second[i].m_PosY;
				curveObject->AppendNewPoint(temp,1);
			}
			CDC* pDc=GetDC();
			CSize CurveNameSize;
			CurveNameSize = pDc->GetTextExtent(curveName);
			m_CurveNameMaxSize.cx=m_CurveNameMaxSize.cx>CurveNameSize.cx?m_CurveNameMaxSize.cx:CurveNameSize.cx;
			m_CurveNameMaxSize.cy=m_CurveNameMaxSize.cy>CurveNameSize.cy?m_CurveNameMaxSize.cy:CurveNameSize.cy;
			ReleaseDC(pDc);
			++iterRef;
		}
		//处理对比图谱PCA
		this->DestroyMapCurve(m_CurveMap);

		CCurveObj* curveObject = NULL;
		CString curveName;

		CMultiMapToFloatPtArray::iterator iter=  m_PCATotalArray.begin();
		while(iter!= m_PCATotalArray.end())
		{
			//m_PCATotalMap.GetNextAssoc(pos,curveName, (CObject*&)curveObject);
			CCurveObj* CurveObject =NULL;
			//curveName = iter->first;
			CurveObject = new CCurveObj(m_rectPlot);
			if(CurveObject)
			{
				CurveObject->SetTimeBaseFlag(0);
				m_CurveMap.insert(make_pair(iter->first,CurveObject));
			}
			for(int i = 0; i< PCANum ;++i)
			{
				CFloatPoint temp;
				temp.m_PosX = i+1;
				temp.m_PosY = iter->second[i].m_PosY;
				CurveObject->AppendNewPoint(temp,1);
			}
			CDC* pDc=GetDC();
			CSize CurveNameSize;
			CurveNameSize = pDc->GetTextExtent(curveName);
			m_CurveNameMaxSize.cx=m_CurveNameMaxSize.cx>CurveNameSize.cx?m_CurveNameMaxSize.cx:CurveNameSize.cx;
			m_CurveNameMaxSize.cy=m_CurveNameMaxSize.cy>CurveNameSize.cy?m_CurveNameMaxSize.cy:CurveNameSize.cy;
			ReleaseDC(pDc);
			++iter;
		} 
	}
	this->ReCalculateAllCurveMaxMin();
	this->ResetAllCurveMaxMin(m_ptMaxPos,m_ptMinPos);
	if(m_bDrawLegend)
	{
		SetMargin();
	}

}

BOOL CPCAChart::DestroyMapCurve(CMapStringToOb &m_CurveMap)
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

BOOL CPCAChart::DestroyMapCurve(CMultiMapToCurveObj &RefPCATotalMap)
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

BOOL CPCAChart::DestroyArrayCurve(CMultiMapToFloatPtArray &RefPCATotalArray)
{
	CMultiMapToFloatPtArray::iterator iter = RefPCATotalArray.begin();
	CFloatPoint* pArray = NULL;
	while(iter != RefPCATotalArray.end())
	{
		pArray = iter->second;
		//curveObject->DestroyCurve();
		delete [] pArray;
		pArray = NULL;
		++iter;
	}
	RefPCATotalArray.clear();
	return TRUE;
}

BOOL CPCAChart::DestroyCurve(CString curveName/* = _T("\0")*/)
{
	BOOL rc = FALSE;
	CCurveObj* curveObject = NULL;
	if(curveName.IsEmpty())  //删除所有曲线
	{
		CMultiMapToCurveObj::iterator iter = m_CurveMap.begin();
		CCurveObj* curveObject = NULL;
		while(iter != m_CurveMap.end())
		{
			curveObject = iter->second;
			curveObject->DestroyCurve();
			//delete [] pArray;
			curveObject = NULL;
			++iter;
		}
		m_CurveMap.clear();
	}
	else
	{
		CMultiMapToCurveObj::iterator iter;
		;// find(m_CurveMap.begin(),;
		while( (iter =m_CurveMap.find(curveName))!=m_CurveMap.end())//m_CurveMap.Lookup(curveName,(CObject*&)curveObject))
		{
			curveObject = iter->second;
			delete curveObject;
			curveObject = NULL;
			m_CurveMap.erase(iter);
			
		}
		rc = TRUE;
	}
	//GetMaxDataRect();
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

bool CPCAChart::AddSelectedCurve(CString curveName)
{
	m_SetSelCurveName.insert(curveName);
	this->InvalidateChart();
	return true;
}

void CPCAChart::SetMargin(int xMargin/*=30*/,int yMargin/* = 20*/)
{
	if(xMargin>0 && yMargin >0 )
	{
		//重新修改作图区大小
		if(m_bDrawLegend&&m_CurveNameMaxSize.cx>0)
		{
			m_rectPlot = m_rectClient;
			m_ptMargin = CPoint(xMargin,yMargin);
			m_rectPlot.DeflateRect(m_ptMargin.x*2+30+m_CurveNameMaxSize.cx/2,m_ptMargin.y);  
			m_rectPlot.OffsetRect(m_ptMargin.x-30-m_CurveNameMaxSize.cx/2,-m_ptMargin.y/2);//真正的绘图区域
		}
		else
		{
			m_rectPlot = m_rectClient;
			m_ptMargin = CPoint(xMargin,yMargin);
			m_rectPlot.DeflateRect(m_ptMargin.x*2,m_ptMargin.y);  
			m_rectPlot.OffsetRect(m_ptMargin.x,-m_ptMargin.y/2);//真正的绘图区域

		}
		this->InvalidateChart();
	}
}

//曲线单击测试，看选中的是那条曲线

void CPCAChart::MoveCurve(CString curveName /*= _T("\0")*/,int moveType /*= MOVE_RESET*/,double percent /*= 5*/)
{
	if(m_sCurrentControlCurve != curveName)
		m_sCurrentControlCurve = curveName;
	if(curveName.IsEmpty())  //所有曲线都进行移动操作
	{
		CString _curveName;
		CCurveObj* curveObject = NULL;

		CMultiMapToCurveObj::iterator iter1 = m_CurveMap.begin();
		while(iter1!= m_CurveMap.end())
		{
			curveObject = iter1->second;
			if(curveObject)
			{
				//IndividualCurveMove(_curveName,moveType,percent);
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
		CMultiMapToCurveObj::iterator iter2 = m_RefCurveMap.begin();
		while(iter2!= m_RefCurveMap.end())
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
//曲线缩放
//##ModelId=48A145890040
void CPCAChart::ZoomCurve(CString curveName /*= _T("\0")*/,int zoomType /*= ZOOM_RESET*/,double percent /*= 5*/)
{

}

//##ModelId=48A145880222
void CPCAChart::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	this->SetFocus();
	if(m_rectPlot.PtInRect(point))
	{
		if(!m_bLeftButtonDown)
		{
			m_recMouseSelect.TopLeft() = point;
			m_Line.m_Begin=point;


			CMultiMapToCurveObj::iterator iter = m_CurveMap.begin();
			CString _curveName;
			CCurveObj* curveObject = iter->second;

			if(curveObject)
			{
				m_BeginPoint = curveObject->Position2Data(m_Line.m_Begin,m_Line.m_Begin); //将初始点置于图谱上
	
				m_bLeftButtonDown = true;
			}
		}
	}
	CWnd::OnLButtonDown(nFlags, point);
}

//##ModelId=48A145880233
void CPCAChart::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	if(m_rectPlot.PtInRect(point))
	{
		//不需要左键按下的操作m_iControlMode == CURVE_MOVE |||| m_iControlMode == CURVE_SELECT 
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
					CMultiMapToCurveObj::iterator iter = m_CurveMap.begin();
					CCurveObj* curveObject = iter->second;
					CFloatPoint _PointEnd;
					if(curveObject)
					{
						m_EndPoint = curveObject->Position2Data(m_Line.m_End,m_Line.m_End);  //将结束点置于图谱上
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
		//需要左键按下的操作
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
					//::SetCursor(AfxGetApp()->LoadCursor(IDC_CURSORMOVE));
					m_recMouseSelect.BottomRight() = point;
					if(m_sCurrentControlCurve.IsEmpty())
					{

						CString curveName;
						CCurveObj* curveObject = NULL;
						//POSITION pos = m_CurveMap.GetStartPosition();
						CMultiMapToCurveObj::iterator iter1 = m_CurveMap.begin();
						while(iter1!= m_CurveMap.end())
						{
							curveObject = iter1->second;
							if(curveObject)
							{
								this->IndividualCurveControl(curveObject,m_iControlMode,m_recMouseSelect);
							}
							++iter1;
						}
						CMultiMapToCurveObj::iterator iter = m_RefCurveMap.begin();
						while(iter!= m_RefCurveMap.end())
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

//##ModelId=48A145880251
void CPCAChart::OnLButtonUp(UINT nFlags, CPoint point) 
{
	if(m_rectPlot.PtInRect(point) )
	{
		if(m_bLeftButtonDown)
		{
			m_recMouseSelect.BottomRight() = point;

			m_Line.m_End = point;

			CString curveName;
			CCurveObj* curveObject = NULL;
			
			CMultiMapToCurveObj::iterator iter1 = m_CurveMap.begin();
			curveObject = iter1->second;

			//m_CurveMap.GetNextAssoc(pos,curveName, (CObject*&)curveObject);

			
			if(curveObject)
			{
				curveObject->Position2Data(point);
			}

			if(m_sCurrentControlCurve.IsEmpty())
			{
				CString curveName;
				CCurveObj* curveObject = NULL;
				CMultiMapToCurveObj::iterator iter1 = m_CurveMap.begin();
				while(iter1!=m_CurveMap.end())
				{
					curveObject = iter1->second;
					if(curveObject)
					{
						this->IndividualCurveControl(curveObject,m_iControlMode,m_recMouseSelect);
					}
					++iter1;
				}
				CMultiMapToCurveObj::iterator iter = m_RefCurveMap.begin();
				while(iter!= m_RefCurveMap.end())
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

//##ModelId=48A145880263
BOOL CPCAChart::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt) 
{
	CString curveName;
	CCurveObj* curveObject = NULL;
	CMultiMapToCurveObj::iterator iter1 = m_CurveMap.begin();
	while(iter1!=m_CurveMap.end())
	{
		curveObject = iter1->second;
		if(curveObject)
		{
			IndividualCurveWheelContol(curveObject,m_iControlMode,m_iVerticalControlMode,zDelta,pt);
		}
		++iter1;
	}
	CMultiMapToCurveObj::iterator iter = m_RefCurveMap.begin();
	while(iter!= m_RefCurveMap.end())
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
//##ModelId=48A1458900BB
void CPCAChart::IndividualCurveWheelContol(CCurveObj* curveObj/* = NULL*/,int controlMode/* = CURVE_MOVE*/,bool verticalControl/* = false*/,int zDelta/* = 0*/,CPoint point)
{
	if(curveObj == NULL)
		return ;
	switch(controlMode)
	{
	case CURVE_SELECT:
	case CURVE_MOVE:
		{
			if(!verticalControl)  //垂直移动
			{
				if(zDelta>0)
					curveObj->Move(2,0);
				else
					curveObj->Move(-2,0);
			}
			else//水平移动
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
			if(!verticalControl)  //垂直缩放
			{
				if(zDelta>0)
					curveObj->ZoomIn(2,0);
				else
					curveObj->ZoomIn(-2,0);
			}
			else  //水平缩放
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
//##ModelId=48A1458900DA
void CPCAChart::IndividualCurveControl(CCurveObj* curveObj/* = NULL*/,int controlMode /*= CURVE_MOVE*/,CRect controlRect)
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
			if(controlRect.TopLeft() == controlRect.BottomRight()) //如果是单击左键,则放大固定的区域
			{
				controlRect.InflateRect(150,150);
			}
			curveObj->MouseSelectZoomIn(controlRect); //use mouse to ZoomIn the curve
			break;
		}
	case CURVE_ZOOM_OUT:
		{
			if(controlRect.TopLeft() == controlRect.BottomRight()) //如果是单击左键,则缩小固定的区域
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
BOOL CPCAChart::GetMaxDataRect(CString curveName/*=_T("\0")*/)  //获得所有曲线中最大的数据区域值
{	
	BOOL rc = FALSE;
	CCurveObj* curveObject = NULL;

	if(curveName.IsEmpty())  //
	{
		CCurveObj* curveObject = NULL;
		CMultiMapToCurveObj::iterator iter = m_CurveMap.begin();
		if(iter!=m_CurveMap.end())
		{
			curveObject = iter->second;
			if(curveObject)
			{
				curveObject->GetDrawCurvePar(m_ptMaxPos,m_ptMinPos,m_ptMoveOffset,m_ptZoomOffset);
				//如果是进行缩放，则直接返回
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
		iter = m_RefCurveMap.begin();
		if(iter!=m_RefCurveMap.end())
		{
			curveObject = iter->second;
			if(curveObject)
			{
				curveObject->GetDrawCurvePar(m_ptMaxPos,m_ptMinPos,m_ptMoveOffset,m_ptZoomOffset);
				//如果是进行缩放，则直接返回
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

	}
	return rc;
}
int CPCAChart::ReCalculateAllCurveMaxMin()
{
	CCurveObj* curveObject = NULL;

	m_ptMaxPos.InitPoint(-10000000000.0,-10000000000.0);//当前最大点
	m_ptMinPos.InitPoint(10000000000.0,10000000000.0);//当前最小点

	CFloatPoint MaxPos;
	CFloatPoint MinPos;

	CDC *pDc=GetDC();
	CSize CurveNameSize;

	//对比图谱PCA
	CString _curveName;
	CMultiMapToCurveObj::iterator iter1 = m_CurveMap.begin();
	while(iter1!=m_CurveMap.end())
	{
		curveObject = iter1->second;
		if(curveObject)
		{
			//curveObject->ReCalculateMaxMinPar();
			curveObject->GetDrawCurvePar(MaxPos,MinPos,m_ptMoveOffset,m_ptZoomOffset);
			this->CalculateMaxPoint(m_ptMaxPos,&MaxPos);
			this->CalculateMinPoint(m_ptMinPos,&MinPos);

			//计算曲线图例间隙
			CurveNameSize = pDc->GetTextExtent(_curveName);
			m_CurveNameMaxSize.cx=m_CurveNameMaxSize.cx>CurveNameSize.cx?m_CurveNameMaxSize.cx:CurveNameSize.cx;
			m_CurveNameMaxSize.cy=m_CurveNameMaxSize.cy>CurveNameSize.cy?m_CurveNameMaxSize.cy:CurveNameSize.cy;
		}
		++iter1;
	}
	//参考图谱
	CMultiMapToCurveObj::iterator iter = m_RefCurveMap.begin();
	while(iter!=m_RefCurveMap.end())
	{
		curveObject = iter->second;
		if(curveObject)
		{
			//curveObject->ReCalculateMaxMinPar();
			curveObject->GetDrawCurvePar(MaxPos,MinPos,m_ptMoveOffset,m_ptZoomOffset);
			this->CalculateMaxPoint(m_ptMaxPos,&MaxPos);
			this->CalculateMinPoint(m_ptMinPos,&MinPos);

			//计算曲线图例间隙
			CurveNameSize = pDc->GetTextExtent(_curveName);
			m_CurveNameMaxSize.cx=m_CurveNameMaxSize.cx>CurveNameSize.cx?m_CurveNameMaxSize.cx:CurveNameSize.cx;
			m_CurveNameMaxSize.cy=m_CurveNameMaxSize.cy>CurveNameSize.cy?m_CurveNameMaxSize.cy:CurveNameSize.cy;
		}
		++iter;
	}
	if(PCANum == 2)
	{
		double X_range = (m_ptMaxPos.m_PosX-m_ptMinPos.m_PosX);
		double Y_range = (m_ptMaxPos.m_PosY-m_ptMinPos.m_PosY);
		m_ptMaxPos.m_PosX += X_range/10;
		m_ptMaxPos.m_PosY += Y_range/10;

		m_ptMinPos.m_PosX -= X_range/10;
		m_ptMinPos.m_PosY -= Y_range/10;
	}

	//对比图谱PCA
	m_ptMaxPosConst = m_ptMaxPos;
	m_ptMinPosConst = m_ptMinPos;

	return 0;
}
int CPCAChart::ResetAllCurveMaxMin(CFloatPoint MaxPos,CFloatPoint MinPos)
{
	CCurveObj* curveObject = NULL;

	CMultiMapToCurveObj::iterator iter1 = m_CurveMap.begin();
	while(iter1!=m_CurveMap.end())
	{
		curveObject = iter1->second;
		if(curveObject)
		{
			curveObject->SetCurveMaxMinPar(MaxPos,MinPos);
		}
		++iter1;
	}
	//参考图谱
	CMultiMapToCurveObj::iterator iter = m_RefCurveMap.begin();
	while(iter!=m_RefCurveMap.end())
	{
		curveObject = iter->second;
		if(curveObject)
		{
			//curveObject->SetAllCurveMaxMinPar(m_ptMaxPos,m_ptMinPos);
			curveObject->SetCurveMaxMinPar(MaxPos,MinPos);
		}
		++iter;
	}
	return  0;
}

//##ModelId=48A14589006E
void CPCAChart::CalculateMaxPoint(CFloatPoint& pointDes,const CFloatPoint* PointSource)
{
	if(PointSource->m_PosX > pointDes.m_PosX)
		pointDes.m_PosX = PointSource->m_PosX;
	if(PointSource->m_PosY > pointDes.m_PosY)
		pointDes.m_PosY = PointSource->m_PosY;
}
//##ModelId=48A14589008D
void CPCAChart::CalculateMinPoint(CFloatPoint& pointDes,const CFloatPoint* PointSource)
{
	if(PointSource->m_PosX < pointDes.m_PosX)
		pointDes.m_PosX = PointSource->m_PosX;
	if(PointSource->m_PosY < pointDes.m_PosY)
		pointDes.m_PosY = PointSource->m_PosY;
}
//##ModelId=48A145880282
void CPCAChart::OnRButtonDown(UINT nFlags, CPoint point) 
{
	this->SetFocus();  //获得焦点
	CWnd::OnRButtonDown(nFlags, point);
}

//##ModelId=48A1458802A0
void CPCAChart::OnContextMenu(CWnd* pWnd, CPoint point) 
{
	if(!m_bShowMenum)
		return;

	CPoint _Point = point;
	this->ScreenToClient(&_Point);
	if(m_rectPlot.PtInRect(_Point))
	{
		CString str;
		SetForegroundWindow();

		CMenu menu;
		CCmdUI cmdUI;
		menu.CreatePopupMenu();

		menu.AppendMenu(MF_STRING,10008,L"普通鼠标");
		menu.AppendMenu(MF_STRING,10006,L"移动");
		menu.AppendMenu(MF_STRING,10002,L"鼠标控制放大");
		menu.AppendMenu(MF_STRING,10003,L"鼠标控制缩小");

		menu.AppendMenu(MF_STRING,10007,L"复位");


		for(int n =0;n<menu.GetMenuItemCount();n++)  //目前只考虑到两层菜单的控制，需要控制更多层，可以考虑使用递归方式
		{   
			CMenu* pSubMenu = menu.GetSubMenu(n); 	
			cmdUI.m_nIndexMax = n+1;  //此处必须加1，否则会出现断言错误
			if(pSubMenu!=NULL)  // pop-up menu
			{
				cmdUI.m_nIndexMax =pSubMenu->GetMenuItemCount(); 
				for(int i =  0; i< cmdUI.m_nIndexMax;i++)   
				{   
					cmdUI.m_nIndex = i;   
					cmdUI.m_nID = pSubMenu->GetMenuItemID(i);   
					//	pSubMenu->EnableMenuItem(cmdUI.m_nID,MF_BYCOMMAND|MF_DISABLED|MF_GRAYED);
					cmdUI.m_pMenu = pSubMenu;   
					cmdUI.DoUpdate(this,FALSE);   
				}   
			}
			else
			{

				cmdUI.m_nIndex = n;   
				cmdUI.m_nID = menu.GetMenuItemID(n);   
				//	menu.EnableMenuItem(cmdUI.m_nID,MF_BYCOMMAND|MF_DISABLED|MF_GRAYED);
				cmdUI.m_pMenu = &menu;  

				BOOL ret=cmdUI.DoUpdate(this,FALSE);  
			}
		} 

		menu.TrackPopupMenu(TPM_LEFTALIGN|TPM_BOTTOMALIGN|TPM_NONOTIFY,point.x,point.y,this);
	}
}

//##ModelId=48A1458801A5
BOOL CPCAChart::OnCommand(WPARAM wParam, LPARAM lParam) 
{
	int menuID = LOWORD(wParam);
	switch(menuID)
	{
	case 9998://在独立坐标系于公共坐标系间切换
		{	
			m_bAllCurveSameBase = !m_bAllCurveSameBase;
			this->SetAllCurveDrawMode(m_bAllCurveSameBase);
			this->InvalidateChart();
			break;
		}
	case 9999:  //取数据模式
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
	case 10007:  //复位所有曲线
		{
			m_sCurrentControlCurve.Empty();
			m_pCurSelCurveObj = NULL;
			this->MoveCurve(m_sCurrentControlCurve,CURVE_RESET);
			this->SetFocus();
			break;
		}
	case 10008:  //选择单条曲线
		{
			m_iControlMode = CURVE_SELECT;
			::SendMessage(this->GetParent()->m_hWnd,WM_PCACHANGECHARTMODE,m_iControlMode,0);
			break;
		}
	case 10001:  //select all curve
		{
			m_iControlMode = CURVE_SELECT;
			this->InvalidateChart();
			break;
		}
	case 10006:  //移动曲线，可通过鼠标，方向键，鼠标滚轮控制曲线的移动
		{ 
			m_iControlMode = CURVE_MOVE;
			::SendMessage(this->GetParent()->m_hWnd,WM_PCACHANGECHARTMODE,m_iControlMode,0);
			break;
		}
	case 10002: //曲线放大
		{
			m_iControlMode = CURVE_ZOOM_IN;
			::SendMessage(this->GetParent()->m_hWnd,WM_PCACHANGECHARTMODE,m_iControlMode,0);
			break;
		}
	case 10003: //曲线缩小
		{
			m_iControlMode = CURVE_ZOOM_OUT;
			::SendMessage(this->GetParent()->m_hWnd,WM_PCACHANGECHARTMODE,m_iControlMode,0);
			break;
		}
	case 10004: //水平控制模式
		{
			m_iVerticalControlMode = false;
			break;
		}
	case 10005:  //垂直控制模式
		{
			m_iVerticalControlMode = true;
			break;
		}
	case 10009:  //x轴绘图方向
		{
			m_bDrawReverseX = !m_bDrawReverseX;
			CCurveObj::SetCurveDirction(m_bDrawReverseX,m_bDrawReverseY);
			this->InvalidateChart();
			break;
		}
	case 10010:  //Y绘图方向
		{  
			m_bDrawReverseY = !m_bDrawReverseY;
			CCurveObj::SetCurveDirction(m_bDrawReverseX,m_bDrawReverseY);
			this->InvalidateChart();
			break;
		}
	case 10011:  //特征选择
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
//##ModelId=48A1458802CF
void CPCAChart::OnUpdateMenuState(CCmdUI* pCmdUI)//更新菜单上的状态显示
{
	switch(pCmdUI->m_nID)
	{ 
	case 9998:
		{
			if(m_bAllCurveSameBase)
			{
				pCmdUI->SetCheck(FALSE);
			}
			else
			{
				pCmdUI->SetCheck(TRUE);
			}
			break;
		}
	case 9999:
		{
			if(m_iControlMode == CURVE_GET_DATA)
			{
				pCmdUI->SetCheck(TRUE);
			}
			else
			{
				pCmdUI->SetCheck(FALSE);
			}
			break;
		}
	case 10000: //reset the curve
		{
			break;
		}
	case 10008://select the curve
		{
			if(m_iControlMode == CURVE_SELECT)
			{
				pCmdUI->SetCheck(TRUE);
			}
			else
			{
				pCmdUI->SetCheck(FALSE);
			}
			break;
		}
	case 10007:
		{
			break;
		}
	case 10001: 
		{
			break;
		}
	case 10006:
		{
			if(	m_iControlMode == CURVE_MOVE ||
				m_iControlMode == CURVE_MOVE_UP ||
				m_iControlMode == CURVE_MOVE_DOWN ||
				m_iControlMode == CURVE_MOVE_LEFT ||
				m_iControlMode == CURVE_MOVE_RIGHT)
			{
				pCmdUI->SetCheck(TRUE);
			}
			else
			{
				pCmdUI->SetCheck(FALSE);
			}
			break;
		}
	case 10002:  //ZoomIn
		{
			if(m_iControlMode == CURVE_ZOOM_IN)
			{
				pCmdUI->SetCheck(TRUE);
			}
			else
			{
				pCmdUI->SetCheck(FALSE);
			}
			break;
		}
	case 10003:  //ZoomOut
		{
			if(m_iControlMode == CURVE_ZOOM_OUT)
			{
				pCmdUI->SetCheck(TRUE);
			}
			else
			{
				pCmdUI->SetCheck(FALSE);
			}
			break;
		}
	case 10011:
		{
			if(m_iControlMode == CURVE_FEATURE_SELECT)
			{
				pCmdUI->SetCheck(TRUE);
			}
			else
			{
				pCmdUI->SetCheck(FALSE);
			}
			break;
		}
	case 10004:  //水平控制模式
		{
			if(!m_iVerticalControlMode)
			{
				pCmdUI->SetCheck(TRUE);
			}
			else
			{
				pCmdUI->SetCheck(FALSE);
			}
			break;
		}
	case 10005:
		{
			if(!m_iVerticalControlMode)
			{
				pCmdUI->SetCheck(FALSE);
			}
			else
			{
				pCmdUI->SetCheck(TRUE);
			}
			break;
		}
	case 10009:  //x轴绘图方向
		{
			if(!m_bDrawReverseX)
			{
				pCmdUI->SetCheck(TRUE);
			}
			else
			{
				pCmdUI->SetCheck(FALSE);
			}
			break;
		}
	case 10010:  //Y绘图方向
		{  
			if(!m_bDrawReverseY)
			{
				pCmdUI->SetCheck(TRUE);
			}
			else
			{
				pCmdUI->SetCheck(FALSE);
			}
			break;
		}

	}
}

//##ModelId=48A145880196
BOOL CPCAChart::PreTranslateMessage(MSG* pMsg) 
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

//##ModelId=48A1458802BF
void CPCAChart::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	ResetCurve();
	this->SetFocus();

	CWnd::OnLButtonDblClk(nFlags, point);
}

//##ModelId=48A1458801B7
LRESULT CPCAChart::DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	return CWnd::DefWindowProc(message, wParam, lParam);
}

//##ModelId=48A1458901E4
int CPCAChart::FloatDataPrecise(double f)
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
/*void CPCAChart::DrawSelectFocus(CDC* pDc,CPoint&  point)
{
	CString _curveName;
	CCurveObj* curveObject = NULL;
	POSITION pos = m_CurveMap.GetStartPosition();
	CPoint minPoint,tempPoint,scrPoint(0,0);
	double minVal=point.y;
	double _minVal = 0.0;
	COLORREF color;
	while(pos)
	{
		m_CurveMap.GetNextAssoc(pos,_curveName, (CObject*&)curveObject);
		if(curveObject)
		{
			curveObject->Position2Data(point,scrPoint);
			tempPoint = point - scrPoint;
			_minVal = sqrt((double)(tempPoint.x*tempPoint.x+tempPoint.y*tempPoint.y));
			if(_minVal < minVal)
			{	
				minVal = _minVal;
				minPoint = scrPoint;
				color  = curveObject->GetCurveColor();

			}
		}
	}
	this->DrawDataFocus(pDc,minPoint,color);
}*/
void CPCAChart::DrawZoomRect(CDC* pDc,CRect rect)
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
void CPCAChart::DrawSelectRange(CDC *pDc,CLine Line)
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
				//画垂直线
				_begin.x = Line.m_Begin.x;
				_begin.y = m_rectPlot.top;
				_end.x = Line.m_End.x;
				_end.y = m_rectPlot.bottom;
				pDc->MoveTo(_begin);
				pDc->LineTo(_end);
				pDc->SelectObject(oldPen);
				return;
			}
			//计算曲线斜率
			m_Slope = ((double)(Line.m_Begin.y-Line.m_End.y))/(Line.m_Begin.x-Line.m_End.x);

			//计算初始点的位置
			_begin.x = m_rectPlot.left;
			_begin.y = (LONG)(m_Slope*(_begin.x - Line.m_Begin.x)+Line.m_Begin.y);

			if(_begin.y > m_rectPlot.bottom)//说明初始点应该在底边
			{
				//重新计算初始点
				_begin.y = m_rectPlot.bottom;
				_begin.x = (LONG)((_begin.y - Line.m_Begin.y)/m_Slope+Line.m_Begin.x);
			}
			else if(_begin.y < m_rectPlot.top)  //说明初始点应该在顶部
			{
				//重新计算初始点
				_begin.y = m_rectPlot.top;
				_begin.x = (LONG)((_begin.y - Line.m_Begin.y)/m_Slope+Line.m_Begin.x);	
			}
			//计算结束点位置
			_end.x = m_rectPlot.right;
			_end.y = (LONG)(m_Slope*(_end.x - Line.m_End.x)+Line.m_End.y);

			if(_end.y > m_rectPlot.bottom)//说明初始点应该在底边
			{
				//重新计算结束点
				_end.y = m_rectPlot.bottom;
				_end.x = (LONG)((_end.y - Line.m_End.y)/m_Slope+Line.m_End.x);	

			}
			else if(_end.y < m_rectPlot.top)  //说明结束点应该在顶部
			{
				//重新计算结束点
				_end.y = m_rectPlot.top;
				_end.x = (LONG)((_end.y - Line.m_End.y)/m_Slope+Line.m_End.x);	
			}
		}
		else
		{
			//若初始点和结束点一样
			_begin.x = 0;
			_begin.y = Line.m_Begin.y;
			_end.x = 0;
			_end.y = Line.m_Begin.y;
		}




		pDc->MoveTo(_begin);
		pDc->LineTo(_end);
		//画竖线
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
void CPCAChart::DrawSelectRangeText(CDC* pDc)
{
	//画起始位置
	CPoint point;
	CPoint srcPoint;
	GetCursorPos(&point);
	this->ScreenToClient(&point);

	CPen *oldPen;
	CPen newPen(PS_SOLID,1,RGB(0,0,0));
	oldPen = pDc->SelectObject(&newPen);

	CMultiMapToCurveObj::iterator iter = m_CurveMap.begin();
	//POSITION pos = m_CurveMap.GetStartPosition();
	CString _curveName;
	CCurveObj* curveObject = NULL;

	CFloatPoint _PointBegin;
	CFloatPoint _PointEnd;

	if(iter!=m_CurveMap.end())
	{
		curveObject = iter->second;
	}
	
	//m_CurveMap.GetNextAssoc(pos,_curveName, (CObject*&)curveObject);

	if(this->m_bLeftButtonDown)
	{
		if(curveObject)
		{
			_PointBegin = curveObject->Position2Data(m_Line.m_Begin,srcPoint);
			_PointEnd = curveObject->Position2Data(point,srcPoint);
		}

		CString tempStrX;
		CString tempStrY;
		tempStrX.Format(L"起始位置:%.2lf",_PointBegin.m_PosX);

		tempStrY.Format(L"结束位置:%.2lf",_PointEnd.m_PosX);


		CSize textSizeX = pDc->GetTextExtent(tempStrX); 
		CSize textSizeY = pDc->GetTextExtent(tempStrY); 



		pDc->SetTextColor(RGB(0,0,0));
		COLORREF bkColor = pDc->SetBkColor(m_crPlotAreaBackColor);
		int oldmode = pDc->SetBkMode(TRANSPARENT);
		int Maxtextsize = textSizeX.cx>textSizeY.cx?textSizeX.cx:textSizeY.cx;

		if( (point.x+ Maxtextsize) < m_rectPlot.right)
		{
			pDc->SetTextAlign (TA_LEFT|TA_TOP);//显示文字方向从左向右

			if(point.y+5+textSizeX.cy+2+textSizeX.cy> m_rectPlot.bottom)//超出作图范围
			{
				pDc->TextOut(point.x+15,m_rectPlot.bottom -2*textSizeX.cy-2,tempStrX);
				pDc->TextOut(point.x+15,m_rectPlot.bottom -textSizeX.cy,tempStrY);
			}
			else
			{
				pDc->TextOut(point.x+15,point.y+5,tempStrX);
				pDc->TextOut(point.x+15,point.y+5+textSizeX.cy+2,tempStrY);
			}

		}
		else
		{
			pDc->SetTextAlign (TA_RIGHT|TA_TOP);//显示文字方向从右向左
			if(point.y+5+textSizeX.cy+2+textSizeX.cy> m_rectPlot.bottom)//超出作图范围
			{
				pDc->TextOut(point.x+15,m_rectPlot.bottom -2*textSizeX.cy-2,tempStrX);
				pDc->TextOut(point.x+15,m_rectPlot.bottom -textSizeX.cy,tempStrY);
			}
			else
			{
				pDc->TextOut(point.x+15,point.y+5,tempStrX);
				pDc->TextOut(point.x+15,point.y+5+textSizeX.cy+2,tempStrY);
			}
		}
		pDc->SetBkMode(oldmode);
		pDc->SetBkColor(bkColor);

	}
	else
	{
		if(curveObject)
		{
			_PointBegin = curveObject->Position2Data(point,srcPoint);
			//_PointEnd = curveObject->Position2Data(point,srcPoint);
		}

		COLORREF textColor = RGB(0,0,0);
		//this->DrawDataFocus(pDc,srcPoint,textColor);

		CString tempStr;
		tempStr.Format(L"起始位置:%.2lf",_PointBegin.m_PosX);

		CSize textSizeX = pDc->GetTextExtent(tempStr); 

		pDc->SetTextColor(textColor);
		COLORREF bkColor = pDc->SetBkColor(m_crPlotAreaBackColor);
		int oldmode = pDc->SetBkMode(TRANSPARENT);

		if( point.x+ textSizeX.cx < m_rectPlot.right)
		{
			pDc->SetTextAlign (TA_LEFT|TA_TOP);//显示文字方向从左向右
			if(point.y+5+textSizeX.cy+2+textSizeX.cy> m_rectPlot.bottom)//超出作图范围
			{
				pDc->TextOut(point.x+15,m_rectPlot.bottom -textSizeX.cy,tempStr);
			}
			else
			{
				pDc->TextOut(point.x+15,point.y+5+textSizeX.cy+2,tempStr);
			}
		}
		else
		{
			pDc->SetTextAlign (TA_RIGHT|TA_TOP);//显示文字方向从右向左
			pDc->TextOut(point.x,point.y+5,tempStr);
		}
		pDc->SetBkMode(oldmode);
		pDc->SetBkColor(bkColor);
	}
	//画结束位置
	pDc->SelectObject(oldPen);

}
void CPCAChart::DrawNearestPt2Cursor(CDC *pDc,CPoint _point)
{
	CPen zoomRectPen(PS_SOLID,2,RGB(0,0,255));
	CPen* oldPen = pDc->SelectObject(&zoomRectPen);

	pDc->Rectangle(_point.x-2,_point.y-2,_point.x+2,_point.y+2);
	pDc->SelectObject(oldPen);
}

void CPCAChart::PreSubclassWindow() 
{
	// TODO: Add your specialized code here and/or call the base class

	CWnd::PreSubclassWindow();
}
void CPCAChart::DrawDataFocus(CDC *pDC,CPoint point,COLORREF color)
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
//设置是独立坐标系还是公用坐标系,每调用一次状态将发生翻转，默认是独立坐标系
void CPCAChart::ChangeToIndividualCoordinateMode(bool individualMode/* = true*/)
{
	m_bAllCurveSameBase = !individualMode;
	this->SetAllCurveDrawMode(m_bAllCurveSameBase);
}

void CPCAChart::ChangeToGetDataMode()  //让对象切换到取数据模式，其他模式将被屏蔽
{
	WPARAM controlCode = MAKELONG(9999,0);
	this->PostMessage(WM_COMMAND,controlCode,0);
}
void CPCAChart::ResetCurve(bool allCurve/* = false*/)  //复位区县，如果选中了一条曲线，则复位此条曲线，否则，将复位所有的曲线
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
void CPCAChart::ChangeToSelectMode(bool allCurve/* = false*/)  //切换到选择曲线模式
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
void CPCAChart::ChangeToMoveMode()  //切换到移动曲线模式
{
	WPARAM controlCode = MAKELONG(10006,0);
	this->PostMessage(WM_COMMAND,controlCode,0);
}
void CPCAChart::ChangeToZoomInMode() //
{
	WPARAM controlCode = MAKELONG(10002,0);
	this->PostMessage(WM_COMMAND,controlCode,0);
}
void CPCAChart::ChangeToZoomOutMode()
{
	WPARAM controlCode = MAKELONG(10003,0);
	this->PostMessage(WM_COMMAND,controlCode,0);
}
void CPCAChart::ChangeToVerticalMode()
{
	WPARAM controlCode = MAKELONG(10005,0);
	this->PostMessage(WM_COMMAND,controlCode,0);
}
void CPCAChart::ChageToHorizontalMode()
{
	WPARAM controlCode = MAKELONG(10004,0);
	this->PostMessage(WM_COMMAND,controlCode,0);
}
void CPCAChart::ChangeToPeakAreaCalMode()
{
	WPARAM controlCode = MAKELONG(10014,0);
	this->PostMessage(WM_COMMAND,controlCode,0);
}
void CPCAChart::ChangeToLableMode()
{
	WPARAM controlCode = MAKELONG(10015,0);
	this->PostMessage(WM_COMMAND,controlCode,0);
}

void CPCAChart::ChageDrawDirectionX()
{
	WPARAM controlCode = MAKELONG(10009,0);
	this->PostMessage(WM_COMMAND,controlCode,0);
}
void CPCAChart::ChageDrawDirectionY()
{
	WPARAM controlCode = MAKELONG(10010,0);
	this->PostMessage(WM_COMMAND,controlCode,0);
}
void CPCAChart::HideUnselectCurve(bool hide/* = false*/)
{
	m_bHideUnselectCurve = hide;
	InvalidateChart();
}
void CPCAChart::DataShowFollowCursor(bool follow/* = false*/)
{
	m_bShowDataFlowMouse = follow;
}
void CPCAChart::SetBackgroundColor(COLORREF backColor)
{

	m_brushBack.DeleteObject() ;
	m_crBackColor = backColor;
	m_brushBack.CreateSolidBrush(m_crBackColor) ;


	InvalidateChart();
}
void CPCAChart::SetGridLineColor(COLORREF gridLienColor)
{
	m_crGridColor = gridLienColor;
	InvalidateChart();
}
void CPCAChart::SavePlotAsBmp(CString filename)
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
		//memDC.BitBlt(0,0,m_nClientWidth, m_nClientHeight,&m_dcGrid, 0, 0, SRCCOPY);//绘制网格及坐标轴
		memDC.BitBlt(0, 0, m_nClientWidth, m_nClientHeight,&m_dcPlot, 0,0,SRCPAINT);  //最后一个参数SRCPAINT指定两个DC内容进行OR运算,即叠加运算
		memDC.SelectObject(oldBitmap);
	}

	BITMAP btm;  //获取bmp文件信息
	memBitmap.GetBitmap(&btm); 

	//分配内存
	DWORD size=btm.bmWidthBytes*btm.bmHeight; 
	LPSTR lpData=(LPSTR)GlobalAlloc(GPTR,size); 
	if(lpData != NULL) 
	{ 
		//填充bmp头文件信息
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
		GetDIBits(dc.m_hDC,memBitmap,0,bih.biHeight,lpData,(BITMAPINFO*)&bih,DIB_RGB_COLORS); //从bm中获取bmp数据

		BITMAPFILEHEADER bfh; //bmp文件头定义
		bfh.bfReserved1=bfh.bfReserved2=0; 
		bfh.bfType=((WORD)('M'<<8)|'B'); 
		bfh.bfSize=sizeof(bfh)+size; 
		bfh.bfOffBits=sizeof(bfh); 

		CFile bf; 
		if(bf.Open(filename,CFile::modeCreate|CFile::modeWrite)) 
		{ 
			bf.Write(&bfh,sizeof(BITMAPFILEHEADER)); 
			bf.Write(&bih,sizeof(BITMAPINFOHEADER)); //要把打包的代码改为这样. 
			bf.Write(lpData,size); 
			bf.Close(); 
		} 
		::GlobalFree(lpData); 
	} 
	dc.DeleteDC(); 
}
/*void CPCAChart::SaveData(CString filePath)
{
	CFloatPoint point;
	CString data;
	CString _curveName;
	CCurveDataSetArray* dataSet = NULL;
	CCurveObj* curveObject = NULL;

	CPtrArray _allCurvedataSets;
	int pointCounter = 100000000;

	POSITION pos = m_CurveMap.GetStartPosition();
	while(pos)
	{
		m_CurveMap.GetNextAssoc(pos,_curveName, (CObject*&)curveObject);
		if(curveObject)
		{
			dataSet = curveObject->GetCurveDataCollection();
			if(dataSet)
			{
				_allCurvedataSets.Add(dataSet);
				int size = dataSet->GetSize();
				if(size <pointCounter)
				{
					pointCounter = size;
				}
				CStdioFile df; 
				CString filename = filePath +"\\"+ _curveName+".txt";
				if(df.Open(filename,CFile::modeCreate|CFile::modeWrite)) 
				{ 
					for(int i=0;i<dataSet->GetSize();i++)
					{
						point = dataSet->GetAt(i);
						data.Format(L"%lf\t%lf\r\n",point.m_PosX,point.m_PosY);//csv 格式保存数据
						df.WriteString(data);
					}
					df.Flush();
					df.Close(); 
				} 
			}
		}
	}
	data.Empty();
	CStdioFile allDataFile; 
	CString filename = filePath +"\\"+"data.txt";
	CString tempStr;
	if(allDataFile.Open(filename,CFile::modeCreate|CFile::modeWrite)) 
	{ 
		for(int i=0;i<pointCounter;i++)
		{
			for(int j=0;j<_allCurvedataSets.GetSize();j++)
			{
				dataSet = (CCurveDataSetArray*)_allCurvedataSets.GetAt(j);
				point = dataSet->GetAt(i);
				tempStr.Format(L"%lf\t",point.m_PosY);
				data+=tempStr;
			}
			data+="\r\n";
			allDataFile.WriteString(data);
			data.Empty();
		}  
	}
	allDataFile.Flush();
	allDataFile.Close(); 
}*/


/*int CPCAChart::SetCurveTransAbs(bool _transAbs)
{
	//set<CString>::iterator iter = m_SetSelCurveName.begin();           

	m_ShowTransAbs = _transAbs;
	CString _curveName;
	CCurveObj *_curveObject = NULL;

	POSITION pos = m_CurveMap.GetStartPosition();
	//m_CurveMap.GetNextAssoc(pos,_curveName,(CObject*&)_curveObject);

	if(_transAbs)
	{
		while(pos)
		{	
			m_CurveMap.GetNextAssoc(pos,_curveName,(CObject*&)_curveObject);
			if(_curveObject)
			{
				_curveObject->ChangeToTransmittance();
			}	
		}
	}
	else
	{
		while(pos)
		{	
			m_CurveMap.GetNextAssoc(pos,_curveName,(CObject*&)_curveObject);
			if(_curveObject)
			{
				_curveObject->ChangeToAbsorbance();
			}
		}
	}
	ReCalculateAllCurveMaxMin();
	GetMaxDataRect();
	this->ResetCurve();
	this->InvalidateChart();
	return 0;
}*/

/*int CPCAChart::SmoothCurve(CCurveDataSetArray& tempArray,CString _curveName,int SmoothMethod,int N)
{
	//set<CString>::iterator iter = m_SetSelCurveName.begin();           //选中曲线名称数组
	tempArray.RemoveAll();
	CCurveObj* curveObject = NULL;
	//CString _newCurveName;
	//CCurveDataSetArray tempArray;
	m_CurveMap.Lookup(_curveName,(CObject*&)curveObject);
	if(curveObject)
	{
		switch(SmoothMethod)
		{
		case MOVE_MEAN:
			{
				//_newCurveName = *iter;
				//_newCurveName += L"(移动平滑)";
				//this->CreateNewCurve(_newCurveName,0,this->m_SelectedCurveColor,2);

				curveObject->SmoothCurve(tempArray,N);
				//for( int i=0;i<tempArray.GetCount();i++)
				//{
				//	tempArray,tempArray[i].m_PosX,tempArray[i].m_PosY);
				//}
				//int curveNamelenth=_newCurveName.GetLength();
				//wchar_t *str=(wchar_t *)malloc(sizeof(wchar_t)*(curveNamelenth*2));
				//wcscpy(str,CT2CW(_newCurveName));
				//::SendMessage(this->GetParent()->m_hWnd,WM_CHANGESELECTEDCURVE,m_SetSelCurveName.size(),(LPARAM)str);
				break;
			}
		case SACITZKY_GOLAY:
			{

			}
		}			
	}
	//this->SelectCurve(_newCurveName);
	return 0;
}*/

/*int CPCAChart::ShowSelectedNoise()
{
	//
	std::set<CString>::iterator iter = m_SetSelCurveName.begin();
	CCurveObj *_curveObject = NULL;
	m_CurveMap.Lookup(*iter,(CObject*&)_curveObject);

	double minX;
	double maxX;
	double maxY;
	double minY;
	double Rms;
	if(_curveObject)
	{
		//获取x范围
		_curveObject->GetXRange(minX,maxX);
		//计算峰峰值
		_curveObject->GetYRange(minY,maxY);

		//计算均方根rms值
		Rms = _curveObject->CalculateYRMS();

	}
	CString Info;
	Info.Format(L"范围：%f  %f\n峰-峰值：%f\nRMS值：%f",minX,maxX,maxY,Rms);

	//CSpecNoiseDlg SpecNoiseDlg;
	//SpecNoiseDlg.m_NoiseInfo = Info;
	//SpecNoiseDlg.DoModal();

	MessageBox(Info,L"噪声测量",MB_ICONINFORMATION);

	return 0;
}
int CPCAChart::ShowSelectedMean()
{
	//
	std::set<CString>::iterator iter = m_SetSelCurveName.begin();
	CCurveObj *_curveObject = NULL;
	m_CurveMap.Lookup(*iter,(CObject*&)_curveObject);

	double minX;
	double maxX;

	double Mean;
	if(_curveObject)
	{
		//获取x范围
		_curveObject->GetXRange(minX,maxX);
		//计算峰峰值
		//_curveObject->GetYRange(minY,maxY);

		//计算均方根rms值
		Mean = _curveObject->CalculateYMean();

	}
	CString Info;
	Info.Format(L"范围：%f  %f\n平均值：%f",minX,maxX,Mean);

	//CSpecNoiseDlg SpecNoiseDlg;
	//SpecNoiseDlg.m_NoiseInfo = Info;
	//SpecNoiseDlg.DoModal();

	MessageBox(Info,L"平均值测量",MB_ICONINFORMATION);

	return 0;
}

void CPCAChart::ShowSelectedPeakArea() //计算峰面积
{
	CCurveObj *_curveObject = NULL;
	CString _curveName;
	POSITION pos = m_CurveMap.GetStartPosition();
	m_CurveMap.GetNextAssoc(pos,_curveName,(CObject *&)_curveObject);

	if(_curveObject)
	{
		//计算开始结束位置
		int IndexBegin = _curveObject->GetDataIndex(m_BeginPoint);
		int IndexEnd = _curveObject->GetDataIndex(m_EndPoint);

		if(IndexBegin >= 0 && IndexEnd >= 0)
		{
			double UnCorrectArea = _curveObject->CalculateYSum(IndexBegin,IndexEnd);

			double CorrectArea = UnCorrectArea - (abs(IndexEnd - IndexBegin)+1)*(m_BeginPoint.m_PosY + m_EndPoint.m_PosY)/2;

			CString Info;
			Info.Format(L"峰范围：%f  %f  \n未校正峰面积： %f\n校正后峰面积: %f",m_BeginPoint.m_PosX,m_EndPoint.m_PosX,UnCorrectArea,CorrectArea);

			MessageBox(Info,L"峰面积",MB_ICONINFORMATION);

		}
	} 


}*/

/*BOOL CPCAChart::CurveExist(const CString &_curveName)   //判断该名称的曲线是否存在
{
	CCurveObj *_curveObj = NULL;
	return m_CurveMap.Lookup(_curveName,(CObject*&)_curveObj) ;
}*/

BOOL CPCAChart::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此添加专用代码和/或调用基类
	if (cs.lpszClass == NULL)
		cs.lpszClass = AfxRegisterWndClass(CS_DBLCLKS);
	//return CScrollView::PreCreateWindow(cs);
	return CWnd::PreCreateWindow(cs);
}


BOOL CPCAChart::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
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
void CPCAChart::RemoveAllLabel()
{
	m_SetLabelPoint.clear();
	this->InvalidateChart();
}

