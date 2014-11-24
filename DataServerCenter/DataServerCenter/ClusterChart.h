#pragma once

#include "Chart.h"
// CPCAChart

typedef std::multimap<CString,CCurveObj*> CMultiMapToCurveObj;
typedef std::map<CString,CCurveObj*> CMapToCurveObj;
typedef std::multimap<CString,CFloatPoint*> CMultiMapToFloatPtArray;
typedef std::map<CString,CFloatPoint*> CMapToFloatPtArray;

class CClusterChart : public CWnd
{
	DECLARE_DYNAMIC(CClusterChart)

public:
	CClusterChart();
	virtual ~CClusterChart();
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	virtual LRESULT DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	virtual void PreSubclassWindow();
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
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
	//���ݽӿں���,����˳��Ҳ����
public:	
	BOOL Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID=NULL); //������ͼ����

	CCurveObj& CreateNewPCACurve(CString curveName,bool timeBaseFlag ,bool REF); //����һ���µ�����

	CCurveObj& CreateNewPoint(CString curveName,bool timeBaseFlag ,bool Fake); //����һ���µĵ�

	CFloatPoint * CreateNewPCAArray(CString curveName,size_t PCNum ,bool REF); //����һ���µ�����

	void AdjustTotal2Show(); //������ʾPCA

	BOOL AppendPoint(CString curveName,double Posx,double Posy,BOOL Fake); //�����ά�����

	void InvalidateChart();  //��ͼ�����Ƴ���������
	void DrawRealtimeCurve();  //����ʵʩ����,ÿ����һ�����ݸ��¶���Ҫ����һ�δ˺�������ʵ�ֶ�̬Ч��
	BOOL DestroyCurve(CString curveName = _T("\0")); //�����ƶ����������ߣ���ָ����������������������

	BOOL DestroyMapCurve(CMapStringToOb &m_CurveMap);
	
	BOOL DestroyMapCurve(CMultiMapToCurveObj &RefPCATotalMap);
	
	BOOL DestroyArrayCurve(CMultiMapToFloatPtArray &RefPCATotalArray);
public:
	void PrintChart(CDC* pDC, CPrintInfo* pInfo);//�ṩ�Ĵ�ӡ�ӿ�

	void SetShowLegend(bool ShowLegend= true);

	void SaveData(CString filePath);//������������Ϊ�ı��ĵ�,�м������߾ͱ���Ϊ�����ļ�
	void SavePlotAsBmp(CString filePath);//�����ͼ����Ϊbmpͼ��
	void ShowMenum(bool showMenum = true){m_bShowMenum = showMenum;};  //��ʵ�Ҽ�̽���˵�
	void ChangeToIndividualCoordinateMode(bool individualMode = true);	//�����Ƕ�������ϵ���ǹ�������ϵ,ÿ����һ��״̬��������ת��Ĭ���Ƕ�������ϵ
	void ChangeToGetDataMode();  //�ö����л���ȡ����ģʽ������ģʽ��������
	void ResetCurve(bool allCurve = false);  //��λ���أ����ѡ����һ�����ߣ���λ�������ߣ����򣬽���λ���е�����

	inline bool GetShowType() { return this->m_ShowTransAbs;};

	void ChangeToSelectMode(bool allCurve = false);  //�л���ѡ������ģʽ
	void ChangeToMoveMode();  //�л����ƶ�����ģʽ
	void ChangeToZoomInMode();  //�л����Ŵ�ģʽ  ò��zoom out����С 
	void ChangeToZoomOutMode();   //�л�����Сģʽ
	void ChangeToVerticalMode();   //�л�����ģʽ����ֱģʽ����Ҫָ�������ƶ�������ģʽ
	void ChageToHorizontalMode();  //�л�����ģʽ��ˮƽģʽ����Ҫָ�������ƶ�������ģʽ
	void ChangeToPeakAreaCalMode();   //�л�����������ģʽ
	void ChangeToLableMode();       //�л�����עģʽ


	void ChageDrawDirectionX();
	void ChageDrawDirectionY();
	


	void HideUnselectCurve(bool hide = false);  //��ѡ��һ�����ߺ��Ƿ���ʵ���������ߣ�Ĭ������ʾ
	void DataShowFollowCursor(bool follow = false); //�Ƿ��ȡ����ʱ������ʾ��������
	void SetBackgroundColor(COLORREF backColor);  //���ñ�����ɫ
	void SetGridLineColor(COLORREF gridLienColor);//������������ɫ
	void SetFrameColor(COLORREF frameColor); //���ñ߿���ɫ
	void SetMargin(int xMargin=30,int yMargin = 30);  //���û�ͼ����ĵؿհ�����
	void SetCurveColor(CString curveName,COLORREF color,int lineWidth); 	//�������ߵ���ɫ���߿�
	void SelectCurve(CString curveName = _T("\0"));


	BOOL CurveExist(const CString &_curveName);   //�жϸ����Ƶ������Ƿ����
	//ѡ������
	bool DeleteCurveSelected();
	bool AddSelectedCurve(CString curveName);
	int SmoothCurve(CCurveDataSetArray& tempArray,CString _curveName,int SmoothMethod,int N);
	int ReCalculateAllCurveMaxMin();
	int ResetAllCurveMaxMin(CFloatPoint MaxPos,CFloatPoint MinPos);
	int ShowSelectedNoise();
	int ShowSelectedMean();
	void ShowSelectedPeakArea();
	void SetTimeBaseCurveProperty(CString curveName=_T(""),double timeFactor=1);//����ʱ����������
	void SetRealTimeBaseCurveDataRange(double lower,double upper);  //���ö�̬ʱ������Y������������

