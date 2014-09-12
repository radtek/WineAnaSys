// SpectrumCompareDlg.cpp : 实现文件
#include "stdafx.h"
#include "DataServerCenter.h"
#include "SpectrumCompareDlg.h"
#include "afxdialogex.h"
#include "ClusterDlg.h"
#include "WaveletAnalasis.h"
#include "SpecPretrSettingDlg.h"
#include "DiffSettingDlg.h"
#include "SmoothSettingDlg.h"
#include "WdenSettingDlg.h"
#include "ClusterSettingDlg.h"
#include "PrintFrame.h"
#include "SkinPPWTL.h"
using std::set;
// CSpectrumCompareDlg 对话框
bool LoadSkin = 0;
IMPLEMENT_DYNAMIC(CSpectrumCompareDlg, CDialogEx)

CSpectrumCompareDlg::CSpectrumCompareDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CSpectrumCompareDlg::IDD, pParent),
	m_pPrintFrame(NULL)
{
	CWinApp* pApp=AfxGetApp();
	m_pMFrame = pApp->m_pMainWnd;

	m_LineWidth=2;
	m_LineColor=RGB(255,0,0);
	m_crBackColor=RGB(192,192,192);
	m_MaxChart=0;
	m_ShowTransAbs =true;

	m_TTppe = L"";
	m_TScal = L"";
	m_TSORH = L"";

	m_IsDenoiseChecked = false; //降噪复选框是否选中
	m_IsCorrChecked = false;   //校正复选框是否选中
	m_RadioDenoise = -1;  //降噪
	m_RadioCorrelation = -1;//校正	

	m_AnaPts = 0;
	m_SmoothPtsIndex = -1; //平滑点数索引
	m_Wname1Index= -1; //小波名1索引
	m_Wname2Index= -1;  //小波名2索引
	m_DecLevelIndex = -1; //分解层数索引
	m_DerivativeOrderIndex = -1; //导数阶数索引
	m_TScalIndex = -1;
	m_TTppeIndex = -1;
	m_TSORHIndex = -1;
}

CSpectrumCompareDlg::~CSpectrumCompareDlg()
{

}

void CSpectrumCompareDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_SPECTOCOMPARE, m_ListSelectedProject);
	DDX_Control(pDX, IDC_LIST_SPECINLIB, m_ListSpecFromLib);
	DDX_Control(pDX, IDC_BUTTON_SELECTSPECINLIB, m_btnImportSpec);
	DDX_Control(pDX, IDC_CHART, m_ChartSpectrumCompare);
	DDX_Control(pDX, IDC_COMBO_SELECTEDSPECNAME, m_ComboSelectedSpecName);
	DDX_Control(pDX, IDC_BUTTON_CLEAR, m_btnClear);
	DDX_Control(pDX, IDC_BUTTON_PLOT, m_btnPlot);
	DDX_Control(pDX, IDC_BUTTON_TRANS, m_btnTrans);
	DDX_Control(pDX, IDC_BUTTON_ABS, m_btnAbs);
	DDX_Control(pDX, IDC_BUTTON_SELECT, m_btnSelect);
	DDX_Control(pDX, IDC_BUTTON_MOVE, m_btnMove);
	DDX_Control(pDX, IDC_BUTTON_ZOOMIN, m_btnZommin);
	DDX_Control(pDX, IDC_BUTTON_ZOOMOUT, m_btnZoomout);
	DDX_Control(pDX, IDC_BUTTON_LABEL, m_btnLabel);
}


BEGIN_MESSAGE_MAP(CSpectrumCompareDlg, CDialogEx)
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_BUTTON_SELECTSPECINLIB, &CSpectrumCompareDlg::OnBnClickedButtonSelectspecinlib)
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONDOWN()
	ON_WM_NCLBUTTONDOWN()
	ON_NOTIFY(NM_CLICK, IDC_LIST_SPECTOCOMPARE, &CSpectrumCompareDlg::OnNMClickListSpectocompare)
	ON_NOTIFY(NM_CLICK, IDC_LIST_SPECINLIB, &CSpectrumCompareDlg::OnNMClickListSpecinlib)
	ON_WM_DESTROY()
	ON_WM_PAINT()
	ON_CBN_SELCHANGE(IDC_COMBO_SELECTEDSPECNAME, &CSpectrumCompareDlg::OnCbnSelchangeComboSelectedspecname)
	ON_MESSAGE(WM_CHANGESELECTEDCURVE, &CSpectrumCompareDlg::OnChangeselectedcurve)
	ON_UPDATE_COMMAND_UI(ID_MAXCHARTSHOW, &CSpectrumCompareDlg::OnUpdateMaxchartshow)
	ON_COMMAND(ID_MAXCHARTSHOW, &CSpectrumCompareDlg::OnMaxchartshow)
	ON_COMMAND(ID_SPECMINUS, &CSpectrumCompareDlg::OnSpecminus)
	ON_COMMAND(ID_CHANGETOABS, &CSpectrumCompareDlg::OnChangetoabs)
	ON_COMMAND(ID_CHANGETOTRANS, &CSpectrumCompareDlg::OnChangetotrans)
	ON_COMMAND(ID_DELETESELECTEDCURVE, &CSpectrumCompareDlg::OnDeleteselectedcurve)
	ON_COMMAND(ID_DELETEALLCURVE, &CSpectrumCompareDlg::OnDeleteallcurve)
	ON_COMMAND(ID_SMOOTHSPEC, &CSpectrumCompareDlg::OnSmoothspec)
	ON_COMMAND(ID_SHOWNOISE, &CSpectrumCompareDlg::OnShownoise)
	ON_COMMAND(ID_SPECMEAN, &CSpectrumCompareDlg::OnSpecmean)
	ON_COMMAND(ID_SPECMOVE, &CSpectrumCompareDlg::OnSpecmove)
	ON_COMMAND(ID_SPECSELECT, &CSpectrumCompareDlg::OnSpecselect)
	ON_COMMAND(ID_SPECZOOMIN, &CSpectrumCompareDlg::OnSpeczoomin)
	ON_COMMAND(ID_SPECZOOMOUT, &CSpectrumCompareDlg::OnSpeczoomout)
	ON_COMMAND(ID_PEAKAREA, &CSpectrumCompareDlg::OnPeakarea)
	ON_COMMAND(ID_LABEL, &CSpectrumCompareDlg::OnLabel)
	ON_COMMAND(ID_DELETELABEL, &CSpectrumCompareDlg::OnDeletelabel)
	ON_COMMAND(ID_PCAANALASIS, &CSpectrumCompareDlg::OnPcaanalasis)
	ON_MESSAGE(WM_CHANGECHARTMODE, &CSpectrumCompareDlg::OnChangechartmode)
	ON_COMMAND(ID_FAKEWINECLUSTERING, &CSpectrumCompareDlg::OnFakewineclustering)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_SPECTOCOMPARE, &CSpectrumCompareDlg::OnNMDblclkListSpectocompare)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_SPECINLIB, &CSpectrumCompareDlg::OnNMDblclkListSpecinlib)
	ON_COMMAND(ID_PLOT, &CSpectrumCompareDlg::OnPlot)
	ON_COMMAND(ID_SPECANAPRETRSETTING, &CSpectrumCompareDlg::OnSpecanapretrsetting)
	ON_COMMAND(ID_WDEN, &CSpectrumCompareDlg::OnWden)
	ON_COMMAND(ID_DIFF, &CSpectrumCompareDlg::OnDiff)
	ON_BN_CLICKED(IDC_BUTTON_CLEAR, &CSpectrumCompareDlg::OnBnClickedButtonClear)
	ON_COMMAND(ID_PRINTPREVIEW, &CSpectrumCompareDlg::OnPrintpreview)
	ON_COMMAND(ID_PRINT, &CSpectrumCompareDlg::OnPrint)
	ON_MESSAGE(WM_MY_PRINT, &CSpectrumCompareDlg::OnMyPrint)
	ON_MESSAGE(WM_BEGIN_PRINTING, &CSpectrumCompareDlg::OnBeginPrinting)
	ON_MESSAGE(WM_END_PRINTING, &CSpectrumCompareDlg::OnEndPrinting)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BUTTON_PLOT, &CSpectrumCompareDlg::OnBnClickedButtonPlot)
	ON_BN_CLICKED(IDC_BUTTON_TRANS, &CSpectrumCompareDlg::OnBnClickedButtonTrans)
	ON_BN_CLICKED(IDC_BUTTON_ABS, &CSpectrumCompareDlg::OnBnClickedButtonAbs)
	ON_BN_CLICKED(IDC_BUTTON_SELECT, &CSpectrumCompareDlg::OnBnClickedButtonSelect)
	ON_BN_CLICKED(IDC_BUTTON_MOVE, &CSpectrumCompareDlg::OnBnClickedButtonMove)
	ON_BN_CLICKED(IDC_BUTTON_ZOOMIN, &CSpectrumCompareDlg::OnBnClickedButtonZoomin)
	ON_BN_CLICKED(IDC_BUTTON_ZOOMOUT, &CSpectrumCompareDlg::OnBnClickedButtonZoomout)
	ON_BN_CLICKED(IDC_BUTTON_LABEL, &CSpectrumCompareDlg::OnBnClickedButtonLabel)
