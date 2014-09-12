// CurveObj.cpp: implementation of the CCurveObj class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CurveObj.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
//##ModelId=48A14586012A
void CFloatPoint::InitPoint(double x,double y)
{
	m_PosX = x;
	m_PosY = y;
}
//##ModelId=48A14586013A
CPoint CFloatPoint::Transform()
{
	CPoint point((int)this->m_PosX,(int)this->m_PosY);
	return point;
}
//##ModelId=48A145860148
inline bool CFloatPoint::operator ==(const CFloatPoint &point) const
{
	if(fabs((this->m_PosX - point.m_PosX))<ZERO)
		if(fabs((this->m_PosY - point.m_PosY))<ZERO)
			return true;
		return false;
}
//##ModelId=48A145860157
CFloatPoint CFloatPoint::operator = (const CFloatPoint& point)
{
	this->m_PosX =point.m_PosX;
	this->m_PosY =point.m_PosY;
	return *this;
}
//##ModelId=48A145860167
CFloatPoint CFloatPoint::operator - (const CFloatPoint &point)
{
	CFloatPoint tempPoint;
	tempPoint.m_PosX = this->m_PosX-point.m_PosX;
	tempPoint.m_PosY = this->m_PosY-point.m_PosY;
	return tempPoint;
}
//##ModelId=48A145860177
CFloatPoint CFloatPoint::operator +(const CFloatPoint &point)
{
	CFloatPoint tempPoint;
	tempPoint.m_PosX = this->m_PosX+point.m_PosX;
	tempPoint.m_PosY = this->m_PosY+point.m_PosY;
	return tempPoint;
}
//##ModelId=48A145860179
CFloatPoint CFloatPoint::operator +=(const CFloatPoint &point)
{
	this->m_PosX = this->m_PosX + point.m_PosX;
	this->m_PosY = this->m_PosY + point.m_PosY;
	return (*this);
}
//##ModelId=48A145860187
CFloatPoint CFloatPoint::operator /(const CFloatPoint &point)
{
	CFloatPoint tempPoint;
	tempPoint.m_PosX = this->m_PosX/point.m_PosX;
	tempPoint.m_PosY = this->m_PosY/point.m_PosY;
	return tempPoint;
}
//##ModelId=48A1458601A5
CFloatPoint CFloatPoint::operator /=(const double value)
{
	if(value>0.00000001)
	{
		this->m_PosX = this->m_PosX/value;
		this->m_PosY = this->m_PosY/value;
	}
	return (*this);
}
//##ModelId=48A145860197
CFloatPoint CFloatPoint::operator /(const double value)
{
	CFloatPoint tempPoint;
	if(value>0.00000001)
	{
		tempPoint.m_PosX = this->m_PosX/value;
		tempPoint.m_PosY = this->m_PosY/value;
	}
	return tempPoint;
}
//##ModelId=48A1458601B5
CFloatPoint CFloatPoint::operator *(const double value)
{
	CFloatPoint tempPoint;
	if(value>0.00000001)
	{
		tempPoint.m_PosX = this->m_PosX * value;
		tempPoint.m_PosY = this->m_PosY * value;
	}
	return tempPoint;
}

