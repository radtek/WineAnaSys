// ClusterDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "DataServerCenter.h"
#include "ClusterDlg.h"
#include "afxdialogex.h"
#include "SkinPPWTL.h"
#include "PrintFrame.h"
#include "ClusterSettingDlg.h"
//static bool LoadSkin = 0;
// CClusterDlg 对话框

IMPLEMENT_DYNAMIC(CClusterDlg, CDialogEx)

CClusterDlg::CClusterDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CClusterDlg::IDD, pParent)
	,m_pPrintFrame(0)
{
	CWinApp* pApp=AfxGetApp();
	m_pMFrame = pApp->m_pMainWnd;
	m_ClusterChart.m_pParent = this;
}

CClusterDlg::~CClusterDlg()
{
	if(m_FakeWineLib.IsOpen())
	{
		m_FakeWineLib.Close();
	}
}

void CClusterDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BUTTON_SELECT, m_btnSelect);
	DDX_Control(pDX, IDC_BUTTON_MOVE, m_btnMove);
	DDX_Control(pDX, IDC_BUTTON_ZOOMIN, m_btnZommin);
	DDX_Control(pDX, IDC_BUTTON_ZOOMOUT, m_btnZoomout);

	DDX_Control(pDX, IDC_CLUSTERCHART, m_ClusterChart);
	DDX_Control(pDX, IDC_LIST_TOTALFAKE, m_ListTotalFake);
	DDX_Control(pDX, IDC_LIST_MATCHEDFAKE, m_ListMatchedFake);
	DDX_Control(pDX, IDC_STATIC_ALLFAKE, m_StaticAll);
	DDX_Control(pDX, IDC_STATIC_SIMILARITY, m_StaticSimilarity);
}


BEGIN_MESSAGE_MAP(CClusterDlg, CDialogEx)
	ON_WM_SIZE()
	ON_WM_PAINT()
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_TOTALFAKE, &CClusterDlg::OnNMDblclkListTotalfake)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_MATCHEDFAKE, &CClusterDlg::OnNMDblclkListMatchedfake)
	ON_WM_PAINT()

	ON_COMMAND(ID_COMMON_CURSOR, &CClusterDlg::OnCommonCursor)
	ON_COMMAND(ID_MOVE_CURSOR, &CClusterDlg::OnMoveCursor)
	ON_COMMAND(ID_ZOOMIN_CURSOR, &CClusterDlg::OnZoominCursor)
	ON_COMMAND(ID_ZOOMOUT_CURSOR, &CClusterDlg::OnZoomoutCursor)
	ON_MESSAGE(WM_CHANGECHARTMODE, &CClusterDlg::OnChangechartmode)

	ON_COMMAND(ID_PRINTPREVIEW, &CClusterDlg::OnPrintpreview)
	ON_MESSAGE(WM_MY_PRINT, &CClusterDlg::OnMyPrint)
	ON_MESSAGE(WM_BEGIN_PRINTING, &CClusterDlg::OnBeginPrinting)
	ON_MESSAGE(WM_END_PRINTING, &CClusterDlg::OnEndPrinting)
	ON_COMMAND(ID_PRINTPREVIEW, &CClusterDlg::OnPrintpreview)
	ON_COMMAND(ID_PRINT, &CClusterDlg::OnPrint)
	ON_BN_CLICKED(IDC_BUTTON_SELECT, &CClusterDlg::OnBnClickedButtonSelect)
	ON_BN_CLICKED(IDC_BUTTON_MOVE, &CClusterDlg::OnBnClickedButtonMove)
	ON_BN_CLICKED(IDC_BUTTON_ZOOMIN, &CClusterDlg::OnBnClickedButtonZoomin)
	ON_BN_CLICKED(IDC_BUTTON_ZOOMOUT, &CClusterDlg::OnBnClickedButtonZoomout)
	ON_WM_CLOSE()
	ON_COMMAND(ID_CLUSTER_SETTING, &CClusterDlg::OnClusterSetting)
