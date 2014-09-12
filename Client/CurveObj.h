// CurveObj.h: interface for the CCurveObj class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CURVEOBJ_H__14F38B2B_017F_4F4C_B4C1_DA2A03A67DB1__INCLUDED_)
#define AFX_CURVEOBJ_H__14F38B2B_017F_4F4C_B4C1_DA2A03A67DB1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


/*
绘图控件(MFC)	ver 1.0

编译器: VC6.0,其它未测试过;
适用于MFC程序,如果要适应其他程序，请自己修改。修改好了不嫌麻烦发送一份给我,同时请保留此信息。
代码可以任意使用，任意修改，但本人不对使用本控件造成的损失负任何责任。
欢迎就任何bug及改进意见邮件给我，让我们一起完善此控件。

copyright(c) 2008 David  m__point@163.com ; m__point@126.com
*/

#include  <stdio.h>
#include  <math.h>
#include  <stdlib.h>
#include <afxtempl.h>
#include "Matrix.h"
//点类型
enum POINT_TYPE
{
	CIRCLE,   //圆形
	SQUARE,	  //方形
};

//#include "ThreadTrace.h"  //调试类

//曲线对象

const double ZERO = 0.000000000000001;
const int CURVE_FIT_TIMES = 6; //最小二乘法曲线拟合次数


class CFloatPoint:public CObject
{
public:
    CFloatPoint(){m_PosX = 0.0;m_PosY = 0.0;}; /*!< 默认构造函数.*/
	CFloatPoint(double x,double y){m_PosX = x;m_PosY = y;}; /*!< 构造函数.*/
	CFloatPoint(CFloatPoint& point){m_PosX = point.m_PosX;m_PosY = point.m_PosY;}; /*!< 构造函数.*/
	~CFloatPoint(){} /*!< 析构函数.*/
public:
	void InitPoint(double x,double y); /*!< 数据对初始化函数.*/
	CPoint Transform(); /*!< 把浮点数据对变换为整数数据对函数.*/
public:
	bool operator ==(const CFloatPoint &point) const; /*!< 对象等于运算符重载.*/
	CFloatPoint operator = (const CFloatPoint&);/*!< 对象赋值运算符重载.*/
    CFloatPoint operator -(const CFloatPoint &point);/*!< 对象运算符重载.*/
	CFloatPoint operator +(const CFloatPoint &point);/*!< 对象运算符重载.*/
	CFloatPoint operator +=(const CFloatPoint &point);/*!< 对象运算符重载.*/
	CFloatPoint operator /(const CFloatPoint &point);/*!< 对象运算符重载.*/
	CFloatPoint operator /(const double value);	/*!< 对象运算符重载.*/
	CFloatPoint operator /=(const double value);/*!< 对象运算符重载.*/
	CFloatPoint operator *(const double value);/*!< 对象运算符重载.*/
	CFloatPoint operator *=(const double value);/*!< 对象运算符重载.*/
public:
	double m_PosX;
	double m_PosY;
};	


typedef CArray <CFloatPoint,CFloatPoint&> CCurveDataSetArray;/*!< 曲线定义.*/