CFloatPoint CFloatPoint::operator *=(const double value)
{
	if(value>0.00000001)
	{
		this->m_PosX = this->m_PosX * value;
		this->m_PosY = this->m_PosY * value;
	}
    return (*this);
}
///////////////////////////////////////////////////////////////////////////////
CCurveObj::AvlTree::AvlTree()
{
	root = NULL;
	unbalanced = 0;
}
CCurveObj::AvlTree::~AvlTree()
{
	Destroye(root);//销毁整棵树
	root = NULL;
	unbalanced = 0;//初始状态，是一个空树，是平衡的。
};
void CCurveObj::AvlTree::Inser(CFloatPoint& point)
{
	BST_NODE node;
	node.nodeKey = point.m_PosX;
    node.data = point;
	node.lc = NULL;
	node.rc = NULL;

   	AvlInsert(root,node,unbalanced);
}
void CCurveObj::AvlTree::Destroye(struct TREE_NODE *&root)
{
	if(root)
	{
		if(root->lc) 
			Destroye(root->lc); 
		if(root->rc) 
			Destroye(root->rc);

		delete root; 
		root=NULL; 
	}
}
void CCurveObj::AvlTree::Search(CFloatPoint& point)
{
	BST_NODE* p = root;
	BST_NODE* s = NULL;
	
	double minKey=1000000000.0;
	double t=0;
	
	while(p) 
	{ 
		if(p->nodeKey==point.m_PosX) {s = p;break;} //找到匹配节点
		t= AvlAbs(p->data.m_PosX - point.m_PosX);
		if(t < minKey)
		{
            minKey = t;
			s = p;
		}
		p=(point.m_PosX<p->nodeKey)?p->lc:p->rc;	
	}
	if(s)
	  point = s->data;
}
void CCurveObj::AvlTree::AvlInsert(struct TREE_NODE *&root, struct TREE_NODE& node,int &unbalanced)
{
	if(root == NULL)//查找到叶子节仍然没有找到，则插入新节点。
	{
		root  = new TREE_NODE();
		root->nodeKey = node.nodeKey;
		root->data = node.data;
		root->bf = 0;
		root->lc = root->rc = NULL;
		unbalanced = TRUE;
	} 
	else if(node.nodeKey < root->nodeKey)
	{
		AvlInsert(root->lc,node,unbalanced);  //插入新节点在左子树上.
		if(unbalanced) //
		{
			switch(root->bf) //判断插入新节点后的根节点是否平衡
			{
			case 0:    //以前是平衡的  
				{
					root->bf = 1;  //因为在左子树插入了一个节点，所以插入节点的根节点将变得不平衡，平衡因子变为1
					unbalanced = TRUE; //整个树不平衡标记
					break;
				}
			case -1:           //以前右子树重
				{
					root->bf = 0;  //因为新插入的节点在左子树上，所以新的以root为根的树将变为平衡
					unbalanced = FALSE;  
					break;
				}
			case 1:            //以前左子树重，新插入的节点还是插入在左子树上，则左子树变为更重，则需要以当前根节点root做平衡处理 
				{
					AvlLeftRotation(root);
					unbalanced = FALSE;
					break;
				}
			}
		}
	} 
	else if(node.nodeKey > root->nodeKey) //插入新节点在右子树上.
	{
		AvlInsert(root->rc,node,unbalanced);
		if(unbalanced) 
		{
			switch(root->bf)
			{
			case 0:  //以前是平衡的   
				{
					root->bf = -1;//插入新节点将导致数的右边偏重
					unbalanced = TRUE;  
					break;
				}
			case 1:    //以前左子树偏重，新插入节点后将平衡     
				{
					root->bf = 0;  //刚好平衡
					unbalanced = FALSE;
					break;
				}
			case -1: //以前右子树偏重，新插入节点后将加剧不平衡，需要做平衡处理。      
				{
					AvlRightRotation(root);
					unbalanced = FALSE;
					break;
				}
			}
		}
	} 
	else //要插入的节点已经存在,覆盖
	{
		root->data = node.data;
		unbalanced = FALSE;
	}
}
void CCurveObj::AvlTree::AvlRightRotation(struct TREE_NODE *&root)
{
	TREE_NODE *child = root->rc;
	if(child->bf == -1) 
	{
		root->rc = child->lc;
		child->lc = root;
		root->bf = 0;
		root = child;
	} 
	else 
	{
		TREE_NODE *grand_child = child->lc;
		root->rc = grand_child->lc;
		child->lc = grand_child->rc;
		grand_child->rc = child;
		grand_child->lc = root;
		switch(grand_child->bf) {
		case 0:    
			root->bf = child->bf = 0;
			break;
		case -1:     
			root->bf = 1;
			child->bf = 0;
			break;
		case 1:  
			root->bf = 0;
			child->bf = -1;
		}
		root = grand_child;
	}
	root->bf = 0;
}
void CCurveObj::AvlTree::AvlLeftRotation(struct TREE_NODE *&root)
{
	TREE_NODE *child = root->lc;
	if(child->bf == 1)
	{
		root->lc = child->rc;
		child->rc = root;
		root->bf = 0;
		root = child;
	}
	else 
	{
		TREE_NODE *grand_child = child->rc;
		root->lc = grand_child->rc;
		child->rc = grand_child->lc;
		grand_child->rc = root;
		grand_child->lc = child;
		switch(grand_child->bf)
		{
		case 0:        
			root->bf = child->bf = 0;
			break;
		case 1:        
			root->bf = -1;
			child->bf = 0;
			break;
		case -1:     
			root->bf = 0;
			child->bf = 1; 
		}
		root = grand_child;
	}
	root->bf = 0;
}
double CCurveObj::AvlTree::AvlAbs(double value)
{
    double val = value<ZERO ?-value:value;
	return val;
}
///////////////////////////////////////////////////////////////////////////////
int CCurveObj::m_nShiftPixels = 2;
bool CCurveObj::m_bDrawReverseX = false;
bool CCurveObj::m_bDrawReverseY = false;
CFloatPoint CCurveObj::m_RealTimeBaseCurveDataRange(0.0,0.0); 