END_MESSAGE_MAP()


// CClusterDlg 消息处理程序

BOOL CClusterDlg::OnInitDialog()
{
	//skinppRemoveSkin(); //移除皮肤	
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	HICON m_hIcon;
	m_hIcon=AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	SetIcon(m_hIcon,TRUE); //设置为大图标
	//更新列表
	m_ListTotalFake.InsertColumn(0,L"",LVCFMT_LEFT,30);
	m_ListTotalFake.InsertColumn(1,L"光谱ID",LVCFMT_LEFT,100);
	m_ListTotalFake.InsertColumn(2,L"聚类结果",LVCFMT_LEFT,100);

	DWORD dwStyle = m_ListTotalFake.GetExtendedStyle(); //获取当前扩展样式
	dwStyle |= LVS_EX_FULLROWSELECT;                    //选中某行使整行高亮（report风格时）
	dwStyle |= LVS_EX_GRIDLINES;                        //网格线（report风格时）
	m_ListTotalFake.SetExtendedStyle(dwStyle);          //设置扩展风格

	m_ListMatchedFake.InsertColumn(0,L"",LVCFMT_LEFT,30);
	m_ListMatchedFake.InsertColumn(1,L"光谱ID",LVCFMT_LEFT,100);
	m_ListMatchedFake.InsertColumn(2,L"聚类结果",LVCFMT_LEFT,100);

	dwStyle = m_ListMatchedFake.GetExtendedStyle();    //获取当前扩展样式
	dwStyle |= LVS_EX_FULLROWSELECT;                   //选中某行使整行高亮（report风格时）
	dwStyle |= LVS_EX_GRIDLINES;                       //网格线（report风格时）
	m_ListMatchedFake.SetExtendedStyle(dwStyle);       //设置扩展风格


	//获取所有数据
	m_ListTotalFake.SetHeaderBackColor(168,193,221,2);
	m_ListTotalFake.SetRowHeigt(28);                   //设置扩展风格
	m_ListMatchedFake.SetHeaderBackColor(168,193,221,2);
	m_ListMatchedFake.SetRowHeigt(28);                 //设置扩展风格

	int xSize=::GetSystemMetrics(SM_CXSCREEN);
	int ySize=::GetSystemMetrics(SM_CYSCREEN);
	int cx=xSize*8/10;
	int cy=ySize*8/10;
	this->MoveWindow((xSize-cx)/2,(ySize-cy)/2,xSize*8/10,ySize*8/10);


	m_Menu.LoadMenu(IDR_MENU_CLUSTER); 
	SetMenu(&m_Menu);

	HICON hIcon=AfxGetApp()->LoadIcon(IDI_ICON_SELECT); 
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


	ShowClusterResult();
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CClusterDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
	GetClientRect(&m_rect);
	this->Invalidate();
}


void CClusterDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 在此处添加消息处理程序代码
	// 不为绘图消息调用 CDialogEx::OnPaint()
	m_btnSelect.MoveWindow(0,m_rect.top,40,40);
	m_btnMove.MoveWindow(40*1,m_rect.top,40,40);
	m_btnZommin.MoveWindow(40*2,m_rect.top,40,40);
	m_btnZoomout.MoveWindow(40*3,m_rect.top,40,40);

	m_ClusterChart.MoveWindow(0,50,m_rect.Width()-250,m_rect.Height()-50);
	m_StaticAll.MoveWindow(m_rect.Width()-240,0,240,m_rect.Height()/2-5);
	m_ListTotalFake.MoveWindow(m_rect.Width()-235,20,230,m_rect.Height()/2-25);
	m_StaticSimilarity.MoveWindow(m_rect.Width()-240,m_rect.Height()*0.5+5,240,m_rect.Height()/2-5);
	m_ListMatchedFake.MoveWindow(m_rect.Width()-235,m_rect.Height()*0.5+25,230,m_rect.Height()/2-25);
}


