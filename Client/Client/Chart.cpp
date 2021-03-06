// Chart.cpp : implementation file
#include "stdafx.h"
//#include "TestMyChart.h"
#include "Chart.h"
#include "SpectrumCompareDlg.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
using std::set;
using std::find;
using std::vector;
using std::map;
/////////////////////////////////////////////////////////////////////////////
// CChart
//##ModelId=48A145880178

CChart::CChart()
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

	m_brushBack.CreateSolidBrush(m_crBackColor) ;
	m_brushGrid.CreateSolidBrush(m_crGridColor) ;

	m_brushPlotAreaBack.CreateSolidBrush(m_crPlotAreaBackColor) ;

	m_CurveMap.RemoveAll();

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

//##ModelId=48A145880186
CChart::~CChart()
{
	POSITION  pos=m_CurveMap.GetStartPosition();
	CString strKey;
	CObject*    pOb;
	while(pos)
	{
		m_CurveMap.GetNextAssoc(pos,strKey,pOb);
		delete  pOb;
		pOb = NULL;
	}
}


BEGIN_MESSAGE_MAP(CChart, CWnd)
	//{{AFX_MSG_MAP(CChart)
	ON_WM_SIZE()
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEWHEEL()
	ON_WM_RBUTTONDOWN()
	ON_WM_CONTEXTMENU()
	ON_WM_LBUTTONDBLCLK()
	ON_UPDATE_COMMAND_UI_RANGE(9990,10015,OnUpdateMenuState)
	ON_WM_SETCURSOR()
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CChart message handlers

BOOL CChart::Create(DWORD dwStyle, const RECT &rect, CWnd *pParentWnd, UINT nID)
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
void CChart::SetShowLegend(bool ShowLegend/*= true*/)
{
	m_bDrawLegend=ShowLegend;
	InvalidateChart();
}

BOOL CChart::InvalidateCtrl()   //
{
	CClientDC dc(this) ;  

	//绘制曲线内存对象
	m_dcPlot.DeleteDC();
	m_dcPlot.CreateCompatibleDC(&dc) ;

	m_bitmapPlot.DeleteObject();
	m_bitmapPlot.CreateCompatibleBitmap(&dc, m_nClientWidth, m_nClientHeight) ;
	m_pbitmapOldPlot = m_dcPlot.SelectObject(&m_bitmapPlot) ;
	this->InvalidateChart();
	return TRUE;
}

//##ModelId=48A1458801E4
void CChart::OnSize(UINT nType, int cx, int cy) 
{
	CWnd::OnSize(nType, cx, cy);
	GetClientRect(m_rectClient) ;

	m_nClientHeight = m_rectClient.Height();
	m_nClientWidth = m_rectClient.Width();

	this->SetMargin(m_ptMargin.x,m_ptMargin.y);	//重新设定边框
	this->InvalidateCtrl();
}