CCurveObj::CCurveObj()
{
	;
}
//##ModelId=48A145860251
CCurveObj::CCurveObj(CRect rect)
{
	m_bAllCurveSameBase = false; //所有的曲线都基于一个坐标系
	m_bTimeBaseCurve = false;
	m_CurveColor = RGB(255,0,0);  //曲线颜色
	m_AxesColor = RGB(255,0,0);
	m_CurveTitle = _T("X");
	m_CurvePen.CreatePen(PS_SOLID, 1, m_CurveColor) ;
	m_CurrentPosFp.InitPoint(0.0,0.0);//当前点
	m_OldPosFp.InitPoint(0.0,0.0);//上一个
	m_MaxPosFp.InitPoint(-10000000000.0,-10000000000.0);//当前最大点
	m_MinPosFp.InitPoint(10000000000.0,10000000000.0);//当前最小点
	m_MaxPosFpConst.InitPoint(-10000000000.0,-10000000000.0);//当前最大点
	m_MinPosFpConst.InitPoint(10000000000.0,10000000000.0);//当前最小点

	m_MoveOffsetFp.InitPoint(0.0,0.0);
	m_ZoomOffsetFp.InitPoint(0.0,0.0);
	m_ZoomOffsetPrevious.InitPoint(0.0,0.0);

    m_cPlotRect = rect;
	int regionPoints  = 1;
	m_iRegionSampleCycle = 1;

	m_iRegionPointCounter = regionPoints*2;

    m_LinePointArray.SetSize(0,8192);
	m_iPointCounter = 0;  
	m_dTimeBaseFactor = 1.0; //此值是时间曲线的基数，默认值为一表示每次累加1，如果为0.002，则表示为每次累加0.002
	m_dVerticalFactor = 0.00001;
	m_dHorizontalFactor = 0.00001;

	m_ShowTransAbs = 1; //默认是透过率

	m_pDc = NULL;
}
////
CCurveObj::CCurveObj(CRect rect,bool ShowTransAbs)
{
	m_bAllCurveSameBase = false; //所有的曲线都基于一个坐标系
	m_bTimeBaseCurve = false;
	m_CurveColor = RGB(255,0,0);  //曲线颜色
	m_AxesColor = RGB(255,0,0);
	m_CurveTitle = _T("X");
	m_CurvePen.CreatePen(PS_SOLID, 1, m_CurveColor) ;
	m_CurrentPosFp.InitPoint(0.0,0.0);//当前点
	m_OldPosFp.InitPoint(0.0,0.0);//上一个
	m_MaxPosFp.InitPoint(-10000000000.0,-10000000000.0);//当前最大点
	m_MinPosFp.InitPoint(10000000000.0,10000000000.0);//当前最小点
	m_MaxPosFpConst.InitPoint(-10000000000.0,-10000000000.0);//当前最大点
	m_MinPosFpConst.InitPoint(10000000000.0,10000000000.0);//当前最小点

	m_MoveOffsetFp.InitPoint(0.0,0.0);
	m_ZoomOffsetFp.InitPoint(0.0,0.0);
	m_ZoomOffsetPrevious.InitPoint(0.0,0.0);

    m_cPlotRect = rect;
	int regionPoints  = 1;
	m_iRegionSampleCycle = 1;
	
	m_iRegionPointCounter = regionPoints*2;

    m_LinePointArray.SetSize(0,8192);
	m_iPointCounter = 0;  
	m_dTimeBaseFactor = 1.0; //此值是时间曲线的基数，默认值为一表示每次累加1，如果为0.002，则表示为每次累加0.002
	m_dVerticalFactor = 0.00001;
	m_dHorizontalFactor = 0.00001;

	m_ShowTransAbs = ShowTransAbs; //默认是透过率

	m_pDc = NULL;
}
//##ModelId=48A145860253
CCurveObj::~CCurveObj()
{
      m_LinePointArray.RemoveAll();
}
//##ModelId=48A14586037C
void CCurveObj::CalculateMaxPoint(CFloatPoint& pointDes,const CFloatPoint* PointSource)
{
	if(PointSource->m_PosX > pointDes.m_PosX)
		pointDes.m_PosX = PointSource->m_PosX;
	if(PointSource->m_PosY > pointDes.m_PosY)
		pointDes.m_PosY = PointSource->m_PosY;
}
//##ModelId=48A145860399
void CCurveObj::CalculateMinPoint(CFloatPoint& pointDes,const CFloatPoint* PointSource)
{
	if(PointSource->m_PosX < pointDes.m_PosX)
		pointDes.m_PosX = PointSource->m_PosX;
	if(PointSource->m_PosY < pointDes.m_PosY)
		pointDes.m_PosY = PointSource->m_PosY;
}
//##ModelId=48A14586034D/* = false*/
void CCurveObj::AppendNewPoint(CFloatPoint& point)
{
	//独立坐标系绘图用
	this->CalculateMaxPoint(m_MaxPosFp,&point);  //计算当前曲线的极值,用于在每条曲线自己的坐标系下绘图
	this->CalculateMinPoint(m_MinPosFp,&point);
	m_MaxPosFpConst = m_MaxPosFp; //保存每条曲线的坐标变化区域
	m_MinPosFpConst = m_MinPosFp;
	m_ZoomOffsetFp = m_MaxPosFp-m_MinPosFp;

	m_CurrentPosFp = point;

	if(m_iPointCounter == 0)
	{
		m_OldPosFp = point;
	}
	m_iPointCounter++; 

	m_LinePointArray.Add(point);

	CFloatPoint _point = point;
	if(_point.m_PosY < 0)
	{
		_point.m_PosY = 6.00;
	}
	else
	{
		_point.m_PosY = log10(100/point.m_PosY);
	}
	m_LinePointArrayBak.Add(_point);

	m_cAvlTree.Inser(point);
}
void CCurveObj::AppendNewPoint(CFloatPoint& point,BOOL TransAbs)
{
	//独立坐标系绘图用
	this->CalculateMaxPoint(m_MaxPosFp,&point);  //计算当前曲线的极值,用于在每条曲线自己的坐标系下绘图
	this->CalculateMinPoint(m_MinPosFp,&point);
	m_MaxPosFpConst = m_MaxPosFp; //保存每条曲线的坐标变化区域
	m_MinPosFpConst = m_MinPosFp;
	m_ZoomOffsetFp = m_MaxPosFp-m_MinPosFp;

	m_OldPosFp = m_CurrentPosFp;//用于动态绘制曲线
	m_CurrentPosFp = point;

	if(m_iPointCounter == 0)
	{
		m_OldPosFp = point;
	}
	m_iPointCounter++; 
	if(TransAbs)
	{
		//如果是透射率

		m_LinePointArray.Add(point);

		//计算吸光度
		CFloatPoint _point = point;
		if(_point.m_PosY < 0)
		{
			_point.m_PosY = 6.00;
		}
		else
		{
			_point.m_PosY = log10(100/point.m_PosY);
		}
		m_LinePointArrayBak.Add(_point);
		//把透过率插入二叉树树
		m_cAvlTree.Inser(point);
	}
	else
	{
		//如果是吸光度
		//首先把点存到吸光度数组
		m_LinePointArrayBak.Add(point);

		//计算透过率
		CFloatPoint _point = point;
		_point.m_PosY = 100/pow(10,point.m_PosY);
		m_LinePointArray.Add(_point);

		//把透过率插入二叉树树
		m_cAvlTree.Inser(_point);
	}
}

