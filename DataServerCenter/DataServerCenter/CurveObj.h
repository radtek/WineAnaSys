// CurveObj.h: interface for the CCurveObj class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CURVEOBJ_H__14F38B2B_017F_4F4C_B4C1_DA2A03A67DB1__INCLUDED_)
#define AFX_CURVEOBJ_H__14F38B2B_017F_4F4C_B4C1_DA2A03A67DB1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


/*
��ͼ�ؼ�(MFC)	ver 1.0

������: VC6.0,����δ���Թ�;
������MFC����,���Ҫ��Ӧ�����������Լ��޸ġ��޸ĺ��˲����鷳����һ�ݸ���,ͬʱ�뱣������Ϣ��
�����������ʹ�ã������޸ģ������˲���ʹ�ñ��ؼ���ɵ���ʧ���κ����Ρ�
��ӭ���κ�bug���Ľ�����ʼ����ң�������һ�����ƴ˿ؼ���

copyright(c) 2008 David  m__point@163.com ; m__point@126.com
*/

#include  <stdio.h>
#include  <math.h>
#include  <stdlib.h>
#include <afxtempl.h>
#include "Matrix.h"
//������
enum POINT_TYPE
{
	CIRCLE,   //Բ��
	SQUARE,	  //����
};

//#include "ThreadTrace.h"  //������

//���߶���

const double ZERO = 0.000000000000001;
const int CURVE_FIT_TIMES = 6; //��С���˷�������ϴ���


class CFloatPoint:public CObject
{
public:
    CFloatPoint(){m_PosX = 0.0;m_PosY = 0.0;}; /*!< Ĭ�Ϲ��캯��.*/
	CFloatPoint(double x,double y){m_PosX = x;m_PosY = y;}; /*!< ���캯��.*/
	CFloatPoint(CFloatPoint& point){m_PosX = point.m_PosX;m_PosY = point.m_PosY;}; /*!< ���캯��.*/
	~CFloatPoint(){} /*!< ��������.*/
public:
	void InitPoint(double x,double y); /*!< ���ݶԳ�ʼ������.*/
	CPoint Transform(); /*!< �Ѹ������ݶԱ任Ϊ�������ݶԺ���.*/
public:
	bool operator ==(const CFloatPoint &point) const; /*!< ����������������.*/
	CFloatPoint operator = (const CFloatPoint&);/*!< ����ֵ���������.*/
    CFloatPoint operator -(const CFloatPoint &point);/*!< �������������.*/
	CFloatPoint operator +(const CFloatPoint &point);/*!< �������������.*/
	CFloatPoint operator +=(const CFloatPoint &point);/*!< �������������.*/
	CFloatPoint operator /(const CFloatPoint &point);/*!< �������������.*/
	CFloatPoint operator /(const double value);	/*!< �������������.*/
	CFloatPoint operator /=(const double value);/*!< �������������.*/
	CFloatPoint operator *(const double value);/*!< �������������.*/
	CFloatPoint operator *=(const double value);/*!< �������������.*/
public:
	double m_PosX;
	double m_PosY;
};	


typedef CArray <CFloatPoint,CFloatPoint&> CCurveDataSetArray;/*!< ���߶���.*/

