
// PCAChart.cpp : 实现文件
//

#include "stdafx.h"
#include "DataServerCenter.h"
#include "PCAChart.h"
//#include "TestMyChart.h"
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
	m_iVerticalControlMode = false; //默认是水平控制模式(指管轮操作模式的移动即缩放功能)
	m_recMouseSelect.SetRectEmpty();
	m_pCurSelCurveObj = NULL;

	m_CurveNameMaxSize=0;//pDC->GetTextExtent(L"假酒库图谱");

	m_LegendLineLenth=30;

	m_SelectedCurveColor = RGB(255,0,0);  //选中曲线颜色

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
		this->DrawGrid();
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

	this->SetMargin(m_ptMargin.x,m_ptMargin.y);	//重新设定边框
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
		//memDC.BitBlt(0,0,m_nClientWidth, m_nClientHeight,&m_dcGrid, 0, 0, SRCCOPY);//绘制网格及坐标轴
		memDC.BitBlt(0, 0, m_nClientWidth, m_nClientHeight,&m_dcPlot, 0,0,SRCPAINT);  //最后一个参数SRCPAINT指定两个DC内容进行OR运算,即叠加运算
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
				//若鼠标距离曲线某点很近
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
		dc.BitBlt(0,0,m_nClientWidth, m_nClientHeight,&memDC,0,0,SRCCOPY); //拷贝内存DC到屏幕
	}
	memDC.SelectObject(oldBitmap) ;
	memBitmap.DeleteObject(); 
	memDC.DeleteDC(); 
}
//##ModelId=48A145890129
void CClusterChart::DrawLegend()
{
	m_dcPlot.SetTextAlign (TA_LEFT|TA_TOP);//显示文字方向从左向右
	//COLORREF LegendColor;

	CSize CurveNameSize;

	int i=0;

	m_dcPlot.SetBkColor (m_crPlotAreaBackColor) ;

	//填充
	m_dcPlot.FillRect(CRect(m_rectPlot.right+10 ,m_rectPlot.top,m_rectPlot.right+63+m_CurveNameMaxSize.cx,m_rectPlot.top+(m_CurveNameMaxSize.cy+5)*2),&m_brushPlotAreaBack);
	int r = 5;

    //画参考图谱图例
	CPen* oldPen;
	CPen LegendPen1(PS_SOLID,2,m_FakeColor);

	oldPen = m_dcPlot.SelectObject(&LegendPen1);

	m_dcPlot.Ellipse(m_rectPlot.right+10,m_rectPlot.top+i*(CurveNameSize.cy+5)+5+3,
		m_rectPlot.right+20,m_rectPlot.top+i*(CurveNameSize.cy+5)+5+10+3);

	m_dcPlot.SelectObject(&oldPen);
	m_dcPlot.SetTextColor(m_FakeColor);
	CurveNameSize = m_dcPlot.GetTextExtent(L"假酒库图谱");

	m_dcPlot.SelectObject(&oldPen);
	m_dcPlot.SetTextColor(m_FakeColor);
	m_dcPlot.TextOut(m_rectPlot.right+23,m_rectPlot.top+i*(CurveNameSize.cy+5)+5,L"假酒库图谱");
	++i;

	//LegendPen.DeleteObject();

	CPen LegendPen2(PS_SOLID,2,m_AnaColor);

	oldPen = m_dcPlot.SelectObject(&LegendPen2);
	m_dcPlot.Ellipse(m_rectPlot.right+10,m_rectPlot.top+i*(CurveNameSize.cy+5)+5+3,
		m_rectPlot.right+20,m_rectPlot.top+i*(CurveNameSize.cy+5)+5+10+3);

	m_dcPlot.SelectObject(&oldPen);
	m_dcPlot.SetTextColor(m_AnaColor);

	CurveNameSize = m_dcPlot.GetTextExtent(L"待分析图谱");

	m_dcPlot.SelectObject(&oldPen);
	m_dcPlot.SetTextColor(m_AnaColor);
	m_dcPlot.TextOut(m_rectPlot.right+23,m_rectPlot.top+i*(CurveNameSize.cy+5)+5,L"待分析图谱");

	m_dcPlot.SelectObject(&oldPen);
	InvalidateRect(m_rectClient);
}