//实时绘图函数,上下限必须预先确定
//##ModelId=48ACE0A500DA
void CCurveObj::DrawRealtimeCurve(CDC* pDC,CRect plotRect)
{
	if((!m_bAllCurveSameBase) && (!m_bTimeBaseCurve))  //要动态绘图，必须是具有相同坐标系，否则将毫无意义
		return ;

	double dataX;
	double dataY;
	double dVerticalFactor = plotRect.Height()/(m_RealTimeBaseCurveDataRange.m_PosX - m_RealTimeBaseCurveDataRange.m_PosY);
	CPen* oldPen = pDC->SelectObject(&m_CurvePen);

	CFloatPoint _Point;//基于时基变换如下
	if(m_bTimeBaseCurve)
	{
		dataX = (m_bDrawReverseX)?plotRect.right-m_nShiftPixels:
	                              plotRect.left+m_nShiftPixels;
        dataY = (m_bDrawReverseY)?plotRect.bottom-(m_OldPosFp.m_PosY-m_RealTimeBaseCurveDataRange.m_PosY)*dVerticalFactor:
	                              plotRect.top+(m_OldPosFp.m_PosY-m_RealTimeBaseCurveDataRange.m_PosY)*dVerticalFactor;
	    _Point.InitPoint(dataX,dataY);
	}
	pDC->MoveTo(_Point.Transform());
    if(m_bTimeBaseCurve)
	{
		dataX = (m_bDrawReverseX)?plotRect.right:
	                              plotRect.left;
        dataY = (m_bDrawReverseY)?plotRect.bottom-(m_CurrentPosFp.m_PosY-m_RealTimeBaseCurveDataRange.m_PosY)*dVerticalFactor:
	                              plotRect.top+(m_CurrentPosFp.m_PosY-m_RealTimeBaseCurveDataRange.m_PosY)*dVerticalFactor;
	   
		_Point.InitPoint(dataX,dataY);

	}
	pDC->LineTo(_Point.Transform());
	pDC->SelectObject(oldPen);
}
//##ModelId=48A14586036C
CPoint CCurveObj::CFloat2Point(const CFloatPoint& floatPoint,CRect plotRect)
{

	m_dHorizontalFactor =plotRect.Width()/(m_ZoomOffsetFp.m_PosX+0.000000001);
	m_dVerticalFactor =plotRect.Height()/(m_ZoomOffsetFp.m_PosY+0.000000001);
	
	double dataX = 0.0;
	double dataY = 0.0;
	


	dataX = (m_bDrawReverseX)?plotRect.right-(floatPoint.m_PosX - m_MinPosFp.m_PosX + m_MoveOffsetFp.m_PosX - m_ZoomOffsetPrevious.m_PosX/2)*m_dHorizontalFactor:
	                          plotRect.left+(floatPoint.m_PosX - m_MinPosFp.m_PosX + m_MoveOffsetFp.m_PosX - m_ZoomOffsetPrevious.m_PosX/2)*m_dHorizontalFactor;
    dataY = (m_bDrawReverseY)?plotRect.top+(floatPoint.m_PosY-m_MinPosFp.m_PosY+m_MoveOffsetFp.m_PosY- m_ZoomOffsetPrevious.m_PosY/2)*m_dVerticalFactor:
							  plotRect.bottom-(floatPoint.m_PosY-m_MinPosFp.m_PosY+m_MoveOffsetFp.m_PosY- m_ZoomOffsetPrevious.m_PosY/2)*m_dVerticalFactor;
	CFloatPoint _Point;
	_Point.InitPoint(dataX,dataY);


	return _Point.Transform();

}
void CCurveObj::DrawCurve(CDC* pDC,CRect plotRect)
{
	if(m_LinePointArray.GetSize()==0)
		return ;
	
	m_pDc = pDC;
	m_cPlotRect = plotRect;	
	
	CFloatPoint _Point = m_LinePointArray.GetAt(0);

	if((m_MaxPosFp.m_PosY == m_MinPosFp.m_PosY))//处理纵坐标全为0的情况
	{
         m_MaxPosFp.m_PosY +=1;
		 m_MinPosFp.m_PosY -=1;
         m_ZoomOffsetFp = m_MaxPosFp - m_MinPosFp;
	}
	
	m_dHorizontalFactor =plotRect.Width()/(m_ZoomOffsetFp.m_PosX+0.000000001);
	m_dVerticalFactor =plotRect.Height()/(m_ZoomOffsetFp.m_PosY+0.000000001);
	
	CPen* oldPen = pDC->SelectObject(&m_CurvePen);
	double dataX = 0.0;
	double dataY = 0.0;
	
	dataX = (m_bDrawReverseX)?plotRect.right-(_Point.m_PosX - m_MinPosFp.m_PosX + m_MoveOffsetFp.m_PosX - m_ZoomOffsetPrevious.m_PosX/2)*m_dHorizontalFactor:
	                          plotRect.left+(_Point.m_PosX - m_MinPosFp.m_PosX + m_MoveOffsetFp.m_PosX - m_ZoomOffsetPrevious.m_PosX/2)*m_dHorizontalFactor;
    dataY = (m_bDrawReverseY)?plotRect.top+(_Point.m_PosY-m_MinPosFp.m_PosY+m_MoveOffsetFp.m_PosY- m_ZoomOffsetPrevious.m_PosY/2)*m_dVerticalFactor:
							  plotRect.bottom-(_Point.m_PosY-m_MinPosFp.m_PosY+m_MoveOffsetFp.m_PosY- m_ZoomOffsetPrevious.m_PosY/2)*m_dVerticalFactor;

	_Point.InitPoint(dataX,dataY);

	//绘制曲线
	CPoint plotPoint,oldPlotPoint; //用于计算包含曲线最小区域

	plotPoint=_Point.Transform();
    pDC->MoveTo(_Point.Transform());
	
	int counter = m_LinePointArray.GetSize();
	int j=0;
	for(int i=1;i<counter;i++)
	{
		_Point = m_LinePointArray.GetAt(i);

		dataX = (m_bDrawReverseX)?plotRect.right-(_Point.m_PosX - m_MinPosFp.m_PosX + m_MoveOffsetFp.m_PosX - m_ZoomOffsetPrevious.m_PosX/2)*m_dHorizontalFactor:plotRect.left+(_Point.m_PosX - m_MinPosFp.m_PosX + m_MoveOffsetFp.m_PosX - m_ZoomOffsetPrevious.m_PosX/2)*m_dHorizontalFactor;
        dataY = (m_bDrawReverseY)?plotRect.top+(_Point.m_PosY-m_MinPosFp.m_PosY+m_MoveOffsetFp.m_PosY- m_ZoomOffsetPrevious.m_PosY/2)*m_dVerticalFactor:plotRect.bottom-(_Point.m_PosY-m_MinPosFp.m_PosY+m_MoveOffsetFp.m_PosY- m_ZoomOffsetPrevious.m_PosY/2)*m_dVerticalFactor;

		_Point.InitPoint(dataX,dataY);
			            
		plotPoint = _Point.Transform();
		pDC->LineTo(plotPoint);
	}
	pDC->SelectObject(oldPen);
}
//绘制点
void CCurveObj::DrawPoint(CDC *pDC,CRect plotRect,size_t _pointType)
{
	if(m_LinePointArray.GetSize()==0)
		return ;
	
	m_pDc = pDC;
	m_cPlotRect = plotRect;	
	
	CFloatPoint _Point = m_LinePointArray.GetAt(0);
	
	m_dHorizontalFactor =plotRect.Width()/(m_ZoomOffsetFp.m_PosX+0.000000001);
	m_dVerticalFactor =plotRect.Height()/(m_ZoomOffsetFp.m_PosY+0.000000001);

	CPen* oldPen = pDC->SelectObject(&m_CurvePen);
	double dataX = 0.0;
	double dataY = 0.0;
	
	dataX = (m_bDrawReverseX)?plotRect.right-(_Point.m_PosX - m_MinPosFp.m_PosX + m_MoveOffsetFp.m_PosX - m_ZoomOffsetPrevious.m_PosX/2)*m_dHorizontalFactor:
	                          plotRect.left+(_Point.m_PosX - m_MinPosFp.m_PosX + m_MoveOffsetFp.m_PosX - m_ZoomOffsetPrevious.m_PosX/2)*m_dHorizontalFactor;
    dataY = (m_bDrawReverseY)?plotRect.top+(_Point.m_PosY-m_MinPosFp.m_PosY+m_MoveOffsetFp.m_PosY- m_ZoomOffsetPrevious.m_PosY/2)*m_dVerticalFactor:
							  plotRect.bottom-(_Point.m_PosY-m_MinPosFp.m_PosY+m_MoveOffsetFp.m_PosY- m_ZoomOffsetPrevious.m_PosY/2)*m_dVerticalFactor;

	_Point.InitPoint(dataX,dataY);
	//绘制点
	CPoint plotPoint; //用于计算包含曲线最小区域
	plotPoint=_Point.Transform(); //获取该数据点在屏幕的位置

	int r = 5; //半径
	switch(_pointType)
	{
	case CIRCLE:
		pDC->Ellipse(plotPoint.x-r, plotPoint.y-r, plotPoint.x+r, plotPoint.y+r);  //绘制半径为3的员
		break;
	case SQUARE:
		pDC->Rectangle(plotPoint.x-r, plotPoint.y-r, plotPoint.x+r, plotPoint.y+r); //绘制边长为6的方形
		break;
	}
	pDC->SelectObject(oldPen);
}

