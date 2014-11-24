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
	m_bDrawReverseX = false;  //��ͼ����
	m_bDrawReverseY = false;  //��ͼ����

	m_bShowMenum = true;
	m_bAllCurveSameBase = false;

	m_bHideUnselectCurve = false;
	m_bShowDataFlowMouse = true;  //ȡ����ʱ�Ƿ������ݸ��������ʾ
	m_bDrawLegend=true;    //�Ƿ����ͼ��

	m_crBackColor  =RGB(255,255,255);


	m_crGridColor = RGB(105,105,105); 
	m_crFrameColor =RGB(0,0,0);
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
	m_iVerticalControlMode = false; //Ĭ����ˮƽ����ģʽ(ָ���ֲ���ģʽ���ƶ������Ź���)
	m_recMouseSelect.SetRectEmpty();
	m_pCurSelCurveObj = NULL;
	m_CurveNameMaxSize=0;
	m_LegendLineLenth=30;

	m_SelectedCurveColor = RGB(255,0,0);  //ѡ��������ɫ

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

	//���������ڴ����
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

	this->SetMargin(m_ptMargin.x,m_ptMargin.y);	//�����趨�߿�
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
				//������������ĳ��ܽ�
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
		dc.BitBlt(0,0,m_nClientWidth, m_nClientHeight,&memDC,0,0,SRCCOPY); //�����ڴ�DC����Ļ
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
	m_dcPlot.SetTextAlign (TA_LEFT|TA_TOP);//��ʾ���ַ����������
	COLORREF LegendColor;

	CSize CurveNameSize;

	CString _curveName;
	int i=0;
	CCurveObj* curveObject = NULL;


	m_dcPlot.SetBkColor (m_crPlotAreaBackColor) ;

	//���
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
		//�õ����������ַ����ߴ�
		CurveNameSize=m_dcPlot.GetTextExtent(_curveName);
		//������ɫָʾ��
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


	//���������
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

	//ȡ�����귶Χ��׼����������̶�
	this->GetMaxDataRect(curveName);

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
		tempY2 = (m_bDrawReverseY)?nScaleHeightY*(nVerScaleLines-j):nScaleHeightY*j;  //���߻��Ʒ���ı�ʱ����ҲҪ��Ӧ�ĸı�

		tempY = m_ptZoomOffset.m_PosY - tempY2 + m_ptMinPos.m_PosY - m_ptMoveOffset.m_PosY ;  //����̶�ֵ����

		//������Сʱ������	
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
		//����ˮƽ����������
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
			for(k =0;k<nVerScaleLines;k++)  //����С�ı�ȳ�
			{
				for(i = m_rectPlot.left; i<m_rectPlot.left+2; i++)
				{
					m_dcPlot.SetPixel (i+1,tempY+(dGridHeight/nVerScaleLines)*k,frameColor) ;
				}

			}
		}
	}

	//���ƺ�����̶���,10�ȷ�,�߳�10����
	int nHorScaleLines = 10;  
	double nScaleWidthX = point.m_PosX/nHorScaleLines;
	double dGridWidth = m_rectPlot.Width()/nHorScaleLines;
	double tempX = 0.0;	
	double tempX2;

	for(j=0;j<=nHorScaleLines;j++)
	{
		tempX2 = (m_bDrawReverseX)?nScaleWidthX*j:nScaleWidthX*(nHorScaleLines-j);  //���߻��Ʒ���ı�ʱ����ҲҪ��Ӧ�ĸı�

		tempX = m_ptZoomOffset.m_PosX - tempX2 + m_ptMinPos.m_PosX - m_ptMoveOffset.m_PosX ;  //����̶�ֵ����

		//������Сʱ������
		if(m_iControlMode == CURVE_ZOOM_OUT)
		{

			tempX -= (point.m_PosX-(m_ptMaxPosConst.m_PosX-m_ptMinPosConst.m_PosX))/2;
		}

		str.Format (L"%.*lf",_iPreciseX, tempX) ;
		//textSize = m_dcGrid.GetTextExtent(str); 
		textSize = m_dcPlot.GetTextExtent(str); 
		//���ƴ�ֱ����������
		tempX = m_rectPlot.left + (long)(dGridWidth*j);	
		for (i=m_rectPlot.top+2; i<m_rectPlot.bottom && (j!=nHorScaleLines) && (j!=0); i+=2)	 
		{
			m_dcPlot.SetPixel(tempX,i,girdColor) ;
		}
		//���Ʊ��
		if(!bNoCurveSelect)
		{
			m_dcPlot.TextOut (tempX+textSize.cx/2,m_rectPlot.bottom+5, str);	
		}

		if(j<nHorScaleLines)
		{
			for(k =0;k<nHorScaleLines;k++)  //����С�ı�ȳ�
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


void CChart::DrawLabel()  //���Ʊ�ע
{
	DrawLabel(m_dcPlot,m_rectPlot);
}
void CChart::DrawLabel(CDC &m_dcPlot,CRect m_rectPlot) //���Ʊ�ע
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
			std::map<int,CFloatPoint>::iterator iterMap = m_MapLabelPoint.begin(); //��ű�ע��Ͷ�Ӧ������
			while(iterMap!=m_MapLabelPoint.end() )
			{
				//��ȡ��ǩ��λ��
				CPoint _point;// = *iter;
				//��ȡ��ǩ������
				CFloatPoint _fPoint = iterMap->second;
				_point = _curveObject->CFloat2Point(_fPoint,this->m_rectPlot);
				_point.y = (int)_fPoint.m_PosY;
				CString szxData;
				szxData.Format(L"%.2f",_fPoint.m_PosX);
				CSize szDatat = m_dcPlot.GetTextExtent(szxData);
				LOGFONT lf; //��������ṹ
				lf.lfWeight=0; //�������=10
				lf.lfHeight=0;	 //����߶�(��ת���������)=56
				lf.lfWidth=0;   //������(��ת�������߶�)=20
				lf.lfUnderline=FALSE; //���»���
				lf.lfStrikeOut=FALSE; //��ɾ����
				lf.lfItalic=FALSE; //��б��
				lf.lfEscapement=900; //������ʾ�Ƕ�=90��
				lf.lfCharSet=DEFAULT_CHARSET; //ʹ��ȱʡ�ַ���
				//wcscpy(lf.lfFaceName,L"@system"); //������=@system
				CFont myLogFont; //�����������
				myLogFont.CreateFontIndirect(&lf); //�����߼�����
				CFont *pOldFont=m_dcPlot.SelectObject(&myLogFont);//ѡ���豸������
				m_dcPlot.SetTextColor(RGB(0,0,0));
				//��ͼ
				m_dcPlot.MoveTo(_point);
				m_dcPlot.LineTo(CPoint(_point.x,m_rectPlot.bottom));
				if((_point.y - szDatat.cx) < m_rectPlot.top)
				{
					// m_dcPlot.LineTo(CPoint(_point.x,m_rectPlot.top));
					//��������
					m_dcPlot.TextOut(_point.x,_point.y+szDatat.cx,szxData);
				}
				else
				{
					//��������
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
	m_ptZoomOffset.InitPoint(-10000000.0,-10000000.0);  //�˴�����������ʼ��,
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
	//���������
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

	//ȡ�����귶Χ��׼����������̶�
	this->GetMaxDataRect(curveName);

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
		tempY2 = (m_bDrawReverseY)?nScaleHeightY*(nVerScaleLines-j):nScaleHeightY*j;  //���߻��Ʒ���ı�ʱ����ҲҪ��Ӧ�ĸı�

		tempY = m_ptZoomOffset.m_PosY - tempY2 + m_ptMinPos.m_PosY - m_ptMoveOffset.m_PosY ;  //����̶�ֵ����

		//������Сʱ������	
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
		//����ˮƽ����������
		tempY = m_rectPlot.top + (long)(dGridHeight*j);	
		if(!bNoCurveSelect)
		{
			m_dcPlot.SetTextAlign (TA_RIGHT|TA_TOP) ;
			m_dcPlot.TextOut (m_rectPlot.left-textSize.cy/4, tempY-textSize.cy/2, str) ;
		}

		if(j<nVerScaleLines)
		{
			for(k =0;k<nVerScaleLines;k++)  //����С�ı�ȳ�
			{
				for(i = m_rectPlot.left; i<m_rectPlot.left+2; i++)
				{
					m_dcPlot.SetPixel (i+1,tempY+(dGridHeight/nVerScaleLines)*k,frameColor) ;
				}

			}
		}
	}

	//���ƺ�����̶���,10�ȷ�,�߳�10����
	int nHorScaleLines = 10;  
	double nScaleWidthX = point.m_PosX/nHorScaleLines;
	double dGridWidth = m_rectPlot.Width()/nHorScaleLines;
	double tempX = 0.0;	
	double tempX2;

	for(j=0;j<=nHorScaleLines;j++)
	{
		tempX2 = (m_bDrawReverseX)?nScaleWidthX*j:nScaleWidthX*(nHorScaleLines-j);  //���߻��Ʒ���ı�ʱ����ҲҪ��Ӧ�ĸı�

		tempX = m_ptZoomOffset.m_PosX - tempX2 + m_ptMinPos.m_PosX - m_ptMoveOffset.m_PosX ;  //����̶�ֵ����

		//������Сʱ������	
		if(m_iControlMode == CURVE_ZOOM_OUT)
		{

			tempX -= (point.m_PosX-(m_ptMaxPosConst.m_PosX-m_ptMinPosConst.m_PosX))/2;
		}


		str.Format (L"%.*lf",_iPreciseX, tempX) ;
		textSize = m_dcPlot.GetTextExtent(str); 
		//���ƴ�ֱ����������
		tempX = m_rectPlot.left + (long)(dGridWidth*j);	
		//���Ʊ��
		if(!bNoCurveSelect)
		{
			m_dcPlot.TextOut (tempX+textSize.cx/2,m_rectPlot.bottom+5, str);	
		}

		if(j<nHorScaleLines)
		{
			for(k =0;k<nHorScaleLines;k++)  //����С�ı�ȳ�
			{
				for(i = m_rectPlot.bottom-2; i<m_rectPlot.bottom; i++)
				{
					m_dcPlot.SetPixel(tempX+(dGridWidth/nHorScaleLines)*k,i,frameColor) ;
				}
			}
		}
	}

	//��ʾ�������굥λ
	m_dcPlot.SelectObject (oldPen) ;

	CString XUnit;
	CString YUnit = L"������cm-1��";
	CSize szYUnit = m_dcPlot.GetTextExtent(YUnit);
	CSize sz = m_dcPlot.GetTextExtent(L"��");
	if(m_ShowTransAbs)
	{
		//͸����
		XUnit = L"%͸����";
		CSize szXUnit = m_dcPlot.GetTextExtent(XUnit);
		LOGFONT lf; //��������ṹ
		lf.lfWeight=0; //�������=10
		lf.lfHeight=0;	 //����߶�(��ת���������)=56
		lf.lfWidth=0;   //������(��ת�������߶�)=20
		lf.lfUnderline=FALSE; //���»���
		lf.lfStrikeOut=FALSE; //��ɾ����
		lf.lfItalic=FALSE; //��б��
		lf.lfEscapement=900; //������ʾ�Ƕ�=90��
		lf.lfCharSet=DEFAULT_CHARSET; //ʹ��ȱʡ�ַ���
		//wcscpy(lf.lfFaceName,L"@system"); //������=@system
		CFont myLogFont; //�����������
		myLogFont.CreateFontIndirect(&lf); //�����߼�����
		CFont *pOldFont=m_dcPlot.SelectObject(&myLogFont);//ѡ���豸������

		m_dcPlot.TextOut(2,m_rectClient.Height()/2-szXUnit.cx/2,XUnit);
		myLogFont.DeleteObject();
		lf.lfEscapement = 0;
		myLogFont.CreateFontIndirect(&lf); //�����߼�����
		m_dcPlot.SelectObject(&myLogFont);//ѡ���豸������

		m_dcPlot.TextOut(m_rectClient.Width()/2+szYUnit.cx/2,m_rectClient.Height()-20,YUnit);

		m_dcPlot.SelectObject(pOldFont); //��myFont���豸�����з���
		myLogFont.DeleteObject();

	}
	else
	{
		//�����
		XUnit = L"�����";
		CSize szXUnit = m_dcPlot.GetTextExtent(XUnit);
		LOGFONT lf; //��������ṹ
		lf.lfWeight=0; //�������=10
		lf.lfHeight=0;	 //����߶�(��ת���������)=56
		lf.lfWidth=0;   //������(��ת�������߶�)=20
		lf.lfUnderline=FALSE; //���»���
		lf.lfStrikeOut=FALSE; //��ɾ����
		lf.lfItalic=FALSE; //��б��
		lf.lfEscapement=900; //������ʾ�Ƕ�=90��
		lf.lfCharSet=DEFAULT_CHARSET; //ʹ��ȱʡ�ַ���
		//wcscpy(lf.lfFaceName,L"@system"); //������=@system
		CFont myLogFont; //�����������
		myLogFont.CreateFontIndirect(&lf); //�����߼�����
		CFont *pOldFont=m_dcPlot.SelectObject(&myLogFont);//ѡ���豸������

		m_dcPlot.TextOut(2,m_rectClient.Height()/2-szXUnit.cx/2,XUnit);

		myLogFont.DeleteObject();
		lf.lfEscapement = 0;
		myLogFont.CreateFontIndirect(&lf); //�����߼�����
		m_dcPlot.SelectObject(&myLogFont);//ѡ���豸������

		m_dcPlot.TextOut(m_rectClient.Width()/2+szYUnit.cx/2,m_rectClient.Height()-20,YUnit);

		m_dcPlot.SelectObject(pOldFont); //��myFont���豸�����з���
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
	//������
	DrawBackGnd();
	//������
	DrawGrid();
	//����ע
	DrawLabel();
	//������
	int colorIndex=0;
	if(m_sCurrentControlCurve.IsEmpty() || !m_bHideUnselectCurve) //�����Ƿ���ʾδ��ѡ�е�����
	{
		POSITION pos = m_CurveMap.GetStartPosition();
		while(pos)
		{
			m_CurveMap.GetNextAssoc(pos,_curveName, (CObject*&)curveObject);

			set<CString>::iterator iter =find(m_SetSelCurveName.begin(),m_SetSelCurveName.end(),_curveName);

			if(curveObject&&iter == m_SetSelCurveName.end())
			{
				curveObject->SetCurveColor(m_ArrayCurveColor[colorIndex]);
				curveObject->DrawCurve(&m_dcPlot,m_rectPlot);//��������
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
				curveObject->DrawCurve(&m_dcPlot,m_rectPlot);//��������
			}
			++iter;
		}
	}
	else
	{
		CCurveObj* curveObject = NULL;
		if(m_CurveMap.Lookup(m_sCurrentControlCurve,(CObject*&)curveObject))
		{
			curveObject->DrawCurve(&m_dcPlot,m_rectPlot);//��������
		}
	}


	DrawUnPlotArea();
	//��ͼ��
	if(m_bDrawLegend)
	{
		DrawLegend();
	}

	InvalidateRect(m_rectClient);
}
void CChart::DrawRealtimeCurve()
{
	int shiftPixels = CCurveObj::GetShiftPixels();  //ȡ��ͼ���ƶ��ٶ�
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
			curveObject->DrawRealtimeCurve(&m_dcPlot,m_rectPlot);//��������
		}
	}

	InvalidateRect(m_rectPlot);
}
//��ָ��index�����߶�������������
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
void CChart::AppendPoint(CString curveName,double Posy)   //����ʱ���׼���ߵ�
{
	CCurveObj* curveObject = NULL;
	if(m_CurveMap.Lookup(curveName,(CObject*&)curveObject))
	{
		if(!curveObject->IsTimeBaseCurve())  //���ǰ��û�ж���Ϊʱ���׼����,���޷��������ݵ�
			return ;

		CFloatPoint point;
		point.InitPoint(curveObject->GetPointCounter() * curveObject->GetTimeBaseFactor(),Posy);
		curveObject->AppendNewPoint(point);

	}
} 
//�����Ƿ��������߹���һ������ϵ,Ĭ����ÿһ���������Լ�������ϵ�л�ͼ��trueΪ�������߹���һ������ϵ
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
		this->SetWindowPos(NULL,0,0,cx,cy,SWP_SHOWWINDOW);  //���ô˺���������WM_SIZE��Ϣ
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
				str+= L"  ���߳�ʼ����Ϊʱ�����ߣ��������ô˲���.";
				::MessageBox(NULL,str,L"����",MB_OK|MB_ICONERROR);
				return;
			}
			curveObject->SetTimeBaseFactor(timeFactor);
		}
	}
}
//���ö�̬ʱ������Y������������,�����ڶ�̬��ͼ֮ǰ�趨�ô˲���
void CChart::SetRealTimeBaseCurveDataRange(double lower,double upper)  
{
	CFloatPoint point(lower,upper);
	CCurveObj::SetRealTimeBaseCurveDataRange(point);
}
//�����µ����߶���
//##ModelId=48A14588031D
CCurveObj& CChart::CreateNewCurve(CString curveName,bool timeBaseFlag /*=false*/ )
{
	CCurveObj* curveObject = NULL;
	//����������

	if(m_CurveMap.IsEmpty())
	{
		CDC* pDc=GetDC();
		CSize CurveNameSize;
		CurveNameSize = pDc->GetTextExtent(curveName);
		m_CurveNameMaxSize=CurveNameSize;
		ReleaseDC(pDc);
	}
	//����������

	if(m_CurveMap.Lookup(curveName,(CObject*&)curveObject)==TRUE)
	{
		curveObject->DestroyCurve();
		curveObject = NULL;
		m_CurveMap.RemoveKey(curveName);
	}

	//����������Ϊѡ������
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
	//����������
	if(m_CurveMap.IsEmpty())
	{
		CDC* pDc=GetDC();
		CSize CurveNameSize;
		CurveNameSize = pDc->GetTextExtent(curveName);
		m_CurveNameMaxSize=CurveNameSize;
		ReleaseDC(pDc);
	}
	//����������
	if(m_CurveMap.Lookup(curveName,(CObject*&)curveObject)==FALSE)
	{
		curveObject = new CCurveObj(m_rectPlot);
		if(curveObject)
		{
			curveObject->SetTimeBaseFlag(timeBaseFlag);
			curveObject->SetCurveColor(curveColor,lineWidth);
			m_CurveMap.SetAt(curveName,curveObject);
		}
		//������������߳ߴ磬���ڻ���ͼ��
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
//ɾ��ѡ������
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
	if(curveName.IsEmpty())  //ɾ����������
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
	//m_iControlMode = CURVE_MOVE;  //ѡ�����ߺ��Ĭ�ϲ������ƶ�����
	this->InvalidateChart();
}
bool CChart::SetCurveSelected(CString curveName/* = _T("\0")*/)
{
	//�����ѡ�������
	m_SetSelCurveName.clear();
	//���ѡ������
	CCurveObj* curveObject = NULL;
	if(curveName.IsEmpty())  //ɾ����������
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
	//�ػ�
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
		//�����޸���ͼ����С
		if(m_bDrawLegend&&m_CurveNameMaxSize.cx>0)
		{
			m_rectPlot = m_rectClient;
			m_ptMargin = CPoint(xMargin,yMargin);
			m_rectPlot.DeflateRect(m_ptMargin.x*2+30+m_CurveNameMaxSize.cx/2,m_ptMargin.y);  
			m_rectPlot.OffsetRect(m_ptMargin.x-30-m_CurveNameMaxSize.cx/2,-m_ptMargin.y/2);//�����Ļ�ͼ����
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
	if(curveName.IsEmpty())  //�������߶������ƶ�����
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
//��������
//##ModelId=48A145890040
void CChart::ZoomCurve(CString curveName /*= _T("\0")*/,int zoomType /*= ZOOM_RESET*/,double percent /*= 5*/)
{
	if(m_sCurrentControlCurve != curveName)
		m_sCurrentControlCurve = curveName;
	if(curveName.IsEmpty())  //�������߶��������Ų���
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
					m_BeginPoint = curveObject->Position2Data(m_Line.m_Begin,m_Line.m_Begin); //����ʼ������ͼ����
					//_PointEnd = curveObject->Position2Data(point,point);
				}

				m_bLeftButtonDown = true;

				//ѡ������
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

							//�õ���������߾����Լ���������
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
								//��ȡ����������
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
					POSITION pos = m_CurveMap.GetStartPosition();
					CString _curveName;
					CCurveObj* curveObject = NULL;
					CFloatPoint _PointEnd;
					m_CurveMap.GetNextAssoc(pos,_curveName, (CObject*&)curveObject);

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
}
//##ModelId=48A14589005E
BOOL CChart::GetMaxDataRect(CString curveName/*=_T("\0")*/)  //�������������������������ֵ
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
				//����ǽ������ţ���ֱ�ӷ���
				curveObject->GetDrawCurvePar(m_ptMaxPos,m_ptMinPos,m_ptMoveOffset,m_ptZoomOffset);
				if(m_iControlMode == CURVE_ZOOM_IN ||m_iControlMode == CURVE_ZOOM_IN_V||
					m_iControlMode == CURVE_ZOOM_IN_H||m_iControlMode == CURVE_ZOOM_OUT||
					m_iControlMode == CURVE_ZOOM_OUT_V||m_iControlMode == CURVE_ZOOM_OUT_H);		
				else
					//���������������
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

	m_ptMaxPos.InitPoint(-10000000000.0,-10000000000.0);//��ǰ����
	m_ptMinPos.InitPoint(10000000000.0,10000000000.0);//��ǰ��С��

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

			//��������ͼ����϶
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
	this->SetFocus();  //��ý���
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
		//ѡ�����߲˵���
		CMenu submenuSelect;
		submenuSelect.CreatePopupMenu();
		menu.AppendMenu(MF_POPUP,(UINT_PTR)submenuSelect.m_hMenu,L"ѡ��ͼ��");

		submenuSelect.AppendMenu(MF_STRING,10008,L"ѡ����ͼ��");
		submenuSelect.AppendMenu(MF_STRING,10001,L"ѡ������ͼ��");



		menu.AppendMenu(MF_STRING,10006,L"�ƶ�ͼ��");
		menu.AppendMenu(MF_STRING,10002,L"�����ƷŴ�");
		menu.AppendMenu(MF_STRING,10003,L"��������С");

		menu.AppendMenu(MF_STRING,10007,L"��λ");


		for(int n =0;n<menu.GetMenuItemCount();n++)  //Ŀǰֻ���ǵ�����˵��Ŀ��ƣ���Ҫ���Ƹ���㣬���Կ���ʹ�õݹ鷽ʽ
		{   
			CMenu* pSubMenu = menu.GetSubMenu(n); 	
			cmdUI.m_nIndexMax = n+1;  //�˴������1���������ֶ��Դ���
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
	case 9998://�ڶ�������ϵ�ڹ�������ϵ���л�
		{	
			m_bAllCurveSameBase = !m_bAllCurveSameBase;
			this->SetAllCurveDrawMode(m_bAllCurveSameBase);
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
	case 10006:  //�ƶ����ߣ���ͨ����꣬������������ֿ������ߵ��ƶ�
		{ 
			m_iControlMode = CURVE_MOVE;
			::SendMessage(this->GetParent()->m_hWnd,WM_CHANGECHARTMODE,m_iControlMode,0);
			break;
		}
	case 10002: //���߷Ŵ�
		{
			m_iControlMode = CURVE_ZOOM_IN;
			::SendMessage(this->GetParent()->m_hWnd,WM_CHANGECHARTMODE,m_iControlMode,0);
			break;
		}
	case 10003: //������С
		{
			m_iControlMode = CURVE_ZOOM_OUT;
			::SendMessage(this->GetParent()->m_hWnd,WM_CHANGECHARTMODE,m_iControlMode,0);
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
//##ModelId=48A1458802CF
void CChart::OnUpdateMenuState(CCmdUI* pCmdUI)//���²˵��ϵ�״̬��ʾ
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
	case 10004:  //ˮƽ����ģʽ
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
	case 10009:  //x���ͼ����
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
	case 10010:  //Y��ͼ����
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
void CChart::DrawSelectRangeText(CDC* pDc)
{
	//����ʼλ��
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
		tempStrX.Format(L"��ʼλ��:%.2lf",_PointBegin.m_PosX);

		tempStrY.Format(L"����λ��:%.2lf",_PointEnd.m_PosX);


		CSize textSizeX = pDc->GetTextExtent(tempStrX); 
		CSize textSizeY = pDc->GetTextExtent(tempStrY); 



		pDc->SetTextColor(RGB(0,0,0));
		COLORREF bkColor = pDc->SetBkColor(m_crPlotAreaBackColor);
		int oldmode = pDc->SetBkMode(TRANSPARENT);
		int Maxtextsize = textSizeX.cx>textSizeY.cx?textSizeX.cx:textSizeY.cx;

		if( (point.x+ Maxtextsize) < m_rectPlot.right)
		{
			pDc->SetTextAlign (TA_LEFT|TA_TOP);//��ʾ���ַ����������

			if(point.y+5+textSizeX.cy+2+textSizeX.cy> m_rectPlot.bottom)//������ͼ��Χ
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
			pDc->SetTextAlign (TA_RIGHT|TA_TOP);//��ʾ���ַ����������
			if(point.y+5+textSizeX.cy+2+textSizeX.cy> m_rectPlot.bottom)//������ͼ��Χ
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
		tempStr.Format(L"��ʼλ��:%.2lf",_PointBegin.m_PosX);

		CSize textSizeX = pDc->GetTextExtent(tempStr); 

		pDc->SetTextColor(textColor);
		COLORREF bkColor = pDc->SetBkColor(m_crPlotAreaBackColor);
		int oldmode = pDc->SetBkMode(TRANSPARENT);

		if( point.x+ textSizeX.cx < m_rectPlot.right)
		{
			pDc->SetTextAlign (TA_LEFT|TA_TOP);//��ʾ���ַ����������
			if(point.y+5+textSizeX.cy+2+textSizeX.cy> m_rectPlot.bottom)//������ͼ��Χ
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
			pDc->SetTextAlign (TA_RIGHT|TA_TOP);//��ʾ���ַ����������
			pDc->TextOut(point.x,point.y+5,tempStr);
		}
		pDc->SetBkMode(oldmode);
		pDc->SetBkColor(bkColor);
	}
	//������λ��
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
	//����ʮ����
	pDc->MoveTo(m_ptCrossCenter.x,m_rectPlot.top);
	pDc->LineTo(m_ptCrossCenter.x,m_rectPlot.bottom+1);
	pDc->MoveTo(m_rectPlot.left,m_ptCrossCenter.y);
	pDc->LineTo(m_rectPlot.right+1,m_ptCrossCenter.y);

	if(m_bShowDataFlowMouse)  //������ʾ�������
	{
		//pDc->SetTextAlign (TA_RIGHT|TA_TOP);//��ʾ���ַ����������
		pDc->SetTextAlign (TA_LEFT|TA_TOP);//��ʾ���ַ����������
	}
	else
	{
		pDc->SetTextAlign (TA_LEFT|TA_TOP);//��ʾ���ַ����������
	}

	if(m_pCurSelCurveObj)//��ʾ�������ߵ�����ֵ
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
	else      //��ʾ�������ߵ�����ֵ
	{
		bool leftsideshow=false; //�Ƿ������ʾ

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
			//��ȡ������Ϣ
			CSize strGroupsInfoSize;
			CString strGroupsInfo;

			strGroupsInfo.Format(L"���ܵĻ��ţ�");
			int ShortestLen=strGroupsInfo.GetLength();

			if(abs(_Point.m_PosX-2962)<=10||abs(_Point.m_PosX-2872)<=10||abs(_Point.m_PosX-1375)<=5)
			{
				strGroupsInfo+=L"��CH3(ǿ),";
			}

			if(abs(_Point.m_PosX-1450)<=20)
			{
				strGroupsInfo+=L"��CH3(��),";
			}

			if(abs(_Point.m_PosX-2926)<=10||abs(_Point.m_PosX-2853)<=10)
			{
				strGroupsInfo+=L"��CH2��(ǿ),";
			}

			if(abs(_Point.m_PosX-1465)<=10)
			{
				strGroupsInfo+=L"��CH2��(ǿ),";
			}

			if(abs(_Point.m_PosX-2890)<=10||abs(_Point.m_PosX-1340)<=1)
			{
				strGroupsInfo+=L"��CH��(��),";
			}

			if(abs(_Point.m_PosX-1390)<=5||abs(_Point.m_PosX-1225)<=25)
			{
				strGroupsInfo+=L"��C(CH3)3��(��),";
			}

			if(abs(_Point.m_PosX-1367.5)<=2.5)
			{
				strGroupsInfo+=L"��C(CH3)3��(ǿ),";
			}

			if(abs(_Point.m_PosX-1367.5)<=2.5)
			{
				strGroupsInfo+=L"��C(CH3)3��(ǿ),";
			}

			if(abs(_Point.m_PosX-835.5)<=172.5)
			{
				strGroupsInfo+=L"��CH�TCH��(ǿ),";
			}

			if(abs(_Point.m_PosX-3305)<=5)
			{
				strGroupsInfo+=L"R��R��CH(��),";
			}

			if(abs(_Point.m_PosX-2245)<=25)
			{
				strGroupsInfo+=L"R��R��C��R(��),";
			}


			if(abs(_Point.m_PosX-2245)<=25)
			{
				strGroupsInfo+=L"R��R��C��R(��),";
			}

			if(abs(_Point.m_PosX-1735)<=85||abs(_Point.m_PosX-1225)<=75)
			{
				strGroupsInfo+=L"����(ǿ),";
			}

			if(abs(_Point.m_PosX-1825)<=25||abs(_Point.m_PosX-1760)<=20||abs(_Point.m_PosX-1110)<=60)
			{
				strGroupsInfo+=L"������(ǿ),";
			}

			if(abs(_Point.m_PosX-1845)<=25||abs(_Point.m_PosX-1775)<=25||abs(_Point.m_PosX-1250)<=50)
			{
				strGroupsInfo+=L"������(ǿ),";
			}

			if(abs(_Point.m_PosX-1730)<=10||abs(_Point.m_PosX-1702.5)<=12.5||abs(_Point.m_PosX-1695)<=45)
			{
				strGroupsInfo+=L"��COOH(ǿ),";
			}

			if(abs(_Point.m_PosX-2917.5)<=417.5||abs(_Point.m_PosX-1235.5)<=30.5)
			{
				strGroupsInfo+=L"��COOH(��),";
			}

			if(abs(_Point.m_PosX-1430)<=20||abs(_Point.m_PosX-930)<=30||abs(_Point.m_PosX-1712.5)<=12.5)
			{
				strGroupsInfo+=L"��COOH(��),";
			}

			if(abs(_Point.m_PosX-1635)<=95)
			{
				strGroupsInfo+=L"ͪ��(ǿ),";
			}

			if(abs(_Point.m_PosX-1610)<=45)
			{
				strGroupsInfo+=L"��CHO(ǿ),";
			}

			if(abs(_Point.m_PosX-1102.5)<=97.5)
			{
				strGroupsInfo+=L"����(ǿ),";
			}

			if(abs(_Point.m_PosX-3415)<=290||abs(_Point.m_PosX-1250)<=85)
			{
				strGroupsInfo+=L"����(ǿ),";
			}

			if(abs(_Point.m_PosX-1352.5)<=37.5)
			{
				strGroupsInfo+=L"����(��),";
			}

			if(abs(_Point.m_PosX-3450)<=250||abs(_Point.m_PosX-1125)<=125||abs(_Point.m_PosX-700)<=50)
			{
				strGroupsInfo+=L"����(ǿ),";
			}

			if(abs(_Point.m_PosX-2237.5)<=22.5)
			{
				strGroupsInfo+=L"��C��N(ǿ),";
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
//�����Ƕ�������ϵ���ǹ�������ϵ,ÿ����һ��״̬��������ת��Ĭ���Ƕ�������ϵ
void CChart::ChangeToIndividualCoordinateMode(bool individualMode/* = true*/)
{
	m_bAllCurveSameBase = !individualMode;
	this->SetAllCurveDrawMode(m_bAllCurveSameBase);
}

void CChart::ChangeToGetDataMode()  //�ö����л���ȡ����ģʽ������ģʽ��������
{
	WPARAM controlCode = MAKELONG(9999,0);
	this->PostMessage(WM_COMMAND,controlCode,0);
}
void CChart::ResetCurve(bool allCurve/* = false*/)  //��λ���أ����ѡ����һ�����ߣ���λ�������ߣ����򣬽���λ���е�����
{
	WPARAM controlCode = 0x0;
	controlCode = MAKELONG(10007,0);
	this->PostMessage(WM_COMMAND,controlCode,0);
}
void CChart::ChangeToSelectMode(bool allCurve/* = false*/)  //�л���ѡ������ģʽ
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
void CChart::ChangeToMoveMode()  //�л����ƶ�����ģʽ
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



void CChart::PrintChart(CDC* pDC, CPrintInfo* pInfo)//�ṩ�Ĵ�ӡ�ӿ�
{
	if( pDC->GetSafeHdc()==NULL )
		return ;
	CString _curveName;
	CCurveObj* curveObject = NULL;


	CDC MemDC; //���ȶ���һ����ʾ�豸����
	CBitmap MemBitmap;//����һ��λͼ����

	//���������Ļ��ʾ���ݵ��ڴ���ʾ�豸
	MemDC.CreateCompatibleDC(NULL);
	//��ʱ�����ܻ�ͼ����Ϊû�еط��� ^_^
	//���潨��һ������Ļ��ʾ���ݵ�λͼ������λͼ�Ĵ�С������ô��ڵĴ�С
	MemBitmap.CreateCompatibleBitmap(pDC,m_rectClient.Width(),m_rectClient.Height());

	//��λͼѡ�뵽�ڴ���ʾ�豸��
	//ֻ��ѡ����λͼ���ڴ���ʾ�豸���еط���ͼ������ָ����λͼ��
	CBitmap *pOldBit=MemDC.SelectObject(&MemBitmap);

	//���ñ���ɫ��λͼ����ɾ�����ɫ��Ϊ����
	MemDC.FillSolidRect(0,0,m_rectClient.Width(),m_rectClient.Height(),RGB(255,255,255));

	//������
	DrawBackGnd(MemDC,m_rectPlot);
	//������
	DrawGrid(MemDC,m_rectPlot,m_rectClient);
	//������
	int colorIndex=0;
	POSITION pos = m_CurveMap.GetStartPosition();
	while(pos)
	{
		m_CurveMap.GetNextAssoc(pos,_curveName, (CObject*&)curveObject);

		set<CString>::iterator iter =find(m_SetSelCurveName.begin(),m_SetSelCurveName.end(),_curveName);

		if(curveObject&&iter == m_SetSelCurveName.end())
		{
			curveObject->SetCurveColor(m_ArrayCurveColor[colorIndex]);
			curveObject->DrawCurve(&MemDC,m_rectPlot);//��������
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
			curveObject->DrawCurve(&MemDC,m_rectPlot);//��������
		}
		++iter;
	}
	//����ע
	DrawLabel(MemDC,m_rectPlot);
	//������ͼ��
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
						data.Format(L"%lf\t%lf\r\n",point.m_PosX,point.m_PosY);//csv ��ʽ��������
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
	//set<CString>::iterator iter = m_SetSelCurveName.begin();           //ѡ��������������
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
				//_newCurveName += L"(�ƶ�ƽ��)";
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
		throw invalid_argument("���߲�����");
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
		throw invalid_argument("���߲�����");
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
		//��ȡx��Χ
		_curveObject->GetXRange(minX,maxX);
		//������ֵ
		_curveObject->GetYRange(minY,maxY);

		//���������rmsֵ
		Rms = _curveObject->CalculateYRMS();

	}
	CString Info;
	Info.Format(L"��Χ��%f  %f\n��-��ֵ��%f\nRMSֵ��%f",minX,maxX,maxY,Rms);

	//CSpecNoiseDlg SpecNoiseDlg;
	//SpecNoiseDlg.m_NoiseInfo = Info;
	//SpecNoiseDlg.DoModal();

	MessageBox(Info,L"��������",MB_ICONINFORMATION);

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
		//��ȡx��Χ
		_curveObject->GetXRange(minX,maxX);
		//������ֵ
		//_curveObject->GetYRange(minY,maxY);

		//���������rmsֵ
		Mean = _curveObject->CalculateYMean();

	}
	CString Info;
	Info.Format(L"��Χ��%f  %f\nƽ��ֵ��%f",minX,maxX,Mean);

	//CSpecNoiseDlg SpecNoiseDlg;
	//SpecNoiseDlg.m_NoiseInfo = Info;
	//SpecNoiseDlg.DoModal();

	MessageBox(Info,L"ƽ��ֵ����",MB_ICONINFORMATION);

	return 0;
}

void CChart::ShowSelectedPeakArea() //��������
{
	CCurveObj *_curveObject = NULL;
	CString _curveName;
	POSITION pos = m_CurveMap.GetStartPosition();
	m_CurveMap.GetNextAssoc(pos,_curveName,(CObject *&)_curveObject);

	if(_curveObject)
	{
		//���㿪ʼ����λ��
		int IndexBegin = _curveObject->GetDataIndex(m_BeginPoint);
		int IndexEnd = _curveObject->GetDataIndex(m_EndPoint);

		if(IndexBegin >= 0 && IndexEnd >= 0)
		{
			double UnCorrectArea = _curveObject->CalculateYSum(IndexBegin,IndexEnd);

			double CorrectArea = UnCorrectArea - (abs(IndexEnd - IndexBegin)+1)*(m_BeginPoint.m_PosY + m_EndPoint.m_PosY)/2;

			CString Info;
			Info.Format(L"�巶Χ��%f  %f  \nδУ��������� %f\nУ��������: %f",m_BeginPoint.m_PosX,m_EndPoint.m_PosX,UnCorrectArea,CorrectArea);

			MessageBox(Info,L"�����",MB_ICONINFORMATION);
			return;
		}
	} 


}

BOOL CChart::CurveExist(const CString &_curveName)   //�жϸ����Ƶ������Ƿ����
{
	CCurveObj *_curveObj = NULL;
	return m_CurveMap.Lookup(_curveName,(CObject*&)_curveObj) ;
}

BOOL CChart::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: �ڴ����ר�ô����/����û���
	if (cs.lpszClass == NULL)
		cs.lpszClass = AfxRegisterWndClass(CS_DBLCLKS);
	return CWnd::PreCreateWindow(cs);
}


BOOL CChart::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
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