void CClusterChart::DrawBackGnd()
{
	m_dcPlot.FillRect(m_rectPlot, &m_brushPlotAreaBack);
}

void CClusterChart::DrawGrid(CString curveName/*=_T("\0")*/)
{
}
void CClusterChart::DrawLabel()  //绘制标注
{
}

void CClusterChart::DrawUnPlotArea(CString curveName/*=_T("\0")*/)
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
	/*if(m_CurveMap.Lookup(curveName,(CObject*&)curveObject))
	{
		textColor = curveObject->GetCurveColor();  
	}*/

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
	this->GetMaxDataRect();

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

	int nVerScaleLines = CluserNum+2;  
	
	double nScaleHeightY = point.m_PosY/11;
	
	double dGridHeight = m_rectPlot.Height()/nVerScaleLines;
	
	double tempY = 0.0;	
	double tempY2=0.0;

	CPen girdSolidPen(PS_SOLID, 1, girdColor);
	oldPen = m_dcPlot.SelectObject(&girdSolidPen);

	m_dcPlot.SetTextAlign(TA_LEFT|TA_TOP) ;
	for(j=1;j<CluserNum+2;j++)
	{
		str.Format(L"%d",CluserNum-j+1); 
		textSize = m_dcPlot.GetTextExtent(str); 
		//绘制水平方向小标尺
		tempY = m_rectPlot.top + (long)(dGridHeight*j);	
		m_dcPlot.MoveTo(m_rectPlot.left,tempY);
		m_dcPlot.LineTo(m_rectPlot.left+6,tempY);
		if(!bNoCurveSelect)
		{
			m_dcPlot.SetTextAlign (TA_RIGHT|TA_TOP) ;
			m_dcPlot.TextOut (m_rectPlot.left-textSize.cy/4, tempY-textSize.cy/2, str) ;
		}
	}

	int nHorScaleLines = (m_FakeSpecCluster.size()+m_AnaSpecCluster.size())/10 + 1;  

	int nHorScale = 10;
	double dGridWidth = m_rectPlot.Width()/nHorScaleLines;
	int tempX = 0;	
	//double tempX2;

	for(j=0;j<=nHorScaleLines;++j)
	{
		tempX = m_rectPlot.left + (long)(dGridWidth*j);	
		m_dcPlot.MoveTo(tempX,m_rectPlot.bottom);
		m_dcPlot.LineTo(tempX,m_rectPlot.bottom+6);


		str.Format (L"%d", j*10) ;
		textSize = m_dcPlot.GetTextExtent(str); 
		m_dcPlot.TextOut (tempX+textSize.cx/2,m_rectPlot.bottom+5, str);	

		if(j<nHorScaleLines)
		{
			for(k =0;k<nHorScale;k++)  //绘制小的标度尺
			{
				for(i = m_rectPlot.bottom-2; i<m_rectPlot.bottom; i++)
				{
					m_dcPlot.SetPixel(tempX+(dGridWidth/nHorScale)*k,i,frameColor) ;
				}
			}
		}
	}

	CString YUnit;
	CString XUnit = L"聚类编号(编号0表示样本没有聚到任一簇)";

	YUnit.Format(L"图谱编号(1-%d为假酒谱图，%d-%d为进行分析的图谱)",m_FakeSpecCluster.size(),m_FakeSpecCluster.size()+1,m_FakeSpecCluster.size()+m_AnaSpecCluster.size());

	CSize szYUnit = m_dcPlot.GetTextExtent(YUnit);
	CSize szXUnit = m_dcPlot.GetTextExtent(XUnit);

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

	m_dcPlot.TextOut(2,m_rectClient.Height()/2-szXUnit.cx/2,XUnit);
	myLogFont.DeleteObject();
	lf.lfEscapement = 0;
	myLogFont.CreateFontIndirect(&lf); //创建逻辑字体
	m_dcPlot.SelectObject(&myLogFont);//选入设备描述表

	m_dcPlot.TextOut(m_rectClient.Width()/2+szYUnit.cx/2,m_rectClient.Height()-20,YUnit);

	m_dcPlot.SelectObject(pOldFont); //将myFont从设备环境中分离
	myLogFont.DeleteObject();


	m_ptZoomOffset.InitPoint(-10000000.0,-10000000.0);  //此处必须这样初始化,
	m_ptMoveOffset.InitPoint(-10000000.0,-10000000.0);
	InvalidateRect(m_rectClient);
}
//##ModelId=48A14588034B
void CClusterChart::InvalidateChart()
{
	if( m_dcPlot.GetSafeHdc()==NULL )//m_dcGrid.GetSafeHdc()==NULL ||
		return ;
	CString _curveName;
	CCurveObj* curveObject = NULL;

	//m_dcPlot.FillRect(m_rectClient,&m_brushBack) ;

	//画背景
	DrawBackGnd();

	//画假酒光谱聚类结果
	CMapToCurveObj::iterator iter = m_FakeSpecCluster.begin();

	while(iter!=m_FakeSpecCluster.end())
	{
		iter->second->SetCurveColor(m_FakeColor);

		iter->second->DrawPoint(&m_dcPlot,m_rectPlot,CIRCLE);//绘制点

		++iter;
	}
	//画分析光谱聚类结果
	iter = m_AnaSpecCluster.begin();
	while(iter!=m_AnaSpecCluster.end())
	{
		iter->second->SetCurveColor(m_AnaColor);
		iter->second->DrawPoint(&m_dcPlot,m_rectPlot,CIRCLE);//绘制点
		++iter;
	}

	DrawUnPlotArea();
	//画图例
	if(m_bDrawLegend)
	{
		//m_dcLegend.FillRect(m_rectClient,&m_brushBack) ;
		DrawLegend();
	}

	InvalidateRect(m_rectClient);
}

