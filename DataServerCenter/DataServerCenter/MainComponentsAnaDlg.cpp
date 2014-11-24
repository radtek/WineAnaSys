// MainComponentsAnaDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "DataServerCenter.h"
#include "MainComponentsAnaDlg.h"
#include "afxdialogex.h"
#include "ExcelOperator.h"
#include "PrintFrame.h"
#include "PrintPreviewView.h"
#include "PrintView.h"
#include "ComponentPredictModel.h"
#include "ClusterSettingDlg.h"

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
#include "vcshadow.h"
#include "math.h"

// CMainComponentsAnaDlg 对话框

IMPLEMENT_DYNAMIC(CMainComponentsAnaDlg, CDialogEx)

CMainComponentsAnaDlg::CMainComponentsAnaDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CMainComponentsAnaDlg::IDD, pParent)
	,m_pPrintFrame(0)
	,m_Offset(1,COMPONENT_COUNT)
	,m_CurMaxValueShow(0.0)
	,m_bShowAnaComSetting(false)
	,m_pCImportProjectDataProgressDlg(0)
{
	m_ComponentArray.Add(L"乙醛");
	m_ComponentArray.Add(L"乙酸乙酯");
	m_ComponentArray.Add(L"甲醇");
	m_ComponentArray.Add(L"丁酸乙酯");
	m_ComponentArray.Add(L"正丙醇");
	m_ComponentArray.Add(L"异丁醇");
	m_ComponentArray.Add(L"戊酸乙酯");
	m_ComponentArray.Add(L"正丁醇");
	m_ComponentArray.Add(L"异戊醇");
	m_ComponentArray.Add(L"乙酸乙酯");
	m_ComponentArray.Add(L"庚酸乙酯");
	m_ComponentArray.Add(L"乳酸乙酯");
	m_ComponentArray.Add(L"糠醛");
	m_ComponentArray.Add(L"己酸");
	m_ComponentArray.Add(L"β-苯乙醇");
	m_ComponentArray.Add(L"乳酸");
	m_ComponentArray.Add(L"乙酸");

}

CMainComponentsAnaDlg::~CMainComponentsAnaDlg()
{
}

void CMainComponentsAnaDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_PROSPECTRUM, m_ListCtrlAnaSpec);
	DDX_Control(pDX, IDC_LIST_COMPONENTS, m_ListCtrlAnaComponents);
	DDX_Control(pDX, IDC_STATIC_ANASPEC, m_StaticAnaSpec);
	DDX_Control(pDX, IDC_STATIC_ANACOMPONENT, m_StaticAnaComponent);
	DDX_Control(pDX, IDC_STATIC_COMPARESAMPLE, m_StaticCompareSampleSetting);
	DDX_Control(pDX, IDC_LIST_LIBSAMPLESETTING, m_ListSpecFromLib);
	DDX_Control(pDX, IDC_BUTTON_IMPORTSPECFROMLIB, m_btnImportSpecFromLib);
	DDX_Control(pDX, IDC_BUTTON_CLEARLIST, m_btnClearList);
}


BEGIN_MESSAGE_MAP(CMainComponentsAnaDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_NOTIFY(NM_CLICK, IDC_LIST_COMPONENTS, &CMainComponentsAnaDlg::OnNMClickListComponents)
	ON_NOTIFY(NM_CLICK, IDC_LIST_PROSPECTRUM, &CMainComponentsAnaDlg::OnNMClickListProspectrum)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_PROSPECTRUM, &CMainComponentsAnaDlg::OnNMDblclkListProspectrum)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_COMPONENTS, &CMainComponentsAnaDlg::OnNMDblclkListComponents)
	ON_COMMAND(ID_PRINT, &CMainComponentsAnaDlg::OnPrint)
	ON_COMMAND(ID_PRINTPREVIEW, &CMainComponentsAnaDlg::OnPrintpreview)
	ON_MESSAGE(WM_MY_PRINT, &CMainComponentsAnaDlg::OnMyPrint)
	ON_COMMAND(ID_ANACOMPONENTSETTING, &CMainComponentsAnaDlg::OnAnacomponentsetting)
	ON_UPDATE_COMMAND_UI(ID_ANACOMPONENTSETTING, &CMainComponentsAnaDlg::OnUpdateAnacomponentsetting)
	ON_COMMAND(ID_IMPORTSPECTRUM, &CMainComponentsAnaDlg::OnImportspectrum)
	ON_BN_CLICKED(IDC_BUTTON_IMPORTSPECFROMLIB, &CMainComponentsAnaDlg::OnBnClickedButtonImportspecfromlib)
	ON_BN_CLICKED(IDC_BUTTON_CLEARLIST, &CMainComponentsAnaDlg::OnBnClickedButtonClearlist)
	ON_NOTIFY(NM_CLICK, IDC_LIST_LIBSAMPLESETTING, &CMainComponentsAnaDlg::OnNMClickListLibsamplesetting)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_LIBSAMPLESETTING, &CMainComponentsAnaDlg::OnNMDblclkListLibsamplesetting)
	ON_MESSAGE(WM_BEGINIMPORTDATA, &CMainComponentsAnaDlg::OnBeginimportdata)
	ON_MESSAGE(WM_ENDIMPORTDATA, &CMainComponentsAnaDlg::OnEndimportdata)
	ON_COMMAND(ID_PCA, &CMainComponentsAnaDlg::OnPca)
	ON_COMMAND(ID_CLUSTER, &CMainComponentsAnaDlg::OnCluster)
	ON_MESSAGE(WM_BEGINCLUSTER, &CMainComponentsAnaDlg::OnBegincluster)
	ON_MESSAGE(WM_ENDCLUSTER, &CMainComponentsAnaDlg::OnEndcluster)
END_MESSAGE_MAP()


// CMainComponentsAnaDlg 消息处理程序


