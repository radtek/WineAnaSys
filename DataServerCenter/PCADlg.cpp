// PCADlg.cpp : 实现文件
//

#include "stdafx.h"
#include "DataServerCenter.h"
#include "PCADlg.h"
#include "afxdialogex.h"
#include "PrintFrame.h"
#include "SkinPPWTL.h"
// CPCADlg 对话框
static bool LoadSkin = 0;
IMPLEMENT_DYNAMIC(CPCADlg, CDialogEx)

CPCADlg::CPCADlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CPCADlg::IDD, pParent),
	m_pPrintFrame(NULL)
{
	CWinApp* pApp=AfxGetApp();
	m_pMFrame = pApp->m_pMainWnd;

	m_ShowToolBar = true;
}

CPCADlg::~CPCADlg()
{

}

void CPCADlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PCACHART, m_PCAChart);
	DDX_Control(pDX, IDC_BUTTON_SELECT, m_btnSelect);
	DDX_Control(pDX, IDC_BUTTON_MOVE, m_btnMove);
	DDX_Control(pDX, IDC_BUTTON_ZOOMIN, m_btnZommin);
	DDX_Control(pDX, IDC_BUTTON_ZOOMOUT, m_btnZoomout);
}


BEGIN_MESSAGE_MAP(CPCADlg, CDialogEx)
	ON_WM_PAINT()
	ON_COMMAND(ID_PCACOMMON_CURSOR, &CPCADlg::OnPcacommonCursor)
	ON_COMMAND(ID_PCAMOVE_CURSOR, &CPCADlg::OnPcamoveCursor)
	ON_COMMAND(ID_PCAZOOMIN_CURSOR, &CPCADlg::OnPcazoominCursor)
	ON_COMMAND(ID_PCAZOOMOUT_CURSOR, &CPCADlg::OnPcazoomoutCursor)
	ON_MESSAGE(WM_PCACHANGECHARTMODE, &CPCADlg::OnPcachangechartmode)
	ON_COMMAND(ID_PCASHOWTOOLBAR, &CPCADlg::OnPcashowtoolbar)
	ON_COMMAND(ID_PCA_SETTING, &CPCADlg::OnPcaSetting)
	ON_COMMAND(ID_PRINTPREVIEW, &CPCADlg::OnPrintpreview)
	ON_MESSAGE(WM_MY_PRINT, &CPCADlg::OnMyPrint)
	ON_MESSAGE(WM_BEGIN_PRINTING, &CPCADlg::OnBeginPrinting)
	ON_MESSAGE(WM_END_PRINTING, &CPCADlg::OnEndPrinting)
	ON_COMMAND(ID_PRINTPREVIEW, &CPCADlg::OnPrintpreview)
	ON_COMMAND(ID_PRINT, &CPCADlg::OnPrint)
	ON_BN_CLICKED(IDC_BUTTON_SELECT, &CPCADlg::OnBnClickedButtonSelect)
	ON_BN_CLICKED(IDC_BUTTON_MOVE, &CPCADlg::OnBnClickedButtonMove)
	ON_BN_CLICKED(IDC_BUTTON_ZOOMIN, &CPCADlg::OnBnClickedButtonZoomin)
	ON_BN_CLICKED(IDC_BUTTON_ZOOMOUT, &CPCADlg::OnBnClickedButtonZoomout)
	ON_WM_CLOSE()
END_MESSAGE_MAP()


// CPCADlg 消息处理程序