void CClusterDlg::OnNMDblclkListMatchedfake(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
	if(0 == m_ListMatchedFake.GetSelectedCount())
		return;
	int index = m_ArraySimilaNo[m_ListMatchedFake.GetSelectionMark()];

	CString specInfo;
	specInfo.Format(L"光谱编号：%s\n仿冒白酒名称：%s\n来源说明：%s\n入库时间：%s\n操作员：%s",
		m_ArraySpectrumID[index],m_ArrayWineNameFaked[index],m_ArrayComfrom[index],m_ArrayImportDate[index],m_ArrayManager[index]);
	MessageBox(specInfo,L"光谱信息",MB_ICONINFORMATION);
}


void CClusterDlg::OnNMDblclkListTotalfake(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
	*pResult = 0;

	if(0 == m_ListTotalFake.GetSelectedCount())
		return;
	int index = m_ListTotalFake.GetSelectionMark();
	CString specInfo;
	specInfo.Format(L"光谱编号：%s\n仿冒白酒名称：%s\n来源说明：%s\n入库时间：%s\n操作员：%s",
		m_ArraySpectrumID[index],m_ArrayWineNameFaked[index],m_ArrayComfrom[index],m_ArrayImportDate[index],m_ArrayManager[index]);

	MessageBox(specInfo,L"光谱信息",MB_ICONINFORMATION);
}

void CClusterDlg::OnCommonCursor()
{
	// TODO: 在此添加命令处理程序代码
	m_ClusterChart.ChangeToSelectMode();
	m_Menu.CheckMenuItem(ID_COMMON_CURSOR,MF_CHECKED);
	m_Menu.CheckMenuItem(ID_MOVE_CURSOR,MF_UNCHECKED);
	m_Menu.CheckMenuItem(ID_ZOOMIN_CURSOR,MF_UNCHECKED);
	m_Menu.CheckMenuItem(ID_ZOOMOUT_CURSOR,MF_UNCHECKED);

}


void CClusterDlg::OnMoveCursor()
{
	// TODO: 在此添加命令处理程序代码
	m_ClusterChart.ChangeToMoveMode();
	m_Menu.CheckMenuItem(ID_COMMON_CURSOR,MF_UNCHECKED);
	m_Menu.CheckMenuItem(ID_MOVE_CURSOR,MF_CHECKED);
	m_Menu.CheckMenuItem(ID_ZOOMIN_CURSOR,MF_UNCHECKED);
	m_Menu.CheckMenuItem(ID_ZOOMOUT_CURSOR,MF_UNCHECKED);
}


void CClusterDlg::OnZoominCursor()
{
	// TODO: 在此添加命令处理程序代码
	m_ClusterChart.ChangeToZoomInMode();
	m_Menu.CheckMenuItem(ID_COMMON_CURSOR,MF_UNCHECKED);
	m_Menu.CheckMenuItem(ID_MOVE_CURSOR,MF_UNCHECKED);
	m_Menu.CheckMenuItem(ID_ZOOMIN_CURSOR,MF_CHECKED);
	m_Menu.CheckMenuItem(ID_ZOOMOUT_CURSOR,MF_UNCHECKED);
}


void CClusterDlg::OnZoomoutCursor()
{
	// TODO: 在此添加命令处理程序代码
	m_ClusterChart.ChangeToZoomOutMode();
	m_Menu.CheckMenuItem(ID_COMMON_CURSOR,MF_UNCHECKED);
	m_Menu.CheckMenuItem(ID_MOVE_CURSOR,MF_UNCHECKED);
	m_Menu.CheckMenuItem(ID_ZOOMIN_CURSOR,MF_UNCHECKED);
	m_Menu.CheckMenuItem(ID_ZOOMOUT_CURSOR,MF_CHECKED);
}

void CClusterDlg::OnBnClickedButtonSelect()
{
	// TODO: 在此添加控件通知处理程序代码
	this->OnCommonCursor();
}


void CClusterDlg::OnBnClickedButtonMove()
{
	// TODO: 在此添加控件通知处理程序代码
	this->OnMoveCursor();
}


