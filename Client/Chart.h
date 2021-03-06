#if !defined(AFX_CHART_H__5955D1B9_F38E_4AAE_8BDC_B41945EAB191__INCLUDED_)
#define AFX_CHART_H__5955D1B9_F38E_4AAE_8BDC_B41945EAB191__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Chart.h : header file
//
/*
绘图控件(MFC)	ver 1.0

编译器: VC6.0,其它未测试过;
适用于MFC程序,如果要适应其他程序，请自己修改。修改好了不嫌麻烦发送一份给我,同时请保留此信息。
代码可以任意使用，任意修改，但本人不对使用本控件造成的损失负任何责任。
欢迎就任何bug及改进意见邮件给我，我们一起完善此控件。

copyright(c) 2008 David  m__point@163.com ; m__point@126.com
*/
#include "CurveObj.h"
#include <map>
#include <vector>
#include <algorithm>


//#include "SpecNoiseDlg.h"
#include <set>
#include "resource.h"

//##ModelId=48A14587030D
enum CURVE_CONTROL_ID
{
	CURVE_NONE=0,
	CURVE_GET_DATA,
	CURVE_SELECT,
	CURVE_ZOOM_IN ,
	CURVE_ZOOM_IN_V,
	CURVE_ZOOM_IN_H,
	CURVE_ZOOM_OUT,
	CURVE_ZOOM_OUT_V,
	CURVE_ZOOM_OUT_H,
	CURVE_MOVE,
	CURVE_MOVE_UP,
	CURVE_MOVE_DOWN,
	CURVE_MOVE_LEFT,
	CURVE_MOVE_RIGHT,
	CURVE_RESET,
	CURVE_FEATURE_SELECT,
	CURVE_FEATURE_SAVE,
	CURVE_FEATURE_MATCH,
	CURVE_PEAKAREA_CAL,
	CURVE_LABEL,
};
enum SMOOTHMETHOD
{
	MOVE_MEAN,
	SACITZKY_GOLAY,	
};
/////////////////////////////////////////////////////////////////////////////

struct CLine
{
	CPoint m_Begin;  //起始位置
	CPoint m_End;    //结束位置
};


// CChart window
class CChart : public CWnd
{
public:
	CChart();
	virtual ~CChart();
public:
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CChart)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	virtual LRESULT DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL
// Implementation
protected:
	//{{AFX_MSG(CChart)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	afx_msg void OnUpdateMenuState(CCmdUI* pCmdUI);	
	//数据接口函数,调用顺序也如下
public:	
	BOOL Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID=NULL); //创建绘图对象
	CCurveObj& CreateNewCurve(CString curveName,bool timeBaseFlag = false); //创建一条新的曲线
	CCurveObj& CreateNewCurve(CString curveName,bool timeBaseFlag,COLORREF curveColor,int lineWidth); //创建一条新的曲线
	void AppendPoint(CString curveName,double Posx,double Posy); //加入二维坐标点
	void AppendPoint(CString curveName,double Posy);   //加入时间基准曲线点，时间基准曲线是二维曲线的一个特例。

    void AppendPoint( CString curveName,double Posx,double Posy,BOOL TransAbs);	

	void InvalidateChart();  //绘图，绘制出所有曲线
	void DrawRealtimeCurve();  //绘制实施曲线,每发生一次数据更新都需要调用一次此函数，以实现动态效果
	BOOL DestroyCurve(CString curveName = _T("\0")); //销毁制定命名的曲线，不指定曲线名将销毁所有曲线