END_MESSAGE_MAP()


// CSpectrumCompareDlg 消息处理程序


BOOL CSpectrumCompareDlg::OnInitDialog()
{
	skinppRemoveSkin(); //移除皮肤	
	CDialogEx::OnInitDialog();
	// TODO:  在此添加额外的初始化
	//添加菜单
	m_Menu.LoadMenu(IDR_MENU_SPECCOMPARE); 
    SetMenu(&m_Menu);

	//列表初始化
	m_ListSelectedProject.InsertColumn(0,L"项目名称",LVCFMT_LEFT,100);
	m_ListSelectedProject.InsertColumn(1,L"光谱文件",LVCFMT_LEFT,100);
	m_ListSelectedProject.InsertColumn(2,L"鉴定结果",LVCFMT_LEFT,100);


	//m_ListSpecFromLib.InsertColumn(0,L"",LVCFMT_LEFT,150);

	DWORD dwStyle = m_ListSelectedProject.GetExtendedStyle(); //获取当前扩展样式
	dwStyle |= LVS_EX_FULLROWSELECT; //选中某行使整行高亮（report风格时）
	dwStyle |= LVS_EX_GRIDLINES; //网格线（report风格时）
	//dwStyle |= LVS_EX_FULLROWSELECT; //
	m_ListSelectedProject.SetExtendedStyle(dwStyle); //设置扩展风格
	
	m_ListSpecFromLib.InsertColumn( 0, L"", LVCFMT_LEFT, 50 );//插入列 
	m_ListSpecFromLib.InsertColumn( 1, L"光谱编号", LVCFMT_LEFT, 100 );//插入列 
	m_ListSpecFromLib.InsertColumn( 2, L"白酒名称", LVCFMT_LEFT, 100 );//插入列 
	m_ListSpecFromLib.InsertColumn( 3, L"白酒信息", LVCFMT_LEFT, 150 );//插入列 
	m_ListSpecFromLib.InsertColumn( 4, L"生产日期", LVCFMT_LEFT, 100 );//插入列
	m_ListSpecFromLib.InsertColumn( 5, L"生产批号", LVCFMT_LEFT, 100 );//插入列
	m_ListSpecFromLib.InsertColumn( 6, L"来源说明", LVCFMT_LEFT, 100 );//插入列
	m_ListSpecFromLib.InsertColumn( 7, L"入库日期", LVCFMT_LEFT, 100 );//插入列
	m_ListSpecFromLib.InsertColumn( 8, L"操作人员", LVCFMT_LEFT, 100 );//插入列


	dwStyle = m_ListSpecFromLib.GetExtendedStyle(); //获取当前扩展样式
	dwStyle |= LVS_EX_FULLROWSELECT; //选中某行使整行高亮（report风格时）
	dwStyle |= LVS_EX_GRIDLINES; //网格线（report风格时）
	//dwStyle |= LVS_EX_CHECKBOXES; //item前生成checkbox控件
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
	szResultPath+=L"\\鉴定结果.xlsx";

	CExcelOperator ExcelOperator;
	ExcelOperator.LoadExcel(szResultPath);

	//m_ListProPreResultDetail.DeleteAllItems();
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
		m_ListSelectedProject.InsertItem(j,m_projectName);
		m_ListSelectedProject.SetItemText(j,1,specFileTitle);
		m_ListSelectedProject.SetItemText(j,2,PredictResult);
		m_ArraySpecPreResult.Add(PredictResult);

		j++;
		ExcelOperator.GetItem(j+1,1,L"String",specFileName);
		ExcelOperator.GetItem(j+1,2,L"String",PredictResult);
	}

	ExcelOperator.ReleaseExcel();


	m_Menu.CheckMenuItem(ID_CHANGETOTRANS,MF_CHECKED);

	GetClientRect(&m_rect);

	int xSize=::GetSystemMetrics(SM_CXSCREEN);
	int ySize=::GetSystemMetrics(SM_CYSCREEN);
	int cx=xSize*9/10;
	int cy=ySize*9/10;
	this->MoveWindow((xSize-cx)/2,(ySize-cy)/2,xSize*9/10,ySize*9/10);

	//初始化图标按钮
	HICON hIcon=AfxGetApp()->LoadIcon(IDI_ICON_PLOT); 
	m_btnPlot.SetIcon(hIcon); 
	m_btnPlot.DrawTransparent(TRUE);
	m_btnPlot.SetTooltipText(L"绘制图谱");

	hIcon=AfxGetApp()->LoadIcon(IDI_ICON_TRANS); 
	m_btnTrans.SetIcon(hIcon); 
	m_btnTrans.DrawTransparent(TRUE);
	m_btnTrans.SetTooltipText(L"透过率");

	hIcon=AfxGetApp()->LoadIcon(IDI_ICON_ABS); 
	m_btnAbs.SetIcon(hIcon); 
	m_btnAbs.DrawTransparent(TRUE);
	m_btnAbs.SetTooltipText(L"吸光度");

	hIcon=AfxGetApp()->LoadIcon(IDI_ICON_SELECT); 
	m_btnSelect.SetIcon(hIcon); 
	m_btnSelect.DrawTransparent(TRUE);
	m_btnSelect.SetTooltipText(L"选择图谱");

	hIcon=AfxGetApp()->LoadIcon(IDI_ICON_MOVE); 
	m_btnMove.SetIcon(hIcon); 
	m_btnMove.DrawTransparent(TRUE);
	m_btnMove.SetTooltipText(L"移动图谱");

	hIcon=AfxGetApp()->LoadIcon(IDI_ICON_ZOOMIN); 
	m_btnZommin.SetIcon(hIcon); 
	m_btnZommin.DrawTransparent(TRUE);
	m_btnZommin.SetTooltipText(L"鼠标控制放大");

	hIcon=AfxGetApp()->LoadIcon(IDI_ICON_ZOOMOUT); 
	m_btnZoomout.SetIcon(hIcon); 
	m_btnZoomout.DrawTransparent(TRUE);
	m_btnZoomout.SetTooltipText(L"鼠标控制缩小");

	hIcon=AfxGetApp()->LoadIcon(IDI_ICON_LABEL); 
	m_btnLabel.SetIcon(hIcon); 
	m_btnLabel.DrawTransparent(TRUE);
	m_btnLabel.SetTooltipText(L"标注");


	LoadSkin = 1;

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CSpectrumCompareDlg::OnBnClickedButtonSelectspecinlib()
{
	// TODO: 在此添加控件通知处理程序代码

	CSelectSpecFromLibDlg SelectSpecFromLibDlg;
	if(IDOK == SelectSpecFromLibDlg.DoModal())
	{
		if(SelectSpecFromLibDlg.m_TrueWineLib.IsOpen())
		{		
			//int count=m_ArraySpecInfoFromLib.GetCount();
			while(!SelectSpecFromLibDlg.m_TrueWineLib.IsEOF())
			{

				SpecInfo tempSpecInfo;
				tempSpecInfo.m_SpecData=SelectSpecFromLibDlg.m_TrueWineLib.m_SpectrumData;
				tempSpecInfo.m_WaveNumber=SelectSpecFromLibDlg.m_TrueWineLib.m_WaveNumber;
				tempSpecInfo.m_SpecID=SelectSpecFromLibDlg.m_TrueWineLib.m_SpecID;
				tempSpecInfo.m_WineName=SelectSpecFromLibDlg.m_TrueWineLib.m_WineName;
				tempSpecInfo.m_AlcoholContent=SelectSpecFromLibDlg.m_TrueWineLib.m_AlcoholContent;
				tempSpecInfo.m_Flavour=SelectSpecFromLibDlg.m_TrueWineLib.m_Flavour;
				tempSpecInfo.m_Brand=SelectSpecFromLibDlg.m_TrueWineLib.m_Brand;
				tempSpecInfo.m_ProductionBatchNo=SelectSpecFromLibDlg.m_TrueWineLib.m_ProductionBatchNo;
				tempSpecInfo.m_ComFrom=SelectSpecFromLibDlg.m_TrueWineLib.m_ComFrom;
				tempSpecInfo.m_ImportDate=SelectSpecFromLibDlg.m_TrueWineLib.m_ImportDate;
				tempSpecInfo.m_Manager=SelectSpecFromLibDlg.m_TrueWineLib.m_Manager;

				m_ArraySpecInfoFromLib.Add(tempSpecInfo);
				SelectSpecFromLibDlg.m_TrueWineLib.MoveNext();
				//count++;
			}
			UpDateCListCtrl();
			return ;
		}
		else
		{
			return ;
		}
		
	}

}
int CSpectrumCompareDlg::UpDateCListCtrl(void)
{

	//m_pTrueWineLib->MoveFirst();
	//int count=0;
	m_ListSpecFromLib.SetRedraw(FALSE);
	//int 
	m_ListSpecFromLib.DeleteAllItems();
	for(int i=0;i<m_ArraySpecInfoFromLib.GetCount();i++)
	{
		CString tempNum;
		tempNum.Format(L"%d",i+1);
		//m_ListSpecFromLib.SetItemText(i,0,tempNum);
		//m_ListSpecFromLib
		CString temp;
		temp.Format(L"%s,%s,%s",m_ArraySpecInfoFromLib[i].m_AlcoholContent,
			         m_ArraySpecInfoFromLib[i].m_Flavour,m_ArraySpecInfoFromLib[i].m_Brand);

		m_ListSpecFromLib.InsertItem(i,tempNum);
		m_ListSpecFromLib.SetItemText(i,1,m_ArraySpecInfoFromLib[i].m_SpecID);
		m_ListSpecFromLib.SetItemText(i,2,m_ArraySpecInfoFromLib[i].m_WineName);
		m_ListSpecFromLib.SetItemText(i,3,temp);
		m_ListSpecFromLib.SetItemText(i,4,m_ArraySpecInfoFromLib[i].m_ProductionDate);
		m_ListSpecFromLib.SetItemText(i,5,m_ArraySpecInfoFromLib[i].m_ProductionBatchNo);
		m_ListSpecFromLib.SetItemText(i,6,m_ArraySpecInfoFromLib[i].m_ComFrom);
		m_ListSpecFromLib.SetItemText(i,7,m_ArraySpecInfoFromLib[i].m_ImportDate);
		m_ListSpecFromLib.SetItemText(i,8,m_ArraySpecInfoFromLib[i].m_Manager);
	}
	m_ListSpecFromLib.SetRedraw(TRUE);
	m_ListSpecFromLib.UpdateWindow();
	return 0;
}


void CSpectrumCompareDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	m_ListSelectedProject.SetItemState(-1,0,LVIS_SELECTED);
	m_ListSpecFromLib.SetItemState(-1,0,LVIS_SELECTED);

	CDialogEx::OnLButtonDown(nFlags, point);

}


void CSpectrumCompareDlg::OnNcLButtonDown(UINT nHitTest, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	SystemParametersInfo(SPI_GETDRAGFULLWINDOWS, 0, &m_bDragFullWindow, NULL);
	// 2，如果需要修改设置，则在每次进入CDialog::OnNcLButtonDown默认处理之前修改
	if(m_bDragFullWindow)
		SystemParametersInfo(SPI_SETDRAGFULLWINDOWS, FALSE, NULL, NULL);

	CDialogEx::OnNcLButtonDown(nHitTest, point);

	if(m_bDragFullWindow)
		SystemParametersInfo(SPI_SETDRAGFULLWINDOWS, TRUE, NULL, NULL);
}



void CSpectrumCompareDlg::OnNMClickListSpectocompare(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;

	m_ListSpecFromLib.SetItemState(-1,0,LVIS_SELECTED);

}


void CSpectrumCompareDlg::OnNMClickListSpecinlib(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
	m_ListSelectedProject.SetItemState(-1,0,LVIS_SELECTED);
}





void CSpectrumCompareDlg::OnDestroy()
{
	CDialogEx::OnDestroy();	
	// TODO: 在此处添加消息处理程序代码
	//m_ChartSpectrumCompare.DestroyWindow();
}


void CSpectrumCompareDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 在此处添加消息处理程序代码
	// 不为绘图消息调用 CDialogEx::OnPaint()
	if(m_MaxChart)
	{
		m_ListSelectedProject.MoveWindow(0,0,0,0);
		m_btnImportSpec.MoveWindow(0,0,0,0);
		m_btnClear.MoveWindow(0,0,0,0);
		m_ListSpecFromLib.MoveWindow(0,0,0,0);
		m_ChartSpectrumCompare.MoveWindow(50,m_rect.top+20,m_rect.Width()-50,m_rect.Height()-20);
		m_ComboSelectedSpecName.MoveWindow(50,m_rect.top,m_rect.Width()-50,m_rect.top+20);		

		m_btnPlot.MoveWindow(5,m_rect.top+40*0,40,40);
		m_btnTrans.MoveWindow(5,m_rect.top+40*1,40,40);
		m_btnAbs.MoveWindow(5,m_rect.top+40*2,40,40);
		m_btnSelect.MoveWindow(5,m_rect.top+40*3,40,40);
		m_btnMove.MoveWindow(5,m_rect.top+40*4,40,40);
		m_btnZommin.MoveWindow(5,m_rect.top+40*5,40,40);
		m_btnZoomout.MoveWindow(5,m_rect.top+40*6,40,40);
		m_btnLabel.MoveWindow(5,m_rect.top+40*7,40,40);

	}
	else
	{
		m_ListSelectedProject.MoveWindow(m_rect.left,m_rect.top,350,m_rect.Height()*0.4);
		m_btnImportSpec.MoveWindow(m_rect.left,m_rect.Height()*0.4+2,80,25);
		m_btnClear.MoveWindow(m_rect.left+90,m_rect.Height()*0.4+2,50,25);
		m_ListSpecFromLib.MoveWindow(m_rect.left+2,m_rect.Height()*0.4+30,350,m_rect.Height()*0.6-30);	
		m_ChartSpectrumCompare.MoveWindow(350+50,m_rect.top+20,m_rect.Width()-350-50,m_rect.Height()-20);
		m_ComboSelectedSpecName.MoveWindow(350+50,m_rect.top,m_rect.Width()-350-50,m_rect.top+20);

		m_btnPlot.MoveWindow(350+5,m_rect.top+40*0,40,40);
		m_btnTrans.MoveWindow(350+5,m_rect.top+40*1,40,40);
		m_btnAbs.MoveWindow(350+5,m_rect.top+40*2,40,40);
		m_btnSelect.MoveWindow(350+5,m_rect.top+40*3,40,40);
		m_btnMove.MoveWindow(350+5,m_rect.top+40*4,40,40);
		m_btnZommin.MoveWindow(350+5,m_rect.top+40*5,40,40);
		m_btnZoomout.MoveWindow(350+5,m_rect.top+40*6,40,40);
		m_btnLabel.MoveWindow(350+5,m_rect.top+40*7,40,40);

		if(LoadSkin)
		{
			LoadSkin = 0;
			skinppLoadSkin(skin_path); //加载皮肤
		}

	}
}
void CSpectrumCompareDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
	GetClientRect(&m_rect);
	/*if(m_MaxChart)
	{
		if(m_ListSelectedProject)
		{
			m_ListSelectedProject.MoveWindow(0,0,0,0);
		}
		if(m_btnImportSpec)
		{
			m_btnImportSpec.MoveWindow(0,0,0,0);
		}
		if(m_ListSpecFromLib)
		{
			m_ListSpecFromLib.MoveWindow(0,0,0,0);
		}
		if(m_ChartSpectrumCompare)
		{
			m_ChartSpectrumCompare.MoveWindow(50,m_rect.top+20,m_rect.Width()-50,m_rect.Height()-20);
		}
		if(m_ComboSelectedSpecName)
		{
			m_ComboSelectedSpecName.MoveWindow(50,m_rect.top,m_rect.Width()-50,m_rect.top+20);
		}

		if(m_ToolBar)
		{
			m_ToolBar.MoveWindow(5,m_rect.top,40,45*10);
		}

	}
	else
	{
		if(m_ListSelectedProject)
		{
			m_ListSelectedProject.MoveWindow(m_rect.left,m_rect.top,350,m_rect.Height()*0.4);
		}
		if(m_btnImportSpec)
		{
			m_btnImportSpec.MoveWindow(m_rect.left,m_rect.Height()*0.4+2,80,25);
		}
		if(m_ListSpecFromLib)
		{
			m_ListSpecFromLib.MoveWindow(m_rect.left+2,m_rect.Height()*0.4+30,350,m_rect.Height()*0.6-30);	
		}
		if(m_ChartSpectrumCompare)
		{
			m_ChartSpectrumCompare.MoveWindow(350+50,m_rect.top+20,m_rect.Width()-350-50,m_rect.Height()-20);
		}
		if(m_ComboSelectedSpecName)
		{
			m_ComboSelectedSpecName.MoveWindow(350+50,m_rect.top,m_rect.Width()-350-50,m_rect.top+20);
		}
		if(m_ToolBar)
		{
			m_ToolBar.MoveWindow(350+5,m_rect.top,40,45*10);
		}		
	}*/
	this->Invalidate();
}