class CCurveObj : public CObject  
{
	//ƽ���������,�˶���Ĵ��ڴ�����ڸ߼����ܵ�ʵ��,����ʶ�������������
	//���͵��Կռ任ʱ����Ӧ��
	//Ǳ�����⣺��ΪAvl�Ĵ����㷨�ǰ��չؼ�����Avl�в���λ����ʵ�֣��������ؼ���
	//һ�������ᵼ���½ڵ��ֵ���Ǿ�����ͬ�ؼ��ֵ��Ͻڵ㡣����˵������һ��Բ��������
	//���λ�������ݵ㲻��Ӧ���������Ŀǰ��˵�����㷨��ȫ������Ҫ��
	//���㷨����������:ʵ��1000�������(��0 ~ 9999999),IBM T60 �ʼǱ�(T7200 2.0G˫��CPU,2GB DDR2 667M�ڴ�)
	//������ʱ��:      3.2s
	//����ƽ��ʱ��:    3.2ns
	//�����ڴ棺       275M
	//ƽ������ʱ��:    0.18ns
	//ƽ���������:    23
	//AVL����ƽ�����  24
	//�������������ܽϺã���ȫ�����������Ӧ�á�
    class AvlTree  
	{
		//ƽ��������ڵ㶨�壬���������ʹ���Ӷ��󣬵�nodeKey������һ����ֵ.
		typedef struct TREE_NODE  
		{
			double nodeKey;  /*!< �ڵ�ؼ���.*/
			int bf;  /*!< ƽ������.*/
			CFloatPoint data;/*!< ������.*/
			struct TREE_NODE* lc;/*!< ������ָ��.*/
			struct TREE_NODE* rc;/*!< ������ָ��.*/

			public:
				TREE_NODE(){;}
				~TREE_NODE(){;};

		}*BST_NODE_PTR,BST_NODE;
	public:
		AvlTree();
		~AvlTree();
	public:
		void Inser(CFloatPoint& point);//����һ���ڵ�
		void Search(CFloatPoint& point);
		void DestroyeNode(struct TREE_NODE *&root);  //����ĳһ��ڵ㣬������������AVL��
	private: 
		void Destroye(struct TREE_NODE *&root);//��������
		void AvlInsert(struct TREE_NODE *&root, struct TREE_NODE& node,int &unbalanced);
		void AvlRightRotation(struct TREE_NODE *&root);
		void AvlLeftRotation(struct TREE_NODE *&root);
		double AvlAbs(double value);
	private:
		BST_NODE* root;
		int unbalanced;  //����������Ƿ�ƽ��ı��,��ʼ״̬����ƽ���,��Ĭ��ֵ��0;��ֵֻ������״̬-1��0��1
	};
	//���캯��
public:
	CCurveObj();
	CCurveObj(CRect plotRect);
	CCurveObj(CRect rect,bool ShowTransAbs);
	virtual ~CCurveObj();
public:
	void DestroyCurve();
	CFloatPoint Position2Data(CPoint point,CPoint& screenPos);//��Ļ���굽��������ı任
	CFloatPoint Position2Data(CPoint point);	//CPoint תΪ��ǰ�����CFloatPoint
	CFloatPoint Position2CurveData(CPoint point);	//CPoint תΪ��ǰ���ߵ�CFloatPoint
	CPoint CFloat2Point(const CFloatPoint& floatPoint,CRect plotRect);

public:
	void AppendNewPoint(CFloatPoint& point);//����һ�����ݵ�
	void AppendNewPoint(CFloatPoint& point,BOOL TransAbs);//����һ�����ݵ�

	void DrawRealtimeCurve(CDC* pDC,CRect plotRect);  //ʵʱ���ƶ�̬���ߣ�Ŀǰ��Ӧʱ������
	void DrawCurve(CDC* pDC,CRect plotRect);//���ƾ�̬����

	void DrawPoint(CDC *pDC,CRect plotRect,size_t _pointType);//���Ƶ�

public:
	void MouseSelectZoomIn(CRect zoomRect);  //���ѡ��Ŵ�
	void MouseSelectMove(CRect zoomRect);  //���ѡ���ƶ�

	void ResetCurve(CFloatPoint MaxPosAllCurve,CFloatPoint MinPosAllCurve);

	void Move(double percentX = 5,double percentY = 5 );  //�ƶ�����
	void ZoomIn(double percentX = 5,double percentY = 5);//����Ŵ�
	void ZoomOut(double percentX=5,double percentY=5);	
public:	 
	static void SetCurveDirction(bool xReverse = false,bool yReverse = false){m_bDrawReverseX = xReverse;m_bDrawReverseY = yReverse;};
	void GetDrawCurvePar(CFloatPoint& maxPos,CFloatPoint& minPos,CFloatPoint& moveOffset,CFloatPoint& zoomOffset);//�������ߵĻ��Ʋ��������ڻ���������
	CCurveDataSetArray* GetCurveDataCollection();
	bool IsTimeBaseCurve(){return m_bTimeBaseCurve;};
	void SetTimeBaseFlag(bool timeBaseFlag = false){m_bTimeBaseCurve = timeBaseFlag;};	//�趨ʱ����������
	void SetTimeBaseFactor(double timeBaseFactor = 1.0){m_dTimeBaseFactor = timeBaseFactor;};
	DWORD GetPointCounter(){return m_iPointCounter;};
	double GetTimeBaseFactor(){return m_dTimeBaseFactor;};
	void SetCurveColor(COLORREF color = RGB(255,0,0),int lineWidth = 1 );
	COLORREF GetCurveColor(){return m_CurveColor;};	
	//void SetAllCureveSameBase(bool sameBase = false);	//����������ͬһ������ϵ�»�ͼ���������ڸ��Ե�����ϵ�»�ͼ������������ֻ��ʱ�������������ʱ�����ߣ���ֻ���ڸ��Ե�����ϵ�»�ͼ
	static void SetRealTimeBaseCurveDataRange(CFloatPoint dataRange);
	static int GetShiftPixels(){return m_nShiftPixels;};