BOOL CMainComponentsAnaDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	//添加菜单
	m_Menu.LoadMenu(IDR_MENU_COMPONENT); 
	SetMenu(&m_Menu);

	m_ListCtrlAnaSpec.InsertColumn(0,L"项目名称",LVCFMT_LEFT,100);
	m_ListCtrlAnaSpec.InsertColumn(1,L"光谱文件",LVCFMT_LEFT,100);
	m_ListCtrlAnaSpec.InsertColumn(2,L"溯源结果",LVCFMT_LEFT,100);

	DWORD dwStyle = m_ListCtrlAnaComponents.GetExtendedStyle(); //获取当前扩展样式
	dwStyle |= LVS_EX_FULLROWSELECT; //选中某行使整行高亮（report风格时）
	dwStyle |= LVS_EX_CHECKBOXES; //item前生成checkbox控件
	m_ListCtrlAnaComponents.InsertColumn(0,L"分析物质",LVCFMT_LEFT,300);
	m_ListCtrlAnaComponents.SetExtendedStyle(dwStyle);
	//dwStyle |= LVS_EX_GRIDLINES; //网格线（report风格时）
	m_ListCtrlAnaSpec.SetExtendedStyle(dwStyle);

	m_ListSpecFromLib.InsertColumn( 0, L"", LVCFMT_LEFT,30);//插入列 
	m_ListSpecFromLib.InsertColumn( 1, L"光谱编号", LVCFMT_LEFT, 100 );//插入列 
	m_ListSpecFromLib.InsertColumn( 2, L"(仿冒)白酒名称", LVCFMT_LEFT, 150 );//插入列 
	m_ListSpecFromLib.InsertColumn( 3, L"来源说明", LVCFMT_LEFT, 100 );//插入列
	m_ListSpecFromLib.InsertColumn( 4, L"入库日期", LVCFMT_LEFT, 100 );//插入列
	m_ListSpecFromLib.InsertColumn( 5, L"操作人员", LVCFMT_LEFT, 100 );//插入列
	m_ListSpecFromLib.SetExtendedStyle(dwStyle);

	//获取项目光谱路径
	CFileFind finder;
	CString SearchPath=m_projectPath;
	SearchPath+=L"\\*.*";
	BOOL bWorking=finder.FindFile(SearchPath);
	while(bWorking)
	{
		bWorking=finder.FindNextFile();
		CString str=finder.GetFileName();
		if(str.Right(4)==L".CSV"||str.Right(4)==L".csv" &&L"." !=str &&L".." != str)//注意该句需要排除“.”“..”
		{
			m_ArraySpecPath.Add(finder.GetFilePath());
			m_ArraySpecName.Add(finder.GetFileTitle());
		}
	}
	//排序
	SortCArrayByFirst(m_ArraySpecName,m_ArraySpecPath);
	//列表显示

	CString szResultPath=m_projectPath;
	szResultPath+=L"\\溯源结果.xlsx";

	CExcelOperator ExcelOperator;
	ExcelOperator.LoadExcel(szResultPath);

	CString specFileName;
	CString PredictResult;

	int j=0;
	ExcelOperator.GetItem(j+1,1,L"String",specFileName);
	ExcelOperator.GetItem(j+1,2,L"String",PredictResult);
	m_ArraySpecPreResult.Add(PredictResult);

	while(!specFileName.IsEmpty())
	{
		CString specFileTitle;
		specFileTitle=specFileName.Left(specFileName.Find('.'));
		m_ListCtrlAnaSpec.InsertItem(j,m_projectName);
		m_ListCtrlAnaSpec.SetItemText(j,1,specFileTitle);
		m_ListCtrlAnaSpec.SetItemText(j,2,PredictResult);
		m_ArraySpecPreResult.Add(PredictResult);

		j++;
		ExcelOperator.GetItem(j+1,1,L"String",specFileName);
		ExcelOperator.GetItem(j+1,2,L"String",PredictResult);
	}
	ExcelOperator.ReleaseExcel();

	LOGFONT lf; //定义字体结构
	lf.lfWeight=0; //字体磅数=10
	lf.lfHeight=25;	 //字体高度(旋转后的字体宽度)=56
	lf.lfWidth=25;   //字体宽度(旋转后的字体高度)=20
	lf.lfUnderline=FALSE; //无下划线
	lf.lfStrikeOut=FALSE; //无删除线
	lf.lfItalic=FALSE; //非斜体
	lf.lfEscapement=0; //字体显示角度=90°
	lf.lfCharSet=DEFAULT_CHARSET; //使用缺省字符集
	//wcscpy(lf.lfFaceName,L"Times New Roman"); //字体名=@system
	CFont font;
	font.CreateFontIndirect(&lf);
	m_ListCtrlAnaComponents.SetFont(&font);
	m_ListCtrlAnaSpec.SetFont(&font);
	m_ListSpecFromLib.SetFont(&font);

	for(int i = 0;i<m_ComponentArray.GetCount();++i)
	{
		m_ListCtrlAnaComponents.InsertItem(i,m_ComponentArray[i]);
		m_ListCtrlAnaComponents.SetCheck(i);
	}

	//初始化成分预测模型系数和偏置
	CComponentPredictModel ComponentPredictModel;
	ComponentPredictModel.Open();

	CMatrix tmp_w(1,869);
	for(int i = 0;i<m_ComponentArray.GetCount();++i)
	{
		ComponentPredictModel.m_strFilter.Format(L"CAST ([Component] as varchar(1000)) = '%s'",m_ComponentArray[i]);
		ComponentPredictModel.Requery();

		CString2Matrix(tmp_w,ComponentPredictModel.m_Weight,L",");
		m_Weight.AddRows(tmp_w);
		m_Offset.SetItem(1,i+1,ComponentPredictModel.m_Offset);
	}
	m_Weight = m_Weight.Transposition();

	ComponentPredictModel.Close();
	//创建图表
	if(!m_MsChart.Create(L"mschart", WS_CHILD| WS_VISIBLE, CRect(0,0,10,10), this, 10))
	{
		return FALSE;
	}
	//设置对话框图标
	HICON m_hIcon;
	m_hIcon=AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	SetIcon(m_hIcon,TRUE); //设置为大图标

	//设置大小
	int xSize=::GetSystemMetrics(SM_CXSCREEN);
	int ySize=::GetSystemMetrics(SM_CYSCREEN);
	int cx=xSize*9/10;
	int cy=ySize*9/10;
	this->MoveWindow((xSize-cx)/2,(ySize-cy)/2,xSize*9/10,ySize*9/10);
	InitChart();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}



void CMainComponentsAnaDlg::InitChart()
{
	// 设置标题 
	m_MsChart.SetTitleText(L"特征物质浓度"); 
	// 下面两句改变背景色 
	m_MsChart.GetBackdrop().GetFill().SetStyle(1); 
	m_MsChart.GetBackdrop().GetFill().GetBrush().GetFillColor().Set(255, 255, 255); 

	// 显示图例 
	m_MsChart.SetShowLegend(TRUE); 

	// Y轴设置 
	VARIANT var; 
	m_MsChart.GetPlot().GetAxis(1,var).GetValueScale().SetAuto(FALSE); // 不自动标注Y轴刻度 TRUE
	m_MsChart.GetPlot().GetAxis(1,var).GetValueScale().SetMaximum(m_CurMaxValueShow); // Y轴最大刻度 
	m_MsChart.GetPlot().GetAxis(1,var).GetValueScale().SetMinimum(0); // Y轴最小刻度 
	m_MsChart.GetPlot().GetAxis(1,var).GetValueScale().SetMajorDivision(5); // Y轴刻度5等分 
	m_MsChart.GetPlot().GetAxis(1,var).GetValueScale().SetMinorDivision(1); // 每刻度一个刻度线
	//m_MsChart.GetPlot().GetAxis(1,var).GetAxisTitle().SetText(L"物质浓度g/L"); // Y轴名称 


	m_MsChart.GetPlot().GetAxis(0,var).GetCategoryScale().SetAuto(FALSE);			// 不自动标注X轴刻度
	m_MsChart.GetPlot().GetAxis(0,var).GetCategoryScale().SetDivisionsPerLabel(1);// 每刻度一个标注
	m_MsChart.GetPlot().GetAxis(0,var).GetCategoryScale().SetDivisionsPerTick(1); // 每刻度一个刻度线
	m_MsChart.GetPlot().GetAxis(0,var).GetAxisTitle().SetText(L"特征物质");			// X轴名称

	DrawChartSample();
}

void CMainComponentsAnaDlg::DrawChartSample()
{
	m_MsChart.SetRowCount(COMPONENT_COUNT);
	m_MsChart.SetColumnCount(1);
	m_MsChart.SetColumn(1); 
	m_MsChart.SetColumnLabel(L"样本示例");

	srand( (unsigned)time( NULL ) );
	for(int j = 1;j<=COMPONENT_COUNT;++j)
	{
		m_MsChart.SetRow(j);
		m_MsChart.SetRowLabel(m_ComponentArray[j-1]);

		double tmp = rand();
		tmp /= RAND_MAX;
		m_MsChart.GetDataGrid().SetData(j, 1,tmp, 0);//rand() * 100 / RAND_MAX

	}
	m_MsChart.Refresh();
}