BOOL CPCADlg::OnInitDialog()
{
	skinppRemoveSkin(); //移除皮肤	
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_Menu.LoadMenu(IDR_MENU_PCA); 
    SetMenu(&m_Menu);	


	CRect rect;
    GetDlgItem(IDC_PCACHART)->GetWindowRect(rect);
	this->ScreenToClient(&rect);
	m_PCAChart.MoveWindow(rect);	
	//添加参考图谱PCA
	m_PCAChart.RefSpecType = m_RefName;
	CCurveObj* CurveObject = NULL;

	CFloatPoint *pArray =NULL;
	int count = 0;
	for(int i = 0; i<m_VectorEachRefNameCount.size();++i)
	{
		for(int j = 0;j<m_VectorEachRefNameCount[i];++j)
		{
			pArray = m_PCAChart.CreateNewPCAArray(m_VectorRefName[i],m_MaxPCNum,1);
			for(int k =0;k<m_MaxPCNum;++k)
			{
				double tmp = m_PCAData(j+count+1,k+1);
				pArray[k]=CFloatPoint(k+1,m_PCAData(j+count+1,k+1));
			}
		}
		count += m_VectorEachRefNameCount[i];
	}

	//添加项目光谱PCA
	for(int i = count;i<count+m_AnaSpecNum;++i)
	{
		//CString CurveName = m_VectorCurveName[i-count];

		pArray = m_PCAChart.CreateNewPCAArray(L"项目光谱",m_MaxPCNum,0);

		for(int j =0;j<m_MaxPCNum;++j)
			pArray[j]=CFloatPoint(j+1,m_PCAData(i+1,j+1));
	}

	//显示的PC数目
	m_PCAChart.PCANum = m_PCNumShow;
	if(2 == m_PCNumShow)
	{
		m_PCAChart.XPC = m_xPC;
		m_PCAChart.YPC = m_yPC;
	}

	GetClientRect(&m_rect);
	int xSize=::GetSystemMetrics(SM_CXSCREEN);
	int ySize=::GetSystemMetrics(SM_CYSCREEN);
	int cx=xSize*8/10;
	int cy=ySize*8/10;
	this->MoveWindow((xSize-cx)/2,(ySize-cy)/2,xSize*8/10,ySize*8/10);

	
	m_PCAChart.AdjustTotal2Show(); //调整显示的PCA内容	
	m_PCAChart.InvalidateChart();

	OnPcacommonCursor();
	m_Menu.CheckMenuItem(ID_PCASHOWTOOLBAR,MF_CHECKED);

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


	LoadSkin = 1;
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CPCADlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 在此处添加消息处理程序代码
	// 不为绘图消息调用 CDialogEx::OnPaint()
	GetClientRect(&m_rect);
	m_PCAChart.MoveWindow(0,50,m_rect.Width(),m_rect.Height()-50);
	m_btnSelect.MoveWindow(0,m_rect.top,40,40);
	m_btnMove.MoveWindow(40*1,m_rect.top,40,40);
	m_btnZommin.MoveWindow(40*2,m_rect.top,40,40);
	m_btnZoomout.MoveWindow(40*3,m_rect.top,40,40);

	if(LoadSkin)
	{
		LoadSkin = 0;
		skinppLoadSkin(skin_path); //加载皮肤
	}

}
void CPCADlg::OnPcacommonCursor()
{
	// TODO: 在此添加命令处理程序代码
	m_PCAChart.ChangeToSelectMode();
	m_Menu.CheckMenuItem(ID_PCACOMMON_CURSOR,MF_CHECKED);
	m_Menu.CheckMenuItem(ID_PCAMOVE_CURSOR,MF_UNCHECKED);
	m_Menu.CheckMenuItem(ID_PCAZOOMIN_CURSOR,MF_UNCHECKED);
	m_Menu.CheckMenuItem(ID_PCAZOOMOUT_CURSOR,MF_UNCHECKED);

}


void CPCADlg::OnPcamoveCursor()
{
	// TODO: 在此添加命令处理程序代码
	m_PCAChart.ChangeToMoveMode();
	m_Menu.CheckMenuItem(ID_PCACOMMON_CURSOR,MF_UNCHECKED);
	m_Menu.CheckMenuItem(ID_PCAMOVE_CURSOR,MF_CHECKED);
	m_Menu.CheckMenuItem(ID_PCAZOOMIN_CURSOR,MF_UNCHECKED);
	m_Menu.CheckMenuItem(ID_PCAZOOMOUT_CURSOR,MF_UNCHECKED);
}


void CPCADlg::OnPcazoominCursor()
{
	// TODO: 在此添加命令处理程序代码
	m_PCAChart.ChangeToZoomInMode();
	m_Menu.CheckMenuItem(ID_PCACOMMON_CURSOR,MF_UNCHECKED);
	m_Menu.CheckMenuItem(ID_PCAMOVE_CURSOR,MF_UNCHECKED);
	m_Menu.CheckMenuItem(ID_PCAZOOMIN_CURSOR,MF_CHECKED);
	m_Menu.CheckMenuItem(ID_PCAZOOMOUT_CURSOR,MF_UNCHECKED);
}


void CPCADlg::OnPcazoomoutCursor()
{
	// TODO: 在此添加命令处理程序代码
	m_PCAChart.ChangeToZoomOutMode();
	m_Menu.CheckMenuItem(ID_PCACOMMON_CURSOR,MF_UNCHECKED);
	m_Menu.CheckMenuItem(ID_PCAMOVE_CURSOR,MF_UNCHECKED);
	m_Menu.CheckMenuItem(ID_PCAZOOMIN_CURSOR,MF_UNCHECKED);
	m_Menu.CheckMenuItem(ID_PCAZOOMOUT_CURSOR,MF_CHECKED);
}
void CPCADlg::OnBnClickedButtonSelect()
{
	// TODO: 在此添加控件通知处理程序代码
	this->OnPcacommonCursor();
}


void CPCADlg::OnBnClickedButtonMove()
{
	// TODO: 在此添加控件通知处理程序代码
	this->OnPcamoveCursor();
}