	int ChangeToAbsorbance();     //�л��������
	int ChangeToTransmittance();  //�л���͸����
	int ReCalculateMaxMinPar();
	int SetCurveMaxMinPar(CFloatPoint MaxPoint,CFloatPoint MinPoint );

	void SmoothCurve(CCurveDataSetArray &SmoothedDataSetArray,int N);
	inline int GetXRange(double &MinX,double &MaxX) { MinX = m_MinPosFp.m_PosX; MaxX = m_MaxPosFp.m_PosX; return 0;   };
	inline int GetYRange(double &MinY,double &MaxY) { MinY = m_MinPosFp.m_PosY; MaxY = m_MaxPosFp.m_PosY; return 0;   };

	double CalculateYSum();
	double CalculateYMean();
	double CalculateXSum();
	double CalculateXMean();
	double CalculateYRMS();

	double CalculateYSum(size_t _begin,size_t _end);

	int GetDataIndex(CFloatPoint _point);

	CMatrix GetYData();  //��ȡY�����ݣ��Ծ�����ʽ
	CMatrix GetXData();  //��ȡX�����ݣ��Ծ�����ʽ
	inline CFloatPoint GetPoint(int Index) {return m_LinePointArray[Index];};


private:
	double CalculateY(double x);
	BOOL Smooth(double* a);
	void CalculateMaxPoint(CFloatPoint& pointDes,const CFloatPoint* PointSource);
	void CalculateMinPoint(CFloatPoint& pointDes,const CFloatPoint* PointSource);
private:
	CPen     m_CurvePen; 
	COLORREF m_CurveColor;  //������ɫ
	COLORREF m_AxesColor;  //��������ɫ
	CString  m_CurveTitle;  //���߱���
	CFont   m_TitleFront;  //���߱�������
	COLORREF m_FrontColor;  //������ɫ
	CDC* m_pDc;
public:
	CRect m_cPlotRect;      //��ͼ�����С
	
private:
	bool m_bTimeBaseCurve;
	bool m_bAllCurveSameBase; //���е����߶�����һ������ϵ
	static bool m_bDrawReverseX;
	static bool m_bDrawReverseY;
	static int m_nShiftPixels;
	static CFloatPoint   m_RealTimeBaseCurveDataRange;
	

	CFloatPoint  m_CurrentPosFp; //��ǰ��
	CFloatPoint  m_OldPosFp;     //��һ��,���������ڶ�̬��ͼ
	//���ڰ���������任���߼������ã�����ͬ����ϵ��ͼ,�������ߴ�ֵ����ͬ
	CFloatPoint m_MaxPosFp;   //��ǰ����
	CFloatPoint m_MinPosFp;   //��ǰ��С��
	//���ڰ���������任���߼������ã�����ͬ����ϵ��ͼ,�������ߴ�ֵ����ͬ
	double m_dVerticalFactor;
	double m_dHorizontalFactor;

    //���ڸ�λ������,��Ҫ����ÿ�������ڸ��Ե�����ϵ�»�ͼ��
	CFloatPoint m_MaxPosFpConst;
	CFloatPoint m_MinPosFpConst;
	CFloatPoint m_MoveOffsetFp;   //�����ƶ�ƫ����
	CFloatPoint m_ZoomOffsetFp;   //��������ƫ����
    CFloatPoint m_ZoomOffsetPrevious; //��������ƫ����

	double m_dTimeBaseFactor;//ʱ�����ӣ���Ϊ����ʱ�����߲������е����궼��1���п���Ϊ0.1��0.001����10
	double m_dCurveFitFactor[CURVE_FIT_TIMES];//�������ϵ��,�����������

	DWORD m_iPointCounter; //���ߵ���������
	int m_iRegionPointCounter; 
	int m_iRegionSampleCycle;

	bool m_ShowTransAbs; //�ж���͸���ʻ��������

private:
	AvlTree m_cAvlTree;
	CCurveDataSetArray m_LinePointArray;  //���ڴ洢���ߵ��������� ��͸���ʣ�
	CCurveDataSetArray m_LinePointArrayBak; //���ڴ洢�����
	
};

#endif // !defined(AFX_CURVEOBJ_H__14F38B2B_017F_4F4C_B4C1_DA2A03A67DB1__INCLUDED_)
