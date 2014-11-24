
// MSChartTestDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "MSChartTest.h"
#include "MSChartTestDlg.h"
#include "afxdialogex.h"

#include "VcPlot.h"
#include "VcAxis.h"
#include "VcValueScale.h"
#include "VcSeriesCollection.h"
#include "VcSeries.h"
#include "VcPen.h"
#include "VcCategoryScale.h"
#include "VcColor.h"
#include "VcDataGrid.h"
#include "VcBackdrop.h"
#include "VcFill.h"
#include "VcBrush.h"
#include "VcDataPoints.h"
#include "VcDataPoint.h"
#include "VcDataPointLabel.h"
#include "VcAxisTitle.h"
#include "math.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CMSChartTestDlg 对话框




CMSChartTestDlg::CMSChartTestDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CMSChartTestDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMSChartTestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	//DDX_Control(pDX, IDC_MSCHART1, m_mschart);
}

BEGIN_MESSAGE_MAP(CMSChartTestDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
END_MESSAGE_MAP()


// CMSChartTestDlg 消息处理程序

BOOL CMSChartTestDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	CRect rc;
	GetClientRect(&rc);
	if(!m_Chart.Create(L"白酒样本主要特征物质浓度", WS_CHILD| WS_VISIBLE, rc, this, 10))
	{
		return -1;
	}
	InitChart();
	DrawChart();
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CMSChartTestDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CMSChartTestDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
		
	}
	else
	{
		CRect rect;
		GetClientRect(&rect);
		m_Chart.MoveWindow(rect);
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CMSChartTestDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

//主要设置表的样式：表的XY轴长度色彩等等

void CMSChartTestDlg::InitChart()
{

	// 设置标题 
	m_Chart.SetTitleText(L"mschart 示例 by thinkry@263.net"); 

	// 下面两句改变背景色 
	m_Chart.GetBackdrop().GetFill().SetStyle(1); 
	m_Chart.GetBackdrop().GetFill().GetBrush().GetFillColor().Set(255, 255, 255); 

	// 显示图例 

	m_Chart.SetShowLegend(TRUE); 
	m_Chart.SetColumn(1); 
	m_Chart.SetColumnLabel(L"1号机"); 
	m_Chart.SetColumn(2); 
	m_Chart.SetColumnLabel(L"2号机"); 
	m_Chart.SetColumn(3); 
	m_Chart.SetColumnLabel(L"3号机"); 

	// 栈模式 
	//m_Chart.SetStacking(TRUE); 

	// Y轴设置 
	VARIANT var; 
	m_Chart.GetPlot().GetAxis(1,var).GetValueScale().SetAuto(TRUE); // 不自动标注Y轴刻度 
	m_Chart.GetPlot().GetAxis(1,var).GetValueScale().SetMaximum(100); // Y轴最大刻度 
	m_Chart.GetPlot().GetAxis(1,var).GetValueScale().SetMinimum(0); // Y轴最小刻度 
	m_Chart.GetPlot().GetAxis(1,var).GetValueScale().SetMajorDivision(5); // Y轴刻度5等分 
	m_Chart.GetPlot().GetAxis(1,var).GetValueScale().SetMinorDivision(1); // 每刻度一个刻度线 
	m_Chart.GetPlot().GetAxis(1,var).GetAxisTitle().SetText(L"小时"); // Y轴名称 

	//m_Chart.GetPlot().GetAxis(1,var).GetAxisTitle().GetVtFont().GetVtColor().Set(0,255,0);
	//m_Chart.GetPlot().GetAxis(1,var).GetAxisTitle().GetVtFont().SetName(L"Arial Unicode MS");
	// 3条曲线 
	m_Chart.SetColumnCount(3);  

	// 线色 
	m_Chart.GetPlot().GetSeriesCollection().GetItem(1).GetPen().GetVtColor().Set(0, 0, 255); 
	m_Chart.GetPlot().GetSeriesCollection().GetItem(2).GetPen().GetVtColor().Set(255, 0, 0); 
	m_Chart.GetPlot().GetSeriesCollection().GetItem(3).GetPen().GetVtColor().Set(0, 255, 0); 

	// 线宽(对点线图有效) 
	m_Chart.GetPlot().GetSeriesCollection().GetItem(1).GetPen().SetWidth(50); 
	m_Chart.GetPlot().GetSeriesCollection().GetItem(2).GetPen().SetWidth(100); 
	m_Chart.GetPlot().GetSeriesCollection().GetItem(3).GetPen().SetWidth(2); 

	// 数据点类型显示数据值的模式(对柱柱状图和点线图有效) 
	// 0: 不显示 1: 显示在柱状图外 
	// 2: 显示在柱状图内上方 3: 显示在柱状图内中间 4: 显示在柱状图内下方 
	m_Chart.GetPlot().GetSeriesCollection().GetItem(1).GetDataPoints().GetItem(-1).GetDataPointLabel().SetLocationType(1); 
	m_Chart.GetPlot().GetSeriesCollection().GetItem(2).GetDataPoints().GetItem(-1).GetDataPointLabel().SetLocationType(1); 
	m_Chart.GetPlot().GetSeriesCollection().GetItem(3).GetDataPoints().GetItem(-1).GetDataPointLabel().SetLocationType(1);
	//放大缩小
	
}

void CMSChartTestDlg::DrawChart()
{
	int nRowCount = 6;
	m_Chart.SetRowCount(nRowCount);

	VARIANT var;
	m_Chart.GetPlot().GetAxis(0,var).GetCategoryScale().SetAuto(FALSE);			// 不自动标注X轴刻度
	m_Chart.GetPlot().GetAxis(0,var).GetCategoryScale().SetDivisionsPerLabel(1);// 每刻度一个标注
	m_Chart.GetPlot().GetAxis(0,var).GetCategoryScale().SetDivisionsPerTick(1); // 每刻度一个刻度线
	m_Chart.GetPlot().GetAxis(0,var).GetAxisTitle().SetText(L"日期");			// X轴名称

	//char buf[32];
	srand( (unsigned)time( NULL ) );
	CString Label;
	for(int row = 1; row <= nRowCount; ++row)
	{
		m_Chart.SetRow(row);
		Label.Format(L"%d号", row);

		m_Chart.SetRowLabel(Label);
		m_Chart.GetDataGrid().SetData(row, 1, rand() * 100 / RAND_MAX, 0);
		m_Chart.GetDataGrid().SetData(row, 2, rand() * 100 / RAND_MAX, 0);
		m_Chart.GetDataGrid().SetData(row, 3, rand() * 100 / RAND_MAX, 0);
	}

	m_Chart.Refresh();	
}
