// ClusterDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "DataServerCenter.h"
#include "ClusterDlg.h"
#include "afxdialogex.h"
#include "SkinPPWTL.h"
static bool LoadSkin = 0;
// CClusterDlg 对话框

IMPLEMENT_DYNAMIC(CClusterDlg, CDialogEx)

CClusterDlg::CClusterDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CClusterDlg::IDD, pParent)
{

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
END_MESSAGE_MAP()


// CClusterDlg 消息处理程序


BOOL CClusterDlg::OnInitDialog()
{
	skinppRemoveSkin(); //移除皮肤	
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化

	//获取所有数据
	//CMatrix TotalDataSet;

	//获取假酒图谱数据
	m_FakeWineLib.m_strSort = L"WineNameFaked";
	m_FakeWineLib.Open();
	m_FakeWineLib.MoveFirst();

	m_ArraySpectrumID.Add(m_FakeWineLib.m_SpectrumID);
	m_ArrayWineNameFaked.Add(m_FakeWineLib.m_WineNameFaked);
	m_ArrayComfrom.Add(m_FakeWineLib.m_Comfrom);
	m_ArrayImportDate.Add(m_FakeWineLib.m_ImportDate);
	m_ArrayManager.Add(m_FakeWineLib.m_Manager);

	m_FakeWineLib.MoveNext();

	while(!m_FakeWineLib.IsEOF())
	{
		m_ArraySpectrumID.Add(m_FakeWineLib.m_SpectrumID);
		m_ArrayWineNameFaked.Add(m_FakeWineLib.m_WineNameFaked);
		m_ArrayComfrom.Add(m_FakeWineLib.m_Comfrom);
		m_ArrayImportDate.Add(m_FakeWineLib.m_ImportDate);
		m_ArrayManager.Add(m_FakeWineLib.m_Manager);

		m_FakeWineLib.MoveNext();
	}

	if(m_RadioCluMethod == 0)
	{
		m_ClusterAnalysis.Init(m_TotalDataSet,m_eps,m_Minpts);
		m_ClusterAnalysis.DoDBSCANRecursive();
	}
	else
	{
		m_ClusterAnalysis.Init(m_TotalDataSet,m_ClusterNum,200);    //初始化操作
	}

	int i=0;
	int typeNum = m_ClusterAnalysis.GetTypeNum();
	m_ClusterChart.CluserNum = m_ClusterAnalysis.GetTypeNum();
	//添加假酒光谱聚类结果
	for(;i<m_FakeWineLib.GetRecordCount();++i)
	{
		int id = m_ClusterAnalysis.GetPointID(i);
		m_ClusterChart.CreateNewPoint(m_ArraySpectrumID[i],0,true);
		m_ClusterChart.AppendPoint(m_ArraySpectrumID[i],i+1,m_ClusterAnalysis.GetPointID(i),TRUE);
	}
	//添加待分析光谱聚类结果
	for(;i<m_TotalDataSet.m_row;++i)
	{
		int ClusterID = m_ClusterAnalysis.GetPointID(i);
		if((ClusterID!=0))//&&((i == m_FakeWineLib.GetRecordCount())||(m_AnaSpecClusterID.find(ClusterID)!= m_AnaSpecClusterID.end())))
		{
			m_AnaSpecClusterID.insert(ClusterID);
		}

		m_ClusterChart.CreateNewPoint(m_VectorCurveName[i-m_FakeWineLib.GetRecordCount()],0,false);
		m_ClusterChart.AppendPoint(m_VectorCurveName[i-m_FakeWineLib.GetRecordCount()],i+1,ClusterID,FALSE);
	}
	m_ClusterChart.ReCalculateAllCurveMaxMin();


	//更新列表
	m_ListTotalFake.InsertColumn(0,L"序号",LVCFMT_LEFT,100);
	m_ListTotalFake.InsertColumn(1,L"光谱ID",LVCFMT_LEFT,100);
	m_ListTotalFake.InsertColumn(2,L"聚类结果",LVCFMT_LEFT,100);

	DWORD dwStyle = m_ListTotalFake.GetExtendedStyle(); //获取当前扩展样式
	dwStyle |= LVS_EX_FULLROWSELECT; //选中某行使整行高亮（report风格时）
	dwStyle |= LVS_EX_GRIDLINES; //网格线（report风格时）
	m_ListTotalFake.SetExtendedStyle(dwStyle); //设置扩展风格

	m_ListMatchedFake.InsertColumn(0,L"序号",LVCFMT_LEFT,100);
	m_ListMatchedFake.InsertColumn(1,L"光谱ID",LVCFMT_LEFT,100);
	m_ListMatchedFake.InsertColumn(2,L"聚类结果",LVCFMT_LEFT,100);

	dwStyle = m_ListMatchedFake.GetExtendedStyle(); //获取当前扩展样式
	dwStyle |= LVS_EX_FULLROWSELECT; //选中某行使整行高亮（report风格时）
	dwStyle |= LVS_EX_GRIDLINES; //网格线（report风格时）
	m_ListMatchedFake.SetExtendedStyle(dwStyle); //设置扩展风格


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
			m_ListTotalFake.InsertItem(ListNum,num);
			m_ListTotalFake.SetItemText(ListNum,1,m_ArraySpectrumID[i]);
			ClusterID.Format(L"%d",m_ClusterAnalysis.GetPointID(i));
			m_ListTotalFake.SetItemText(ListNum,2,ClusterID);
			m_ArraySimilaNo.Add(i);
			++ListNum;
		}
	}
	LoadSkin = 1;
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CClusterDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码

	GetClientRect(&m_rect);

	if(m_ClusterChart)
	{
		m_ClusterChart.MoveWindow(0,0,m_rect.Width(),m_rect.Height()*0.7);
	}
	if(m_StaticAll)
	{
		m_StaticAll.MoveWindow(0,m_rect.Height()*0.7,m_rect.Width()/2-5,m_rect.Height()*0.3);
	}
	if(m_ListTotalFake)
	{
		m_ListTotalFake.MoveWindow(5,m_rect.Height()*0.7+20,m_rect.Width()/2-15,m_rect.Height()*0.3-40);
	}
	if(m_StaticSimilarity)
	{
		m_StaticSimilarity.MoveWindow(m_rect.Width()/2+5,m_rect.Height()*0.7,m_rect.Width()/2-5,m_rect.Height()*0.3);
	}
	if(m_ListMatchedFake)
	{
		m_ListMatchedFake.MoveWindow(m_rect.Width()/2+10,m_rect.Height()*0.7+20,m_rect.Width()/2-15,m_rect.Height()*0.3-40);
	}
	this->Invalidate();
}


void CClusterDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 在此处添加消息处理程序代码
	// 不为绘图消息调用 CDialogEx::OnPaint()
	if(LoadSkin)
	{
		LoadSkin = 0;
		skinppLoadSkin(skin_path); //加载皮肤
	}
	m_ClusterChart.MoveWindow(0,0,m_rect.Width(),m_rect.Height()*0.7);
	m_StaticAll.MoveWindow(0,m_rect.Height()*0.7,m_rect.Width()/2-5,m_rect.Height()*0.3);
	m_ListTotalFake.MoveWindow(5,m_rect.Height()*0.7+20,m_rect.Width()/2-15,m_rect.Height()*0.3-40);
	m_StaticSimilarity.MoveWindow(m_rect.Width()/2+5,m_rect.Height()*0.7,m_rect.Width()/2-5,m_rect.Height()*0.3);
	m_ListMatchedFake.MoveWindow(m_rect.Width()/2+10,m_rect.Height()*0.7+20,m_rect.Width()/2-15,m_rect.Height()*0.3-40);
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