//##ModelId=48A145880203
void CChart::OnPaint() 
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
				this->DrawGetDataCross(&memDC);
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
						POSITION pos = m_CurveMap.GetStartPosition();
						while(pos)
						{
							m_CurveMap.GetNextAssoc(pos,curveName, (CObject*&)curveObject);
							if(curveObject)
							{
								this->IndividualCurveControl(curveObject,m_iControlMode,m_recMouseSelect);
							}
						}
					}
					else
					{
						CCurveObj* curveObject = NULL;
						if(m_CurveMap.Lookup(m_sCurrentControlCurve,(CObject*&)curveObject))
						{
							this->IndividualCurveControl(curveObject,m_iControlMode,m_recMouseSelect);
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
		case CURVE_FEATURE_SELECT:
			{
				//若鼠标距离曲线某点很近
				CPoint point;
				CPoint srcPoint;
				GetCursorPos(&point);

				this->ScreenToClient(&point);

				POSITION pos = m_CurveMap.GetStartPosition();
				CString _curveName;
				CCurveObj* curveObject = NULL;

				CFloatPoint _Point;

				m_CurveMap.GetNextAssoc(pos,_curveName, (CObject*&)curveObject);
				if(curveObject)
				{
					_Point = curveObject->Position2Data(point,srcPoint);
				}
				this->DrawSelectRangeText(&memDC);

				CPoint tempPoint=point-srcPoint;
				if(tempPoint.x*tempPoint.x+tempPoint.y*tempPoint.y<16)
				{
					this->DrawNearestPt2Cursor(&memDC,srcPoint);
				}

				if(this->m_bLeftButtonDown)
				{
					this->DrawSelectRange(&memDC,m_Line);
				}
				break;
			}
		case CURVE_PEAKAREA_CAL:
			{
				CPoint point;
				CPoint srcPoint;
				GetCursorPos(&point);

				this->ScreenToClient(&point);

				POSITION pos = m_CurveMap.GetStartPosition();
				CString _curveName;
				CCurveObj* curveObject = NULL;

				CFloatPoint _Point;

				m_CurveMap.GetNextAssoc(pos,_curveName, (CObject*&)curveObject);
				if(curveObject)
				{
					_Point = curveObject->Position2Data(point,srcPoint);
				}
				this->DrawSelectRangeText(&memDC);

				CPoint tempPoint=point-srcPoint;
				if(tempPoint.x*tempPoint.x+tempPoint.y*tempPoint.y<16)
				{
					this->DrawNearestPt2Cursor(&memDC,srcPoint);
				}

				if(this->m_bLeftButtonDown)
				{
					this->DrawSelectRange(&memDC,m_Line);
				}
				break;

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
void CChart::DrawLegend()
{
	DrawLegend(m_dcPlot,m_rectPlot);
	return;	
}
void CChart::DrawLegend(CDC &m_dcPlot,CRect m_rectPlot)
{
	m_dcPlot.SetTextAlign (TA_LEFT|TA_TOP);//显示文字方向从左向右
	COLORREF LegendColor;

	CSize CurveNameSize;

	CString _curveName;
	int i=0;
	CCurveObj* curveObject = NULL;


	m_dcPlot.SetBkColor (m_crPlotAreaBackColor) ;

	//填充
	int ntotalCurve=m_CurveMap.GetCount();
	m_dcPlot.FillRect(CRect(m_rectPlot.right+10 ,m_rectPlot.top,m_rectPlot.right+63+m_CurveNameMaxSize.cx,m_rectPlot.top+(m_CurveNameMaxSize.cy+5)*ntotalCurve),&m_brushPlotAreaBack);


	POSITION pos = m_CurveMap.GetStartPosition();
	while(pos)
	{
		m_CurveMap.GetNextAssoc(pos,_curveName, (CObject*&)curveObject);
		CPen* oldPen;
		LegendColor=curveObject->GetCurveColor();
		CPen LegendPen(PS_SOLID,2,LegendColor);
		oldPen = m_dcPlot.SelectObject(&LegendPen);
		//得到曲线名称字符串尺寸
		CurveNameSize=m_dcPlot.GetTextExtent(_curveName);
		//绘制颜色指示线
		m_dcPlot.MoveTo(m_rectPlot.right+10,m_rectPlot.top+i*(CurveNameSize.cy+5)+(CurveNameSize.cy+5)/2+5);
		m_dcPlot.LineTo(m_rectPlot.right+60,m_rectPlot.top+i*(CurveNameSize.cy+5)+(CurveNameSize.cy+5)/2+5);

		m_dcPlot.SelectObject(oldPen);
		m_dcPlot.SetTextColor(LegendColor);
		m_dcPlot.TextOut(m_rectPlot.right+63,m_rectPlot.top+i*(CurveNameSize.cy+5)+5,_curveName);

		i++;
	}
	InvalidateRect(m_rectClient);
}
void CChart::DrawBackGnd()
{
	m_dcPlot.FillRect(m_rectPlot, &m_brushPlotAreaBack);
}

void CChart::DrawBackGnd(CDC &m_dcPlot,CRect m_rectPlot)
{
	m_dcPlot.FillRect(m_rectPlot, &m_brushPlotAreaBack);
}

void CChart::DrawGrid()
{
	DrawGrid(m_dcPlot, m_rectPlot, m_rectClient);
	m_ptZoomOffset.InitPoint(0.0,0.0);
	m_ptMoveOffset.InitPoint(0.0,0.0);
	InvalidateRect(m_rectClient);
	return;
}
void CChart::DrawGrid(CDC &m_dcPlot,CRect m_rectPlot,CRect m_rectClient)
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
	if(m_CurveMap.Lookup(curveName,(CObject*&)curveObject))
	{
		textColor = curveObject->GetCurveColor();  
	}

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

		//处理缩小时的问题	
		if(m_iControlMode == CURVE_ZOOM_OUT)
		{

			tempY -= (point.m_PosY-(m_ptMaxPosConst.m_PosY-m_ptMinPosConst.m_PosY))/2;
		}

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

		//处理缩小时的问题
		if(m_iControlMode == CURVE_ZOOM_OUT)
		{

			tempX -= (point.m_PosX-(m_ptMaxPosConst.m_PosX-m_ptMinPosConst.m_PosX))/2;
		}

		str.Format (L"%.*lf",_iPreciseX, tempX) ;
		//textSize = m_dcGrid.GetTextExtent(str); 
		textSize = m_dcPlot.GetTextExtent(str); 
		//绘制垂直方向网格线
		tempX = m_rectPlot.left + (long)(dGridWidth*j);	
		for (i=m_rectPlot.top+2; i<m_rectPlot.bottom && (j!=nHorScaleLines) && (j!=0); i+=2)	 
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


void CChart::DrawLabel()  //绘制标注
{
	DrawLabel(m_dcPlot,m_rectPlot);
}
void CChart::DrawLabel(CDC &m_dcPlot,CRect m_rectPlot) //绘制标注
{
	POSITION pos = this->m_CurveMap.GetStartPosition();
	CString _curveName;
	CCurveObj *_curveObject = NULL;
	CPen* oldPen;
	CPen LabelPen(PS_SOLID,1,RGB(0,0,0));
	oldPen = m_dcPlot.SelectObject(&LabelPen);
	COLORREF oldbkColor = m_dcPlot.SetBkColor(m_crPlotAreaBackColor);
	COLORREF oldTextColor = m_dcPlot.GetTextColor();
	int oldmode = m_dcPlot.SetBkMode(TRANSPARENT);
	m_dcPlot.SetTextAlign(TA_LEFT|TA_TOP);

	if(pos)
	{
		m_CurveMap.GetNextAssoc(pos,_curveName,(CObject *&)_curveObject);
		if(_curveObject)
		{
			std::map<int,CFloatPoint>::iterator iterMap = m_MapLabelPoint.begin(); //存放标注点和对应的索引
			while(iterMap!=m_MapLabelPoint.end() )
			{
				//获取标签的位置
				CPoint _point;// = *iter;
				//获取标签的数据
				CFloatPoint _fPoint = iterMap->second;
				_point = _curveObject->CFloat2Point(_fPoint,this->m_rectPlot);
				_point.y = (int)_fPoint.m_PosY;
				CString szxData;
				szxData.Format(L"%.2f",_fPoint.m_PosX);
				CSize szDatat = m_dcPlot.GetTextExtent(szxData);
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
				m_dcPlot.SetTextColor(RGB(0,0,0));
				//作图
				m_dcPlot.MoveTo(_point);
				m_dcPlot.LineTo(CPoint(_point.x,m_rectPlot.bottom));
				if((_point.y - szDatat.cx) < m_rectPlot.top)
				{
					// m_dcPlot.LineTo(CPoint(_point.x,m_rectPlot.top));
					//绘制文字
					m_dcPlot.TextOut(_point.x,_point.y+szDatat.cx,szxData);
				}
				else
				{
					//绘制文字
					m_dcPlot.TextOut(_point.x,_point.y,szxData);
				}
				myLogFont.DeleteObject();
				m_dcPlot.SelectObject(pOldFont);
				++iterMap;
			}

		}
	}
	else
	{

	}
	m_dcPlot.SetBkMode(oldmode);
	m_dcPlot.SetTextColor(oldTextColor);
	m_dcPlot.SetBkColor(oldbkColor);
	m_dcPlot.SelectObject(oldPen);
	return;
}

void CChart::DrawUnPlotArea()
{
	DrawUnPlotArea(m_dcPlot,m_rectPlot,m_rectClient);
	m_ptZoomOffset.InitPoint(-10000000.0,-10000000.0);  //此处必须这样初始化,
	m_ptMoveOffset.InitPoint(-10000000.0,-10000000.0);
	InvalidateRect(m_rectClient);
	return;
}
void CChart::DrawUnPlotArea(CDC &m_dcPlot,CRect m_rectPlot,CRect m_rectClient)
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
	if(m_CurveMap.Lookup(curveName,(CObject*&)curveObject))
	{
		textColor = curveObject->GetCurveColor();  
	}
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

		//处理缩小时的问题	
		if(m_iControlMode == CURVE_ZOOM_OUT)
		{

			tempY -= (point.m_PosY-(m_ptMaxPosConst.m_PosY-m_ptMinPosConst.m_PosY))/2;
		}

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
	int nHorScaleLines = 10;  
	double nScaleWidthX = point.m_PosX/nHorScaleLines;
	double dGridWidth = m_rectPlot.Width()/nHorScaleLines;
	double tempX = 0.0;	
	double tempX2;

	for(j=0;j<=nHorScaleLines;j++)
	{
		tempX2 = (m_bDrawReverseX)?nScaleWidthX*j:nScaleWidthX*(nHorScaleLines-j);  //曲线绘制方向改变时坐标也要相应的改变

		tempX = m_ptZoomOffset.m_PosX - tempX2 + m_ptMinPos.m_PosX - m_ptMoveOffset.m_PosX ;  //求出刻度值坐标

		//处理缩小时的问题	
		if(m_iControlMode == CURVE_ZOOM_OUT)
		{

			tempX -= (point.m_PosX-(m_ptMaxPosConst.m_PosX-m_ptMinPosConst.m_PosX))/2;
		}


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

	//显示横纵坐标单位
	m_dcPlot.SelectObject (oldPen) ;

	CString XUnit;
	CString YUnit = L"波数（cm-1）";
	CSize szYUnit = m_dcPlot.GetTextExtent(YUnit);
	CSize sz = m_dcPlot.GetTextExtent(L"啊");
	if(m_ShowTransAbs)
	{
		//透射率
		XUnit = L"%透过率";
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

	}
	else
	{
		//吸光度
		XUnit = L"吸光度";
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
	}

	m_dcPlot.SelectObject (oldPen) ;

}


//##ModelId=48A14588034B
void CChart::InvalidateChart()
{
	if( m_dcPlot.GetSafeHdc()==NULL )
		return ;
	CString _curveName;
	CCurveObj* curveObject = NULL;
	//画背景
	DrawBackGnd();
	//画网格
	DrawGrid();
	//画标注
	DrawLabel();
	//画曲线
	int colorIndex=0;
	if(m_sCurrentControlCurve.IsEmpty() || !m_bHideUnselectCurve) //控制是否显示未被选中的曲线
	{
		POSITION pos = m_CurveMap.GetStartPosition();
		while(pos)
		{
			m_CurveMap.GetNextAssoc(pos,_curveName, (CObject*&)curveObject);

			set<CString>::iterator iter =find(m_SetSelCurveName.begin(),m_SetSelCurveName.end(),_curveName);

			if(curveObject&&iter == m_SetSelCurveName.end())
			{
				curveObject->SetCurveColor(m_ArrayCurveColor[colorIndex]);
				curveObject->DrawCurve(&m_dcPlot,m_rectPlot);//绘制曲线
				if(colorIndex != m_ArrayCurveColor.GetCount()-1)
					colorIndex++;
				else
					colorIndex=0;
			}
			else if(curveObject&&iter != m_SetSelCurveName.end())
			{
				continue;
			}
		}
		set<CString>::iterator iter =m_SetSelCurveName.begin();
		while(iter!= m_SetSelCurveName.end())
		{
			m_CurveMap.Lookup(*iter, (CObject*&)curveObject);
			if(curveObject)
			{
				curveObject->SetCurveColor(m_SelectedCurveColor);
				curveObject->DrawCurve(&m_dcPlot,m_rectPlot);//绘制曲线
			}
			++iter;
		}
	}
	else
	{
		CCurveObj* curveObject = NULL;
		if(m_CurveMap.Lookup(m_sCurrentControlCurve,(CObject*&)curveObject))
		{
			curveObject->DrawCurve(&m_dcPlot,m_rectPlot);//绘制曲线
		}
	}


	DrawUnPlotArea();
	//画图例
	if(m_bDrawLegend)
	{
		DrawLegend();
	}

	InvalidateRect(m_rectClient);
}
void CChart::DrawRealtimeCurve()
{
	int shiftPixels = CCurveObj::GetShiftPixels();  //取得图像移动速度
	int clearLeft = m_rectPlot.left+shiftPixels;

	m_dcPlot.BitBlt(m_rectPlot.left, m_rectPlot.top, 
		m_rectPlot.Width()+1, m_rectPlot.Height(),&m_dcPlot, 
		clearLeft,m_rectPlot.top,SRCCOPY) ;

	CRect rectCleanUp = m_rectPlot ;
	rectCleanUp.left  = m_bDrawReverseX?(rectCleanUp.right-shiftPixels+1):(rectCleanUp.left+shiftPixels-1);
	CString _curveName;
	CCurveObj* curveObject = NULL;

	POSITION pos = m_CurveMap.GetStartPosition();
	while(pos)
	{
		m_CurveMap.GetNextAssoc(pos,_curveName, (CObject*&)curveObject);
		if(curveObject)
		{
			curveObject->DrawRealtimeCurve(&m_dcPlot,m_rectPlot);//绘制曲线
		}
	}

	InvalidateRect(m_rectPlot);
}
//往指定index的曲线对象中添加坐标点
//##ModelId=48A1458802EE
void CChart::AppendPoint( CString curveName,double Posx,double Posy)
{
	CCurveObj* curveObject = NULL;
	if(m_CurveMap.Lookup(curveName,(CObject*&)curveObject))
	{
		CFloatPoint point;
		point.InitPoint(Posx,Posy);
		curveObject->AppendNewPoint(point);
	}
}
void CChart::AppendPoint( CString curveName,double Posx,double Posy,BOOL TransAbs)
{
	CCurveObj* curveObject = NULL;
	if(m_CurveMap.Lookup(curveName,(CObject*&)curveObject))
	{
		CFloatPoint point;
		point.InitPoint(Posx,Posy);
		curveObject->AppendNewPoint(point,TransAbs);
	}
}

//##ModelId=48A14588030D
void CChart::AppendPoint(CString curveName,double Posy)   //加入时间基准曲线点
{
	CCurveObj* curveObject = NULL;
	if(m_CurveMap.Lookup(curveName,(CObject*&)curveObject))
	{
		if(!curveObject->IsTimeBaseCurve())  //如果前面没有定义为时间基准曲线,则无法加入数据点
			return ;

		CFloatPoint point;
		point.InitPoint(curveObject->GetPointCounter() * curveObject->GetTimeBaseFactor(),Posy);
		curveObject->AppendNewPoint(point);

	}
} 
//控制是否所有曲线共用一个坐标系,默认是每一条曲线在自己的坐标系中绘图。true为所有曲线共用一个坐标系
void CChart::SetAllCurveDrawMode(bool onePlot/*=false*/)
{
	CString _curveName;
	CCurveObj* curveObject = NULL;
	POSITION pos = m_CurveMap.GetStartPosition();
	while(pos)
	{
		m_CurveMap.GetNextAssoc(pos,_curveName, (CObject*&)curveObject);
		if(curveObject)
		{
		}
	}
}
void CChart::OnSize(int cx,int cy)
{
	if(this->GetSafeHwnd())
	{
		this->SetWindowPos(NULL,0,0,cx,cy,SWP_SHOWWINDOW);  //调用此函数将激发WM_SIZE消息
	}
}
void CChart::SetTimeBaseCurveProperty(CString curveName/*=_T("")*/,double timeFactor/*=1*/)
{
	if(curveName.IsEmpty())
	{
		return ;
	}
	else
	{
		CCurveObj* curveObject = NULL;
		if(m_CurveMap.Lookup(curveName,(CObject*&)curveObject))
		{
			if(!curveObject->IsTimeBaseCurve())
			{
				CString str = curveName;
				str+= L"  曲线初始化不为时间曲线，不能设置此参数.";
				::MessageBox(NULL,str,L"错误",MB_OK|MB_ICONERROR);
				return;
			}
			curveObject->SetTimeBaseFactor(timeFactor);
		}
	}
}
//设置动态时间曲线Y轴数据上下限,必须在动态绘图之前设定好此参数
void CChart::SetRealTimeBaseCurveDataRange(double lower,double upper)  
{
	CFloatPoint point(lower,upper);
	CCurveObj::SetRealTimeBaseCurveDataRange(point);
}
//创建新的曲线对象
//##ModelId=48A14588031D
CCurveObj& CChart::CreateNewCurve(CString curveName,bool timeBaseFlag /*=false*/ )
{
	CCurveObj* curveObject = NULL;
	//若尚无曲线

	if(m_CurveMap.IsEmpty())
	{
		CDC* pDc=GetDC();
		CSize CurveNameSize;
		CurveNameSize = pDc->GetTextExtent(curveName);
		m_CurveNameMaxSize=CurveNameSize;
		ReleaseDC(pDc);
	}
	//若已有曲线

	if(m_CurveMap.Lookup(curveName,(CObject*&)curveObject)==TRUE)
	{
		curveObject->DestroyCurve();
		curveObject = NULL;
		m_CurveMap.RemoveKey(curveName);
	}

	//将新曲线设为选中曲线
	curveObject = new CCurveObj(m_rectPlot);
	this->SetCurveSelected(curveName);
	if(curveObject)
	{
		curveObject->SetTimeBaseFlag(timeBaseFlag);
		curveObject->SetCurveColor(m_SelectedCurveColor,2);
		m_CurveMap.SetAt(curveName,curveObject);
	}
	CDC* pDc=GetDC();
	CSize CurveNameSize;
	CurveNameSize = pDc->GetTextExtent(curveName);
	m_CurveNameMaxSize.cx=m_CurveNameMaxSize.cx>CurveNameSize.cx?m_CurveNameMaxSize.cx:CurveNameSize.cx;
	m_CurveNameMaxSize.cy=m_CurveNameMaxSize.cy>CurveNameSize.cy?m_CurveNameMaxSize.cy:CurveNameSize.cy;
	ReleaseDC(pDc);

	if(m_bDrawLegend)
	{
		SetMargin();
	}
	return *curveObject;
}
CCurveObj& CChart::CreateNewCurve(CString curveName,bool timeBaseFlag,COLORREF curveColor,int lineWidth)
{
	CCurveObj* curveObject = NULL;
	//若尚无曲线
	if(m_CurveMap.IsEmpty())
	{
		CDC* pDc=GetDC();
		CSize CurveNameSize;
		CurveNameSize = pDc->GetTextExtent(curveName);
		m_CurveNameMaxSize=CurveNameSize;
		ReleaseDC(pDc);
	}
	//若已有曲线
	if(m_CurveMap.Lookup(curveName,(CObject*&)curveObject)==FALSE)
	{
		curveObject = new CCurveObj(m_rectPlot);
		if(curveObject)
		{
			curveObject->SetTimeBaseFlag(timeBaseFlag);
			curveObject->SetCurveColor(curveColor,lineWidth);
			m_CurveMap.SetAt(curveName,curveObject);
		}
		//更新最长名称曲线尺寸，用于绘制图例
		CDC* pDc=GetDC();
		CSize CurveNameSize;
		CurveNameSize = pDc->GetTextExtent(curveName);
		m_CurveNameMaxSize.cx=m_CurveNameMaxSize.cx>CurveNameSize.cx?m_CurveNameMaxSize.cx:CurveNameSize.cx;
		m_CurveNameMaxSize.cy=m_CurveNameMaxSize.cy>CurveNameSize.cy?m_CurveNameMaxSize.cy:CurveNameSize.cy;
		ReleaseDC(pDc);

	}
	if(m_bDrawLegend)
	{
		SetMargin();
	}
	return *curveObject;
}
//删除选中曲线
bool CChart::DeleteCurveSelected()
{
	if(0 == m_SetSelCurveName.size())
		return false;
	CCurveObj* curveObject = NULL;
	set<CString>::iterator iter=m_SetSelCurveName.begin();          
	while(iter!=m_SetSelCurveName.end())
	{
		if(m_CurveMap.Lookup(*iter,(CObject*&)curveObject))
		{
			delete curveObject;
			curveObject = NULL;
			m_CurveMap.RemoveKey(*iter);
		}
		iter++;
	}
	if(m_CurveMap.GetCount() == 0)
	{
		m_ptMaxPos.InitPoint(0.0,0.0);
		m_ptMinPos.InitPoint(0.0,0.0);
		m_ptZoomOffset.InitPoint(0.0,0.0);
		m_ptMoveOffset.InitPoint(0.0,0.0);
		m_CurveNameMaxSize=0;
	}
	else
		ReCalculateAllCurveMaxMin();

	this->SetMargin();
	this->InvalidateChart();
	return true;
}
//##ModelId=48A14588033C
BOOL CChart::DestroyCurve(CString curveName/* = _T("\0")*/)
{
	BOOL rc = FALSE;
	CCurveObj* curveObject = NULL;
	if(curveName.IsEmpty())  //删除所有曲线
	{
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
	}
	else
	{
		if(m_CurveMap.Lookup(curveName,(CObject*&)curveObject))
		{
			delete curveObject;
			curveObject = NULL;
			m_CurveMap.RemoveKey(curveName);
			rc = TRUE;
		}
		else
		{
			rc = FALSE;
		}
	}
	if(m_CurveMap.GetCount() == 0)
	{
		m_ptMaxPos.InitPoint(0.0,0.0);
		m_ptMinPos.InitPoint(0.0,0.0);
		m_ptZoomOffset.InitPoint(0.0,0.0);
		m_ptMoveOffset.InitPoint(0.0,0.0);
		m_CurveNameMaxSize=0;
	}
	else
		ReCalculateAllCurveMaxMin();
	this->SetMargin();	
	InvalidateCtrl();
	this->InvalidateChart();
	return rc;
}
//##ModelId=48A145890010
void CChart::SetCurveColor(CString curveName,COLORREF color/* = RGB(255,0,0)*/,int lineWidth/*=1*/)
{
	CCurveObj* curveObject = NULL;
	if(m_CurveMap.Lookup(curveName,(CObject*&)curveObject))
	{
		curveObject->SetCurveColor(color,lineWidth);
	}
}
void CChart::SelectCurve(CString curveName/* = _T("\0")*/)
{
	if(curveName.IsEmpty())
	{
		m_sCurrentControlCurve = _T("\0");
		m_pCurSelCurveObj = NULL;
	}
	else
	{
		CCurveObj* curveObject = NULL;
		if(m_CurveMap.Lookup(curveName,(CObject*&)curveObject))
		{
			m_sCurrentControlCurve = curveName;
			m_pCurSelCurveObj = curveObject;
		}
	}
	//m_iControlMode = CURVE_MOVE;  //选中曲线后的默认操作是移动曲线
	this->InvalidateChart();
}
bool CChart::SetCurveSelected(CString curveName/* = _T("\0")*/)
{
	//清空已选择的曲线
	m_SetSelCurveName.clear();
	//添加选中曲线
	CCurveObj* curveObject = NULL;
	if(curveName.IsEmpty())  //删除所有曲线
	{
		POSITION pos = m_CurveMap.GetStartPosition();	
		while(pos)
		{
			m_CurveMap.GetNextAssoc(pos,curveName, (CObject*&)curveObject);
			if(curveObject)
			{
				m_SetSelCurveName.insert(curveName);
			}
		}   
	}
	else
	{
		m_SetSelCurveName.insert(curveName);
	}
	//重绘
	this->InvalidateChart();
	return true;
}
bool CChart::AddSelectedCurve(CString curveName)
{
	m_SetSelCurveName.insert(curveName);
	this->InvalidateChart();
	return true;
}


void CChart::SetMargin(int xMargin/*=30*/,int yMargin/* = 20*/)
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
//##ModelId=48A145890001
bool CChart::HitTest(CPoint point)
{
	CString _curveName;
	CCurveObj* curveObject = NULL;
	POSITION pos = m_CurveMap.GetStartPosition();
	CPoint minPoint,tempPoint,scrPoint(0,0);
	double minVal=point.y;
	double _minVal = 0.0;
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
				m_sCurrentControlCurve = _curveName;
				m_pCurSelCurveObj = curveObject;
			}
		}
	}
	return true;
}
//##ModelId=48A14589002E
void CChart::MoveCurve(CString curveName /*= _T("\0")*/,int moveType /*= MOVE_RESET*/,double percent /*= 5*/)
{
	if(m_sCurrentControlCurve != curveName)
		m_sCurrentControlCurve = curveName;
	if(curveName.IsEmpty())  //所有曲线都进行移动操作
	{
		CString _curveName;
		CCurveObj* curveObject = NULL;
		POSITION pos = m_CurveMap.GetStartPosition();
		while(pos)
		{
			m_CurveMap.GetNextAssoc(pos,_curveName, (CObject*&)curveObject);
			if(curveObject)
			{
				IndividualCurveMove(_curveName,moveType,percent);
			}
		}
	}
	else
	{
		IndividualCurveMove(curveName,moveType,percent);
	}
	this->InvalidateChart();
}
//曲线缩放
//##ModelId=48A145890040
void CChart::ZoomCurve(CString curveName /*= _T("\0")*/,int zoomType /*= ZOOM_RESET*/,double percent /*= 5*/)
{
	if(m_sCurrentControlCurve != curveName)
		m_sCurrentControlCurve = curveName;
	if(curveName.IsEmpty())  //所有曲线都进行缩放操作
	{
		CString _curveName;
		CCurveObj* curveObject = NULL;
		POSITION pos = m_CurveMap.GetStartPosition();
		while(pos)
		{
			m_CurveMap.GetNextAssoc(pos,_curveName, (CObject*&)curveObject);
			if(curveObject)
			{
				IndividualCurveZoom(_curveName,zoomType,percent);
			}
		}
	}
	else
	{
		IndividualCurveZoom(curveName,zoomType,percent);
	}
	this->InvalidateChart();
}
//##ModelId=48A1458900EC
void CChart::IndividualCurveMove(CString curveName,int moveType,double percent)
{
	if(percent <0 || percent >100)
		return ;
	CCurveObj* curveObject = NULL;
	if(m_CurveMap.Lookup(curveName,(CObject*&)curveObject))
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
}
//##ModelId=48A14589010B
void CChart::IndividualCurveZoom(CString curveName,int zoomType,double percent)
{
	if(percent <0 || percent >100)
		return ;
	CCurveObj* curveObject = NULL;
	if(m_CurveMap.Lookup(curveName,(CObject*&)curveObject))
	{
		switch(zoomType)
		{
		case CURVE_ZOOM_IN:
			{
				curveObject->ZoomIn(percent,percent);
				break;
			}
		case CURVE_ZOOM_IN_V:
			{
				curveObject->ZoomIn(0,percent);
				break;
			}
		case CURVE_ZOOM_IN_H:
			{
				curveObject->ZoomIn(percent,0);
				break;
			}
		case CURVE_ZOOM_OUT:
			{
				curveObject->ZoomOut(percent,percent);
				break;
			}
		case CURVE_ZOOM_OUT_V:
			{
				curveObject->ZoomOut(0,percent);
				break;
			}
		case CURVE_ZOOM_OUT_H:
			{
				curveObject->ZoomOut(percent,0);
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
}
//##ModelId=48A145880222
void CChart::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	this->SetFocus();
	if(m_rectPlot.PtInRect(point))
	{
		if(!m_bLeftButtonDown)
		{
			m_recMouseSelect.TopLeft() = point;
			m_Line.m_Begin=point;

			POSITION pos = m_CurveMap.GetStartPosition();
			CString _curveName;
			CCurveObj* curveObject = NULL;
			if(pos)
			{

				m_CurveMap.GetNextAssoc(pos,_curveName, (CObject*&)curveObject);

				if(curveObject)
				{
					m_BeginPoint = curveObject->Position2Data(m_Line.m_Begin,m_Line.m_Begin); //将初始点置于图谱上
					//_PointEnd = curveObject->Position2Data(point,point);
				}

				m_bLeftButtonDown = true;

				//选择曲线
				if(CURVE_SELECT == m_iControlMode)
				{
					CString nearestCurveName;
					CString _curveName;
					CCurveObj* curveObject = NULL;

					CPoint minPoint,tempPoint,scrPoint(0,0);
					double threshold = 10;
					double minVal;//=10;
					double _minVal;// = 0.0;
					//COLORREF color;
					//
					if(!m_CurveMap.IsEmpty())
					{
						POSITION pos = m_CurveMap.GetStartPosition();
						m_CurveMap.GetNextAssoc(pos,_curveName, (CObject*&)curveObject);
						if(curveObject)
						{
							curveObject->Position2Data(point,scrPoint);
							tempPoint = point - scrPoint;
							minVal = sqrt((double)(tempPoint.x*tempPoint.x+tempPoint.y*tempPoint.y));
							nearestCurveName = _curveName;

							//得到最近的曲线距离以及曲线名称
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
										nearestCurveName = _curveName;
									}
								}
							}
							int i=minVal<threshold;
							if(minVal<threshold)
							{
								int retn = GetAsyncKeyState(VK_CONTROL);
								if(GetAsyncKeyState(VK_CONTROL))
									this->AddSelectedCurve(nearestCurveName);
								else
									this->SetCurveSelected(nearestCurveName);
								//获取曲线名长度
								int curveNamelenth=nearestCurveName.GetLength();
								wchar_t *str=(wchar_t *)malloc(sizeof(wchar_t)*(curveNamelenth*2));
								wcscpy(str,CT2CW(nearestCurveName));

								::SendMessage(this->GetParent()->m_hWnd,WM_CHANGESELECTEDCURVE,m_SetSelCurveName.size(),(LPARAM)str);
							}
						}
					}
				}
			}
		}
	}
	CWnd::OnLButtonDown(nFlags, point);
}