void CSpectrumCompareDlg::OnCbnSelchangeComboSelectedspecname()
{
	// TODO: 在此添加控件通知处理程序代码
	int nItem=m_ComboSelectedSpecName.GetCurSel();
	CString curveName;
	m_ComboSelectedSpecName.GetLBText(nItem,curveName);
	m_ChartSpectrumCompare.SetCurveSelected(curveName);
}


afx_msg LRESULT CSpectrumCompareDlg::OnChangeselectedcurve(WPARAM wParam, LPARAM lParam)
{
	CString curveName=(LPCTSTR)lParam;
	free((wchar_t *)lParam);
	lParam = NULL;
	//m_ChartSpectrumCompare.SetCurveSelected(curveName);
	int selNum=wParam;
	if(1 == selNum)
	{
		int nItem=0;
		
		if((nItem = m_ComboSelectedSpecName.FindString(nItem, curveName)) == CB_ERR)
		{
			//找不到相应的内容，恢复combobox的内容
			m_ComboSelectedSpecName.ResetContent();
			for(int i=0;i<m_ArrayComboBak.GetCount();i++)
			{
				m_ComboSelectedSpecName.AddString(m_ArrayComboBak[i]);
			}
			nItem = m_ComboSelectedSpecName.FindString(nItem,curveName);
		}		
		m_ArrayComboBak.RemoveAll();  //清空名称备份
		m_ComboSelectedSpecName.SetCurSel(nItem);
	}
	else
	{
		//建立备份存放之前的曲线名称
		if(m_ArrayComboBak.GetCount()==0)
		{
			for(int i=0;i<m_ComboSelectedSpecName.GetCount();i++)
			{
				CString tempStr;
				m_ComboSelectedSpecName.GetLBText(i,tempStr);
				m_ArrayComboBak.Add(tempStr);
			}
		}
		m_ComboSelectedSpecName.ResetContent();

		CString tempShow;
		tempShow.Format(L"当前选中%d条光谱(",selNum);
		std::set<CString> NameSet = m_ChartSpectrumCompare.GetSelectedCurveName();

		std::set<CString>::iterator iterSet = NameSet.begin();
		while(iterSet!= NameSet.end())
		{
			tempShow+= *iterSet;
			tempShow+= L",";
			++iterSet;
		}
		tempShow = tempShow.Left(tempShow.GetLength()-1);
		tempShow+=L")";
		//tempShow += m_ChartSpectrumCompare.GetSelectedCurveNames();
		m_ComboSelectedSpecName.AddString(tempShow);
		m_ComboSelectedSpecName.SetCurSel(0);
	}
	return 0;
}


void CSpectrumCompareDlg::OnUpdateMaxchartshow(CCmdUI *pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	m_MaxChart = !m_MaxChart;
	if(m_MaxChart)
		m_Menu.CheckMenuItem(ID_MAXCHARTSHOW,MF_CHECKED);
	else
		m_Menu.CheckMenuItem(ID_MAXCHARTSHOW,MF_UNCHECKED);

	
	//pCmdUI->SetCheck(m_MaxChart);
}


void CSpectrumCompareDlg::OnMaxchartshow()
{
	// TODO: 在此添加命令处理程序代码
	//this->OnPaint();
	this->Invalidate();
}


void CSpectrumCompareDlg::OnSpecminus()
{
	// TODO: 在此添加命令处理程序代码
	if(2 != m_ChartSpectrumCompare.GetSelCurveCount())
	{
		MessageBox(L"选择2个图谱才能运行此命令",L"图谱处理",MB_ICONINFORMATION);
		return;
	}

}


void CSpectrumCompareDlg::OnChangetoabs()
{
	// TODO: 在此添加命令处理程序代码
	m_ShowTransAbs = false;
	m_ChartSpectrumCompare.SetCurveTransAbs(m_ShowTransAbs);

	//改变勾选
	m_Menu.CheckMenuItem(ID_CHANGETOABS,MF_CHECKED);
	m_Menu.CheckMenuItem(ID_CHANGETOTRANS,MF_UNCHECKED);
}


void CSpectrumCompareDlg::OnChangetotrans()
{
	// TODO: 在此添加命令处理程序代码
	m_ShowTransAbs = true;
	m_ChartSpectrumCompare.SetCurveTransAbs(m_ShowTransAbs);
	//改变勾选
	m_Menu.CheckMenuItem(ID_CHANGETOTRANS,MF_CHECKED);
	m_Menu.CheckMenuItem(ID_CHANGETOABS,MF_UNCHECKED);
}


void CSpectrumCompareDlg::OnDeleteselectedcurve()
{
	// TODO: 在此添加命令处理程序代码
	if(m_ChartSpectrumCompare.m_SetSelCurveName.size() == 1)
	{
		set<CString>::iterator iter = m_ChartSpectrumCompare.m_SetSelCurveName.begin();

		int nItem=0;
		if(iter!=m_ChartSpectrumCompare.m_SetSelCurveName.end())
		{
			if((nItem = m_ComboSelectedSpecName.FindString(nItem, *iter)) != CB_ERR)
			{
				m_ComboSelectedSpecName.DeleteString(nItem);
			}		
		}
	}
	else
	{
		//恢复
		for (int i = m_ComboSelectedSpecName.GetCount() - 1; i >= 0; i--)
		{
			m_ComboSelectedSpecName.DeleteString(i);
		}
		for(int i=0;i<m_ArrayComboBak.GetCount();i++)
		{
			m_ComboSelectedSpecName.AddString(m_ArrayComboBak[i]);
		}
		
		set<CString>::iterator iter = m_ChartSpectrumCompare.m_SetSelCurveName.begin();
		int nItem=0;
		while(iter!=m_ChartSpectrumCompare.m_SetSelCurveName.end())
		{
			if((nItem = m_ComboSelectedSpecName.FindString(nItem, *iter)) != CB_ERR)
			{
				m_ComboSelectedSpecName.DeleteString(nItem);
			}		
			iter++;
			nItem = 0;
		}
	}
	m_ChartSpectrumCompare.DeleteCurveSelected();

	if(m_ComboSelectedSpecName.GetCount()!=0)
	{
		m_ComboSelectedSpecName.SetCurSel(0);
		CString _curveName;
		m_ComboSelectedSpecName.GetLBText(0,_curveName);
		m_ChartSpectrumCompare.SetCurveSelected(_curveName);
	}
}



void CSpectrumCompareDlg::OnShownoise()
{
	// TODO: 在此添加命令处理程序代码
	if(m_ChartSpectrumCompare.GetSelCurveCount()!=1)
	{
		MessageBox(L"选择一条图谱才能运行此命令！",L"信息",MB_ICONWARNING);
		return;
	}
	m_ChartSpectrumCompare.ShowSelectedNoise();

}


void CSpectrumCompareDlg::OnSpecmean()
{
	// TODO: 在此添加命令处理程序代码
	if(m_ChartSpectrumCompare.GetSelCurveCount()!=1)
	{
		MessageBox(L"选择一条图谱才能运行此命令！",L"信息",MB_ICONWARNING);
		return;
	}
	m_ChartSpectrumCompare.ShowSelectedMean();

}




void CSpectrumCompareDlg::OnSpecselect()
{
	// TODO: 在此添加命令处理程序代码
	m_ChartSpectrumCompare.ChangeToSelectMode();
}

void CSpectrumCompareDlg::OnSpecmove()
{
	// TODO: 在此添加命令处理程序代码
	m_ChartSpectrumCompare.ChangeToMoveMode();
}

void CSpectrumCompareDlg::OnSpeczoomin()
{
	// TODO: 在此添加命令处理程序代码
	m_ChartSpectrumCompare.ChangeToZoomInMode();
}


void CSpectrumCompareDlg::OnSpeczoomout()
{
	// TODO: 在此添加命令处理程序代码
	m_ChartSpectrumCompare.ChangeToZoomOutMode();
}