//##ModelId=48A1458602DF
void CCurveObj::SetCurveColor(COLORREF color/* = RGB(255,0,0)*/,int lineWidth/* = 1*/)
{
  m_CurveColor = color;  
  m_CurvePen.DeleteObject() ;
  m_CurvePen.CreatePen(PS_SOLID, lineWidth, m_CurveColor);
}
//##ModelId=48A1458602FE
void CCurveObj::Move(double percentX/* = 5*/,double percentY/* = 5*/)
{
    CFloatPoint point = m_ZoomOffsetFp;
	point.m_PosX *=percentX/100.0;
	point.m_PosY *=percentY/100.0;
	m_MoveOffsetFp += point;
}
//##ModelId=48A14586030F
void CCurveObj::ZoomIn(double percentX /*= 5*/,double percentY/*=5*/)
{
	CFloatPoint point = m_ZoomOffsetFp;
	point.m_PosX *=(1-percentX/100.0);
	point.m_PosY *=(1-percentY/100.0);
	
	m_ZoomOffsetPrevious += m_ZoomOffsetFp - point;
	m_ZoomOffsetFp =  point;
}
//##ModelId=48A14586031E
void CCurveObj::MouseSelectZoomIn(CRect zoomRect)
{
	CFloatPoint leftTop;
	CFloatPoint rightBottom;
	CPoint point;
///////////////////////////////////////////////////////////	
	point = zoomRect.TopLeft();
	double dataX = (m_bDrawReverseX)?(m_cPlotRect.right - point.x)/m_dHorizontalFactor + m_MinPosFp.m_PosX - m_MoveOffsetFp.m_PosX + m_ZoomOffsetPrevious.m_PosX/2:
	                          (point.x - m_cPlotRect.left)/m_dHorizontalFactor + m_MinPosFp.m_PosX - m_MoveOffsetFp.m_PosX + m_ZoomOffsetPrevious.m_PosX/2;
    double dataY = (m_bDrawReverseY)?(point.y - m_cPlotRect.top)/m_dVerticalFactor + m_MinPosFp.m_PosY - m_MoveOffsetFp.m_PosY + m_ZoomOffsetPrevious.m_PosY/2:
							  (m_cPlotRect.bottom - point.y)/m_dVerticalFactor + m_MinPosFp.m_PosY - m_MoveOffsetFp.m_PosY + m_ZoomOffsetPrevious.m_PosY/2;
	leftTop.InitPoint(dataX,dataY);
/////////////////////////////////////////////////////////////
	point = zoomRect.BottomRight();
	dataX = (m_bDrawReverseX)?(m_cPlotRect.right - point.x)/m_dHorizontalFactor + m_MinPosFp.m_PosX - m_MoveOffsetFp.m_PosX + m_ZoomOffsetPrevious.m_PosX/2:
	                          (point.x - m_cPlotRect.left)/m_dHorizontalFactor + m_MinPosFp.m_PosX - m_MoveOffsetFp.m_PosX + m_ZoomOffsetPrevious.m_PosX/2;
    dataY = (m_bDrawReverseY)?(point.y - m_cPlotRect.top)/m_dVerticalFactor + m_MinPosFp.m_PosY - m_MoveOffsetFp.m_PosY + m_ZoomOffsetPrevious.m_PosY/2:
							  (m_cPlotRect.bottom - point.y)/m_dVerticalFactor + m_MinPosFp.m_PosY - m_MoveOffsetFp.m_PosY + m_ZoomOffsetPrevious.m_PosY/2;
	rightBottom.InitPoint(dataX,dataY);
/////////////////////////////////////////////////////////////////
	m_MaxPosFp.InitPoint(-10000000000.0,-10000000000.0);
	m_MinPosFp.InitPoint(10000000000.0,10000000000.0);
	m_MoveOffsetFp.InitPoint(0.0,0.0);
	m_ZoomOffsetPrevious.InitPoint(0.0,0.0);

	this->CalculateMaxPoint(m_MaxPosFp,&leftTop);
    this->CalculateMaxPoint(m_MaxPosFp,&rightBottom);

	this->CalculateMinPoint(m_MinPosFp,&leftTop);
    this->CalculateMinPoint(m_MinPosFp,&rightBottom);

	m_ZoomOffsetFp = m_MaxPosFp - m_MinPosFp;
}
//##ModelId=48A14586032D
void CCurveObj::MouseSelectMove(CRect zoomRect)
{

	double percentX = ((zoomRect.left - zoomRect.right)/m_dHorizontalFactor)/m_ZoomOffsetFp.m_PosX *100;
	double percentY = ((zoomRect.top - zoomRect.bottom)/m_dVerticalFactor)/m_ZoomOffsetFp.m_PosY *100;
    
	percentX = (m_bDrawReverseX)?percentX:percentX*(-1);
	percentY = (m_bDrawReverseY)?percentY*(-1):percentY;
	this->Move(percentX,percentY);
}
//##ModelId=48A14586033D
void CCurveObj::ZoomOut(double percentX /*= 5*/,double percentY /*=5*/)
{
	CFloatPoint point = m_ZoomOffsetFp;
	point.m_PosX *=(1+percentX/100.0);
	point.m_PosY *=(1+percentY/100.0);

	m_ZoomOffsetPrevious += m_ZoomOffsetFp - point;
	m_ZoomOffsetFp = point;
}
void CCurveObj::GetDrawCurvePar(CFloatPoint& maxPos,CFloatPoint& minPos,CFloatPoint& moveOffset,CFloatPoint& zoomOffset)
{
	
	maxPos = m_MaxPosFp;
	minPos = m_MinPosFp;
	moveOffset = m_MoveOffsetFp;
	zoomOffset = m_ZoomOffsetFp;
	
}