void CMainComponentsAnaDlg::DrawChart(const int &newItem,const bool &bCheck,const bool &bSpecOrComponent,bool bProOrLib)
{
	m_CurMaxValueShow = 0;

	if(bSpecOrComponent)
	{
		int nSelSpecCount = 0;
		for(int i = 0;i<m_ListCtrlAnaSpec.GetItemCount();++i)
		{
			if((m_ListCtrlAnaSpec.GetCheck(i)&&i!=newItem)||(m_ListCtrlAnaSpec.GetCheck(i)&&bProOrLib==false)
				||(bCheck == true&&bProOrLib == true&&i == newItem))
			{
				++nSelSpecCount;

				int nSelComponentCount = 0;
				for(int j = 0;j<m_ListCtrlAnaComponents.GetItemCount();++j)
				{
					if(m_ListCtrlAnaComponents.GetCheck(j))
					{
						++nSelComponentCount;
					}
				}
				if(0 == nSelComponentCount)
				{
					m_MsChart.SetRowCount(0);
					m_MsChart.SetColumnCount(0);
					m_MsChart.Refresh();
					DrawChartSample();
					return;
				}
				m_MsChart.SetRowCount(nSelComponentCount);	
			}
		}

		for(int i = 0;i<m_ListSpecFromLib.GetItemCount();++i)
		{
			if((m_ListSpecFromLib.GetCheck(i)&&i!=newItem)||(m_ListSpecFromLib.GetCheck(i)&&bProOrLib == true)
				||(bCheck == 1&&bProOrLib == false&&i == newItem))
			{
				++nSelSpecCount;

				int nSelComponentCount = 0;
				for(int j = 0;j<m_ListCtrlAnaComponents.GetItemCount();++j)
				{
					if(m_ListCtrlAnaComponents.GetCheck(j))
					{
						++nSelComponentCount;
					}
				}
				if(0 == nSelComponentCount)
				{
					m_MsChart.SetRowCount(0);
					m_MsChart.SetColumnCount(0);
					m_MsChart.Refresh();
					DrawChartSample();
					return;
				}
				m_MsChart.SetRowCount(nSelComponentCount);	
			}
		}
		if(0 == nSelSpecCount)
		{
			m_MsChart.SetRowCount(0);
			m_MsChart.SetColumnCount(0);
			m_MsChart.Refresh();
			DrawChartSample();
			return;
		}
		m_MsChart.SetColumnCount(nSelSpecCount);  

		nSelSpecCount = 0;
		for(int i = 0;i<m_ListCtrlAnaSpec.GetItemCount();++i)
		{
			if((m_ListCtrlAnaSpec.GetCheck(i)&&i!=newItem)||(m_ListCtrlAnaSpec.GetCheck(i)&&bProOrLib==false)
				||(bCheck == true&&bProOrLib == true&&i == newItem))
			{
				++nSelSpecCount;
				m_MsChart.SetColumn(nSelSpecCount); 
				m_MsChart.SetColumnLabel(m_ArraySpecName[i]); 
				// 数据点类型显示数据值的模式(对柱柱状图和点线图有效) 
				// 0: 不显示 1: 显示在柱状图外 
				// 2: 显示在柱状图内上方 3: 显示在柱状图内中间 4: 显示在柱状图内下方 
				m_MsChart.GetPlot().GetSeriesCollection().GetItem(nSelSpecCount).GetDataPoints().GetItem(-1).GetDataPointLabel().SetLocationType(1); 
				int nSelComponentCount = 0;
				//srand( (unsigned)time( NULL ) );
				for(int j = 0;j<m_ListCtrlAnaComponents.GetItemCount();++j)
				{
					if(m_ListCtrlAnaComponents.GetCheck(j))
					{
						++nSelComponentCount;
						m_MsChart.SetRow(nSelComponentCount);
						m_MsChart.SetRowLabel(m_ComponentArray[j]);
						m_CurMaxValueShow = m_CurMaxValueShow>m_MapProSpecNameComponent[m_ArraySpecName[i]].GetMax()?m_CurMaxValueShow:m_MapProSpecNameComponent[m_ArraySpecName[i]].GetMax();
						m_MsChart.GetDataGrid().SetData(nSelComponentCount, nSelSpecCount,
							                            m_MapProSpecNameComponent[m_ArraySpecName[i]](1,j+1), 0);//rand() * 100 / RAND_MAX
					}
				}
			}
		}
		for(int i = 0;i<m_ListSpecFromLib.GetItemCount();++i)
		{
			if((m_ListSpecFromLib.GetCheck(i)&&i!=newItem)||(m_ListSpecFromLib.GetCheck(i)&&bProOrLib == true)
				||(bCheck == 1&&bProOrLib == false&&i == newItem))
			{
				++nSelSpecCount;
				m_MsChart.SetColumn(nSelSpecCount); 
				m_MsChart.SetColumnLabel(m_ArraySpecInfoFromLib[i].m_SpecID); 
				// 数据点类型显示数据值的模式(对柱柱状图和点线图有效) 
				// 0: 不显示 1: 显示在柱状图外 
				// 2: 显示在柱状图内上方 3: 显示在柱状图内中间 4: 显示在柱状图内下方 
				m_MsChart.GetPlot().GetSeriesCollection().GetItem(nSelSpecCount).GetDataPoints().GetItem(-1).GetDataPointLabel().SetLocationType(1); 
				int nSelComponentCount = 0;
				//srand( (unsigned)time( NULL ) );
				for(int j = 0;j<m_ListCtrlAnaComponents.GetItemCount();++j)
				{
					if((m_ListCtrlAnaComponents.GetCheck(j)&&j!=newItem)||(bCheck == 1&&j == newItem))
					{
						++nSelComponentCount;
						m_MsChart.SetRow(nSelComponentCount);
						m_MsChart.SetRowLabel(m_ComponentArray[j]);
						m_CurMaxValueShow = m_CurMaxValueShow>m_MapLibSpecIDComponent[m_ArraySpecInfoFromLib[i].m_SpecID].GetMax()?m_CurMaxValueShow:m_MapLibSpecIDComponent[m_ArraySpecInfoFromLib[i].m_SpecID].GetMax();
						m_MsChart.GetDataGrid().SetData(nSelComponentCount, nSelSpecCount, 
							m_MapLibSpecIDComponent[m_ArraySpecInfoFromLib[i].m_SpecID](1,j+1), 0);//rand() * 100 / RAND_MAX
					}
				}
			}
		}
	}
	else
	{
		int nSelSpecCount = 0;
		for(int i = 0;i<m_ListCtrlAnaSpec.GetItemCount();++i)
		{
			if(m_ListCtrlAnaSpec.GetCheck(i))
			{
				++nSelSpecCount;

				int nSelComponentCount = 0;
				for(int j = 0;j<m_ListCtrlAnaComponents.GetItemCount();++j)
				{
					if((m_ListCtrlAnaComponents.GetCheck(j)&&j!=newItem)||(bCheck == 1&&j == newItem))
					{
						++nSelComponentCount;
					}
				}
				if(0 == nSelComponentCount)
				{
					m_MsChart.SetRowCount(0);
					m_MsChart.SetColumnCount(0);
					m_MsChart.Refresh();
					DrawChartSample();
					return;
				}
				m_MsChart.SetRowCount(nSelComponentCount);	
			}
		}

		for(int i = 0;i<m_ListSpecFromLib.GetItemCount();++i)
		{
			if(m_ListSpecFromLib.GetCheck(i))
			{
				++nSelSpecCount;

				int nSelComponentCount = 0;
				for(int j = 0;j<m_ListCtrlAnaComponents.GetItemCount();++j)
				{
					if((m_ListCtrlAnaComponents.GetCheck(j)&&j!=newItem)||(bCheck == 1&&j == newItem))
					{
						++nSelComponentCount;
					}
				}
				if(0 == nSelComponentCount)
				{
					m_MsChart.SetRowCount(0);
					m_MsChart.SetColumnCount(0);
					m_MsChart.Refresh();
					DrawChartSample();
					return;
				}
				m_MsChart.SetRowCount(nSelComponentCount);	
			}
		}

		if(0 == nSelSpecCount)
		{
			m_MsChart.SetRowCount(0);
			m_MsChart.SetColumnCount(0);
			m_MsChart.Refresh();
			DrawChartSample();
			return;
		}
		m_MsChart.SetColumnCount(nSelSpecCount);  

		nSelSpecCount = 0;
		for(int i = 0;i<m_ListCtrlAnaSpec.GetItemCount();++i)
		{
			if(m_ListCtrlAnaSpec.GetCheck(i))
			{
				++nSelSpecCount;
				m_MsChart.SetColumn(nSelSpecCount); 
				m_MsChart.SetColumnLabel(m_ArraySpecName[i]); 
				// 数据点类型显示数据值的模式(对柱柱状图和点线图有效) 
				// 0: 不显示 1: 显示在柱状图外 
				// 2: 显示在柱状图内上方 3: 显示在柱状图内中间 4: 显示在柱状图内下方 
				m_MsChart.GetPlot().GetSeriesCollection().GetItem(nSelSpecCount).GetDataPoints().GetItem(-1).GetDataPointLabel().SetLocationType(1); 
				int nSelComponentCount = 0;
				srand( (unsigned)time( NULL ) );
				for(int j = 0;j<m_ListCtrlAnaComponents.GetItemCount();++j)
				{
					if((m_ListCtrlAnaComponents.GetCheck(j)&&j!=newItem)||(bCheck == 1&&j == newItem))
					{
						++nSelComponentCount;
						m_MsChart.SetRow(nSelComponentCount);
						m_MsChart.SetRowLabel(m_ComponentArray[j]);
						m_CurMaxValueShow = m_CurMaxValueShow>m_MapProSpecNameComponent[m_ArraySpecName[i]].GetMax()?m_CurMaxValueShow:m_MapProSpecNameComponent[m_ArraySpecName[i]].GetMax();
						m_MsChart.GetDataGrid().SetData(nSelComponentCount, nSelSpecCount, 
							                            m_MapProSpecNameComponent[m_ArraySpecName[i]](1,j+1), 0);//rand() * 100 / RAND_MAX
					}
				}
			}
		}

		for(int i = 0;i<m_ListSpecFromLib.GetItemCount();++i)
		{
			if(m_ListSpecFromLib.GetCheck(i))
			{
				++nSelSpecCount;
				m_MsChart.SetColumn(nSelSpecCount); 
				m_MsChart.SetColumnLabel(m_ArraySpecInfoFromLib[i].m_SpecID); 
				// 数据点类型显示数据值的模式(对柱柱状图和点线图有效) 
				// 0: 不显示 1: 显示在柱状图外 
				// 2: 显示在柱状图内上方 3: 显示在柱状图内中间 4: 显示在柱状图内下方 
				m_MsChart.GetPlot().GetSeriesCollection().GetItem(nSelSpecCount).GetDataPoints().GetItem(-1).GetDataPointLabel().SetLocationType(1); 
				int nSelComponentCount = 0;
				srand( (unsigned)time( NULL ) );
				for(int j = 0;j<m_ListCtrlAnaComponents.GetItemCount();++j)
				{
					if((m_ListCtrlAnaComponents.GetCheck(j)&&j!=newItem)||(bCheck == 1&&j == newItem))
					{
						++nSelComponentCount;
						m_MsChart.SetRow(nSelComponentCount);
						m_MsChart.SetRowLabel(m_ComponentArray[j]);
						m_CurMaxValueShow = m_CurMaxValueShow>m_MapLibSpecIDComponent[m_ArraySpecInfoFromLib[i].m_SpecID].GetMax()?m_CurMaxValueShow:m_MapLibSpecIDComponent[m_ArraySpecInfoFromLib[i].m_SpecID].GetMax();
						m_MsChart.GetDataGrid().SetData(nSelComponentCount, nSelSpecCount, 
							m_MapLibSpecIDComponent[m_ArraySpecInfoFromLib[i].m_SpecID](1,j+1), 0);//rand() * 100 / RAND_MAX
					}
				}
			}
		}
	}
	VARIANT var;
	m_MsChart.GetPlot().GetAxis(1,var).GetValueScale().SetMaximum(m_CurMaxValueShow); // Y轴最大刻度 
	m_MsChart.Refresh();	
}

void CMainComponentsAnaDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 在此处添加消息处理程序代码
	// 不为绘图消息调用 CDialogEx::OnPaint()
	CRect rc;
	GetClientRect(&rc);
	CFont Font;
	LOGFONT LogFont;
	LogFont.lfWeight=0; //字体磅数=10
	LogFont.lfHeight=0;	 //字体高度(旋转后的字体宽度)=56
	LogFont.lfWidth=0;   //字体宽度(旋转后的字体高度)=20
	LogFont.lfUnderline=FALSE; //无下划线
	LogFont.lfStrikeOut=FALSE; //无删除线
	LogFont.lfItalic=FALSE; //非斜体
	LogFont.lfCharSet=DEFAULT_CHARSET; //使用缺省字符集
	LogFont.lfEscapement=900; //字体显示角度=90°
	wcscpy(LogFont.lfFaceName,L"微软雅黑"); //字体名=@system_GB2312
	Font.CreateFontIndirect(&LogFont);
	CFont *oldFont = dc.SelectObject(&Font);
	COLORREF oldColor = dc.SetTextColor(RGB(0,0,0));
	CSize strsize = dc.GetTextExtent(L"物质浓度（g/L）");
	dc.FillSolidRect(305,0,(strsize.cy+5),rc.Height(),RGB(255,255,255));
	dc.SetBkMode(TRANSPARENT);
	dc.TextOut(305,rc.Height()/2 - strsize.cx/2,L"物质浓度（g/L）");
	dc.SelectObject(oldFont);
	dc.SetTextColor(oldColor);
	
	if(!m_bShowAnaComSetting)
	{
		m_StaticAnaSpec.MoveWindow(0,0,300,rc.Height()*0.5-5);
		m_ListCtrlAnaSpec.MoveWindow(5,15,290,rc.Height()*0.5-25);

		m_StaticCompareSampleSetting.MoveWindow(0,rc.Height()*0.5+5,300,rc.Height()*0.5-5);
		m_btnImportSpecFromLib.MoveWindow(5,rc.Height()*0.5+35,100,30);
		m_btnClearList.MoveWindow(145,rc.Height()*0.5+35,80,30);
		m_ListSpecFromLib.MoveWindow(5,rc.Height()*0.5+35+30+10,290,rc.Height()*0.5-25-30-10-20);

		m_StaticAnaComponent.MoveWindow(rc.Width()-300,0,0,0);
		m_ListCtrlAnaComponents.MoveWindow(rc.Width()-300+5,5,0,0);

		m_MsChart.MoveWindow(305+(strsize.cy+5),0,rc.Width()-305-(strsize.cy+5),rc.Height());
	}
	else
	{
		m_StaticAnaSpec.MoveWindow(0,0,300,rc.Height()*0.5-5);
		m_ListCtrlAnaSpec.MoveWindow(5,15,290,rc.Height()*0.5-25);

		m_StaticCompareSampleSetting.MoveWindow(0,rc.Height()*0.5+5,300,rc.Height()*0.5-5);
		m_btnImportSpecFromLib.MoveWindow(5,rc.Height()*0.5+35,100,30);
		m_btnClearList.MoveWindow(145,rc.Height()*0.5+35,80,30);
		m_ListSpecFromLib.MoveWindow(5,rc.Height()*0.5+35+30+10,290,rc.Height()*0.5-25-30-10-20);

		m_StaticAnaComponent.MoveWindow(rc.Width()-300,0,300,rc.Height());
		m_ListCtrlAnaComponents.MoveWindow(rc.Width()-300+5,15,290,rc.Height()-20);

		m_MsChart.MoveWindow(305+(strsize.cy+5),0,rc.Width()-605-(strsize.cy+5),rc.Height());
	}
	m_MsChart.Refresh();
}


void CMainComponentsAnaDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
	Invalidate();
}


void CMainComponentsAnaDlg::OnNMClickListProspectrum(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	DWORD dwPos = GetMessagePos();  
	CPoint point( LOWORD(dwPos), HIWORD(dwPos) );  
	m_ListCtrlAnaSpec.ScreenToClient(&point);  
	LVHITTESTINFO lvinfo;  
	lvinfo.pt = point;  
	lvinfo.flags = LVHT_ABOVE;  

	UINT nFlag;  
	int nItem = m_ListCtrlAnaSpec.HitTest(point, &nFlag);  
	//判断是否点在checkbox上  
	if(nFlag == LVHT_ONITEMSTATEICON)  
	{
		//执行checkbox响应代码
		bool bcheck = 1;
		if(m_ListCtrlAnaSpec.GetCheck(nItem))
		{
			bcheck = 0;
		}
		else
		{
			if(m_MapProSpecNameComponent.find(m_ArraySpecName[nItem]) == m_MapProSpecNameComponent.end())
			{
				CCSVOperator CSVOperator;
				CSVOperator.LoadCSV(m_ArraySpecPath[nItem]);

				CMatrix tmpData(1,869);
				for(int i = 0;i<SPEC_LENTH;++i)
				{
					double tmp;
					CSVOperator.GetDouble(i+1,2,tmp);
					tmpData.SetItem(1,i+1,tmp);
				}
				CMatrix tmp_Component = tmpData*m_Weight;
				tmp_Component += m_Offset;
				m_MapProSpecNameComponent.insert(make_pair(m_ArraySpecName[nItem],tmp_Component));
			}
		}
		DrawChart(nItem,bcheck,1);
	}	
	*pResult = 0;
}



void CMainComponentsAnaDlg::OnNMDblclkListProspectrum(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	DWORD dwPos = GetMessagePos();  
	CPoint point( LOWORD(dwPos), HIWORD(dwPos) );  
	m_ListCtrlAnaSpec.ScreenToClient(&point);  
	LVHITTESTINFO lvinfo;  
	lvinfo.pt = point;  
	lvinfo.flags = LVHT_ABOVE;  

	UINT nFlag;  
	int nItem = m_ListCtrlAnaSpec.HitTest(point, &nFlag);  
	//判断是否点在checkbox上  
	if(nFlag == LVHT_ONITEMSTATEICON)  
	{
		//执行checkbox响应代码
		bool bcheck = 1;
		if(m_ListCtrlAnaSpec.GetCheck(nItem))
		{
			bcheck = 0;
		}
		else
		{
			if(m_MapProSpecNameComponent.find(m_ArraySpecName[nItem]) == m_MapProSpecNameComponent.end())
			{
				CCSVOperator CSVOperator;
				CSVOperator.LoadCSV(m_ArraySpecPath[nItem]);

				CMatrix tmpData(1,869);
				for(int i = 0;i<SPEC_LENTH;++i)
				{
					double tmp;
					CSVOperator.GetDouble(i+1,2,tmp);
					tmpData.SetItem(1,i+1,tmp);
				}
				CMatrix tmp_Component = tmpData*m_Weight;
				tmp_Component += m_Offset;
				m_MapProSpecNameComponent.insert(make_pair(m_ArraySpecName[nItem],tmp_Component));
			}
		}
		DrawChart(nItem,bcheck,1);
	}	
	*pResult = 0;
}