//##ModelId=48A145880233
void CChart::OnMouseMove(UINT nFlags, CPoint point) 
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
					POSITION pos = m_CurveMap.GetStartPosition();
					CString _curveName;
					CCurveObj* curveObject = NULL;
					CFloatPoint _PointEnd;
					m_CurveMap.GetNextAssoc(pos,_curveName, (CObject*&)curveObject);

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
		else if(m_bLeftButtonDown &&(m_iControlMode == CURVE_MOVE || m_iControlMode == CURVE_SELECT|| 
			m_iControlMode == CURVE_ZOOM_OUT|| m_iControlMode == CURVE_ZOOM_IN))
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
						POSITION pos = m_CurveMap.GetStartPosition();
						while(pos)
						{
							m_CurveMap.GetNextAssoc(pos,curveName, (CObject*&)curveObject);
							if(curveObject)
							{
								this->IndividualCurveControl(curveObject,m_iControlMode,m_recMouseSelect);
							}
						}
					}
					else
					{
						CCurveObj* curveObject = NULL;
						if(m_CurveMap.Lookup(m_sCurrentControlCurve,(CObject*&)curveObject))
						{
							this->IndividualCurveControl(curveObject,m_iControlMode,m_recMouseSelect);
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
void CChart::OnLButtonUp(UINT nFlags, CPoint point) 
{
	if(m_rectPlot.PtInRect(point) )
	{
		if(m_bLeftButtonDown)
		{
			m_recMouseSelect.BottomRight() = point;

			m_Line.m_End = point;

			CString curveName;
			CCurveObj* curveObject = NULL;
			POSITION pos = m_CurveMap.GetStartPosition();

			m_CurveMap.GetNextAssoc(pos,curveName, (CObject*&)curveObject);

			CFloatPoint _fPoint;
			int Index = -1;
			if(curveObject)
			{
				_fPoint = curveObject->Position2Data(m_Line.m_End,m_Line.m_End);
				Index = curveObject->GetDataIndex(_fPoint);
			}
			_fPoint.m_PosY = point.y;

			if(m_sCurrentControlCurve.IsEmpty())
			{

				CString curveName;
				CCurveObj* curveObject = NULL;
				POSITION pos = m_CurveMap.GetStartPosition();
				while(pos)
				{
					m_CurveMap.GetNextAssoc(pos,curveName, (CObject*&)curveObject);
					if(curveObject)
					{
						this->IndividualCurveControl(curveObject,m_iControlMode,m_recMouseSelect);
					}
				}
			}
			else
			{
				CCurveObj* curveObject = NULL;
				if(m_CurveMap.Lookup(m_sCurrentControlCurve,(CObject*&)curveObject))
				{
					this->IndividualCurveControl(curveObject,m_iControlMode,m_recMouseSelect);
				}
			}

			if(m_iControlMode == CURVE_PEAKAREA_CAL) 
				this->ShowSelectedPeakArea();

			if(m_iControlMode == CURVE_LABEL)
			{
				if(Index >=0)	
					m_MapLabelPoint[Index] = _fPoint;
			}


			this->InvalidateChart();
			m_recMouseSelect.SetRectEmpty();
		}	
	}
	m_bLeftButtonDown = false;
	CWnd::OnLButtonUp(nFlags, point);
}
//##ModelId=48A145880263
BOOL CChart::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt) 
{
	CCurveObj* curveObject = NULL;
	if(m_sCurrentControlCurve.IsEmpty())
	{
		CString _curveName;
		CCurveObj* curveObject = NULL;
		POSITION pos = m_CurveMap.GetStartPosition();
		while(pos)
		{
			m_CurveMap.GetNextAssoc(pos,_curveName, (CObject*&)curveObject);
			if(curveObject)
			{
				IndividualCurveWheelContol(curveObject,m_iControlMode,m_iVerticalControlMode,zDelta,pt);
			}
		}
	}
	else
	{
		CCurveObj* curveObject = NULL;
		if(m_CurveMap.Lookup(m_sCurrentControlCurve,(CObject*&)curveObject))
		{
			IndividualCurveWheelContol(curveObject,m_iControlMode,m_iVerticalControlMode,zDelta,pt);
		}
	}
	this->InvalidateChart();
	return CWnd::OnMouseWheel(nFlags, zDelta, pt);
}
//##ModelId=48A1458900BB
void CChart::IndividualCurveWheelContol(CCurveObj* curveObj/* = NULL*/,int controlMode/* = CURVE_MOVE*/,bool verticalControl/* = false*/,int zDelta/* = 0*/,CPoint point)
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
void CChart::IndividualCurveControl(CCurveObj* curveObj/* = NULL*/,int controlMode /*= CURVE_MOVE*/,CRect controlRect)
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
}
//##ModelId=48A14589005E
BOOL CChart::GetMaxDataRect(CString curveName/*=_T("\0")*/)  //获得所有曲线中最大的数据区域值
{	
	BOOL rc = FALSE;
	CCurveObj* curveObject = NULL;

	if(m_bAllCurveSameBase && curveName.IsEmpty())  //
	{
		CString _curveName;
		POSITION pos = m_CurveMap.GetStartPosition();
		if(pos)
		{
			m_CurveMap.GetNextAssoc(pos,_curveName, (CObject*&)curveObject);
			if(curveObject)
			{
				//如果是进行缩放，则直接返回
				curveObject->GetDrawCurvePar(m_ptMaxPos,m_ptMinPos,m_ptMoveOffset,m_ptZoomOffset);
				if(m_iControlMode == CURVE_ZOOM_IN ||m_iControlMode == CURVE_ZOOM_IN_V||
					m_iControlMode == CURVE_ZOOM_IN_H||m_iControlMode == CURVE_ZOOM_OUT||
					m_iControlMode == CURVE_ZOOM_OUT_V||m_iControlMode == CURVE_ZOOM_OUT_H);		
				else
					//否则计算所有曲线
					this->ReCalculateAllCurveMaxMin();
			}
		}
	}
	else
	{
		if(curveName.IsEmpty())
		{
			m_ptMaxPos.InitPoint(0.0,0.0);
			m_ptMinPos.InitPoint(0.0,0.0);
			m_ptZoomOffset.InitPoint(0.0,0.0);
			m_ptMoveOffset.InitPoint(0.0,0.0);

			rc = TRUE;
		}
		else
		{
			if(m_CurveMap.Lookup(curveName,(CObject*&)curveObject))
			{
				curveObject->GetDrawCurvePar(m_ptMaxPos,m_ptMinPos,m_ptMoveOffset,m_ptZoomOffset);
				//curveObject->GetAllCurveCurvePar(m_ptMaxPos,m_ptMinPos,m_ptMoveOffset,m_ptZoomOffset);
				rc = TRUE;
			}
		}
	}
	return rc;
}
int CChart::ReCalculateAllCurveMaxMin()
{
	CCurveObj* curveObject = NULL;

	m_ptMaxPos.InitPoint(-10000000000.0,-10000000000.0);//当前最大点
	m_ptMinPos.InitPoint(10000000000.0,10000000000.0);//当前最小点

	CFloatPoint MaxPos;
	CFloatPoint MinPos;

	CDC *pDc=GetDC();
	CSize CurveNameSize;


	CString _curveName;
	POSITION pos = m_CurveMap.GetStartPosition();
	while(pos)
	{
		m_CurveMap.GetNextAssoc(pos,_curveName, (CObject*&)curveObject);
		if(curveObject)
		{
			curveObject->ReCalculateMaxMinPar();
			curveObject->GetDrawCurvePar(MaxPos,MinPos,m_ptMoveOffset,m_ptZoomOffset);
			this->CalculateMaxPoint(m_ptMaxPos,&MaxPos);
			this->CalculateMinPoint(m_ptMinPos,&MinPos);

			//计算曲线图例间隙
			CurveNameSize = pDc->GetTextExtent(_curveName);
			m_CurveNameMaxSize.cx=m_CurveNameMaxSize.cx>CurveNameSize.cx?m_CurveNameMaxSize.cx:CurveNameSize.cx;
			m_CurveNameMaxSize.cy=m_CurveNameMaxSize.cy>CurveNameSize.cy?m_CurveNameMaxSize.cy:CurveNameSize.cy;
		}
	}
	m_ptMaxPosConst = m_ptMaxPos;
	m_ptMinPosConst = m_ptMinPos;

	//if(curveObject)
	//	curveObject->SetAllCurveMaxMinPar(m_ptMaxPos,m_ptMinPos);
	pos = m_CurveMap.GetStartPosition();
	while(pos)
	{
		m_CurveMap.GetNextAssoc(pos,_curveName, (CObject*&)curveObject);
		if(curveObject)
		{
			curveObject->SetCurveMaxMinPar(m_ptMaxPos,m_ptMinPos);
		}
	}



	return 0;
}
//##ModelId=48A14589006E
void CChart::CalculateMaxPoint(CFloatPoint& pointDes,const CFloatPoint* PointSource)
{
	if(PointSource->m_PosX > pointDes.m_PosX)
		pointDes.m_PosX = PointSource->m_PosX;
	if(PointSource->m_PosY > pointDes.m_PosY)
		pointDes.m_PosY = PointSource->m_PosY;
}
//##ModelId=48A14589008D
void CChart::CalculateMinPoint(CFloatPoint& pointDes,const CFloatPoint* PointSource)
{
	if(PointSource->m_PosX < pointDes.m_PosX)
		pointDes.m_PosX = PointSource->m_PosX;
	if(PointSource->m_PosY < pointDes.m_PosY)
		pointDes.m_PosY = PointSource->m_PosY;
}
//##ModelId=48A145880282
void CChart::OnRButtonDown(UINT nFlags, CPoint point) 
{
	this->SetFocus();  //获得焦点
	CWnd::OnRButtonDown(nFlags, point);
}