//往指定index的曲线对象中添加坐标点
//##ModelId=48A1458802EE
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

//控制是否所有曲线共用一个坐标系,默认是每一条曲线在自己的坐标系中绘图。true为所有曲线共用一个坐标系

void CClusterChart::OnSize(int cx,int cy)
{
	if(this->GetSafeHwnd())
	{
		this->SetWindowPos(NULL,0,0,cx,cy,SWP_SHOWWINDOW);  //调用此函数将激发WM_SIZE消息
	}
}

//设置动态时间曲线Y轴数据上下限,必须在动态绘图之前设定好此参数
void CClusterChart::SetRealTimeBaseCurveDataRange(double lower,double upper)  
{
	CFloatPoint point(lower,upper);
	CCurveObj::SetRealTimeBaseCurveDataRange(point);
}
//创建新的曲线对象
//##ModelId=48A14588031D
CCurveObj& CClusterChart::CreateNewPoint(CString curveName,bool timeBaseFlag ,bool Fake) //创建一条新的点
{
	CCurveObj* curveObject = NULL;
	//CMapToCurveObj m_FakeSpecCluster;  //假酒样本聚类结果
	//CMapToCurveObj m_AnaSpecCluster;   //分析样本聚类结果
	if((m_CurveNameMaxSize.cx == 0)&&(m_CurveNameMaxSize.cy == 0))
	{
		CDC *pDC = GetDC();
		m_CurveNameMaxSize=pDC->GetTextExtent(L"假酒库图谱");
		ReleaseDC(pDC);
	}

	if(Fake)
	{
		//若已有曲线
		if(m_FakeSpecCluster.find(curveName) == m_FakeSpecCluster.end())
		{
			curveObject = new CCurveObj(m_rectPlot);
			//this->SetCurveSelected(curveName);
			if(curveObject)
			{
				curveObject->SetTimeBaseFlag(timeBaseFlag);
				m_FakeSpecCluster.insert(make_pair(curveName,curveObject));

			}
			//CDC* pDc=GetDC();
			//CSize CurveNameSize;
			//CurveNameSize = pDc->GetTextExtent(curveName);
			//m_CurveNameMaxSize.cx=m_CurveNameMaxSize.cx>CurveNameSize.cx?m_CurveNameMaxSize.cx:CurveNameSize.cx;
			//m_CurveNameMaxSize.cy=m_CurveNameMaxSize.cy>CurveNameSize.cy?m_CurveNameMaxSize.cy:CurveNameSize.cy;
			//ReleaseDC(pDc);
		}
	}
	else
	{
		//若已有曲线
		if(m_AnaSpecCluster.find(curveName) == m_AnaSpecCluster.end())
		{
			curveObject = new CCurveObj(m_rectPlot);
			//this->SetCurveSelected(curveName);
			if(curveObject)
			{
				curveObject->SetTimeBaseFlag(timeBaseFlag);
				m_AnaSpecCluster.insert(make_pair(curveName,curveObject));

			}
			//CDC* pDc=GetDC();
			//CSize CurveNameSize;
			//CurveNameSize = pDc->GetTextExtent(curveName);
			//m_CurveNameMaxSize.cx=m_CurveNameMaxSize.cx>CurveNameSize.cx?m_CurveNameMaxSize.cx:CurveNameSize.cx;
			//m_CurveNameMaxSize.cy=m_CurveNameMaxSize.cy>CurveNameSize.cy?m_CurveNameMaxSize.cy:CurveNameSize.cy;
			//ReleaseDC(pDc);
		}

	}
	//if(m_bDrawLegend)
	//{
	//	SetMargin();
	//}


	return *curveObject;
}
//删除选中曲线
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
		//curveObject->DestroyCurve();
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
		//m_CurveMap.GetNextAssoc(pos,curveName, (CObject*&)curveObject);
		curveObject = iter_map->second;
		if(curveObject)
		{
			curveObject->DestroyCurve();
			delete curveObject;
			curveObject = NULL;
			//m_CurveMap.RemoveKey(curveName);
		}
		++iter_map;
	}
	m_FakeSpecCluster.clear();

    iter_map = m_AnaSpecCluster.begin();
	while(iter_map!=m_AnaSpecCluster.end())
	{
		//m_CurveMap.GetNextAssoc(pos,curveName, (CObject*&)curveObject);
		curveObject = iter_map->second;
		if(curveObject)
		{
			curveObject->DestroyCurve();
			delete curveObject;
			curveObject = NULL;
			//m_CurveMap.RemoveKey(curveName);
		}
		++iter_map;
	}
	m_AnaSpecCluster.clear();
 
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
		//重新修改作图区大小
		if(m_bDrawLegend&&m_CurveNameMaxSize.cx>0)
		{
			m_rectPlot = m_rectClient;
			m_ptMargin = CPoint(xMargin,yMargin);
			m_rectPlot.DeflateRect(m_ptMargin.x*2+m_CurveNameMaxSize.cx/2,m_ptMargin.y);  
			m_rectPlot.OffsetRect(m_ptMargin.x-m_CurveNameMaxSize.cx/2,-m_ptMargin.y/2);//真正的绘图区域
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
//##ModelId=48A145890001
bool CClusterChart::HitTest(CPoint point)
{
	return 1;
}
//##ModelId=48A14589002E
void CClusterChart::MoveCurve(CString curveName /*= _T("\0")*/,int moveType /*= MOVE_RESET*/,double percent /*= 5*/)
{
}
//曲线缩放
//##ModelId=48A145890040
void CClusterChart::ZoomCurve(CString curveName /*= _T("\0")*/,int zoomType /*= ZOOM_RESET*/,double percent /*= 5*/)
{
}
//##ModelId=48A1458900EC
void CClusterChart::IndividualCurveMove(CString curveName,int moveType,double percent)
{
}
//##ModelId=48A14589010B
void CClusterChart::IndividualCurveZoom(CString curveName,int zoomType,double percent)
{
}
//##ModelId=48A145880222
void CClusterChart::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	CWnd::OnLButtonDown(nFlags, point);
}