void CPCADlg::OnBnClickedButtonZoomin()
{
	// TODO: 在此添加控件通知处理程序代码
	this->OnPcazoominCursor();
}


void CPCADlg::OnBnClickedButtonZoomout()
{
	// TODO: 在此添加控件通知处理程序代码
	this->OnPcazoomoutCursor();
}
afx_msg LRESULT CPCADlg::OnPcachangechartmode(WPARAM wParam, LPARAM lParam)
{

	switch(wParam)
	{
	case CURVE_SELECT:
		{
			m_Menu.CheckMenuItem(ID_PCACOMMON_CURSOR,MF_CHECKED);
			m_Menu.CheckMenuItem(ID_PCAMOVE_CURSOR,MF_UNCHECKED);
			m_Menu.CheckMenuItem(ID_PCAZOOMIN_CURSOR,MF_UNCHECKED);
			m_Menu.CheckMenuItem(ID_PCAZOOMOUT_CURSOR,MF_UNCHECKED);
			break;
		}
	case CURVE_MOVE:
		{
			m_Menu.CheckMenuItem(ID_PCACOMMON_CURSOR,MF_UNCHECKED);
			m_Menu.CheckMenuItem(ID_PCAMOVE_CURSOR,MF_CHECKED);
			m_Menu.CheckMenuItem(ID_PCAZOOMIN_CURSOR,MF_UNCHECKED);
			m_Menu.CheckMenuItem(ID_PCAZOOMOUT_CURSOR,MF_UNCHECKED);
			break;
		}
	case CURVE_ZOOM_IN:
		{
			m_Menu.CheckMenuItem(ID_PCACOMMON_CURSOR,MF_UNCHECKED);
			m_Menu.CheckMenuItem(ID_PCAMOVE_CURSOR,MF_UNCHECKED);
			m_Menu.CheckMenuItem(ID_PCAZOOMIN_CURSOR,MF_CHECKED);
			m_Menu.CheckMenuItem(ID_PCAZOOMOUT_CURSOR,MF_UNCHECKED);
			break;
		}
	case CURVE_ZOOM_OUT:
		{
			m_Menu.CheckMenuItem(ID_PCACOMMON_CURSOR,MF_UNCHECKED);
			m_Menu.CheckMenuItem(ID_PCAMOVE_CURSOR,MF_UNCHECKED);
			m_Menu.CheckMenuItem(ID_PCAZOOMIN_CURSOR,MF_UNCHECKED);
			m_Menu.CheckMenuItem(ID_PCAZOOMOUT_CURSOR,MF_CHECKED);
			break;
		}
	default:
		break;
	}

	return 0;
}


void CPCADlg::OnPcashowtoolbar()
{
	// TODO: 在此添加命令处理程序代码
	m_ShowToolBar = !m_ShowToolBar;
	if(m_ShowToolBar)
		m_Menu.CheckMenuItem(ID_PCASHOWTOOLBAR,MF_CHECKED);
	else
		m_Menu.CheckMenuItem(ID_PCASHOWTOOLBAR,MF_UNCHECKED);
	this->Invalidate();
}


void CPCADlg::OnPcaSetting()
{
	// TODO: 在此添加命令处理程序代码
	CPCAResettingDlg PCAResettingDlg;

	PCAResettingDlg.TotalPCNum = m_MaxPCNum;
	if(IDOK == PCAResettingDlg.DoModal())
	{
		m_PCAChart.SetShowParameter(PCAResettingDlg.PCNum,PCAResettingDlg.xPC,PCAResettingDlg.yPC);
		m_PCAChart.AdjustTotal2Show();
	}
}
void CPCADlg::OnPrintpreview()
{
	// TODO: 在此添加命令处理程序代码
	if(m_pPrintFrame)
	{
		m_pPrintFrame->DestroyWindow();
		m_pPrintFrame = NULL;
	}
	CPrintFrame* pf=new CPrintFrame(this);

}
afx_msg LRESULT CPCADlg::OnBeginPrinting(WPARAM wParam, LPARAM lParam)
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
afx_msg LRESULT CPCADlg::OnMyPrint(WPARAM wParam, LPARAM lParam)
{
	CDC* pDC			= (CDC*)wParam;
	CPrintInfo* pInfo	= (CPrintInfo *)lParam;
	m_PCAChart.PrintChart(pDC,pInfo);
	return 0;
}
afx_msg LRESULT CPCADlg::OnEndPrinting(WPARAM wParam, LPARAM lParam)
{
	if(m_fontPrinter.m_hObject!=NULL)
		m_fontPrinter.DeleteObject ();
	return 0;
}

void CPCADlg::OnPrint()
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
		this->m_PCAChart.PrintChart(&dc,&pInfo);
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


void CPCADlg::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CWinApp* pApp=AfxGetApp();
	pApp->m_pMainWnd = m_pMFrame;
	CDialogEx::OnClose();
}