void CSpectrumCompareDlg::OnPeakarea()
{
	// TODO: 在此添加命令处理程序代码
	if(1 != m_ChartSpectrumCompare.GetSelCurveCount())
	{
		MessageBox(L"只有选择一条图谱才能进行峰面积计算",L"峰面积测定",MB_ICONWARNING);
		return ;
	}
	m_ChartSpectrumCompare.ChangeToPeakAreaCalMode();

}


void CSpectrumCompareDlg::OnLabel()
{
	// TODO: 在此添加命令处理程序代码
	m_ChartSpectrumCompare.ChangeToLableMode();
}


BOOL CSpectrumCompareDlg::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此添加专用代码和/或调用基类

	return CDialogEx::PreCreateWindow(cs);
}


void CSpectrumCompareDlg::OnDeletelabel()
{
	// TODO: 在此添加命令处理程序代码
	m_ChartSpectrumCompare.RemoveAllLabel();
}

//PCA
void CSpectrumCompareDlg::OnPcaanalasis()
{
	// TODO: 在此添加命令处理程序代码
	CPCASettingDlg PCASettingDlg;
	if(this->m_AnaPts == 1)
	{
		vector<int> IndexVec = this->m_ChartSpectrumCompare.GetLabelIndex();
		PCASettingDlg.m_InitPCNum = IndexVec.size() - 1;
	}
	else
	{
		PCASettingDlg.m_InitPCNum = 10;
	}
	if(IDOK == PCASettingDlg.DoModal())
	{	
		CMatrix TotalSpecData;

		vector<CString> WineNameVector;
		vector<int> EachWineCount;
		
		int EachNameCount = 0;
		int i = 0;
		if(PCASettingDlg.m_RadioSpecToPCA == 1)
		{
			for(;i<m_ArraySpecInfoFromLib.GetCount();++i)
			{
				if(0 == i)
				{
					WineNameVector.push_back(m_ArraySpecInfoFromLib[i].m_WineName);				
				}
				if(m_ArraySpecInfoFromLib[i].m_WineName != WineNameVector[WineNameVector.size() - 1])
				{
					EachWineCount.push_back(EachNameCount);
					WineNameVector.push_back(m_ArraySpecInfoFromLib[i].m_WineName);
					EachNameCount = 0;
				}
				else
				{
					CMatrix tempRow(1,SPEC_LENTH);
					CString2Matrix(tempRow,m_ArraySpecInfoFromLib[i].m_SpecData,L",");
					TotalSpecData.AddRows(tempRow);
					++EachNameCount;
				}
			}
		}

		EachWineCount.push_back(EachNameCount);

		if(m_MatrixSpecData.m_row != m_ArraySpecPath.GetCount())
		{
			for(int i=0;i<m_ArraySpecPath.GetCount();++i)
			{
				CCSVOperator CSVOperator;
				CMatrix tempRow(1,SPEC_LENTH);
				if(CSVOperator.LoadCSV(m_ArraySpecPath[i]))
				{
					double temp;
					for(int j=0;j<SPEC_LENTH;++j)
					{
						CSVOperator.GetDouble(j+1,2,temp);
						tempRow.SetItem(1,j+1,temp);
					}
				}
				//保存以便下次使用不再重新导入
				m_MatrixSpecData.AddRows(tempRow);
			}
			TotalSpecData.AddRows(m_MatrixSpecData);
		}
		else
		{
			TotalSpecData.AddRows(m_MatrixSpecData);
		}

		//对数据进行预处理
		if(m_IsDenoiseChecked)//需要去噪
		{
			if(m_RadioDenoise == 0) //需要平滑
			{
				TotalSpecData.Smooth(m_SmoothPt);
			}
			else //需要小波降噪
			{
				TotalSpecData.Wden(m_TTppe,m_TSORH,m_TScal,m_DecLevel,m_wName);
			}
		}
		if(m_IsCorrChecked)  //需要校正
		{
			if(m_RadioCorrelation == 0) //MSC
			{
				CMatrix ref;
				CMatrix beta;
				CMatrix alpha;

				TotalSpecData.MSC(ref,beta,alpha,1);
				//TotalSpecData.Smooth(m_SmoothPt);
			}
			else //求导
			{
				TotalSpecData.Derivative(m_DerivativeOrder);
			}
		}

		if(this->m_AnaPts == 1)
		{
			vector<int> IndexVec = this->m_ChartSpectrumCompare.GetLabelIndex();
			CMatrix TmpDataSet(TotalSpecData.m_row,IndexVec.size());

			vector<int>::iterator iter = IndexVec.begin();
			int i =1;
			while(iter != IndexVec.end())
			{
				TmpDataSet.SetColumn(i,TotalSpecData.GetColumn(*iter+1));
				++i;
				++iter;
			}
			TotalSpecData = TmpDataSet;
		}

		CMatrix signals(TotalSpecData.m_row,TotalSpecData.m_column);
		CMatrix PC(TotalSpecData.m_column,TotalSpecData.m_column);
		CMatrix V(1,TotalSpecData.m_column);
		//计算PCA
		int flag=TotalSpecData.PCA(signals,PC,V);

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
		PCADlg.m_xPC = PCASettingDlg.xPC;
		PCADlg.m_yPC = PCASettingDlg.yPC;
		PCADlg.m_PCNumShow = PCASettingDlg.PCNum;
		PCADlg.DoModal();

	}
}
afx_msg LRESULT CSpectrumCompareDlg::OnChangechartmode(WPARAM wParam, LPARAM lParam)
{
	switch(wParam)
	{
	case CURVE_SELECT:
		{
			m_Menu.CheckMenuItem(ID_SPECSELECT,MF_CHECKED);
			m_Menu.CheckMenuItem(ID_SPECMOVE,MF_UNCHECKED);
			m_Menu.CheckMenuItem(ID_SPECZOOMIN,MF_UNCHECKED);
			m_Menu.CheckMenuItem(ID_SPECZOOMOUT,MF_UNCHECKED);
			break;
		}
	case CURVE_MOVE:
		{
			m_Menu.CheckMenuItem(ID_SPECSELECT,MF_UNCHECKED);
			m_Menu.CheckMenuItem(ID_SPECMOVE,MF_CHECKED);
			m_Menu.CheckMenuItem(ID_SPECZOOMIN,MF_UNCHECKED);
			m_Menu.CheckMenuItem(ID_SPECZOOMOUT,MF_UNCHECKED);
			break;
		}
	case CURVE_ZOOM_IN:
		{
			m_Menu.CheckMenuItem(ID_SPECSELECT,MF_UNCHECKED);
			m_Menu.CheckMenuItem(ID_SPECMOVE,MF_UNCHECKED);
			m_Menu.CheckMenuItem(ID_SPECZOOMIN,MF_CHECKED);
			m_Menu.CheckMenuItem(ID_SPECZOOMOUT,MF_UNCHECKED);
			break;
		}
	case CURVE_ZOOM_OUT:
		{
			m_Menu.CheckMenuItem(ID_SPECSELECT,MF_UNCHECKED);
			m_Menu.CheckMenuItem(ID_SPECMOVE,MF_UNCHECKED);
			m_Menu.CheckMenuItem(ID_SPECZOOMIN,MF_UNCHECKED);
			m_Menu.CheckMenuItem(ID_SPECZOOMOUT,MF_CHECKED);
			break;
		}
	default:
		break;
	}


	return 0;
}