//##ModelId=48A145880233
void CClusterChart::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	CWnd::OnMouseMove(nFlags, point);
}

//##ModelId=48A145880251
void CClusterChart::OnLButtonUp(UINT nFlags, CPoint point) 
{
	CWnd::OnLButtonUp(nFlags, point);
}

//##ModelId=48A145880263
BOOL CClusterChart::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt) 
{
	return CWnd::OnMouseWheel(nFlags, zDelta, pt);
}
//##ModelId=48A1458900BB
void CClusterChart::IndividualCurveWheelContol(CCurveObj* curveObj/* = NULL*/,int controlMode/* = CURVE_MOVE*/,bool verticalControl/* = false*/,int zDelta/* = 0*/,CPoint point)
{
}
//##ModelId=48A1458900DA
void CClusterChart::IndividualCurveControl(CCurveObj* curveObj/* = NULL*/,int controlMode /*= CURVE_MOVE*/,CRect controlRect)
{	
}
//##ModelId=48A14589005E
BOOL CClusterChart::GetMaxDataRect(CString curveName/*=_T("\0")*/)  //获得所有曲线中最大的数据区域值
{	
	BOOL rc = FALSE;
	CCurveObj* curveObject = NULL;

	if(curveName.IsEmpty())  //
	{
		CString _curveName;
		//POSITION pos = m_CurveMap.GetStartPosition();
		CMapToCurveObj::iterator iter_map = m_FakeSpecCluster.begin();
		if(iter_map!= m_FakeSpecCluster.end())
		{
			//m_CurveMap.GetNextAssoc(pos,_curveName, (CObject*&)curveObject);
			curveObject = iter_map->second;
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

	m_ptMaxPos.InitPoint(-10000000000.0,-10000000000.0);//当前最大点
	m_ptMinPos.InitPoint(10000000000.0,10000000000.0);//当前最小点

	CFloatPoint MaxPos;
	CFloatPoint MinPos;

	CDC *pDc=GetDC();
	CSize CurveNameSize;

	//对比图谱PCA
	CString _curveName;
	CMapToCurveObj::iterator iter = m_FakeSpecCluster.begin();
	while(iter!=m_FakeSpecCluster.end())
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
			//m_CurveNameMaxSize.cx=m_CurveNameMaxSize.cx>CurveNameSize.cx?m_CurveNameMaxSize.cx:CurveNameSize.cx;
			m_CurveNameMaxSize.cy=m_CurveNameMaxSize.cy>CurveNameSize.cy?m_CurveNameMaxSize.cy:CurveNameSize.cy;
		}
		++iter;
	}
	//参考图谱
    iter = m_AnaSpecCluster.begin();
	while(iter!=m_AnaSpecCluster.end())
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
			//m_CurveNameMaxSize.cx=m_CurveNameMaxSize.cx>CurveNameSize.cx?m_CurveNameMaxSize.cx:CurveNameSize.cx;
			m_CurveNameMaxSize.cy=m_CurveNameMaxSize.cy>CurveNameSize.cy?m_CurveNameMaxSize.cy:CurveNameSize.cy;
		}
		++iter;
	}

	m_ptMaxPos.m_PosX += 10;
	m_ptMaxPos.m_PosY += 1;

	m_ptMinPos.m_PosX = 0;
	m_ptMinPos.m_PosY = -1;


	//m_ptMaxPos.m_PosY =(double)(((int)m_ptMaxPos.m_PosY/10+1)*10);
	//对比图谱PCA


	m_ptMaxPosConst = m_ptMaxPos;
	m_ptMinPosConst = m_ptMinPos;

	this->ResetAllCurveMaxMin(m_ptMaxPos,m_ptMinPos);
	return 0;
}
int CClusterChart::ResetAllCurveMaxMin(CFloatPoint MaxPos,CFloatPoint MinPos)
{
	CCurveObj* curveObject = NULL;

	CMapToCurveObj::iterator iter = m_AnaSpecCluster.begin();
	//CString _curveName;
	while(iter!=m_AnaSpecCluster.end())
	{
		curveObject = iter->second;
		if(curveObject)
		{
			//curveObject->SetAllCurveMaxMinPar(m_ptMaxPos,m_ptMinPos);
			curveObject->SetCurveMaxMinPar(MaxPos,MinPos);
		}
		++iter;
	}
	//参考图谱
	iter = m_FakeSpecCluster.begin();
	while(iter!=m_FakeSpecCluster.end())
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
void CClusterChart::CalculateMaxPoint(CFloatPoint& pointDes,const CFloatPoint* PointSource)
{
	if(PointSource->m_PosX > pointDes.m_PosX)
		pointDes.m_PosX = PointSource->m_PosX;
	if(PointSource->m_PosY > pointDes.m_PosY)
		pointDes.m_PosY = PointSource->m_PosY;
}
//##ModelId=48A14589008D
void CClusterChart::CalculateMinPoint(CFloatPoint& pointDes,const CFloatPoint* PointSource)
{
	if(PointSource->m_PosX < pointDes.m_PosX)
		pointDes.m_PosX = PointSource->m_PosX;
	if(PointSource->m_PosY < pointDes.m_PosY)
		pointDes.m_PosY = PointSource->m_PosY;
}
//##ModelId=48A145880282
void CClusterChart::OnRButtonDown(UINT nFlags, CPoint point) 
{
	this->SetFocus();  //获得焦点
	CWnd::OnRButtonDown(nFlags, point);
}

//##ModelId=48A1458802A0
void CClusterChart::OnContextMenu(CWnd* pWnd, CPoint point) 
{
}

//##ModelId=48A1458801A5
BOOL CClusterChart::OnCommand(WPARAM wParam, LPARAM lParam) 
{
	return CWnd::OnCommand(wParam, lParam);
}
//##ModelId=48A1458802CF
void CClusterChart::OnUpdateMenuState(CCmdUI* pCmdUI)//更新菜单上的状态显示
{
}

//##ModelId=48A145880196
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

//##ModelId=48A1458802BF
void CClusterChart::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	//ResetCurve();
	//this->SetFocus();

	CWnd::OnLButtonDblClk(nFlags, point);
}