void CClusterDlg::OnBnClickedButtonZoomin()
{
	// TODO: 在此添加控件通知处理程序代码
	this->OnZoominCursor();
}


void CClusterDlg::OnBnClickedButtonZoomout()
{
	// TODO: 在此添加控件通知处理程序代码
	this->OnZoomoutCursor();
}

afx_msg LRESULT CClusterDlg::OnChangechartmode(WPARAM wParam, LPARAM lParam)
{

	switch(wParam)
	{
	case CURVE_SELECT:
		{
			m_Menu.CheckMenuItem(ID_COMMON_CURSOR,MF_CHECKED);
			m_Menu.CheckMenuItem(ID_MOVE_CURSOR,MF_UNCHECKED);
			m_Menu.CheckMenuItem(ID_ZOOMIN_CURSOR,MF_UNCHECKED);
			m_Menu.CheckMenuItem(ID_ZOOMOUT_CURSOR,MF_UNCHECKED);
			break;
		}
	case CURVE_MOVE:
		{
			m_Menu.CheckMenuItem(ID_COMMON_CURSOR,MF_UNCHECKED);
			m_Menu.CheckMenuItem(ID_MOVE_CURSOR,MF_CHECKED);
			m_Menu.CheckMenuItem(ID_ZOOMIN_CURSOR,MF_UNCHECKED);
			m_Menu.CheckMenuItem(ID_ZOOMOUT_CURSOR,MF_UNCHECKED);
			break;
		}
	case CURVE_ZOOM_IN:
		{
			m_Menu.CheckMenuItem(ID_COMMON_CURSOR,MF_UNCHECKED);
			m_Menu.CheckMenuItem(ID_MOVE_CURSOR,MF_UNCHECKED);
			m_Menu.CheckMenuItem(ID_ZOOMIN_CURSOR,MF_CHECKED);
			m_Menu.CheckMenuItem(ID_ZOOMOUT_CURSOR,MF_UNCHECKED);
			break;
		}
	case CURVE_ZOOM_OUT:
		{
			m_Menu.CheckMenuItem(ID_COMMON_CURSOR,MF_UNCHECKED);
			m_Menu.CheckMenuItem(ID_MOVE_CURSOR,MF_UNCHECKED);
			m_Menu.CheckMenuItem(ID_ZOOMIN_CURSOR,MF_UNCHECKED);
			m_Menu.CheckMenuItem(ID_ZOOMOUT_CURSOR,MF_CHECKED);
			break;
		}
	default:
		break;
	}

	return 0;
}

void CClusterDlg::OnPrintpreview()
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
afx_msg LRESULT CClusterDlg::OnBeginPrinting(WPARAM wParam, LPARAM lParam)
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
afx_msg LRESULT CClusterDlg::OnMyPrint(WPARAM wParam, LPARAM lParam)
{
	CDC* pDC			= (CDC*)wParam;
	CPrintInfo* pInfo	= (CPrintInfo *)lParam;
	m_ClusterChart.PrintChart(pDC,pInfo);
	return 0;
}
afx_msg LRESULT CClusterDlg::OnEndPrinting(WPARAM wParam, LPARAM lParam)
{
	if(m_fontPrinter.m_hObject!=NULL)
		m_fontPrinter.DeleteObject ();
	return 0;
}

void CClusterDlg::OnPrint()
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
		this->m_ClusterChart.PrintChart(&dc,&pInfo);
		OnEndPrinting((WPARAM)&dc, (LPARAM)&pInfo);
		EndPage(dc);
		//终止打印工作  
		EndDoc(dc);
		dc.DeleteDC();	
	}
}

void CClusterDlg::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CWinApp* pApp=AfxGetApp();
	pApp->m_pMainWnd = m_pMFrame;
	m_Menu.DestroyMenu();
	CDialogEx::OnClose();
}