void CMainComponentsAnaDlg::OnNMClickListLibsamplesetting(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
	DWORD dwPos = GetMessagePos();  
	CPoint point( LOWORD(dwPos), HIWORD(dwPos) );  
	m_ListSpecFromLib.ScreenToClient(&point);  
	LVHITTESTINFO lvinfo;  
	lvinfo.pt = point;  
	lvinfo.flags = LVHT_ABOVE;  
	UINT nFlag;  
	int nItem = m_ListSpecFromLib.HitTest(point, &nFlag);  
	if(nFlag == LVHT_ONITEMSTATEICON)  
	{
		//执行checkbox响应代码
		bool bcheck = 1;
		if(m_ListSpecFromLib.GetCheck(nItem))
		{
			bcheck = 0;
		}
		else
		{
			if(m_MapLibSpecIDComponent.find(m_ArraySpecInfoFromLib[nItem].m_SpecID) == m_MapLibSpecIDComponent.end())
			{
		        CMatrix tmpData(1,869);
				CString2Matrix(tmpData,m_ArraySpecInfoFromLib[nItem].m_SpecData,L",");
				CMatrix tmp_Component = tmpData*m_Weight;
				tmp_Component += m_Offset;
				m_MapLibSpecIDComponent.insert(make_pair(m_ArraySpecInfoFromLib[nItem].m_SpecID,tmp_Component));
			}
		}
		DrawChart(nItem,bcheck,1,false);
	}
}


void CMainComponentsAnaDlg::OnNMDblclkListLibsamplesetting(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	DWORD dwPos = GetMessagePos();  
	CPoint point( LOWORD(dwPos), HIWORD(dwPos) );  
	m_ListSpecFromLib.ScreenToClient(&point);  
	LVHITTESTINFO lvinfo;  
	lvinfo.pt = point;  
	lvinfo.flags = LVHT_ABOVE;
	UINT nFlag;  
	int nItem = m_ListSpecFromLib.HitTest(point, &nFlag);  
	*pResult = 0;
	if(nFlag == LVHT_ONITEMSTATEICON)  
	{
		//执行checkbox响应代码
		bool bcheck = 1;
		if(m_ListSpecFromLib.GetCheck(nItem))
		{
			bcheck = 0;
		}
		else
		{
			if(m_MapLibSpecIDComponent.find(m_ArraySpecInfoFromLib[nItem].m_SpecID) == m_MapLibSpecIDComponent.end())
			{
				CMatrix tmpData(1,869);
				CString2Matrix(tmpData,m_ArraySpecInfoFromLib[nItem].m_SpecData,L",");
				CMatrix tmp_Component = tmpData*m_Weight;
				tmp_Component += m_Offset;
				m_MapLibSpecIDComponent.insert(make_pair(m_ArraySpecInfoFromLib[nItem].m_SpecID,tmp_Component));
			}
			DrawChart(nItem,bcheck,1,false);
		}
	}
}

void CMainComponentsAnaDlg::OnNMClickListComponents(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	DWORD dwPos = GetMessagePos();  
	CPoint point( LOWORD(dwPos), HIWORD(dwPos) );  

	m_ListCtrlAnaComponents.ScreenToClient(&point);  

	LVHITTESTINFO lvinfo;  
	lvinfo.pt = point;  
	lvinfo.flags = LVHT_ABOVE;  

	UINT nFlag;  
	int nItem = m_ListCtrlAnaComponents.HitTest(point, &nFlag);  
	//判断是否点在checkbox上  
	if(nFlag == LVHT_ONITEMSTATEICON)  
	{
		bool bcheck = 1;
		if(m_ListCtrlAnaComponents.GetCheck(nItem))
		{
			bcheck = 0;
		}
		DrawChart(nItem,bcheck,0);
	}	
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
}



void CMainComponentsAnaDlg::OnNMDblclkListComponents(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	DWORD dwPos = GetMessagePos();  
	CPoint point( LOWORD(dwPos), HIWORD(dwPos) );  

	m_ListCtrlAnaComponents.ScreenToClient(&point);  

	LVHITTESTINFO lvinfo;  
	lvinfo.pt = point;  
	lvinfo.flags = LVHT_ABOVE;  

	UINT nFlag;  
	int nItem = m_ListCtrlAnaComponents.HitTest(point, &nFlag);  
	//判断是否点在checkbox上  
	if(nFlag == LVHT_ONITEMSTATEICON)  
	{
		bool bcheck = 1;
		if(m_ListCtrlAnaComponents.GetCheck(nItem))
		{
			bcheck = 0;
		}
		DrawChart(nItem,bcheck,0);
	}	
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
}


void CMainComponentsAnaDlg::OnPrint()
{
	// TODO: 在此添加命令处理程序代码
	HANDLE hDib; 
	HPALETTE hPalette;
	m_MsChart.GetPlot().GetBackdrop()
		.GetFill().GetBrush().GetFillColor().Set(255, 255, 255);
	m_MsChart.GetPlot().GetBackdrop().GetShadow().SetStyle(FALSE);//阴影

	m_MsChart.EditCopy(); 
	if(OpenClipboard())//m_mschart.
	{
		hDib = GetClipboardData(CF_DIB);
		hPalette = (HPALETTE)GetClipboardData(CF_PALETTE);
		CloseClipboard();
	}
	else
	{
		return;
	}
	m_MsChart.GetPlot().GetBackdrop()
		.GetFill().GetBrush().GetFillColor().Set(192, 192, 192);
	m_MsChart.GetPlot().GetBackdrop().GetShadow().SetStyle(TRUE);//阴影

	CPrintDialog mypd(FALSE);// 构造打印设置对话框对象
	mypd.GetDefaults();  // 检取缺省打印设备的设备环境
	DEVMODE *dm = mypd.GetDevMode();// 提取缺省打印机的DEVMODE数据
	dm->dmOrientation = 2;
	// dm->dmPrintQuality = 1200;
	PRINTDLG pd;
	{
		pd.lStructSize = sizeof(PRINTDLG);
		pd.Flags = PD_RETURNDC;
		pd.hDC = NULL;
		pd.hwndOwner = NULL;
		pd.hInstance = NULL;
		pd.nMaxPage = 256;
		pd.nMinPage = 1;
		pd.nFromPage = 1;
		pd.nToPage  = 256;
		pd.nCopies = 1;
		pd.hDevMode = dm;
		pd.hDevNames = NULL;
	}
	if(!PrintDlg(&pd)) 
	{
		return; 
	}
	ASSERT(pd.hDC != NULL);

	CDC pDC;
	pDC.Attach(pd.hDC);
	int nHorRes = GetDeviceCaps(pd.hDC, HORZRES);//返回设备的物理显示宽度mm 
	int nVerRes = GetDeviceCaps(pd.hDC, VERTRES);//返回设备的物理显示高度mm

	if(pd.hDC != NULL)
	{
		DOCINFO di;   
		di.cbSize = sizeof(DOCINFO);  
		di.lpszDocName = L"图表打印";
		di.lpszOutput = (LPTSTR)NULL;  
		di.lpszDatatype = (LPTSTR)NULL; 
		di.fwType = 0;
		pDC.StartDoc(&di); //通知打印机驱动程序执行一新的打印任务  
		pDC.StartPage();   //通知打印机驱动程序打印新页  
		SetMapMode(pd.hDC, MM_TEXT);//设置当前影射模式为：单位0.001英寸
		CRect rectPrint(0, 0, nHorRes, nVerRes);  

//打印图形
		BITMAPINFO* pbi = (BITMAPINFO*)GlobalLock(hDib);
		if (pbi != NULL) 
		{
			BYTE* Data = (BYTE*)(pbi->bmiColors);
			if (pbi->bmiHeader.biBitCount <= 8) 
			{
				int nColor = pbi->bmiHeader.biClrUsed
					? pbi->bmiHeader.biClrUsed : (1<<pbi->bmiHeader.biBitCount);
				Data += sizeof(RGBQUAD) *nColor;
			}
			if (hPalette != NULL) 
				SelectPalette(pd.hDC, hPalette, TRUE);
			StretchDIBits(pd.hDC, rectPrint.left, rectPrint.top, rectPrint.Width(),
				rectPrint.Height(), 0, 0, pbi->bmiHeader.biWidth, 
				pbi->bmiHeader.biHeight, Data, pbi, DIB_RGB_COLORS, SRCCOPY);
			GlobalUnlock(hDib);
		}

		EndPage(pd.hDC); //通知打印机驱动程序页结束  
		EndDoc(pd.hDC); 
		pDC.DeleteDC();
	}
}