//##ModelId=48A1458802A0
void CChart::OnContextMenu(CWnd* pWnd, CPoint point) 
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

		/*if(!m_sCurrentControlCurve.IsEmpty())
		{
		str = L"当前选中曲线:";
		str+=m_sCurrentControlCurve;

		}
		else
		{
		str = L"当前选中曲线:";
		str+= L"ALL";
		}*/	
		//menu.AppendMenu(MF_STRING,9990,str);
		//menu.AppendMenu(MF_SEPARATOR,9990,str);

		//menu.AppendMenu(MF_STRING,9998,L"独立坐标系");
		//menu.AppendMenu(MF_STRING,9999,L"数据获取");

		//复位曲线菜单组
		//CMenu submenuReset;
		//submenuReset.CreatePopupMenu();
		//menu.AppendMenu(MF_POPUP,(UINT_PTR)submenuReset.m_hMenu,L"复位曲线");

		//str = L"复位单条曲线:";
		//str+=m_sCurrentControlCurve;
		//submenuReset.AppendMenu(MF_STRING,10000,str);
		//submenuReset.AppendMenu(MF_STRING,10007,L"复位所有曲线");


		//选择曲线菜单组
		CMenu submenuSelect;
		submenuSelect.CreatePopupMenu();
		menu.AppendMenu(MF_POPUP,(UINT_PTR)submenuSelect.m_hMenu,L"选择图谱");

		submenuSelect.AppendMenu(MF_STRING,10008,L"选择单条图谱");
		submenuSelect.AppendMenu(MF_STRING,10001,L"选择所有图谱");



		menu.AppendMenu(MF_STRING,10006,L"移动图谱");
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


				//int retn=GetLastError();
			}
		} 
		menu.TrackPopupMenu(TPM_LEFTALIGN|TPM_BOTTOMALIGN|TPM_NONOTIFY,point.x,point.y,this);
	}
}