void CSpectrumCompareDlg::OnFakewineclustering()
{
	// TODO: 在此添加命令处理程序代码
	CMatrix TotalDataset;
	CMatrix FakeDataset;
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
		FakeWineLib.MoveFirst();
		CMatrix temp(1,SPEC_LENTH);
		CString2Matrix(temp,FakeWineLib.m_SpectrumData,L",");

		FakeDataset.AddRows(temp);

		FakeWineLib.MoveNext();
		while(!FakeWineLib.IsEOF())
		{
			CString2Matrix(temp,FakeWineLib.m_SpectrumData,L",");
			FakeDataset.AddRows(temp);
			FakeWineLib.MoveNext();
		}
		FakeWineLib.Close();
		TotalDataset.AddRows(FakeDataset);
		//取得待分析光谱数据
		vector<CString> VectorCurveName;
		if(m_MatrixSpecData.m_row != m_ArraySpecPath.GetCount())
		{
			for(int i=0;i<m_ArraySpecPath.GetCount();++i)
			{
				CCSVOperator CSVOperator;
				CMatrix tmpSpecData(1,SPEC_LENTH);
				if(CSVOperator.LoadCSV(m_ArraySpecPath[i]))
				{			
					double temp;
					for(int j=0;j<SPEC_LENTH;++j)
					{
						CSVOperator.GetDouble(j+1,2,temp);	
						tmpSpecData.SetItem(1,j+1,temp);
					}
					//保存以便下次使用不再重新导入
					m_MatrixSpecData.AddRows(tmpSpecData);
				}
				VectorCurveName.push_back(m_ArraySpecName[i]);
			}
		}
		else
		{
			for(int i=0;i<m_ArraySpecPath.GetCount();++i)
			{
				VectorCurveName.push_back(m_ArraySpecName[i]);
			}
		}	
		TotalDataset.AddRows(m_MatrixSpecData);

		//数据预处理
		if(m_IsDenoiseChecked)//需要去噪
		{
			if(m_RadioDenoise == 0) //需要平滑
			{
				TotalDataset.Smooth(m_SmoothPt);
			}
			else //需要小波降噪
			{
				TotalDataset.Wden(m_TTppe,m_TSORH,m_TScal,m_DecLevel,m_wName);
			}
		}
		if(m_IsCorrChecked)  //需要校正
		{
			if(m_RadioCorrelation == 0) //MSC
			{
				CMatrix ref;
				CMatrix beta;
				CMatrix alpha;
				TotalDataset.MSC(ref,beta,alpha,1);
			}
			else //求导
			{
				TotalDataset.Derivative(m_DerivativeOrder);
			}
		}

		if(this->m_AnaPts == 1)
		{
			vector<int> IndexVec = this->m_ChartSpectrumCompare.GetLabelIndex();
			CMatrix TmpDataSet(TotalDataset.m_row,IndexVec.size());

			vector<int>::iterator iter = IndexVec.begin();
			int i =1;
			while(iter != IndexVec.end())
			{
				TmpDataSet.SetColumn(i,TotalDataset.GetColumn(*iter+1));
				++i;
				++iter;
			}
			TotalDataset = TmpDataSet;
		}

		CClusterDlg ClusterDlg;
		ClusterDlg.m_VectorCurveName = VectorCurveName;
		ClusterDlg.m_AnaDataSet = m_MatrixSpecData;
		ClusterDlg.m_FakeDataset = FakeDataset;
		ClusterDlg.m_TotalDataSet = TotalDataset;
		ClusterDlg.m_RadioCluMethod=ClusterSettingDlg.m_RadioCluMethod; //聚类类别
		ClusterDlg.m_eps=ClusterSettingDlg.m_eps;         //扫描半径
		ClusterDlg.m_Minpts =ClusterSettingDlg.m_Minpts;         //最小点数
		ClusterDlg.m_ClusterNum =ClusterSettingDlg.m_ClusterNum;     //聚类类别
		ClusterDlg.DoModal();
	}
}


void CSpectrumCompareDlg::OnNMDblclkListSpectocompare(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
	if(m_ListSelectedProject.GetSelectedCount() == 0)
		return;
	int Index = m_ListSelectedProject.GetSelectionMark();
	CString SpecInfo;
	SpecInfo.Format(L"来自项目：%s\n文件名：%s\n检测结果：%s",m_projectName,m_ArraySpecName[Index],m_ArraySpecPreResult[Index]);
	MessageBox(SpecInfo,L"光谱信息",MB_ICONINFORMATION);
	return;
}


void CSpectrumCompareDlg::OnNMDblclkListSpecinlib(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
	if(m_ListSpecFromLib.GetSelectedCount() == 0)
		return;
	int Index = m_ListSpecFromLib.GetSelectionMark();
	CString SpecInfo;
	SpecInfo.Format(L"光谱编号：%s\n白酒名称：%s\n品牌：%s\n酒精度：%s\n香型：%s\
		\n生产日期：%s\n生产批号：%s\n来源信息：%s\n入库日期：%s\n操作员：%s"
		,m_ArraySpecInfoFromLib[Index].m_SpecID
		,m_ArraySpecInfoFromLib[Index].m_WineName
		,m_ArraySpecInfoFromLib[Index].m_Brand
		,m_ArraySpecInfoFromLib[Index].m_AlcoholContent
		,m_ArraySpecInfoFromLib[Index].m_Flavour
		,m_ArraySpecInfoFromLib[Index].m_ProductionDate
		,m_ArraySpecInfoFromLib[Index].m_ProductionBatchNo
		,m_ArraySpecInfoFromLib[Index].m_ComFrom
		,m_ArraySpecInfoFromLib[Index].m_ImportDate
		,m_ArraySpecInfoFromLib[Index].m_Manager	
		);
	MessageBox(SpecInfo,L"光谱信息",MB_ICONINFORMATION);
	return;


}
void CSpectrumCompareDlg::OnDeleteallcurve()
{
	// TODO: 在此添加命令处理程序代码
	m_ChartSpectrumCompare.DestroyCurve();
	//combobox清空
	for (int i = m_ComboSelectedSpecName.GetCount() - 1; i >= 0; --i)
	{
		m_ComboSelectedSpecName.DeleteString(i);
	}
}


void CSpectrumCompareDlg::OnSmoothspec()
{
	// TODO: 在此添加命令处理程序代码

	std::set<CString> curveNameset = m_ChartSpectrumCompare.GetSelectedCurveName();
	std::set<CString>::iterator iter = curveNameset.begin();

	if(curveNameset.size() == 0)
	{
		MessageBox(L"请至少选择一条光谱",L"平滑",MB_ICONERROR);
		return;
	}

	CSmoothSettingDlg SmoothSettingDlg;

	if(IDOK == SmoothSettingDlg.DoModal())
	{

		CCurveDataSetArray tempArray;
		CString _newCurveName;
		if(!m_ArrayComboBak.IsEmpty())
		{
			//恢复
			m_ComboSelectedSpecName.ResetContent();
			for(int i=0;i<m_ArrayComboBak.GetCount();i++)
			{
				m_ComboSelectedSpecName.AddString(m_ArrayComboBak[i]);
			}
		}
		while(iter != curveNameset.end())
		{
			//得到平滑后的数组
			//tempArray.RemoveAll();
			m_ChartSpectrumCompare.SmoothCurve(tempArray,*iter,MOVE_MEAN,SmoothSettingDlg.m_SmoothPts);
			_newCurveName = *iter;
			CString SmoothPts;
			SmoothPts.Format(L"(移动平滑:%d点)",SmoothSettingDlg.m_SmoothPts);

			_newCurveName += SmoothPts;
			m_ChartSpectrumCompare.CreateNewCurve(_newCurveName,0);

			for(int i=0;i<tempArray.GetCount();i++)
			{
				m_ChartSpectrumCompare.AppendPoint(_newCurveName,tempArray[i].m_PosX,tempArray[i].m_PosY);
			}
			m_ComboSelectedSpecName.AddString(_newCurveName);
			iter++;
		}
		m_ChartSpectrumCompare.SetCurveSelected(_newCurveName);
		m_ChartSpectrumCompare.SetCurveTransAbs(m_ShowTransAbs);
		m_ChartSpectrumCompare.ChangeToIndividualCoordinateMode(false);
		m_ChartSpectrumCompare.InvalidateChart();
	}

}
void CSpectrumCompareDlg::OnDiff()
{
	// TODO: 在此添加命令处理程序代码
	std::set<CString> curveNameset = m_ChartSpectrumCompare.GetSelectedCurveName();
	std::set<CString>::iterator iter = curveNameset.begin();

	if(curveNameset.size() == 0)
	{
		MessageBox(L"请至少选择一条光谱",L"平滑",MB_ICONERROR);
		return;
	}
	CDiffSettingDlg DiffSettingDlg;
	if(IDOK == DiffSettingDlg.DoModal())
	{
		//CCurveDataSetArray tempArray;
		CString _newCurveName;
		if(!m_ArrayComboBak.IsEmpty())
		{
			//恢复
			m_ComboSelectedSpecName.ResetContent();
			for(int i=0;i<m_ArrayComboBak.GetCount();i++)
			{
				m_ComboSelectedSpecName.AddString(m_ArrayComboBak[i]);
			}
		}
		while(iter != curveNameset.end())
		{
			//得到求导后的数组
			_newCurveName.Format(L"%s(%d阶导)",*iter,DiffSettingDlg.m_Order);
			m_ChartSpectrumCompare.CreateNewCurve(_newCurveName,0);

			CMatrix TempY = m_ChartSpectrumCompare.GetCurveYData(*iter);
			CMatrix TempX = m_ChartSpectrumCompare.GetCurveXData(*iter);

			CMatrix TempZ(TempY.m_row,TempY.m_column,0);
			TempY.Derivative(DiffSettingDlg.m_Order);
			
			for(int i = DiffSettingDlg.m_Order;i<TempZ.m_column;++i)
			{
				TempZ.SetItem(1,i+1,TempY(1,i-DiffSettingDlg.m_Order+1));
			}

			for(int i=0;i<TempZ.m_column;i++)
			{
				m_ChartSpectrumCompare.AppendPoint(_newCurveName,TempX(1,i+1),TempZ(1,i+1));
			}
			m_ComboSelectedSpecName.AddString(_newCurveName);
			++iter;
		}
		m_ChartSpectrumCompare.SetCurveSelected(_newCurveName);
		m_ChartSpectrumCompare.SetCurveTransAbs(m_ShowTransAbs);
		m_ChartSpectrumCompare.ChangeToIndividualCoordinateMode(false);
		m_ChartSpectrumCompare.InvalidateChart();
	}
}
void CSpectrumCompareDlg::OnWden()
{
	// TODO: 在此添加命令处理程序代码
	std::set<CString> curveNameset = m_ChartSpectrumCompare.GetSelectedCurveName();
	std::set<CString>::iterator iter = curveNameset.begin();

	if(curveNameset.size() == 0)
	{
		MessageBox(L"请至少选择一条光谱",L"小波降噪",MB_ICONERROR);
		return;
	}
	CString _newCurveName;	
	if(!m_ArrayComboBak.IsEmpty())
	{
		//恢复
		m_ComboSelectedSpecName.ResetContent();
		for(int i=0;i<m_ArrayComboBak.GetCount();i++)
		{
			m_ComboSelectedSpecName.AddString(m_ArrayComboBak[i]);
		}
	}
	CWdenSettingDlg WdenSettingDlg;
	if(IDOK == WdenSettingDlg.DoModal())
	{
		while(iter != curveNameset.end())
		{
			//得到平滑后的数组
			CString SorH;
			if(WdenSettingDlg.m_TSORH == L"s")
				SorH = L"软阈值";
			else
				SorH = L"硬阈值";

			_newCurveName.Format(L"%s(小波降噪:%s、%s、%s)",*iter,WdenSettingDlg.m_TTppe,SorH,WdenSettingDlg.m_TScal);

			m_ChartSpectrumCompare.CreateNewCurve(_newCurveName,0);

			CMatrix TempY = m_ChartSpectrumCompare.GetCurveYData(*iter);
			CMatrix TempX = m_ChartSpectrumCompare.GetCurveXData(*iter);

			TempY.Wden(WdenSettingDlg.m_TTppe,WdenSettingDlg.m_TSORH,WdenSettingDlg.m_TScal,WdenSettingDlg.m_DecLevel,WdenSettingDlg.m_wName);
			for(int i=0;i<TempY.m_column;i++)
			{
				m_ChartSpectrumCompare.AppendPoint(_newCurveName,TempX(1,i+1),TempY(1,i+1));
			}
			m_ComboSelectedSpecName.AddString(_newCurveName);
			++iter;
		}
		m_ChartSpectrumCompare.SetCurveSelected(_newCurveName);
		m_ChartSpectrumCompare.SetCurveTransAbs(m_ShowTransAbs);
		m_ChartSpectrumCompare.ChangeToIndividualCoordinateMode(false);
		m_ChartSpectrumCompare.InvalidateChart();

		m_ComboSelectedSpecName.SetCurSel(m_ComboSelectedSpecName.FindString(0,_newCurveName));

	}
}