void CMainComponentsAnaDlg::OnPrintpreview()
{
	// TODO: 在此添加命令处理程序代码
	if(m_pPrintFrame)
	{
		m_pPrintFrame->DestroyWindow();
		m_pPrintFrame = NULL;
	}
	SetLandscapeMode(2);
	CPrintFrame* pf=new CPrintFrame(this);
}


afx_msg LRESULT CMainComponentsAnaDlg::OnMyPrint(WPARAM wParam, LPARAM lParam)
{
	CDC *pDC			= (CDC*)wParam;
	CPrintInfo* pInfo	= (CPrintInfo *)lParam;

	HANDLE hDib; 
	HPALETTE hPalette;
	m_MsChart.GetPlot().GetBackdrop()
		.GetFill().GetBrush().GetFillColor().Set(255, 255, 255);
	m_MsChart.GetPlot().GetBackdrop().GetShadow().SetStyle(FALSE);//阴影

	m_MsChart.EditCopy(); 
	if(OpenClipboard())//m_mschart.
	{
		hDib = GetClipboardData(CF_DIB);
		hPalette = (HPALETTE)GetClipboardData(CF_PALETTE);
		CloseClipboard();
	}
	else
	{
		return -1;
	}
	m_MsChart.GetPlot().GetBackdrop()
		.GetFill().GetBrush().GetFillColor().Set(192, 192, 192);
	m_MsChart.GetPlot().GetBackdrop().GetShadow().SetStyle(TRUE);//阴影

	//打印图形
	BITMAPINFO* pbi = (BITMAPINFO*)GlobalLock(hDib);
	if (pbi != NULL) 
	{
		BYTE* Data = (BYTE*)(pbi->bmiColors);
		if (pbi->bmiHeader.biBitCount <= 8) 
		{
			int nColor = pbi->bmiHeader.biClrUsed
				? pbi->bmiHeader.biClrUsed : (1<<pbi->bmiHeader.biBitCount);
			Data += sizeof(RGBQUAD) *nColor;
		}
		if (hPalette != NULL) 
			SelectPalette(pDC->m_hDC, hPalette, TRUE);

		StretchDIBits(pDC->m_hDC, 0, 0, pInfo->m_rectDraw.Width(),
			pInfo->m_rectDraw.Height(), 0, 0, pbi->bmiHeader.biWidth, 
			pbi->bmiHeader.biHeight, Data, pbi, DIB_RGB_COLORS, SRCCOPY);
		GlobalUnlock(hDib);
	}
	return 0;
}


void CMainComponentsAnaDlg::OnAnacomponentsetting()
{
	// TODO: 在此添加命令处理程序代码
	//m_bShowAnaComSetting = !m_bShowAnaComSetting;
	Invalidate();
}


void CMainComponentsAnaDlg::OnUpdateAnacomponentsetting(CCmdUI *pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	m_bShowAnaComSetting = !m_bShowAnaComSetting;
	if(m_bShowAnaComSetting)
	{
		m_Menu.CheckMenuItem(ID_ANACOMPONENTSETTING,MF_CHECKED);
	}
	else
	{
		m_Menu.CheckMenuItem(ID_ANACOMPONENTSETTING,MF_UNCHECKED);
	}
}


void CMainComponentsAnaDlg::OnImportspectrum()
{
	// TODO: 在此添加命令处理程序代码
	OnBnClickedButtonImportspecfromlib();
}


void CMainComponentsAnaDlg::OnBnClickedButtonImportspecfromlib()
{
	// TODO: 在此添加控件通知处理程序代码
	CSelectSpecFromLibDlg SelectSpecFromLibDlg;
	if(IDOK == SelectSpecFromLibDlg.DoModal())
	{
		if(SelectSpecFromLibDlg.m_SelPage == 0)
		{
			if(SelectSpecFromLibDlg.m_SelectSpecFromTrueLibDlg.m_TrueWineLib.IsOpen())
			{		
				while(!SelectSpecFromLibDlg.m_SelectSpecFromTrueLibDlg.m_TrueWineLib.IsEOF())
				{
					SpecInfo tempSpecInfo;
					tempSpecInfo.m_SpecData=SelectSpecFromLibDlg.m_SelectSpecFromTrueLibDlg.m_TrueWineLib.m_SpectrumData;
					tempSpecInfo.m_WaveNumber=SelectSpecFromLibDlg.m_SelectSpecFromTrueLibDlg.m_TrueWineLib.m_WaveNumber;
					tempSpecInfo.m_SpecID=SelectSpecFromLibDlg.m_SelectSpecFromTrueLibDlg.m_TrueWineLib.m_SpecID;
					tempSpecInfo.m_WineName=SelectSpecFromLibDlg.m_SelectSpecFromTrueLibDlg.m_TrueWineLib.m_WineName;
					tempSpecInfo.m_ComFrom=SelectSpecFromLibDlg.m_SelectSpecFromTrueLibDlg.m_TrueWineLib.m_ComFrom;
					tempSpecInfo.m_ImportDate=SelectSpecFromLibDlg.m_SelectSpecFromTrueLibDlg.m_TrueWineLib.m_ImportDate;
					tempSpecInfo.m_Manager=SelectSpecFromLibDlg.m_SelectSpecFromTrueLibDlg.m_TrueWineLib.m_Manager;

					m_ArraySpecInfoFromLib.Add(tempSpecInfo);
					SelectSpecFromLibDlg.m_SelectSpecFromTrueLibDlg.m_TrueWineLib.MoveNext();
				}
				UpDateCListCtrlEx();		
			}
		}
		else
		{
			if(SelectSpecFromLibDlg.m_SelectSpecFromFakeLibDlg.m_FakeWineLib.IsOpen())
			{		
				while(!SelectSpecFromLibDlg.m_SelectSpecFromFakeLibDlg.m_FakeWineLib.IsEOF())
				{
					SpecInfo tempSpecInfo;
					tempSpecInfo.m_SpecData=SelectSpecFromLibDlg.m_SelectSpecFromFakeLibDlg.m_FakeWineLib.m_SpectrumData;
					tempSpecInfo.m_WaveNumber=SelectSpecFromLibDlg.m_SelectSpecFromFakeLibDlg.m_FakeWineLib.m_WaveNumber;
					tempSpecInfo.m_SpecID=SelectSpecFromLibDlg.m_SelectSpecFromFakeLibDlg.m_FakeWineLib.m_SpectrumID;
					tempSpecInfo.m_WineName=SelectSpecFromLibDlg.m_SelectSpecFromFakeLibDlg.m_FakeWineLib.m_WineNameFaked;
					tempSpecInfo.m_ComFrom=SelectSpecFromLibDlg.m_SelectSpecFromFakeLibDlg.m_FakeWineLib.m_Comfrom;
					tempSpecInfo.m_ImportDate=SelectSpecFromLibDlg.m_SelectSpecFromFakeLibDlg.m_FakeWineLib.m_ImportDate;
					tempSpecInfo.m_Manager=SelectSpecFromLibDlg.m_SelectSpecFromFakeLibDlg.m_FakeWineLib.m_Manager;

					m_ArraySpecInfoFromLib.Add(tempSpecInfo);
					SelectSpecFromLibDlg.m_SelectSpecFromFakeLibDlg.m_FakeWineLib.MoveNext();
				}
				UpDateCListCtrlEx();		
			}
		}
		return ;
	}
}