//##ModelId=48A1458801A5
BOOL CChart::OnCommand(WPARAM wParam, LPARAM lParam) 
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
			::SendMessage(this->GetParent()->m_hWnd,WM_CHANGECHARTMODE,m_iControlMode,0);
			break;
		}
	case 10001:  //select all curve
		{
			m_iControlMode = CURVE_SELECT;

			this->SetCurveSelected();
			::SendMessage(this->GetParent()->m_hWnd,WM_CHANGESELECTEDCURVE,m_SetSelCurveName.size(),NULL);

			::SendMessage(this->GetParent()->m_hWnd,WM_CHANGECHARTMODE,m_iControlMode,0);
			this->InvalidateChart();
			break;
		}
	case 10006:  //移动曲线，可通过鼠标，方向键，鼠标滚轮控制曲线的移动
		{ 
			m_iControlMode = CURVE_MOVE;
			::SendMessage(this->GetParent()->m_hWnd,WM_CHANGECHARTMODE,m_iControlMode,0);
			break;
		}
	case 10002: //曲线放大
		{
			m_iControlMode = CURVE_ZOOM_IN;
			::SendMessage(this->GetParent()->m_hWnd,WM_CHANGECHARTMODE,m_iControlMode,0);
			break;
		}
	case 10003: //曲线缩小
		{
			m_iControlMode = CURVE_ZOOM_OUT;
			::SendMessage(this->GetParent()->m_hWnd,WM_CHANGECHARTMODE,m_iControlMode,0);
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
void CChart::OnUpdateMenuState(CCmdUI* pCmdUI)//更新菜单上的状态显示
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
BOOL CChart::PreTranslateMessage(MSG* pMsg) 
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
void CChart::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	ResetCurve();
	this->SetFocus();

	CWnd::OnLButtonDblClk(nFlags, point);
}