public:
	void PrintChart(CDC* pDC, CPrintInfo* pInfo);//提供的打印接口
	void SetShowLegend(bool ShowLegend= true);
	void SaveData(CString filePath);//保存曲线数据为文本文档,有几条曲线就保存为几个文件
	void SavePlotAsBmp(CString filePath);//保存绘图区域为bmp图像
	void ShowMenum(bool showMenum = true){m_bShowMenum = showMenum;};  //现实右键探出菜单
	void ChangeToIndividualCoordinateMode(bool individualMode = true);	//设置是独立坐标系还是公用坐标系,每调用一次状态将发生翻转，默认是独立坐标系
	void ChangeToGetDataMode();  //让对象切换到取数据模式，其他模式将被屏蔽
	void ResetCurve(bool allCurve = false);  //复位区县，如果选中了一条曲线，则复位此条曲线，否则，将复位所有的曲线

	inline bool GetShowType() { return this->m_ShowTransAbs;};
	inline int GetCurveCount() { return this->m_CurveMap.GetCount();};


	void ChangeToSelectMode(bool allCurve = false);  //切换到选择曲线模式
	void ChangeToMoveMode();  //切换到移动曲线模式
	void ChangeToZoomInMode();  //切换到放大模式  貌似zoom out是缩小 
	void ChangeToZoomOutMode();   //切换到缩小模式
	void ChangeToVerticalMode();   //切换控制模式到垂直模式，主要指鼠标控制移动，缩放模式
	void ChageToHorizontalMode();  //切换控制模式到水平模式，主要指鼠标控制移动，缩放模式
	void ChangeToPeakAreaCalMode();   //切换到计算峰面积模式
	void ChangeToLableMode();       //切换到标注模式

	void ChageDrawDirectionX();
	void ChageDrawDirectionY();

	void HideUnselectCurve(bool hide = false);  //在选中一条曲线后是否现实其他的曲线，默认是显示
	void DataShowFollowCursor(bool follow = false); //是否读取数据时数据显示与鼠标跟随
	void SetBackgroundColor(COLORREF backColor);  //设置背景颜色
	void SetGridLineColor(COLORREF gridLienColor);//设置网格线颜色
	void SetFrameColor(COLORREF frameColor); //设置边框颜色
	void SetMargin(int xMargin=30,int yMargin = 30);  //设置绘图区域的地空白区域
	void SetCurveColor(CString curveName,COLORREF color,int lineWidth); 	//设置曲线的颜色及线宽
	void SelectCurve(CString curveName = _T("\0"));


	BOOL CurveExist(const CString &_curveName);   //判断该名称的曲线是否存在

	//选中曲线
	bool SetCurveSelected(CString curveName = _T("\0"));
	bool DeleteCurveSelected();
	bool AddSelectedCurve(CString curveName);
	inline int GetSelCurveCount() { return m_SetSelCurveName.size();};
	inline const std::set<CString>& GetSelectedCurveName() { return  m_SetSelCurveName;}; 

	std::vector<int> GetLabelIndex();  //获取选择的特征点索引
	int SetCurveTransAbs(bool _transAbs);

	
	int SmoothCurve(CCurveDataSetArray& tempArray,CString _curveName,int SmoothMethod,int N);

	CMatrix GetCurveYData(CString _curveName);
	CMatrix GetCurveXData(CString _curveName);

	int ReCalculateAllCurveMaxMin();
	int ShowSelectedNoise();
	int ShowSelectedMean();

	void ShowSelectedPeakArea();


	void SetTimeBaseCurveProperty(CString curveName=_T(""),double timeFactor=1);//设置时基曲线属性
	void SetRealTimeBaseCurveDataRange(double lower,double upper);  //设置动态时间曲线Y轴数据上下限

public:  //删除操作
	void RemoveAllLabel();



public:
    void OnSize(int cx,int cy);//绘图对象要自适应串口大小，需要父窗口的OnSize函数中调用此函数
private:
	void SetAllCurveDrawMode(bool onePlot=false);  //控制是否所有曲线共用一个坐标系,默认是每一条曲线在自己的坐标系中绘图。true为所有曲线共用一个坐标系
	BOOL InvalidateCtrl();
	bool HitTest(CPoint point); //点击测试
	void DrawSelectFocus(CDC* pDc,CPoint& point);
	void DrawZoomRect(CDC* pDc,CRect rect);
	void DrawGetDataCross(CDC* pDc);  //绘制取数据时候的十字线


	void DrawNearestPt2Cursor(CDC *pDc,CPoint _point);  //画出离鼠标最近的点
	void DrawSelectRange(CDC* pDc,CLine Line);
	void DrawSelectRangeText(CDC* pDc);

	

	void DrawLabel();  //绘制标注
	void DrawLegend();		   // 绘制图例
	void DrawBackGnd();        //充填背景
	void DrawUnPlotArea();
	void DrawGrid();  //绘制网格线

	void DrawLabel(CDC &m_dcPlot,CRect m_rectPlot);  //绘制标注
	void DrawLegend(CDC &m_dcPlot,CRect m_rectPlot);		   // 绘制图例
	void DrawBackGnd(CDC &m_dcPlot,CRect m_rectPlot);        //充填背景
	void DrawUnPlotArea(CDC &m_dcPlot,CRect m_rectPlot,CRect m_rectClient);
    void DrawGrid(CDC &m_dcPlot,CRect m_rectPlot,CRect m_rectClient);;  //绘制网格线


	void MoveCurve(CString curveName = _T("\0"),int moveType = CURVE_RESET,double percent = 5);	//按照指定模式移动一条曲线或所有曲线
	void ZoomCurve(CString curveName = _T("\0"),int zoomType = CURVE_RESET,double percent = 5);	//按照指定模式缩放一条曲线或所有曲线
	BOOL GetMaxDataRect(CString curveName=_T("\0"));  //获得所有曲线中最大的数据区域值
	void CalculateMaxPoint(CFloatPoint& pointDes,const CFloatPoint* PointSource);
    void CalculateMinPoint(CFloatPoint& pointDes,const CFloatPoint* PointSource);
	void IndividualCurveWheelContol(CCurveObj* curveObj,int controlMode,bool verticalControl,int zDelta,CPoint point);
	void IndividualCurveControl(CCurveObj* curveObj = NULL,int controlMode = CURVE_MOVE,CRect controlRect = CRect(0,0,0,0));
	void IndividualCurveMove(CString curveName,int moveType,double percent); //但前曲线移动控制
	void IndividualCurveZoom(CString curveName,int zoomType,double percent);//单条曲线缩放控制
	
	int  FloatDataPrecise(double f);  //计算浮点数的小数点位数
	void DrawDataFocus(CDC *pDc,CPoint point,COLORREF color);