class CCurveObj : public CObject  
{
	//平衡二叉树类,此对象的存在纯粹出于高级功能的实现,如能识别曲线上坐标点
	//典型的以空间换时间在应用
	//潜在问题：因为Avl的创建算法是按照关键字在Avl中查找位置来实现，当两个关键字
	//一样，将会导致新节点的值覆盖具有相同关键字的老节点。比如说曲线是一个圆，则会出现
	//鼠标位置与数据点不对应的情况，但目前来说，此算法完全满足需要。
	//此算法的性能如下:实测1000万个数据(从0 ~ 9999999),IBM T60 笔记本(T7200 2.0G双核CPU,2GB DDR2 667M内存)
	//插入总时间:      3.2s
	//插入平均时间:    3.2ns
	//消耗内存：       275M
	//平均搜索时间:    0.18ns
	//平均搜索深度:    23
	//AVL数的平均深度  24
	//综上所述，性能较好，完全可以满足更多应用。
    class AvlTree  
	{
		//平衡二叉树节点定义，数据域可以使复杂对象，当nodeKey必须是一整数值.
		typedef struct TREE_NODE  
		{
			double nodeKey;  /*!< 节点关键字.*/
			int bf;  /*!< 平衡因子.*/
			CFloatPoint data;/*!< 数据域.*/
			struct TREE_NODE* lc;/*!< 左子树指针.*/
			struct TREE_NODE* rc;/*!< 右子树指针.*/

			public:
				TREE_NODE(){;}
				~TREE_NODE(){;};

		}*BST_NODE_PTR,BST_NODE;
	public:
		AvlTree();
		~AvlTree();
	public:
		void Inser(CFloatPoint& point);//插入一个节点
		void Search(CFloatPoint& point);
		void DestroyeNode(struct TREE_NODE *&root);  //销毁某一格节点，但整棵树还是AVL树
	private: 
		void Destroye(struct TREE_NODE *&root);//销毁整个
		void AvlInsert(struct TREE_NODE *&root, struct TREE_NODE& node,int &unbalanced);
		void AvlRightRotation(struct TREE_NODE *&root);
		void AvlLeftRotation(struct TREE_NODE *&root);
		double AvlAbs(double value);
	private:
		BST_NODE* root;
		int unbalanced;  //标记整棵树是否平衡的标记,初始状态树是平衡的,即默认值是0;此值只有三种状态-1，0，1
	};
	//构造函数
public:
	CCurveObj();
	CCurveObj(CRect plotRect);
	CCurveObj(CRect rect,bool ShowTransAbs);
	virtual ~CCurveObj();
public:
	void DestroyCurve();
	CFloatPoint Position2Data(CPoint point,CPoint& screenPos);//屏幕坐标到数据坐标的变换
	CFloatPoint Position2Data(CPoint point);	//CPoint 转为当前坐标的CFloatPoint
	CFloatPoint Position2CurveData(CPoint point);	//CPoint 转为当前曲线的CFloatPoint
	CPoint CFloat2Point(const CFloatPoint& floatPoint,CRect plotRect);

	//bool HitTest(CPoint point); //老版本点击测试，看选中哪条曲线，未用
public:
	void AppendNewPoint(CFloatPoint& point);//加入一个数据点
	void AppendNewPoint(CFloatPoint& point,BOOL TransAbs);//加入一个数据点

	void DrawRealtimeCurve(CDC* pDC,CRect plotRect);  //实时绘制动态曲线，目前适应时基曲线
	void DrawCurve(CDC* pDC,CRect plotRect);//绘制静态曲线

	void DrawPoint(CDC *pDC,CRect plotRect,size_t _pointType);//绘制点

public:
	void MouseSelectZoomIn(CRect zoomRect);  //鼠标选择放大
	void MouseSelectMove(CRect zoomRect);  //鼠标选择移动

	//void ResetCurve(); //复位曲线
	void ResetCurve(CFloatPoint MaxPosAllCurve,CFloatPoint MinPosAllCurve);

	void Move(double percentX = 5,double percentY = 5 );  //移动曲线
	void ZoomIn(double percentX = 5,double percentY = 5);//整体放大
	void ZoomOut(double percentX=5,double percentY=5);	
public:	 
	static void SetCurveDirction(bool xReverse = false,bool yReverse = false){m_bDrawReverseX = xReverse;m_bDrawReverseY = yReverse;};
	void GetDrawCurvePar(CFloatPoint& maxPos,CFloatPoint& minPos,CFloatPoint& moveOffset,CFloatPoint& zoomOffset);//返回曲线的绘制参数，用于绘制坐标轴
	CCurveDataSetArray* GetCurveDataCollection();
	bool IsTimeBaseCurve(){return m_bTimeBaseCurve;};
	void SetTimeBaseFlag(bool timeBaseFlag = false){m_bTimeBaseCurve = timeBaseFlag;};	//设定时基曲线属性
	void SetTimeBaseFactor(double timeBaseFactor = 1.0){m_dTimeBaseFactor = timeBaseFactor;};
	DWORD GetPointCounter(){return m_iPointCounter;};
	double GetTimeBaseFactor(){return m_dTimeBaseFactor;};
	void SetCurveColor(COLORREF color = RGB(255,0,0),int lineWidth = 1 );
	COLORREF GetCurveColor(){return m_CurveColor;};	
	//void SetAllCureveSameBase(bool sameBase = false);	//所有曲线在同一个坐标系下绘图，而不是在各自的坐标系下绘图，经过分析，只能时基曲线如果不是时基曲线，则只能在各自的坐标系下绘图
	static void SetRealTimeBaseCurveDataRange(CFloatPoint dataRange);
	static int GetShiftPixels(){return m_nShiftPixels;};

	int ChangeToAbsorbance();     //切换到吸光度
	int ChangeToTransmittance();  //切换到透射率
//    int GetAllCurveCurvePar(CFloatPoint& maxPos,CFloatPoint& minPos,CFloatPoint& moveOffset,CFloatPoint& zoomOffset); //
	int ReCalculateMaxMinPar();
//	int SetAllCurveMaxMinPar(CFloatPoint MaxPoint,CFloatPoint MinPoint );
	int SetCurveMaxMinPar(CFloatPoint MaxPoint,CFloatPoint MinPoint );