void CClusterDlg::OnClusterSetting()
{
	// TODO: 在此添加命令处理程序代码
	CClusterSettingDlg ClusterSettingDlg;
	if(IDOK == ClusterSettingDlg.DoModal())
	{
		m_RadioCluMethod = ClusterSettingDlg.m_RadioCluMethod;
		m_eps = ClusterSettingDlg.m_eps;
		m_Minpts = ClusterSettingDlg.m_Minpts;
		m_ClusterNum = ClusterSettingDlg.m_ClusterNum;

		if(m_RadioCluMethod == 0)
		{
			m_ClusterAnalysis.Init(m_TotalDataSet,m_eps,m_Minpts);
			m_ClusterAnalysis.DoDBSCANRecursive();
		}
		else
		{
			m_ClusterAnalysis.Init(m_TotalDataSet,m_ClusterNum,200);    //初始化操作
			m_ClusterAnalysis.DoKmeans();
		}
		ShowClusterResult();
	}
}

void CClusterDlg::ShowClusterResult()
{
	//初始化
	m_ArraySimilaNo.RemoveAll();
	m_AnaSpecClusterID.clear();
	m_ListMatchedFake.DeleteAllItems();
	m_ListTotalFake.DeleteAllItems();
	m_ClusterChart.DestroyCurve();
	//
	int i=0;
	int typeNum = m_ClusterAnalysis.GetTypeNum();
	m_ClusterChart.CluserNum = m_ClusterAnalysis.GetTypeNum();
	//添加假酒光谱聚类结果
	for(;i<m_ArraySpectrumID.GetCount();++i)
	{
		int id = m_ClusterAnalysis.GetPointID(i);
		m_ClusterChart.CreateNewPoint(m_ArraySpectrumID[i],0,true);
		m_ClusterChart.AppendPoint(m_ArraySpectrumID[i],i+1,m_ClusterAnalysis.GetPointID(i),TRUE);
	}
	//添加待分析光谱聚类结果
	for(;i<m_TotalDataSet.m_row;++i)
	{
		int ClusterID = m_ClusterAnalysis.GetPointID(i);
		if((ClusterID!=0))
		{
			m_AnaSpecClusterID.insert(ClusterID);
		}

		m_ClusterChart.CreateNewPoint(m_VectorCurveName[i-m_ArraySpectrumID.GetCount()],0,false);
		m_ClusterChart.AppendPoint(m_VectorCurveName[i-m_ArraySpectrumID.GetCount()],i+1,ClusterID,FALSE);
	}
	m_ClusterChart.ReCalculateAllCurveMaxMin();

	//首先插入全体假酒数据
	for(int i = 0;i<m_ArraySpectrumID.GetCount();++i)
	{
		CString num;
		CString ClusterID;
		num.Format(L"%d",i+1);
		m_ListTotalFake.InsertItem(i,num);
		m_ListTotalFake.SetItemText(i,1,m_ArraySpectrumID[i]);
		ClusterID.Format(L"%d",m_ClusterAnalysis.GetPointID(i));
		m_ListTotalFake.SetItemText(i,2,ClusterID);
	}

	//插入和分析光谱相似的假酒光谱
	int ListNum = 0;
	for(int i = 0;i<m_ArraySpectrumID.GetCount();++i)
	{
		if(m_AnaSpecClusterID.find(m_ClusterAnalysis.GetPointID(i))!=m_AnaSpecClusterID.end())
		{
			CString num;
			CString ClusterID;
			num.Format(L"%d",ListNum+1);
			m_ListMatchedFake.InsertItem(ListNum,num);
			m_ListMatchedFake.SetItemText(ListNum,1,m_ArraySpectrumID[i]);
			ClusterID.Format(L"%d",m_ClusterAnalysis.GetPointID(i));
			m_ListMatchedFake.SetItemText(ListNum,2,ClusterID);
			m_ArraySimilaNo.Add(i);
			++ListNum;
		}
	}
	InvalidateRect(m_rect);
	m_ClusterChart.InvalidateChart();
	m_ClusterChart.SetMargin();
}