//##ModelId=48A1458801B7
LRESULT CChart::DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	return CWnd::DefWindowProc(message, wParam, lParam);
}

//##ModelId=48A1458901E4
int CChart::FloatDataPrecise(double f)
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
void CChart::DrawSelectFocus(CDC* pDc,CPoint&  point)
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
}
void CChart::DrawZoomRect(CDC* pDc,CRect rect)
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
void CChart::DrawSelectRange(CDC *pDc,CLine Line)
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
void CChart::DrawSelectRangeText(CDC* pDc)
{
	//画起始位置
	CPoint point;
	CPoint srcPoint;
	GetCursorPos(&point);
	this->ScreenToClient(&point);

	CPen *oldPen;
	CPen newPen(PS_SOLID,1,RGB(0,0,0));
	oldPen = pDc->SelectObject(&newPen);

	POSITION pos = m_CurveMap.GetStartPosition();
	CString _curveName;
	CCurveObj* curveObject = NULL;

	CFloatPoint _PointBegin;
	CFloatPoint _PointEnd;
	m_CurveMap.GetNextAssoc(pos,_curveName, (CObject*&)curveObject);

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
		}
		COLORREF textColor = RGB(0,0,0);
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
void CChart::DrawNearestPt2Cursor(CDC *pDc,CPoint _point)
{
	CPen zoomRectPen(PS_SOLID,2,RGB(0,0,255));
	CPen* oldPen = pDc->SelectObject(&zoomRectPen);

	pDc->Rectangle(_point.x-2,_point.y-2,_point.x+2,_point.y+2);
	pDc->SelectObject(oldPen);
}