int CMainComponentsAnaDlg::UpDateCListCtrlEx(void)
{
	//m_ListSpecFromLib.SetRedraw(FALSE);
	m_ListSpecFromLib.DeleteAllItems();
	for(int i=0;i<m_ArraySpecInfoFromLib.GetCount();i++)
	{
		CString tempNum;
		tempNum.Format(L"%d",i+1);

		m_ListSpecFromLib.InsertItem(i,tempNum);
		m_ListSpecFromLib.SetItemText(i,1,m_ArraySpecInfoFromLib[i].m_SpecID);
		m_ListSpecFromLib.SetItemText(i,2,m_ArraySpecInfoFromLib[i].m_WineName);
		m_ListSpecFromLib.SetItemText(i,3,m_ArraySpecInfoFromLib[i].m_ComFrom);
		m_ListSpecFromLib.SetItemText(i,4,m_ArraySpecInfoFromLib[i].m_ImportDate);
		m_ListSpecFromLib.SetItemText(i,5,m_ArraySpecInfoFromLib[i].m_Manager);
	}
	//m_ListSpecFromLib.SetRedraw(TRUE);
	m_ListSpecFromLib.UpdateWindow();
	return 0;
}

void CMainComponentsAnaDlg::OnBnClickedButtonClearlist()
{
	// TODO: 在此添加控件通知处理程序代码
	m_ListSpecFromLib.DeleteAllItems();
	m_MapLibSpecIDComponent.clear();
	m_MsChart.Refresh();
}
namespace MainComponentsAnaDlg
{
	UINT ThreadLoadSpecData(LPVOID lpParam)
	{
		CMainComponentsAnaDlg *pCMainComponentsAnaDlg = (CMainComponentsAnaDlg*) lpParam;
		
		if(pCMainComponentsAnaDlg->isPCA)
		{
			pCMainComponentsAnaDlg->VectorCurveName.clear();
			pCMainComponentsAnaDlg->m_MatrixSpecData.~CMatrix();
			for(int i=0;i<pCMainComponentsAnaDlg->m_ArraySpecPath.GetCount();++i)
			{
				CCSVOperator CSVOperator;
				CMatrix tempRow(1,SPEC_LENTH);
				if(CSVOperator.LoadCSV(pCMainComponentsAnaDlg->m_ArraySpecPath[i]))
				{
					double temp;
					for(int j=0;j<SPEC_LENTH;++j)
					{
						CSVOperator.GetDouble(j+1,2,temp);
						tempRow.SetItem(1,j+1,temp);
					}
				}
				pCMainComponentsAnaDlg->VectorCurveName.push_back(pCMainComponentsAnaDlg->m_ArraySpecName[i]);
				//保存以便下次使用不再重新导入
				pCMainComponentsAnaDlg->m_MatrixSpecData.AddRows(tempRow);
				double percent_double = ((double)i+1)/((double)pCMainComponentsAnaDlg->m_ArraySpecPath.GetCount())*100;
				::SendMessage(pCMainComponentsAnaDlg->m_pCImportProjectDataProgressDlg->m_hWnd,WM_PCAIMPORTDATA,percent_double-1,0);
			}
		}
		else
		{
			pCMainComponentsAnaDlg->FakeDataset.~CMatrix();
			CFakeWineLib FakeWineLib;
			FakeWineLib.m_strSort = L"WineNameFaked";
			if(!FakeWineLib.Open())
			{
				MessageBox(pCMainComponentsAnaDlg->m_hWnd,L"连接假酒库失败！",L"假酒聚类",MB_ICONERROR);
				return -1;
			}
			if(FakeWineLib.GetRecordCount() == 0)
			{
				MessageBox(pCMainComponentsAnaDlg->m_hWnd,L"假酒库无记录！",L"假酒聚类",MB_ICONWARNING);
				return -1;
			}
			int totalSpecNum = 0;
			FakeWineLib.MoveFirst();
			while(!FakeWineLib.IsEOF())
			{
				++totalSpecNum;
				FakeWineLib.MoveNext();
			}

			int i=0;
			int j=0;
			if(pCMainComponentsAnaDlg->m_MatrixSpecData.m_row != pCMainComponentsAnaDlg->m_ArraySpecPath.GetCount())
			{
				totalSpecNum += pCMainComponentsAnaDlg->m_ArraySpecPath.GetCount();
			}
			
			FakeWineLib.MoveFirst();
			for(;!FakeWineLib.IsEOF();++j,FakeWineLib.MoveNext())
			{
				CMatrix temp(1,SPEC_LENTH);
				CString2Matrix(temp,FakeWineLib.m_SpectrumData,L",");
				pCMainComponentsAnaDlg->FakeDataset.AddRows(temp);

				pCMainComponentsAnaDlg->m_ArraySpectrumID.Add(FakeWineLib.m_SpectrumID);
				pCMainComponentsAnaDlg->m_ArrayWineNameFaked.Add(FakeWineLib.m_WineNameFaked);
				pCMainComponentsAnaDlg->m_ArrayComfrom.Add(FakeWineLib.m_Comfrom);
				pCMainComponentsAnaDlg->m_ArrayImportDate.Add(FakeWineLib.m_ImportDate);
				pCMainComponentsAnaDlg->m_ArrayManager.Add(FakeWineLib.m_Manager);

				double percent_double = ((double)(i+j+1))/((double)totalSpecNum)*100;
				::SendMessage(pCMainComponentsAnaDlg->m_pCImportProjectDataProgressDlg->m_hWnd,WM_PCAIMPORTDATA,percent_double-1,0);
			}

			FakeWineLib.Close();
			//取得待分析光谱数据
			if(pCMainComponentsAnaDlg->m_MatrixSpecData.m_row != pCMainComponentsAnaDlg->m_ArraySpecPath.GetCount())
			{
				pCMainComponentsAnaDlg->VectorCurveName.clear();
				pCMainComponentsAnaDlg->m_MatrixSpecData.~CMatrix();
				for(;i<pCMainComponentsAnaDlg->m_ArraySpecPath.GetCount();++i)
				{
					CCSVOperator CSVOperator;
					CMatrix tempRow(1,SPEC_LENTH);
					if(CSVOperator.LoadCSV(pCMainComponentsAnaDlg->m_ArraySpecPath[i]))
					{
						double temp;
						for(int k=0;k<SPEC_LENTH;++k)
						{
							CSVOperator.GetDouble(k+1,2,temp);
							tempRow.SetItem(1,k+1,temp);
						}
					}
					pCMainComponentsAnaDlg->VectorCurveName.push_back(pCMainComponentsAnaDlg->m_ArraySpecName[i]);
					//保存以便下次使用不再重新导入
					pCMainComponentsAnaDlg->m_MatrixSpecData.AddRows(tempRow);
					double percent_double = ((double)(i+j+1))/((double)totalSpecNum)*100;
					::SendMessage(pCMainComponentsAnaDlg->m_pCImportProjectDataProgressDlg->m_hWnd,WM_PCAIMPORTDATA,percent_double-1,0);
				}	
			}
		}	
		::PostMessage(pCMainComponentsAnaDlg->m_hWnd,WM_ENDIMPORTDATA,0,0);
		return 0;
	}
	UINT ThreadDoCluster(LPVOID lpParam)
	{
		CMainComponentsAnaDlg *pCMainComponentsAnaDlg = (CMainComponentsAnaDlg*) lpParam;
		CClusterDlg &m_ClusterDlg = pCMainComponentsAnaDlg->m_ClusterDlg;
		if(m_ClusterDlg.m_RadioCluMethod == 0)
		{
			m_ClusterDlg.m_ClusterAnalysis.Init(m_ClusterDlg.m_TotalDataSet,m_ClusterDlg.m_eps,m_ClusterDlg.m_Minpts);
			m_ClusterDlg.m_ClusterAnalysis.DoDBSCANRecursive();
		}
		else
		{
			m_ClusterDlg.m_ClusterAnalysis.Init(m_ClusterDlg.m_TotalDataSet,m_ClusterDlg.m_ClusterNum,200);    //初始化操作
			m_ClusterDlg.m_ClusterAnalysis.DoKmeans();
		}
		::PostMessage(pCMainComponentsAnaDlg->m_hWnd,WM_ENDCLUSTER,0,0);
		return 0;
	}
}


afx_msg LRESULT CMainComponentsAnaDlg::OnBeginimportdata(WPARAM wParam, LPARAM lParam)
{
	this->m_pCImportProjectDataProgressDlg = new CImportProjectDataProgressDlg;
	m_pCImportProjectDataProgressDlg->Create(IDD_DIALOG_IMPORTDATA_PROGRESSINFO,this);
	m_pCImportProjectDataProgressDlg->ShowWindow(SW_SHOW);
	::SendMessage(m_pCImportProjectDataProgressDlg->m_hWnd,WM_PCAIMPORTDATA,0,0);

	AfxBeginThread(MainComponentsAnaDlg::ThreadLoadSpecData,this,0,0,0);
	return 0;
}