private:
	bool m_bDrawLegend;    //显示图例

	bool m_ShowTransAbs;  //true 表示透射率，false表示吸光度

	bool m_bDrawReverseX;
	bool m_bDrawReverseY;
	bool m_bShowMenum;
	bool m_bAllCurveSameBase;
	bool m_bHideUnselectCurve;  //隐藏未选中的曲线
	bool m_bShowDataFlowMouse;  //数据显示是否跟随鼠标
	bool m_bShowCross;  //显示取数据时后的十字线
	bool m_bLeftButtonDown;
	bool m_iVerticalControlMode;
	int m_iControlMode;
	int m_nClientHeight;
	int m_nClientWidth;
	double m_dVerticalFactor;
	double m_dHorizontalFactor;
	CString m_sCurrentControlCurve;
	COLORREF m_crBackColor;   //背景颜色，一个绘图对象只有一种背景颜色
	COLORREF m_crGridColor;
	COLORREF m_crFrameColor;
	COLORREF m_crPlotAreaBackColor;

	COLORREF m_SelectedCurveColor;


	CBrush m_brushBack;
	CBrush m_brushGrid;
	CBrush m_brushPlotAreaBack;
private:
	CRect m_recMouseSelect; //鼠标选择的放大缩小区域大小
	CRect m_rectClient;
	CRect m_rectPlot;  //绘图区域大小
	
	CLine m_Line;     //当前选择的区域范围
	CFloatPoint m_BeginPoint;
	CFloatPoint m_EndPoint;

	double m_Slope;   //当前m_Line的斜率


	CDC m_dcPlot;  //绘图DC
		
	CPoint m_ptCrossCenter;
	CFloatPoint m_ptMaxPos;
	CFloatPoint m_ptMinPos;
	CFloatPoint m_ptZoomOffset;
	CFloatPoint m_ptMoveOffset;

	//用于复位曲线
	CFloatPoint m_ptMaxPosConst;
	CFloatPoint m_ptMinPosConst;
	//

	CPoint m_ptMargin;

	CSize m_CurveNameMaxSize; //保存曲线中最长的名称长度
	int m_LegendLineLenth;   //图例线条长度
	int m_FeatrueNum;       //选择的特征个数
private:
	CBitmap *m_pbitmapOldBackground;
    CBitmap m_bitmapBackground;
	CBitmap *m_pbitmapOldPlot;
	CBitmap m_bitmapPlot;
	CCurveObj* m_pCurSelCurveObj;
private:
	
	//CArray<CPoint,CPoint> m_ArrayLabelPoint;  //存放标注数组

	//std::set<CPoint> m_SetLabelPoint; //存放标注数组

	std::map<int,CFloatPoint> m_MapLabelPoint; //存放标注点和对应的索引,其中CFloatPoint的m_PosX存放曲线X轴大小，m_PosY存放y屏幕坐标


	CArray<COLORREF,COLORREF> m_ArrayCurveColor; //曲线颜色数组
	int m_CurrentColorIndex;                     //当前曲线颜色
	std::map<CString,CCurveObj> m_FeatureMap;  //特征名与特征波段映射

public:
	std::set<CString> m_SetSelCurveName;           //选中曲线名称数组
	CMapStringToOb m_CurveMap;  //用于管理曲线对象链表的map
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
};

//重载CPoint 关系运算符

BOOL operator <(const CPoint &p1,const CPoint &p2 );

BOOL operator >(const CPoint &p1,const CPoint &p2 );

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CHART_H__5955D1B9_F38E_4AAE_8BDC_B41945EAB191__INCLUDED_)