void CChart::DrawGetDataCross(CDC *pDc)
{
	CString str;
	CString tempStr;
	COLORREF textColor = m_crGridColor;
	COLORREF girdColor = m_crGridColor;
	COLORREF frameColor = m_crFrameColor;
	CSize textSize;
	CPen* oldPen;

	CFloatPoint _Point;	
	CPoint srcPoint;

	CPen crossPen(PS_SOLID,0,m_crGridColor);
	oldPen = pDc->SelectObject(&crossPen);
	//绘制十字线
	pDc->MoveTo(m_ptCrossCenter.x,m_rectPlot.top);
	pDc->LineTo(m_ptCrossCenter.x,m_rectPlot.bottom+1);
	pDc->MoveTo(m_rectPlot.left,m_ptCrossCenter.y);
	pDc->LineTo(m_rectPlot.right+1,m_ptCrossCenter.y);

	if(m_bShowDataFlowMouse)  //数据显示跟随鼠标
	{
		//pDc->SetTextAlign (TA_RIGHT|TA_TOP);//显示文字方向从右向左
		pDc->SetTextAlign (TA_LEFT|TA_TOP);//显示文字方向从左向右
	}
	else
	{
		pDc->SetTextAlign (TA_LEFT|TA_TOP);//显示文字方向从右向左
	}

	if(m_pCurSelCurveObj)//显示单条曲线的坐标值
	{ 
		_Point = m_pCurSelCurveObj->Position2Data(m_ptCrossCenter,srcPoint);	
		textColor = m_pCurSelCurveObj->GetCurveColor();
		this->DrawDataFocus(pDc,srcPoint,textColor);

		str+=m_sCurrentControlCurve;
		tempStr.Format(L":%.2lf,%.2lf",_Point.m_PosX,_Point.m_PosY);
		str+=tempStr;
		textSize = pDc->GetTextExtent(str); 

		pDc->SetTextColor(textColor);
		COLORREF bkColor = pDc->SetBkColor(m_crPlotAreaBackColor);
		if(m_bShowDataFlowMouse)  
		{

			pDc->TextOut(m_ptCrossCenter.x-5,m_ptCrossCenter.y+5,str);
		}
		else
		{
			pDc->TextOut(m_rectPlot.left+5,m_rectPlot.top+2,str);
		}	
		pDc->SetBkColor(bkColor);
	}
	else      //显示多条曲线的坐标值
	{
		bool leftsideshow=false; //是否左边显示

		CString _curveName;
		int i=0;
		CCurveObj* curveObject = NULL;
		str.Empty();

		POSITION pos = m_CurveMap.GetStartPosition();
		if(pos)
		{
			m_CurveMap.GetNextAssoc(pos,_curveName, (CObject*&)curveObject);
			if(curveObject)
			{
				_Point = curveObject->Position2Data(m_ptCrossCenter,srcPoint);
			}
			//获取基团信息
			CSize strGroupsInfoSize;
			CString strGroupsInfo;

			strGroupsInfo.Format(L"可能的基团：");
			int ShortestLen=strGroupsInfo.GetLength();

			if(abs(_Point.m_PosX-2962)<=10||abs(_Point.m_PosX-2872)<=10||abs(_Point.m_PosX-1375)<=5)
			{
				strGroupsInfo+=L"—CH3(强),";
			}

			if(abs(_Point.m_PosX-1450)<=20)
			{
				strGroupsInfo+=L"—CH3(中),";
			}

			if(abs(_Point.m_PosX-2926)<=10||abs(_Point.m_PosX-2853)<=10)
			{
				strGroupsInfo+=L"—CH2—(强),";
			}

			if(abs(_Point.m_PosX-1465)<=10)
			{
				strGroupsInfo+=L"—CH2—(强),";
			}

			if(abs(_Point.m_PosX-2890)<=10||abs(_Point.m_PosX-1340)<=1)
			{
				strGroupsInfo+=L"—CH—(弱),";
			}

			if(abs(_Point.m_PosX-1390)<=5||abs(_Point.m_PosX-1225)<=25)
			{
				strGroupsInfo+=L"—C(CH3)3—(中),";
			}

			if(abs(_Point.m_PosX-1367.5)<=2.5)
			{
				strGroupsInfo+=L"—C(CH3)3—(强),";
			}

			if(abs(_Point.m_PosX-1367.5)<=2.5)
			{
				strGroupsInfo+=L"—C(CH3)3—(强),";
			}

			if(abs(_Point.m_PosX-835.5)<=172.5)
			{
				strGroupsInfo+=L"—CH═CH—(强),";
			}

			if(abs(_Point.m_PosX-3305)<=5)
			{
				strGroupsInfo+=L"R—R≡CH(中),";
			}

			if(abs(_Point.m_PosX-2245)<=25)
			{
				strGroupsInfo+=L"R—R≡C—R(弱),";
			}


			if(abs(_Point.m_PosX-2245)<=25)
			{
				strGroupsInfo+=L"R—R≡C—R(弱),";
			}

			if(abs(_Point.m_PosX-1735)<=85||abs(_Point.m_PosX-1225)<=75)
			{
				strGroupsInfo+=L"酯类(强),";
			}

			if(abs(_Point.m_PosX-1825)<=25||abs(_Point.m_PosX-1760)<=20||abs(_Point.m_PosX-1110)<=60)
			{
				strGroupsInfo+=L"链酸酐(强),";
			}

			if(abs(_Point.m_PosX-1845)<=25||abs(_Point.m_PosX-1775)<=25||abs(_Point.m_PosX-1250)<=50)
			{
				strGroupsInfo+=L"环酸酐(强),";
			}

			if(abs(_Point.m_PosX-1730)<=10||abs(_Point.m_PosX-1702.5)<=12.5||abs(_Point.m_PosX-1695)<=45)
			{
				strGroupsInfo+=L"—COOH(强),";
			}

			if(abs(_Point.m_PosX-2917.5)<=417.5||abs(_Point.m_PosX-1235.5)<=30.5)
			{
				strGroupsInfo+=L"—COOH(中),";
			}

			if(abs(_Point.m_PosX-1430)<=20||abs(_Point.m_PosX-930)<=30||abs(_Point.m_PosX-1712.5)<=12.5)
			{
				strGroupsInfo+=L"—COOH(弱),";
			}

			if(abs(_Point.m_PosX-1635)<=95)
			{
				strGroupsInfo+=L"酮类(强),";
			}

			if(abs(_Point.m_PosX-1610)<=45)
			{
				strGroupsInfo+=L"—CHO(强),";
			}

			if(abs(_Point.m_PosX-1102.5)<=97.5)
			{
				strGroupsInfo+=L"醚类(强),";
			}

			if(abs(_Point.m_PosX-3415)<=290||abs(_Point.m_PosX-1250)<=85)
			{
				strGroupsInfo+=L"酚类(强),";
			}

			if(abs(_Point.m_PosX-1352.5)<=37.5)
			{
				strGroupsInfo+=L"酚类(中),";
			}

			if(abs(_Point.m_PosX-3450)<=250||abs(_Point.m_PosX-1125)<=125||abs(_Point.m_PosX-700)<=50)
			{
				strGroupsInfo+=L"醇类(强),";
			}

			if(abs(_Point.m_PosX-2237.5)<=22.5)
			{
				strGroupsInfo+=L"—C≡N(强),";
			}
			strGroupsInfoSize=pDc->GetTextExtent(strGroupsInfo);
			if(m_ptCrossCenter.x+strGroupsInfoSize.cx>m_rectPlot.right)
			{
				leftsideshow=true;
			}

			pos = m_CurveMap.GetStartPosition();
			while(pos)
			{
				m_CurveMap.GetNextAssoc(pos,_curveName, (CObject*&)curveObject);
				if(curveObject)
				{
					_Point = curveObject->Position2Data(m_ptCrossCenter,srcPoint);
					textColor = curveObject->GetCurveColor();
					this->DrawDataFocus(pDc,srcPoint,textColor);

					str+=_curveName;
					tempStr.Format(L":%.2lf,%.2lf",_Point.m_PosX,_Point.m_PosY);
					str+=tempStr;
					textSize = pDc->GetTextExtent(str); 

					if(m_ptCrossCenter.x+textSize.cx>m_rectPlot.right)
					{
						leftsideshow=true;
					}

					pDc->SetTextColor(textColor);	
					COLORREF bkColor = pDc->SetBkColor(m_crPlotAreaBackColor);
					int oldmode = pDc->SetBkMode(TRANSPARENT);

					if(m_bShowDataFlowMouse)
					{
						if(leftsideshow)
						{
							pDc->TextOut(m_ptCrossCenter.x-textSize.cx,m_ptCrossCenter.y+5+i*textSize.cy,str);
						}
						else
						{
							pDc->TextOut(m_ptCrossCenter.x+25,m_ptCrossCenter.y+5+i*textSize.cy,str);
						}
					}
					else
					{
						pDc->TextOut(m_rectPlot.left+5,m_rectPlot.top+2+i*textSize.cy,str);
					}

					pDc->SetBkMode(oldmode);
					pDc->SetBkColor(bkColor);
					str.Empty();
					i++;
				}
			}
			if(ShortestLen<strGroupsInfo.GetLength())
			{
				strGroupsInfo=strGroupsInfo.Left(strGroupsInfo.GetLength()-1);

				pDc->SetTextColor(RGB(0,0,0));
				COLORREF bkColor = pDc->SetBkColor(m_crPlotAreaBackColor);
				int oldmode = pDc->SetBkMode(TRANSPARENT);
				if(m_bShowDataFlowMouse)
				{
					if(leftsideshow)
					{
						pDc->TextOut(m_ptCrossCenter.x-5-strGroupsInfoSize.cx,m_ptCrossCenter.y+10+i*textSize.cy,strGroupsInfo);
					}
					else
					{
						pDc->TextOut(m_ptCrossCenter.x+5,m_ptCrossCenter.y+10+i*textSize.cy,strGroupsInfo);
					}
				}
				else
				{
					pDc->TextOut(m_rectPlot.left+5,m_rectPlot.top+2+i*textSize.cy,strGroupsInfo);
				}
				pDc->SetBkMode(oldmode);
				pDc->SetBkColor(bkColor);
			}
		}
	}

	this->DrawUnPlotArea();
	if(this->m_bDrawLegend)
		this->DrawLegend();
	pDc->SelectObject(oldPen);
}