void CCurveObj::ResetCurve(CFloatPoint MaxPosAllCurve,CFloatPoint MinPosAllCurve)
{
    if(m_bAllCurveSameBase)
	{
		m_MaxPosFp = MaxPosAllCurve;
		m_MinPosFp = MinPosAllCurve;
	}
	else
	{
		m_MaxPosFp = MaxPosAllCurve;
		m_MinPosFp = MinPosAllCurve;	
	}	
	m_ZoomOffsetFp = m_MaxPosFp-m_MinPosFp;
	m_ZoomOffsetPrevious.InitPoint(0.0,0.0);
	m_MoveOffsetFp.InitPoint(0.0,0.0);
}


//##ModelId=48A145860271
CFloatPoint CCurveObj::Position2Data(CPoint point,CPoint& screenPos)
{
    //处理绘图方向对获取数据的影响
	double dataX = (m_bDrawReverseX)?(m_cPlotRect.right - point.x)/m_dHorizontalFactor + m_MinPosFp.m_PosX - m_MoveOffsetFp.m_PosX + m_ZoomOffsetPrevious.m_PosX/2:
	                          (point.x - m_cPlotRect.left)/m_dHorizontalFactor + m_MinPosFp.m_PosX - m_MoveOffsetFp.m_PosX + m_ZoomOffsetPrevious.m_PosX/2;
    double dataY = (m_bDrawReverseY)?(point.y - m_cPlotRect.top)/m_dVerticalFactor + m_MinPosFp.m_PosY - m_MoveOffsetFp.m_PosY + m_ZoomOffsetPrevious.m_PosY/2:
							  (m_cPlotRect.bottom - point.y)/m_dVerticalFactor + m_MinPosFp.m_PosY - m_MoveOffsetFp.m_PosY + m_ZoomOffsetPrevious.m_PosY/2;


	//把屏幕X坐标变换为数据X坐标，然后再平衡二叉排序数上查找此节点
	CFloatPoint _Point;
	_Point.InitPoint(dataX,dataY);
    
	m_cAvlTree.Search(_Point);//在二叉排序树上查找结点
	
	CFloatPoint scrPoint;
	//把查找出来的一对数据坐标系节点再次变化为屏幕坐标系，用于标记数据点用
	dataX = (m_bDrawReverseX)?m_cPlotRect.right-(_Point.m_PosX - m_MinPosFp.m_PosX + m_MoveOffsetFp.m_PosX - m_ZoomOffsetPrevious.m_PosX/2)*m_dHorizontalFactor:
	                          m_cPlotRect.left+(_Point.m_PosX - m_MinPosFp.m_PosX + m_MoveOffsetFp.m_PosX - m_ZoomOffsetPrevious.m_PosX/2)*m_dHorizontalFactor;
    dataY = (m_bDrawReverseY)?m_cPlotRect.top+(_Point.m_PosY-m_MinPosFp.m_PosY+m_MoveOffsetFp.m_PosY- m_ZoomOffsetPrevious.m_PosY/2)*m_dVerticalFactor:
							  m_cPlotRect.bottom-(_Point.m_PosY-m_MinPosFp.m_PosY+m_MoveOffsetFp.m_PosY- m_ZoomOffsetPrevious.m_PosY/2)*m_dVerticalFactor;

	scrPoint.InitPoint(dataX,dataY);
	screenPos = scrPoint.Transform();

	return _Point;
}
CFloatPoint CCurveObj::Position2CurveData(CPoint point)	//CPoint 转为当前曲线的CFloatPoint
{
    //处理绘图方向对获取数据的影响
	double dataX = (m_bDrawReverseX)?(m_cPlotRect.right - point.x)/m_dHorizontalFactor + m_MinPosFp.m_PosX - m_MoveOffsetFp.m_PosX + m_ZoomOffsetPrevious.m_PosX/2:
	                          (point.x - m_cPlotRect.left)/m_dHorizontalFactor + m_MinPosFp.m_PosX - m_MoveOffsetFp.m_PosX + m_ZoomOffsetPrevious.m_PosX/2;
    double dataY = (m_bDrawReverseY)?(point.y - m_cPlotRect.top)/m_dVerticalFactor + m_MinPosFp.m_PosY - m_MoveOffsetFp.m_PosY + m_ZoomOffsetPrevious.m_PosY/2:
							  (m_cPlotRect.bottom - point.y)/m_dVerticalFactor + m_MinPosFp.m_PosY - m_MoveOffsetFp.m_PosY + m_ZoomOffsetPrevious.m_PosY/2;


	//把屏幕X坐标变换为数据X坐标，然后再平衡二叉排序数上查找此节点
	CFloatPoint _Point;
	_Point.InitPoint(dataX,dataY);
    
	m_cAvlTree.Search(_Point);//在二叉排序树上查找结点

	return _Point;
}