afx_msg LRESULT CMainComponentsAnaDlg::OnEndimportdata(WPARAM wParam, LPARAM lParam)
{
	if(m_pCImportProjectDataProgressDlg)
	{
		m_pCImportProjectDataProgressDlg->DestroyWindow();
		m_pCImportProjectDataProgressDlg = NULL;
	}
	if(isPCA)
	{
		_PCAanalasis();
	}
	else
	{
		_Fakewineclustering();
	}
	return 0;
}

//PCA
void CMainComponentsAnaDlg::_PCAanalasis()
{
	TotalSpecData.AddRows(m_MatrixSpecData);

	CMatrix Offset;
	for(int i = 0; i < TotalSpecData.m_row;++i)
	{
		Offset.AddRows(m_Offset);
	}
	CMatrix TotalSpecComponentConcentration = TotalSpecData*m_Weight+Offset;

	//计算PCA
	CMatrix signals(TotalSpecComponentConcentration.m_row,TotalSpecComponentConcentration.m_column);
	CMatrix PC(TotalSpecComponentConcentration.m_column,TotalSpecComponentConcentration.m_column);
	CMatrix V(1,TotalSpecComponentConcentration.m_column);

	int flag=TotalSpecComponentConcentration.PCA(signals,PC,V);
	//获取主成分个数
	int MaxPCNum = 0;
	while(0!=V(1,MaxPCNum+1))++MaxPCNum;

	//显示
	CPCADlg PCADlg;
	PCADlg.m_PCAData = signals;
	PCADlg.m_MaxPCNum = MaxPCNum;
	PCADlg.m_VectorRefName = WineNameVector;
	PCADlg.m_VectorEachRefNameCount = EachWineCount;
	PCADlg.m_AnaSpecNum = m_ArraySpecPath.GetCount();
	PCADlg.m_xPC = m_xPC;
	PCADlg.m_yPC = m_yPC;
	PCADlg.m_PCNumShow = m_PCNumShow;

	PCADlg.DoModal();

}

void CMainComponentsAnaDlg::OnPca()
{
	// TODO: 在此添加命令处理程序代码
	isPCA = true;
	CPCASettingDlg PCASettingDlg;
	PCASettingDlg.m_InitPCNum = 16;

	if(IDOK == PCASettingDlg.DoModal())
	{	
		TotalSpecData.~CMatrix();
		WineNameVector.clear();
		EachWineCount.clear();	

		int EachNameCount = 0;
		int i = 0;
		if(PCASettingDlg.m_RadioSpecToPCA == 1)
		{
			for(;i<m_ArraySpecInfoFromLib.GetCount();++i)
			{
				CString tmpWinaInfo;
				tmpWinaInfo.Format(L"%s(%s)",m_ArraySpecInfoFromLib[i].m_WineName,m_ArraySpecInfoFromLib[i].m_ComFrom);
				if(0 == i)
				{
					WineNameVector.push_back(tmpWinaInfo);				
				}
				if(tmpWinaInfo!= WineNameVector[WineNameVector.size() - 1])
				{
					EachWineCount.push_back(EachNameCount);
					WineNameVector.push_back(tmpWinaInfo);
					EachNameCount = 0;
				}
				CMatrix tempRow(1,SPEC_LENTH);
				CString2Matrix(tempRow,m_ArraySpecInfoFromLib[i].m_SpecData,L",");
				TotalSpecData.AddRows(tempRow);
				++EachNameCount;
			}
		}
		EachWineCount.push_back(EachNameCount);

		m_xPC = PCASettingDlg.xPC;
		m_yPC = PCASettingDlg.yPC;
		m_PCNumShow = PCASettingDlg.PCNum;

		if(m_MatrixSpecData.m_row != m_ArraySpecPath.GetCount())
		{
			PostMessage(WM_BEGINIMPORTDATA,0,0);
			return;
		}
		_PCAanalasis();
	}
}




void CMainComponentsAnaDlg::OnCluster()
{
	// TODO: 在此添加命令处理程序代码
	isPCA = false;
	TotalDataset.~CMatrix();

	CFakeWineLib FakeWineLib;
	FakeWineLib.m_strSort = L"WineNameFaked";
	if(!FakeWineLib.Open())
	{
		MessageBox(L"连接假酒库失败！",L"假酒聚类",MB_ICONERROR);
		return;
	}
	if(FakeWineLib.GetRecordCount() == 0)
	{
		MessageBox(L"假酒库无记录！",L"假酒聚类",MB_ICONWARNING);
		return;
	}
	CClusterSettingDlg ClusterSettingDlg;
	if(IDOK == ClusterSettingDlg.DoModal())
	{
		m_RadioCluMethod = ClusterSettingDlg.m_RadioCluMethod;
		m_eps = ClusterSettingDlg.m_eps;
		m_Minpts = ClusterSettingDlg.m_Minpts;
		m_ClusterNum = ClusterSettingDlg.m_ClusterNum;

		//取得待分析光谱数据
		
		if(m_MatrixSpecData.m_row != m_ArraySpecPath.GetCount()||FakeDataset.m_row == 0)
		{
			PostMessage(WM_BEGINIMPORTDATA,0,0);
			return;
		}
		else
		{
			_Fakewineclustering();
		}	
		
	}
}

void CMainComponentsAnaDlg::_Fakewineclustering(void)
{	
	TotalDataset.AddRows(FakeDataset);
	TotalDataset.AddRows(m_MatrixSpecData);
	//数据预处理

	CMatrix Offset;
	for(int i = 0; i < TotalDataset.m_row;++i)
	{
		Offset.AddRows(m_Offset);
	}
	CMatrix TotalSpecComponentConcentration = TotalDataset*m_Weight+Offset;

	//计算PCA
	CMatrix signals(TotalSpecComponentConcentration.m_row,TotalSpecComponentConcentration.m_column);
	CMatrix PC(TotalSpecComponentConcentration.m_column,TotalSpecComponentConcentration.m_column);
	CMatrix V(1,TotalSpecComponentConcentration.m_column);

	int flag=TotalSpecComponentConcentration.PCA(signals,PC,V);

	signals = signals.Transposition().GetRows(1,2).Transposition();

	m_ClusterDlg.m_ArraySpectrumID.Copy(m_ArraySpectrumID);
	m_ClusterDlg.m_ArrayWineNameFaked.Copy(m_ArrayWineNameFaked);
	m_ClusterDlg.m_ArrayComfrom.Copy(m_ArrayComfrom);
	m_ClusterDlg.m_ArrayImportDate.Copy(m_ArrayImportDate);
	m_ClusterDlg.m_ArrayManager.Copy(m_ArrayManager);

	m_ClusterDlg.m_VectorCurveName = VectorCurveName;
	m_ClusterDlg.m_AnaDataSet = m_MatrixSpecData;
	m_ClusterDlg.m_FakeDataset = FakeDataset;
	m_ClusterDlg.m_TotalDataSet = signals;

	m_ClusterDlg.m_RadioCluMethod = m_RadioCluMethod; //聚类类别
	m_ClusterDlg.m_eps = m_eps;         //扫描半径
	m_ClusterDlg.m_Minpts = m_Minpts;         //最小点数
	m_ClusterDlg.m_ClusterNum = m_ClusterNum;     //聚类类别
	PostMessage(WM_BEGINCLUSTER,0,0);
}

afx_msg LRESULT CMainComponentsAnaDlg::OnBegincluster(WPARAM wParam, LPARAM lParam)
{
	this->m_pShowInfoDlg = new CShowInfoDlg;
	m_pShowInfoDlg->Create(IDD_DIALOG_SHOWINFO,this);
	m_pShowInfoDlg->ShowWindow(SW_SHOW);
	m_pShowInfoDlg->m_StaticInfoShow = L"正在进行假酒窝点聚类，请耐心等待";
	m_pShowInfoDlg->SetWindowText(L"假酒窝点聚类");
	m_pShowInfoDlg->UpdateData(FALSE);
	AfxBeginThread(MainComponentsAnaDlg::ThreadDoCluster,this,0,0,0);
	return 0;
}


afx_msg LRESULT CMainComponentsAnaDlg::OnEndcluster(WPARAM wParam, LPARAM lParam)
{
	m_pShowInfoDlg->DestroyWindow();
	m_pShowInfoDlg = 0;
	m_ClusterDlg.DoModal();
	return 0;
}