void CChart::PreSubclassWindow() 
{
	// TODO: Add your specialized code here and/or call the base class

	CWnd::PreSubclassWindow();
}
void CChart::DrawDataFocus(CDC *pDC,CPoint point,COLORREF color)
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
void CChart::ChangeToIndividualCoordinateMode(bool individualMode/* = true*/)
{
	m_bAllCurveSameBase = !individualMode;
	this->SetAllCurveDrawMode(m_bAllCurveSameBase);
}

void CChart::ChangeToGetDataMode()  //让对象切换到取数据模式，其他模式将被屏蔽
{
	WPARAM controlCode = MAKELONG(9999,0);
	this->PostMessage(WM_COMMAND,controlCode,0);
}
void CChart::ResetCurve(bool allCurve/* = false*/)  //复位区县，如果选中了一条曲线，则复位此条曲线，否则，将复位所有的曲线
{
	WPARAM controlCode = 0x0;
	controlCode = MAKELONG(10007,0);
	this->PostMessage(WM_COMMAND,controlCode,0);
}
void CChart::ChangeToSelectMode(bool allCurve/* = false*/)  //切换到选择曲线模式
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
void CChart::ChangeToMoveMode()  //切换到移动曲线模式
{
	WPARAM controlCode = MAKELONG(10006,0);
	this->PostMessage(WM_COMMAND,controlCode,0);
}
void CChart::ChangeToZoomInMode() //
{
	WPARAM controlCode = MAKELONG(10002,0);
	this->PostMessage(WM_COMMAND,controlCode,0);
}
void CChart::ChangeToZoomOutMode()
{
	WPARAM controlCode = MAKELONG(10003,0);
	this->PostMessage(WM_COMMAND,controlCode,0);
}
void CChart::ChangeToVerticalMode()
{
	WPARAM controlCode = MAKELONG(10005,0);
	this->PostMessage(WM_COMMAND,controlCode,0);
}
void CChart::ChageToHorizontalMode()
{
	WPARAM controlCode = MAKELONG(10004,0);
	this->PostMessage(WM_COMMAND,controlCode,0);
}
void CChart::ChangeToPeakAreaCalMode()
{
	WPARAM controlCode = MAKELONG(10014,0);
	this->PostMessage(WM_COMMAND,controlCode,0);
}
void CChart::ChangeToLableMode()
{
	WPARAM controlCode = MAKELONG(10015,0);
	this->PostMessage(WM_COMMAND,controlCode,0);
}

void CChart::ChageDrawDirectionX()
{
	WPARAM controlCode = MAKELONG(10009,0);
	this->PostMessage(WM_COMMAND,controlCode,0);
}
void CChart::ChageDrawDirectionY()
{
	WPARAM controlCode = MAKELONG(10010,0);
	this->PostMessage(WM_COMMAND,controlCode,0);
}
void CChart::HideUnselectCurve(bool hide/* = false*/)
{
	m_bHideUnselectCurve = hide;
	InvalidateChart();
}
void CChart::DataShowFollowCursor(bool follow/* = false*/)
{
	m_bShowDataFlowMouse = follow;
}
void CChart::SetBackgroundColor(COLORREF backColor)
{

	m_brushBack.DeleteObject() ;
	m_crBackColor = backColor;
	m_brushBack.CreateSolidBrush(m_crBackColor) ;


	InvalidateChart();
}
void CChart::SetGridLineColor(COLORREF gridLienColor)
{
	m_crGridColor = gridLienColor;
	InvalidateChart();
}



void CChart::PrintChart(CDC* pDC, CPrintInfo* pInfo)//提供的打印接口
{
	if( pDC->GetSafeHdc()==NULL )
		return ;
	CString _curveName;
	CCurveObj* curveObject = NULL;
	//
	CRect rectDraw(0,0, pInfo->m_rectDraw.Width(), pInfo->m_rectDraw.Width()*m_nClientHeight/m_nClientWidth);

	/*	CRect m_rectClient;
	CRect m_rectPlot; */
	int left = rectDraw.Width()*(m_rectClient.Width() - m_rectPlot.Width())/(2*m_rectClient.Width());
	int top = rectDraw.Height()*(m_rectClient.Height() - m_rectPlot.Height())/(2*m_rectClient.Height());
	int right = rectDraw.Width() - left;
	int bottom = rectDraw.Height() - top;

	CRect rectPlot(left,top,right,bottom);

	//画背景
	DrawBackGnd(*pDC,rectPlot);
	//画网格
	DrawGrid(*pDC,rectPlot,rectDraw);
	//画曲线
	int colorIndex=0;
	POSITION pos = m_CurveMap.GetStartPosition();
	while(pos)
	{
		m_CurveMap.GetNextAssoc(pos,_curveName, (CObject*&)curveObject);

		set<CString>::iterator iter =find(m_SetSelCurveName.begin(),m_SetSelCurveName.end(),_curveName);

		if(curveObject&&iter == m_SetSelCurveName.end())
		{
			curveObject->SetCurveColor(m_ArrayCurveColor[colorIndex]);
			curveObject->DrawCurve(pDC,rectPlot);//绘制曲线
			if(colorIndex != m_ArrayCurveColor.GetCount()-1)
				colorIndex++;
			else
				colorIndex=0;
		}
		else if(curveObject&&iter != m_SetSelCurveName.end())
		{
			continue;
		}
	}
	set<CString>::iterator iter =m_SetSelCurveName.begin();
	while(iter!= m_SetSelCurveName.end())
	{
		m_CurveMap.Lookup(*iter, (CObject*&)curveObject);
		if(curveObject)
		{
			curveObject->SetCurveColor(m_SelectedCurveColor);
			curveObject->DrawCurve(pDC,rectPlot);//绘制曲线
		}
		++iter;
	}
	//画标注
	DrawLabel(*pDC,rectPlot);
	//画非作图区
	DrawUnPlotArea(*pDC,rectPlot,rectDraw);
	//画图例
	if(m_bDrawLegend)
	{
		DrawLegend(*pDC,rectPlot);
	}
	return;
}

void CChart::SavePlotAsBmp(CString filename)
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
void CChart::SaveData(CString filePath)
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
}


int CChart::SetCurveTransAbs(bool _transAbs)
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
}

int CChart::SmoothCurve(CCurveDataSetArray& tempArray,CString _curveName,int SmoothMethod,int N)
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
}
CMatrix CChart::GetCurveYData(CString _curveName)
{
	CCurveObj* curveObject = NULL;
	m_CurveMap.Lookup(_curveName,(CObject*&)curveObject);
	if(curveObject)
	{
		return  curveObject->GetYData();

	}
	else
	{
		throw invalid_argument("曲线不存在");
	}
}
CMatrix CChart::GetCurveXData(CString _curveName)
{
	CCurveObj* curveObject = NULL;
	m_CurveMap.Lookup(_curveName,(CObject*&)curveObject);
	if(curveObject)
	{
		return curveObject->GetXData();	
	}
	else
	{
		throw invalid_argument("曲线不存在");
	}
}
int CChart::ShowSelectedNoise()
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
int CChart::ShowSelectedMean()
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

void CChart::ShowSelectedPeakArea() //计算峰面积
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
			return;
		}
	} 


}

BOOL CChart::CurveExist(const CString &_curveName)   //判断该名称的曲线是否存在
{
	CCurveObj *_curveObj = NULL;
	return m_CurveMap.Lookup(_curveName,(CObject*&)_curveObj) ;
}

BOOL CChart::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此添加专用代码和/或调用基类
	if (cs.lpszClass == NULL)
		cs.lpszClass = AfxRegisterWndClass(CS_DBLCLKS);
	return CWnd::PreCreateWindow(cs);
}


BOOL CChart::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
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
void CChart::RemoveAllLabel()
{
	m_MapLabelPoint.clear();
	this->InvalidateChart();
}
vector<int> CChart::GetLabelIndex()
{
	vector<int> tmpIndexVec;

	std::map<int,CFloatPoint>::iterator iterMap = m_MapLabelPoint.begin();

	while(iterMap!=m_MapLabelPoint.end())
	{
		tmpIndexVec.push_back(iterMap->first);
		++iterMap;
	}
	return tmpIndexVec;
}

BOOL operator <(const CPoint &p1,const CPoint &p2 )
{
	return p1.x<p2.x;
}
BOOL operator >(const CPoint &p1,const CPoint &p2 )
{
	return p1.x>p2.x;
}