CFloatPoint CCurveObj::Position2Data(CPoint point)
{
	double dataX = (m_bDrawReverseX)?(m_cPlotRect.right - point.x)/m_dHorizontalFactor + m_MinPosFp.m_PosX - m_MoveOffsetFp.m_PosX + m_ZoomOffsetPrevious.m_PosX/2:
	                          (point.x - m_cPlotRect.left)/m_dHorizontalFactor + m_MinPosFp.m_PosX - m_MoveOffsetFp.m_PosX + m_ZoomOffsetPrevious.m_PosX/2;
    double dataY = (m_bDrawReverseY)?(point.y - m_cPlotRect.top)/m_dVerticalFactor + m_MinPosFp.m_PosY - m_MoveOffsetFp.m_PosY + m_ZoomOffsetPrevious.m_PosY/2:
							  (m_cPlotRect.bottom - point.y)/m_dVerticalFactor + m_MinPosFp.m_PosY - m_MoveOffsetFp.m_PosY + m_ZoomOffsetPrevious.m_PosY/2;


	//把屏幕X坐标变换为数据X坐标，然后再平衡二叉排序数上查找此节点
	CFloatPoint _Point;
	_Point.InitPoint(dataX,dataY);
	return _Point;
}

//最小二乘法曲线拟合算法，对单峰曲线拟合较好,对多峰曲线采用其他的拟合算法
//double *x;                                /*实型一维数组，输入参数，存放节点的xi值*/
//double *y;                               /*实型一维数组，输入参数，存放节点的yi值*/
//double *a;                               /*双精度实型一维数组，长度为m。返回m一1次拟合多项式的m个系数*/
//int n;                                   /*整型变量，输入参数，给定数据点的个数*/
//int m;                                   /*整型变量，输入参数，拟合多项式的项数*/
//double *dt1;                            /*实型变量，输出参数，拟合多项式与数据点偏差的平方和*/
//double *dt2;                            /*实型变量，输出参数，拟合多项式与数据点偏差的绝对值之和*/
//double *dt3;                            /*实型变量，输出参数，拟合多项式与数据点偏差的绝对值最大值*/
BOOL CCurveObj::Smooth(double* a)

{
	int i,j,k;
	int n = m_LinePointArray.GetSize();
	int m = CURVE_FIT_TIMES;//拟合次数	
	double z,d1,p,c,d2,g,q;

	double* x = new double[n];
	double* y = new double[n];	
	double* s = new double[n];
	double* t = new double[n];
	double* b = new double[n];

	if(x == NULL || y == NULL || s == NULL || t == NULL || b == NULL)
	{
		return FALSE;
	}

	CFloatPoint point;
	for(i=0;i<n;i++)
	{
       point = m_LinePointArray.GetAt(i);
	   x[i] = point.m_PosX;
	   y[i] = point.m_PosY;
	}

	z = 0;
	for(i=1;i<=n;i++)	
		z=z+x[i-1]/n;                            /*z为各个x的平均值*/
	b[0]=1;
	d1=n;
	p=0;
	c=0;
	for(i=1;i<=n;i++)	
	{	
		p=p+x[i-1]-z;
		c=c+y[i-1];
	}
	
	c=c/d1;
	p=p/d1;
	a[0]=c*b[0];
	if(m>1)	
	{	
		t[1]=1;	
		t[0]=-p;	
		d2=0;	
		c=0;
		g=0;
		for(i=1;i<=n;i++)	
		{	
			q=x[i-1]-z-p;
			d2=d2+q*q;
			c=y[i-1]*q+c;
			g=(x[i-1]-z)*q*q+g;
		}
		
		c=c/d2;
		p=g/d2;
		q=d2/d1;
		d1=d2;
		a[1]=c*t[1];
		a[0]=c*t[0]+a[0];
	}
	
	for(j=3;j<=m;j++)
	{
		s[j-1]=t[j-2];
		s[j-2]=-p*t[j-2]+t[j-3];
		if(j>=4)	
			for(k=j-2;k>=2;k--)		
				s[k-1]=-p*t[k-1]+t[k-2]-q*b[k-1];
			s[0]=-p*t[0]-q*b[0];
			d2=0;
			c=0;
			g=0;
			for(i=1;i<=n;i++)	
			{	
				q=s[j-1];
				for(k=j-1;k>=1;k--)	
					q=q*(x[i-1]-z)+s[k-1];
				d2=d2+q*q;
				c=y[i-1]*q+c;
				g=(x[i-1]-z)*q*q+g;
			}
			c=c/d2;
			p=g/d2;
			q=d2/d1;
			d1=d2;
			a[j-1]=c*s[j-1];	
			t[j-1]=s[j-1];			
			for(k=j-1;k>=1;k--)				
			{			
				a[k-1]=c*s[k-1]+a[k-1];				
				b[k-1]=t[k-1];				
				t[k-1]=s[k-1];				
			}			
	}
	
	/*释放存储空间*/
	delete []x;
	delete []y;
	delete []s;
	delete []t;
	delete []b;
	
	return TRUE;	
}
double CCurveObj::CalculateY(double x)
{
	double q=m_dCurveFitFactor[CURVE_FIT_TIMES-1];		
	for(int k=CURVE_FIT_TIMES-1;k>=1;k--)	//计算与X之相对应的Y值	y= A(n-1)*X^(n-1) + A(n-2)*X^(n-2) +A(n-3)*X^(n-3) .......... A(0);	
		q=q*x+m_dCurveFitFactor[k-1];	
	return q;
}
void CCurveObj::DestroyCurve()
{
	CCurveObj::m_RealTimeBaseCurveDataRange.InitPoint(0.0,0.0);
}
void CCurveObj::SetRealTimeBaseCurveDataRange(CFloatPoint dataRange)
{
	m_RealTimeBaseCurveDataRange = dataRange;
}
CCurveDataSetArray* CCurveObj::GetCurveDataCollection()
{
	if(m_LinePointArray.GetSize())
		return &m_LinePointArray;
	else
		return NULL;
}