//##ModelId=48A1458801B7
LRESULT CClusterChart::DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	return CWnd::DefWindowProc(message, wParam, lParam);
}

//##ModelId=48A1458901E4
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
//设置是独立坐标系还是公用坐标系,每调用一次状态将发生翻转，默认是独立坐标系
void CClusterChart::ChangeToIndividualCoordinateMode(bool individualMode/* = true*/)
{
	//m_bAllCurveSameBase = !individualMode;
	//this->SetAllCurveDrawMode(m_bAllCurveSameBase);
}

void CClusterChart::ChangeToGetDataMode()  //让对象切换到取数据模式，其他模式将被屏蔽
{
	WPARAM controlCode = MAKELONG(9999,0);
	this->PostMessage(WM_COMMAND,controlCode,0);
}
void CClusterChart::ResetCurve(bool allCurve/* = false*/)  //复位区县，如果选中了一条曲线，则复位此条曲线，否则，将复位所有的曲线
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
void CClusterChart::ChangeToSelectMode(bool allCurve/* = false*/)  //切换到选择曲线模式
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
void CClusterChart::ChangeToMoveMode()  //切换到移动曲线模式
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
void CClusterChart::SaveData(CString filePath)
{
}


/*int CClusterChart::SetCurveTransAbs(bool _transAbs)
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

void CClusterChart::ShowSelectedPeakArea() //计算峰面积
{
}

BOOL CClusterChart::CurveExist(const CString &_curveName)   //判断该名称的曲线是否存在
{
	//CCurveObj *_curveObj = NULL;
	//return m_CurveMap.Lookup(_curveName,(CObject*&)_curveObj) ;
	return 0;
}

BOOL CClusterChart::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此添加专用代码和/或调用基类
	if (cs.lpszClass == NULL)
		cs.lpszClass = AfxRegisterWndClass(CS_DBLCLKS);
	//return CScrollView::PreCreateWindow(cs);
	return CWnd::PreCreateWindow(cs);
}


BOOL CClusterChart::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
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
void CClusterChart::RemoveAllLabel()
{
	m_SetLabelPoint.clear();
	this->InvalidateChart();
}