void CSpectrumCompareDlg::OnPlot()
{
	// TODO: 在此添加命令处理程序代码
	if(this->m_ListSelectedProject.GetSelectedCount()!=0)
	{

		int nIndex=m_ListSelectedProject.GetSelectionMark();
		CString CurveName = m_ArraySpecName[nIndex];

		if(m_ChartSpectrumCompare.CurveExist(CurveName))
		{
			CString tempString;
			tempString.Format(L"已存在名称为%s的图谱",CurveName);
			MessageBox(tempString,L"添加图谱",MB_ICONWARNING);
			return;
		}
		m_ChartSpectrumCompare.CreateNewCurve(CurveName,0);
		CCSVOperator CSVOperator;
		CSVOperator.LoadCSV(m_ArraySpecPath[nIndex]);
		for(int i=0;i<SPEC_LENTH;i++)
		{
			double tempY;
			CSVOperator.GetDouble(i+1,2,tempY);
			double tempX;
			CSVOperator.GetDouble(i+1,1,tempX);

			m_ChartSpectrumCompare.AppendPoint(CurveName,tempX,tempY);
		}

		m_ChartSpectrumCompare.SetCurveTransAbs(m_ShowTransAbs);
		//m_ChartSpectrumCompare.ReCalculateAllCurveMaxMin();

		m_ChartSpectrumCompare.ChangeToIndividualCoordinateMode(false);
		m_ChartSpectrumCompare.InvalidateChart();

		if(!m_ArrayComboBak.IsEmpty())
		{
			m_ComboSelectedSpecName.ResetContent();
			for(int i=0;i<m_ArrayComboBak.GetCount();i++)
			{
				m_ComboSelectedSpecName.AddString(m_ArrayComboBak[i]);		
			}		
			m_ArrayComboBak.RemoveAll();  //清空名称备份
		}	
		m_ComboSelectedSpecName.AddString(CurveName);

		m_ComboSelectedSpecName.SetCurSel(m_ComboSelectedSpecName.FindString(0,CurveName));

		m_ChartSpectrumCompare.SetCurveSelected(CurveName);
		return ;
	}
	else if(this->m_ListSpecFromLib.GetSelectedCount()!=0)
	{
		int nIndex=m_ListSpecFromLib.GetSelectionMark();
		CString CurveName = m_ArraySpecInfoFromLib[nIndex].m_SpecID;

		if(m_ChartSpectrumCompare.CurveExist(CurveName))
		{
			CString tempString;
			tempString.Format(L"已存在名称为%s的图谱",CurveName);
			MessageBox(tempString,L"添加图谱",MB_ICONWARNING);
			return;
		}
		m_ChartSpectrumCompare.CreateNewCurve(CurveName,0);

		CMatrix tempSpecData(1,869);
		CMatrix tempWaveNumber(1,869);
		int m=1;

		CString2Matrix(tempSpecData,m_ArraySpecInfoFromLib[nIndex].m_SpecData,L",");
		CString2Matrix(tempWaveNumber,m_ArraySpecInfoFromLib[nIndex].m_WaveNumber,L",");

		for(int i=0;i<869;i++)
		{
			double tempY=tempSpecData(1,i+1);			
			double tempX=tempWaveNumber(1,i+1);
			m_ChartSpectrumCompare.AppendPoint(CurveName,tempX,tempY);
		}

		m_ChartSpectrumCompare.SetCurveTransAbs(m_ShowTransAbs);
		//m_ChartSpectrumCompare.ReCalculateAllCurveMaxMin();

		m_ChartSpectrumCompare.ChangeToIndividualCoordinateMode(false);
		m_ChartSpectrumCompare.InvalidateChart();

		if(!m_ArrayComboBak.IsEmpty())
		{
			m_ComboSelectedSpecName.ResetContent();
			for(int i=0;i<m_ArrayComboBak.GetCount();i++)
			{
				m_ComboSelectedSpecName.AddString(m_ArrayComboBak[i]);	
			}	
			m_ArrayComboBak.RemoveAll();  //清空名称备份
		}	
		m_ComboSelectedSpecName.AddString(CurveName);
		m_ComboSelectedSpecName.SetCurSel(m_ComboSelectedSpecName.FindString(0,CurveName));
		m_ChartSpectrumCompare.SetCurveSelected(CurveName);
		return ;
	}
	else
	{
		MessageBox(L"请选择一条光谱",L"光谱绘制",MB_ICONWARNING);
		return ;
	}
}