//转换为吸光度
int CCurveObj::ChangeToAbsorbance()
{
	//若本来就是吸光度,则返回
	if(!m_ShowTransAbs)
	{
		return 1;
	}
	//首先将原数据保存到备份
	CCurveDataSetArray tempArray;
	tempArray.Copy(m_LinePointArray);
	m_LinePointArray.Copy(m_LinePointArrayBak);
	m_LinePointArrayBak.Copy(tempArray);
	//更新平衡2叉树
	for(int i=0;i<m_LinePointArray.GetCount();i++)
	{
		CFloatPoint _point = m_LinePointArray.GetAt(i);
		m_cAvlTree.Inser(_point);
	}

	ReCalculateMaxMinPar(); //重新当前曲线计算范围
	m_ShowTransAbs = false;
	return 0;
}
//转换为透射率
int CCurveObj::ChangeToTransmittance()
{
	//若本来就是透射率,则返回
	if(m_ShowTransAbs)
	{
		return 1;
	}
	//首先将原数据保存到备份
	CCurveDataSetArray tempArray;
	tempArray.Copy(m_LinePointArray);
	m_LinePointArray.Copy(m_LinePointArrayBak);
	m_LinePointArrayBak.Copy(tempArray);
	//更新平衡2叉树
	for(int i=0;i<m_LinePointArray.GetCount();i++)
	{
		CFloatPoint _point = m_LinePointArray.GetAt(i);
		m_cAvlTree.Inser(_point);
	}
	//ReCalculateMaxMinPar();  //重新当前曲线计算范围
	m_ShowTransAbs = true;
	return 0;
}

//重新计算
int CCurveObj::ReCalculateMaxMinPar()
{
	m_MaxPosFp.InitPoint(-10000000000.0,-10000000000.0);//当前最大点
	m_MinPosFp.InitPoint(10000000000.0,10000000000.0);//当前最小点

	for(int i=0;i < m_LinePointArray.GetCount();i++)
	{
		CFloatPoint _point = m_LinePointArray[i];

		CalculateMaxPoint(m_MaxPosFp,&_point);
		CalculateMinPoint(m_MinPosFp,&_point);
	}
	//独立坐标系绘图用
	m_MaxPosFpConst = m_MaxPosFp; //保存每条曲线的坐标变化区域
	m_MinPosFpConst = m_MinPosFp;

	m_ZoomOffsetPrevious.InitPoint(0.0,0.0);

	m_ZoomOffsetFp = m_MaxPosFp-m_MinPosFp;

	return 0;
}

int CCurveObj::SetCurveMaxMinPar(CFloatPoint MaxPoint,CFloatPoint MinPoint )
{
	m_MaxPosFp = MaxPoint;   //曲线当前最大点
	m_MinPosFp = MinPoint;   //曲线当前最小点
	m_ZoomOffsetFp = MaxPoint - MinPoint;
	return 0;
}


void CCurveObj::SmoothCurve(CCurveDataSetArray &SmoothedDataSetArray,int N)
{
	SmoothedDataSetArray.RemoveAll();

	int halfwidth = (N-1)/2;
	int tempHalfwidth = 0;			
	for(int i = 0;i<m_LinePointArray.GetCount();++i)
	{
		CFloatPoint tempPoint=m_LinePointArray[i];
		double  meanN = 0;
		if(i < halfwidth)
		{
			tempHalfwidth = i;
			for(int j = i-tempHalfwidth;j<=i+tempHalfwidth;++j)
			{
				meanN += m_LinePointArray[j].m_PosY;
			}
		}
		else if(i >= m_LinePointArray.GetCount() - halfwidth)
		{
			tempHalfwidth = m_LinePointArray.GetCount() - i-1;
			for(int j = i-tempHalfwidth;j<=i+tempHalfwidth;++j)
			{
				meanN += m_LinePointArray[j].m_PosY;
			}
		}
		else
		{	
			tempHalfwidth = halfwidth;
			for(int j = i-tempHalfwidth;j<=i+tempHalfwidth;++j)
			{
				meanN += m_LinePointArray[j].m_PosY;
			}
		}
		tempPoint.m_PosY = meanN/(2*tempHalfwidth+1);
		SmoothedDataSetArray.Add(tempPoint);
	}	
}
double CCurveObj::CalculateYSum()
{
	double Sum = 0;
	for(int i=0;i<m_LinePointArray.GetCount();i++)Sum+=m_LinePointArray[i].m_PosY;
	return Sum;
}

double CCurveObj::CalculateXSum()
{
	double Sum = 0;
	for(int i=0;i<m_LinePointArray.GetCount();i++)Sum+=m_LinePointArray[i].m_PosX;
	return Sum;
}
double CCurveObj::CalculateYMean()
{
	double Sum = CalculateYSum();
	return Sum/m_LinePointArray.GetCount();	
}
double CCurveObj::CalculateXMean()
{
	double Sum = CalculateXSum();
	return Sum/m_LinePointArray.GetCount();
}

double CCurveObj::CalculateYRMS()
{
	double Rms=0;
	double YMean = CalculateYMean();
	for(int i=0;i<m_LinePointArray.GetCount();i++)Rms+=(m_LinePointArray[i].m_PosY-YMean)*(m_LinePointArray[i].m_PosY-YMean);
	Rms = sqrt(Rms/(m_LinePointArray.GetCount()-1));
	return Rms;
}

double CCurveObj::CalculateYSum(size_t _begin,size_t _end)
{
	double Sum = 0;

	size_t begin = _begin<_end?_begin:_end;

	size_t end = _end>_begin?_end:_begin;

	for(size_t i=begin;i <= end;i++)
	{
		Sum += m_LinePointArray[i].m_PosY;
	}
	return Sum;
}

int CCurveObj::GetDataIndex(CFloatPoint _point)
{
	int Index = 0;
	while(m_LinePointArray[Index].m_PosX<_point.m_PosX)Index++;

	if(Index < m_LinePointArray.GetCount())
	{
		return Index;
	}
	else
	{
		return -1;
	}

}
CMatrix CCurveObj::GetYData()
{
	CMatrix temp(1,this->GetPointCounter());

	for(int i =0; i<GetPointCounter();++i)
	{
		temp.SetItem(1,i+1,m_LinePointArray[i].m_PosY);
	}
	return temp;
}
CMatrix CCurveObj::GetXData()
{
	CMatrix temp(1,this->GetPointCounter());

	for(int i =0; i<GetPointCounter();++i)
	{
		temp.SetItem(1,i+1,m_LinePointArray[i].m_PosX);
	}
	return temp;
}