public:  //ɾ������
	void RemoveAllLabel();
public:
    void OnSize(int cx,int cy);//��ͼ����Ҫ����Ӧ���ڴ�С����Ҫ�����ڵ�OnSize�����е��ô˺���
private:
	void SetAllCurveDrawMode(bool onePlot=false);  //�����Ƿ��������߹���һ������ϵ,Ĭ����ÿһ���������Լ�������ϵ�л�ͼ��trueΪ�������߹���һ������ϵ
	BOOL InvalidateCtrl();
	bool HitTest(CPoint point); //�������
	void DrawSelectFocus(CDC* pDc,CPoint& point);
	void DrawZoomRect(CDC* pDc,CRect rect);
	void DrawGetDataCross(CDC* pDc);  //����ȡ����ʱ���ʮ����


	void DrawNearestPt2Cursor(CDC *pDc,CPoint _point);  //�������������ĵ�
	void DrawSelectRange(CDC* pDc,CLine Line);
	void DrawSelectRangeText(CDC* pDc);

	void DrawLegend();		   // ����ͼ��
	void DrawBackGnd();        //�����
	void DrawUnPlotArea();
	void DrawGrid(CString curveName=_T("\0"));  //����������
	void DrawLegend(CDC &m_dcPlot,CRect m_rectPlot);		   // ����ͼ��
	void DrawBackGnd(CDC &m_dcPlot,CRect m_rectPlot);        //�����
	void DrawUnPlotArea(CDC &m_dcPlot,CRect m_rectPlot,CRect m_rectClient);
	void DrawGrid(CDC &m_dcPlot,CRect m_rectPlot,CRect m_rectClient);  //����������

   
	void MoveCurve(CString curveName = _T("\0"),int moveType = CURVE_RESET,double percent = 5);	//����ָ��ģʽ�ƶ�һ�����߻���������
	void ZoomCurve(CString curveName = _T("\0"),int zoomType = CURVE_RESET,double percent = 5);	//����ָ��ģʽ����һ�����߻���������
	BOOL GetMaxDataRect(CString curveName=_T("\0"));  //�������������������������ֵ
	void CalculateMaxPoint(CFloatPoint& pointDes,const CFloatPoint* PointSource);
    void CalculateMinPoint(CFloatPoint& pointDes,const CFloatPoint* PointSource);
	void IndividualCurveWheelContol(CCurveObj* curveObj,int controlMode,bool verticalControl,int zDelta,CPoint point);
	void IndividualCurveControl(CCurveObj* curveObj = NULL,int controlMode = CURVE_MOVE,CRect controlRect = CRect(0,0,0,0));
	void IndividualCurveMove(CString curveName,int moveType,double percent); //��ǰ�����ƶ�����
	void IndividualCurveZoom(CString curveName,int zoomType,double percent);//�����������ſ���
	
	int  FloatDataPrecise(double f);  //���㸡������С����λ��
	void DrawDataFocus(CDC *pDc,CPoint point,COLORREF color);