void CSpectrumCompareDlg::OnSpecanapretrsetting()
{
	// TODO: 在此添加命令处理程序代码
	CSpecPretrSettingDlg SpecPretrSettingDlg;

	SpecPretrSettingDlg.m_IsDenoiseChecked = m_IsDenoiseChecked; //降噪复选框是否选中
	SpecPretrSettingDlg.m_IsCorrChecked = m_IsCorrChecked;   //校正复选框是否选中
	SpecPretrSettingDlg.m_RadioDenoise =m_RadioDenoise;  //降噪
	SpecPretrSettingDlg.m_RadioCorrelation = m_RadioCorrelation;//校正	
	SpecPretrSettingDlg.m_SmoothPtsIndex = m_SmoothPtsIndex; //平滑点数索引
	SpecPretrSettingDlg.m_Wname1Index= m_Wname1Index; //小波名1索引
	SpecPretrSettingDlg.m_Wname2Index= m_Wname2Index;  //小波名2索引
	SpecPretrSettingDlg.m_DecLevelIndex = m_DecLevelIndex; //分解层数索引
	SpecPretrSettingDlg.m_DerivativeOrderIndex = m_DerivativeOrderIndex; //导数阶数索引
	SpecPretrSettingDlg.m_TTppeIndex = m_TTppeIndex;  //阈值类型索引
	SpecPretrSettingDlg.m_TScalIndex = m_TScalIndex; //阈值处理方法索引
	SpecPretrSettingDlg.m_TSORHIndex = m_TSORHIndex;


	if(IDOK == SpecPretrSettingDlg.DoModal())
	{
		//参数
		m_SmoothPt = SpecPretrSettingDlg.m_SmoothPt; //平滑点数
		m_wName = SpecPretrSettingDlg.m_wName;//小波名称
		m_DecLevel = SpecPretrSettingDlg.m_DecLevel; //分解层数
		m_DerivativeOrder = SpecPretrSettingDlg.m_DerivativeOrder; //导数阶次
		m_TTppe = SpecPretrSettingDlg.m_TTppe;
		m_TScal = SpecPretrSettingDlg.m_TScal;
		m_TSORH = SpecPretrSettingDlg.m_TSORH;
		//更新
		m_IsDenoiseChecked = SpecPretrSettingDlg.m_IsDenoiseChecked; //降噪复选框是否选中
		m_IsCorrChecked = SpecPretrSettingDlg.m_IsCorrChecked;   //校正复选框是否选中
		m_RadioDenoise =SpecPretrSettingDlg.m_RadioDenoise;  //降噪
		m_RadioCorrelation = SpecPretrSettingDlg.m_RadioCorrelation;//校正	
		m_SmoothPtsIndex = SpecPretrSettingDlg.m_SmoothPtsIndex; //平滑点数索引
		m_Wname1Index= SpecPretrSettingDlg.m_Wname1Index; //小波名1索引
		m_Wname2Index= SpecPretrSettingDlg.m_Wname2Index;  //小波名2索引
		m_DecLevelIndex = SpecPretrSettingDlg.m_DecLevelIndex; //分解层数索引
		m_DerivativeOrderIndex = SpecPretrSettingDlg.m_DerivativeOrderIndex; //导数阶数索引
	    m_TTppeIndex = SpecPretrSettingDlg.m_TTppeIndex;  //阈值类型索引
	    m_TScalIndex = SpecPretrSettingDlg.m_TScalIndex; //阈值处理方法索引
		m_TSORHIndex = SpecPretrSettingDlg.m_TSORHIndex; //阈值使用方法索引

		m_AnaPts = SpecPretrSettingDlg.m_AnaPts;    //分析的点
		MessageBox(L"设置已保存！",L"预处理设置",MB_ICONINFORMATION);
		return;
	}
	else
	{
		MessageBox(L"设置未保存！",L"预处理设置",MB_ICONINFORMATION);
		return;
	}
}



void CSpectrumCompareDlg::OnBnClickedButtonClear()
{
	// TODO: 在此添加控件通知处理程序代码
	m_ArraySpecInfoFromLib.RemoveAll();
	UpDateCListCtrl();
}

afx_msg LRESULT CSpectrumCompareDlg::OnBeginPrinting(WPARAM wParam, LPARAM lParam)
{
	CDC* pDC			= (CDC*)wParam;
	CPrintInfo* pInfo	= (CPrintInfo *)lParam;
	if(m_fontPrinter.m_hObject==NULL)
		m_fontPrinter.CreatePointFont(120,L"FixedSys",pDC);
	TEXTMETRIC tm;
	CFont* pOldFont = pDC->SelectObject (&m_fontPrinter);
	pDC->GetTextMetrics (&tm);
	m_cyPrinter = tm.tmHeight + tm.tmExternalLeading;
	CSize size = pDC->GetTextExtent (_T ("-----------------------------------------------------------------------------------"), 81);
	pDC->SelectObject (pOldFont);
	m_cxWidth = size.cx;

	m_nLinesPerPage = (pDC->GetDeviceCaps (VERTRES) -
		(m_cyPrinter * (3 + (2 * PRINTMARGIN)))) / m_cyPrinter;

	m_MaxPage =  1;//m_ListDetailResult.GetItemCount()/(m_nLinesPerPage-1);
	//++m_MaxPage;

	pInfo->SetMaxPage (m_MaxPage); //设置最大打印张数

	m_cxOffset = (pDC->GetDeviceCaps (HORZRES) - size.cx) / 2;
	return 0;
}
afx_msg LRESULT CSpectrumCompareDlg::OnMyPrint(WPARAM wParam, LPARAM lParam)
{
	CDC* pDC			= (CDC*)wParam;
	CPrintInfo* pInfo	= (CPrintInfo *)lParam;
	this->m_ChartSpectrumCompare.PrintChart(pDC,pInfo);
	return 0;
}
afx_msg LRESULT CSpectrumCompareDlg::OnEndPrinting(WPARAM wParam, LPARAM lParam)
{
	if(m_fontPrinter.m_hObject!=NULL)
		m_fontPrinter.DeleteObject ();
	return 0;
}

void CSpectrumCompareDlg::OnPrintpreview()
{
	// TODO: 在此添加命令处理程序代码
	if(m_pPrintFrame)
	{
		m_pPrintFrame->DestroyWindow();
		m_pPrintFrame = NULL;
	}
	CPrintFrame* pf=new CPrintFrame(this);
}


void CSpectrumCompareDlg::OnPrint()
{
	// TODO: 在此添加命令处理程序代码
	DWORD dwflags=PD_ALLPAGES | PD_NOPAGENUMS | PD_USEDEVMODECOPIES
		| PD_SELECTION | PD_HIDEPRINTTOFILE; 
	CPrintDialog dlg(FALSE,dwflags,NULL);
	CDC dc;
	CPrintInfo pInfo;
	if(dlg.DoModal()==IDOK)
	{
		dc.Attach(dlg.GetPrinterDC());
		DOCINFOW docInfow;
		//DOCINFO   doc;
		ZeroMemory(&docInfow,sizeof(docInfow));
        docInfow.lpszDocName = (LPCTSTR)"Print Test File";
		//启动打印工作  
		StartDoc(dc,&docInfow);
		StartPage(dc);
		int width = dc.GetDeviceCaps (HORZRES);
		int height = dc.GetDeviceCaps(VERTRES);
		pInfo.m_rectDraw = CRect(0,0,width,height);
		OnBeginPrinting((WPARAM)&dc,(LPARAM)&pInfo);
		this->m_ChartSpectrumCompare.PrintChart(&dc,&pInfo);
		OnEndPrinting((WPARAM)&dc, (LPARAM)&pInfo);
		EndPage(dc);
		//终止打印工作  
		EndDoc(dc);
		if(!dc.DeleteDC())
		{
			return;
		}
		else
		{

		}
	}
}


void CSpectrumCompareDlg::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CWinApp* pApp=AfxGetApp();
	pApp->m_pMainWnd = m_pMFrame;
	CDialogEx::OnClose();
}


void CSpectrumCompareDlg::OnBnClickedButtonPlot()
{
	// TODO: 在此添加控件通知处理程序代码
	this->OnPlot();
}


void CSpectrumCompareDlg::OnBnClickedButtonTrans()
{
	// TODO: 在此添加控件通知处理程序代码
	this->OnChangetotrans();
}


void CSpectrumCompareDlg::OnBnClickedButtonAbs()
{
	// TODO: 在此添加控件通知处理程序代码
	this->OnChangetoabs();
}


void CSpectrumCompareDlg::OnBnClickedButtonSelect()
{
	// TODO: 在此添加控件通知处理程序代码
	this->OnSpecselect();
}


void CSpectrumCompareDlg::OnBnClickedButtonMove()
{
	// TODO: 在此添加控件通知处理程序代码
	this->OnSpecmove();
}


void CSpectrumCompareDlg::OnBnClickedButtonZoomin()
{
	// TODO: 在此添加控件通知处理程序代码
	this->OnSpeczoomin();
}


void CSpectrumCompareDlg::OnBnClickedButtonZoomout()
{
	// TODO: 在此添加控件通知处理程序代码
	this->OnSpeczoomout();
}


void CSpectrumCompareDlg::OnBnClickedButtonLabel()
{
	// TODO: 在此添加控件通知处理程序代码
	this->OnLabel();
}