	void SmoothCurve(CCurveDataSetArray &SmoothedDataSetArray,int N);

	//
	inline int GetXRange(double &MinX,double &MaxX) { MinX = m_MinPosFp.m_PosX; MaxX = m_MaxPosFp.m_PosX; return 0;   };
	inline int GetYRange(double &MinY,double &MaxY) { MinY = m_MinPosFp.m_PosY; MaxY = m_MaxPosFp.m_PosY; return 0;   };

	double CalculateYSum();
	double CalculateYMean();
	double CalculateXSum();
	double CalculateXMean();
	double CalculateYRMS();

	double CalculateYSum(size_t _begin,size_t _end);

	int GetDataIndex(CFloatPoint _point);

	CMatrix GetYData();  //获取Y轴数据，以矩阵形式
	CMatrix GetXData();  //获取X轴数据，以矩阵形式
	inline CFloatPoint GetPoint(int Index) {return m_LinePointArray[Index];};


private:
	double CalculateY(double x);
	BOOL Smooth(double* a);
	void CalculateMaxPoint(CFloatPoint& pointDes,const CFloatPoint* PointSource);
	void CalculateMinPoint(CFloatPoint& pointDes,const CFloatPoint* PointSource);
private:
	CPen     m_CurvePen; 
	COLORREF m_CurveColor;  //曲线颜色
	COLORREF m_AxesColor;  //坐标轴颜色
	CString  m_CurveTitle;  //曲线标题
	CFont   m_TitleFront;  //曲线标题字体
	COLORREF m_FrontColor;  //标题颜色
	CRect m_cPlotRect;      //绘图区域大小
	//POINT* m_pRegionPoints; //老版本HitTest用
	CDC* m_pDc;
private:
	bool m_bTimeBaseCurve;
	bool m_bAllCurveSameBase; //所有的曲线都基于一个坐标系
	static bool m_bDrawReverseX;
	static bool m_bDrawReverseY;
	static int m_nShiftPixels;
	static CFloatPoint   m_RealTimeBaseCurveDataRange;
	
	
	//static CFloatPoint   m_MaxPosAllCurve;   //所有曲线当前最大点
	//static CFloatPoint   m_MinPosAllCurve;   //所有曲线当前最小点

	//static CFloatPoint   m_MaxPosAllCurveConst;      ////所有曲线极值常量，用于复位所有曲线用
    //static CFloatPoint   m_MinPosAllCurveConst;

	//static CFloatPoint   m_ZoomOffsetAllCurve;


	CFloatPoint  m_CurrentPosFp; //当前点
	CFloatPoint  m_OldPosFp;     //上一个,此两点用于动态绘图
	//用于把数据坐标变换到逻辑坐标用，对于同坐标系绘图,所有曲线此值都相同
	CFloatPoint m_MaxPosFp;   //当前最大点
	CFloatPoint m_MinPosFp;   //当前最小点
	//用于把数据坐标变换到逻辑坐标用，对于同坐标系绘图,所有曲线此值都相同
	double m_dVerticalFactor;
	double m_dHorizontalFactor;

    //用于复位曲线用,主要用于每条曲线在各自的坐标系下绘图。
	CFloatPoint m_MaxPosFpConst;
	CFloatPoint m_MinPosFpConst;
	CFloatPoint m_MoveOffsetFp;   //曲线移动偏移量
	CFloatPoint m_ZoomOffsetFp;   //曲线缩放偏移量
    CFloatPoint m_ZoomOffsetPrevious; //曲线缩放偏移量

	double m_dTimeBaseFactor;//时基因子，因为对于时基曲线不是所有的坐标都是1，有可能为0.1，0.001，或10
	double m_dCurveFitFactor[CURVE_FIT_TIMES];//曲线拟合系数,二次曲线拟合

	DWORD m_iPointCounter; //曲线点数计数器
	int m_iRegionPointCounter; 
	int m_iRegionSampleCycle;

	bool m_ShowTransAbs; //判断是透射率还是吸光度

private:
	AvlTree m_cAvlTree;
	CCurveDataSetArray m_LinePointArray;  //用于存储曲线点的数组对象 （透过率）
	CCurveDataSetArray m_LinePointArrayBak; //用于存储吸光度
	
};

#endif // !defined(AFX_CURVEOBJ_H__14F38B2B_017F_4F4C_B4C1_DA2A03A67DB1__INCLUDED_)