private:
	bool m_bDrawLegend;    //��ʾͼ��

	bool m_ShowTransAbs;  //true ��ʾ͸���ʣ�false��ʾ�����

	bool m_bDrawReverseX;
	bool m_bDrawReverseY;
	bool m_bShowMenum;
	bool m_bAllCurveSameBase;
	bool m_bHideUnselectCurve;  //����δѡ�е�����
	bool m_bShowDataFlowMouse;  //������ʾ�Ƿ�������
	bool m_bShowCross;  //��ʾȡ����ʱ���ʮ����
	bool m_bLeftButtonDown;
	bool m_iVerticalControlMode;
	int m_iControlMode;
	int m_nClientHeight;
	int m_nClientWidth;
	double m_dVerticalFactor;
	double m_dHorizontalFactor;
	CString m_sCurrentControlCurve;
	COLORREF m_crBackColor;   //������ɫ��һ����ͼ����ֻ��һ�ֱ�����ɫ
	COLORREF m_crGridColor;
	COLORREF m_crFrameColor;
	COLORREF m_crPlotAreaBackColor;

	COLORREF m_SelectedCurveColor;

	COLORREF m_RefSpecPCAColor; //�ο�ͼ����ɫ

	COLORREF m_FakeColor;
	COLORREF m_AnaColor;


	CBrush m_brushBack;
	CBrush m_brushGrid;
	CBrush m_brushPlotAreaBack;
private:
	CRect m_recMouseSelect; //���ѡ��ķŴ���С�����С
	CRect m_rectClient;
	CRect m_rectPlot;  //��ͼ�����С
	
	CLine m_Line;     //��ǰѡ�������Χ
	CFloatPoint m_BeginPoint;
	CFloatPoint m_EndPoint;

	double m_Slope;   //��ǰm_Line��б��


	CDC m_dcPlot;  //��ͼDC
		
	CPoint m_ptCrossCenter;
	CFloatPoint m_ptMaxPos;
	CFloatPoint m_ptMinPos;

	CFloatPoint m_ptZoomOffset;
	CFloatPoint m_ptMoveOffset;

	CPoint m_ptMargin;

	//���ڸ�λ����
	CFloatPoint m_ptMaxPosConst;
	CFloatPoint m_ptMinPosConst;

	CSize m_CurveNameMaxSize; //����������������Ƴ���
	int m_LegendLineLenth;   //ͼ����������
	int m_FeatrueNum;       //ѡ�����������
private:
	CBitmap *m_pbitmapOldBackground;
    CBitmap m_bitmapBackground;
	CBitmap *m_pbitmapOldPlot;
	CBitmap m_bitmapPlot;
	CCurveObj* m_pCurSelCurveObj;
private:
	std::set<CPoint> m_SetLabelPoint; //��ű�ע����
	CArray<COLORREF,COLORREF> m_ArrayCurveColor; //�ǲο�ͼ����ɫ����
	int m_CurrentColorIndex;                     //��ǰ�ǲο�ͼ����ɫ	
public:
	CWnd *m_pParent;
	//������ʾ��PCA���߻��
	size_t CluserNum;
	CMapToCurveObj m_FakeSpecCluster;  //�پ�����������
	CMapToCurveObj m_AnaSpecCluster;   //��������������
};



