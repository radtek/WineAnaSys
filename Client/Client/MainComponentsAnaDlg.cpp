// MainComponentsAnaDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Client.h"
#include "MainComponentsAnaDlg.h"
#include "afxdialogex.h"

#include "ExcelOperator.h"
#include "PrintFrame.h"
#include "PrintPreviewView.h"
#include "PrintView.h"
#include "ComponentPredictModel.h"
#include "ClusterSettingDlg.h"
#include "CSVOperator.h"
#include "PCADlg.h"
#include "PCASettingDlg.h"

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

// CMainComponentsAnaDlg �Ի���

IMPLEMENT_DYNAMIC(CMainComponentsAnaDlg, CDialogEx)

	CMainComponentsAnaDlg::CMainComponentsAnaDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CMainComponentsAnaDlg::IDD, pParent)
	,m_pPrintFrame(0)
	,m_Offset(1,COMPONENT_COUNT)
	,m_CurMaxValueShow(0.0)
	,m_bShowAnaComSetting(false)
	,m_pCImportProjectDataProgressDlg(0)
{
	m_ComponentArray.Add(L"��ȩ");
	m_ComponentArray.Add(L"��������");
	m_ComponentArray.Add(L"�״�");
	m_ComponentArray.Add(L"��������");
	m_ComponentArray.Add(L"������");
	m_ComponentArray.Add(L"�춡��");
	m_ComponentArray.Add(L"��������");
	m_ComponentArray.Add(L"������");
	m_ComponentArray.Add(L"���촼");
	m_ComponentArray.Add(L"��������");
	m_ComponentArray.Add(L"��������");
	m_ComponentArray.Add(L"��������");
	m_ComponentArray.Add(L"��ȩ");
	m_ComponentArray.Add(L"����");
	m_ComponentArray.Add(L"��-���Ҵ�");
	m_ComponentArray.Add(L"����");
	m_ComponentArray.Add(L"����");

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
	ON_COMMAND(ID_CLOSE, &CMainComponentsAnaDlg::OnClose)
END_MESSAGE_MAP()


// CMainComponentsAnaDlg ��Ϣ�������


BOOL CMainComponentsAnaDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	//��Ӳ˵�
	m_Menu.LoadMenu(IDR_MENU_COMPONENT); 
	SetMenu(&m_Menu);

	m_ListCtrlAnaSpec.InsertColumn(0,L"��Ŀ����",LVCFMT_LEFT,100);
	m_ListCtrlAnaSpec.InsertColumn(1,L"�����ļ�",LVCFMT_LEFT,100);
	m_ListCtrlAnaSpec.InsertColumn(2,L"��Դ���",LVCFMT_LEFT,100);

	DWORD dwStyle = m_ListCtrlAnaComponents.GetExtendedStyle(); //��ȡ��ǰ��չ��ʽ
	dwStyle |= LVS_EX_FULLROWSELECT; //ѡ��ĳ��ʹ���и�����report���ʱ��
	dwStyle |= LVS_EX_CHECKBOXES; //itemǰ����checkbox�ؼ�
	m_ListCtrlAnaComponents.InsertColumn(0,L"��������",LVCFMT_LEFT,300);
	m_ListCtrlAnaComponents.SetExtendedStyle(dwStyle);
	m_ListCtrlAnaSpec.SetExtendedStyle(dwStyle);

	m_ListSpecFromLib.InsertColumn( 0, L"", LVCFMT_LEFT,30);//������ 
	m_ListSpecFromLib.InsertColumn( 1, L"���ױ��", LVCFMT_LEFT, 100 );//������ 
	m_ListSpecFromLib.InsertColumn( 2, L"(��ð)�׾�����", LVCFMT_LEFT, 150 );//������ 
	m_ListSpecFromLib.InsertColumn( 3, L"��Դ˵��", LVCFMT_LEFT, 100 );//������
	m_ListSpecFromLib.InsertColumn( 4, L"�������", LVCFMT_LEFT, 100 );//������
	m_ListSpecFromLib.InsertColumn( 5, L"������Ա", LVCFMT_LEFT, 100 );//������
	m_ListSpecFromLib.SetExtendedStyle(dwStyle);

	//��ȡ��Ŀ����·��
	CFileFind finder;
	CString SearchPath=m_projectPath;
	SearchPath+=L"\\*.*";
	BOOL bWorking=finder.FindFile(SearchPath);
	while(bWorking)
	{
		bWorking=finder.FindNextFile();
		CString str=finder.GetFileName();
		if(str.Right(4)==L".CSV"||str.Right(4)==L".csv" &&L"." !=str &&L".." != str)//ע��þ���Ҫ�ų���.����..��
		{
			m_ArraySpecPath.Add(finder.GetFilePath());
			m_ArraySpecName.Add(finder.GetFileTitle());
		}
	}
	//����
	SortCArrayByFirst(m_ArraySpecName,m_ArraySpecPath);
	//�б���ʾ

	CString szResultPath=m_projectPath;
	szResultPath+=L"\\��Դ���.xlsx";

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

	LOGFONT lf; //��������ṹ
	lf.lfWeight=0; //�������=10
	lf.lfHeight=25;	 //����߶�(��ת���������)=56
	lf.lfWidth=25;   //������(��ת�������߶�)=20
	lf.lfUnderline=FALSE; //���»���
	lf.lfStrikeOut=FALSE; //��ɾ����
	lf.lfItalic=FALSE; //��б��
	lf.lfEscapement=0; //������ʾ�Ƕ�=90��
	lf.lfCharSet=DEFAULT_CHARSET; //ʹ��ȱʡ�ַ���
	//wcscpy(lf.lfFaceName,L"Times New Roman"); //������=@system
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

	//��ʼ���ɷ�Ԥ��ģ��ϵ����ƫ��
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

	//����ͼ��
	if(!m_MsChart.Create(L"mschart", WS_CHILD| WS_VISIBLE, CRect(0,0,10,10), this, 10))
	{
		return FALSE;
	}
	//���öԻ���ͼ��
	HICON m_hIcon;
	m_hIcon=AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	SetIcon(m_hIcon,TRUE); //����Ϊ��ͼ��

	//���ô�С
	int xSize=::GetSystemMetrics(SM_CXSCREEN);
	int ySize=::GetSystemMetrics(SM_CYSCREEN);
	int cx=xSize*9/10;
	int cy=ySize*9/10;
	this->MoveWindow((xSize-cx)/2,(ySize-cy)/2,xSize*9/10,ySize*9/10);
	InitChart();

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}



void CMainComponentsAnaDlg::InitChart()
{
	// ���ñ��� 
	m_MsChart.SetTitleText(L"��������Ũ��"); 
	// ��������ı䱳��ɫ 
	m_MsChart.GetBackdrop().GetFill().SetStyle(1); 
	m_MsChart.GetBackdrop().GetFill().GetBrush().GetFillColor().Set(255, 255, 255); 

	// ��ʾͼ�� 
	m_MsChart.SetShowLegend(TRUE); 

	// Y������ 
	VARIANT var; 
	m_MsChart.GetPlot().GetAxis(1,var).GetValueScale().SetAuto(FALSE); // ���Զ���עY��̶� TRUE
	m_MsChart.GetPlot().GetAxis(1,var).GetValueScale().SetMaximum(m_CurMaxValueShow); // Y�����̶� 
	m_MsChart.GetPlot().GetAxis(1,var).GetValueScale().SetMinimum(0); // Y����С�̶� 
	m_MsChart.GetPlot().GetAxis(1,var).GetValueScale().SetMajorDivision(5); // Y��̶�5�ȷ� 
	m_MsChart.GetPlot().GetAxis(1,var).GetValueScale().SetMinorDivision(1); // ÿ�̶�һ���̶���
	//m_MsChart.GetPlot().GetAxis(1,var).GetAxisTitle().SetText(L"����Ũ��g/L"); // Y������ 


	m_MsChart.GetPlot().GetAxis(0,var).GetCategoryScale().SetAuto(FALSE);			// ���Զ���עX��̶�
	m_MsChart.GetPlot().GetAxis(0,var).GetCategoryScale().SetDivisionsPerLabel(1);// ÿ�̶�һ����ע
	m_MsChart.GetPlot().GetAxis(0,var).GetCategoryScale().SetDivisionsPerTick(1); // ÿ�̶�һ���̶���
	m_MsChart.GetPlot().GetAxis(0,var).GetAxisTitle().SetText(L"��������");			// X������

	DrawChartSample();
}

void CMainComponentsAnaDlg::DrawChartSample()
{
	m_MsChart.SetRowCount(COMPONENT_COUNT);
	m_MsChart.SetColumnCount(1);
	m_MsChart.SetColumn(1); 
	m_MsChart.SetColumnLabel(L"����ʾ��");

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
				// ���ݵ�������ʾ����ֵ��ģʽ(������״ͼ�͵���ͼ��Ч) 
				// 0: ����ʾ 1: ��ʾ����״ͼ�� 
				// 2: ��ʾ����״ͼ���Ϸ� 3: ��ʾ����״ͼ���м� 4: ��ʾ����״ͼ���·� 
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
				// ���ݵ�������ʾ����ֵ��ģʽ(������״ͼ�͵���ͼ��Ч) 
				// 0: ����ʾ 1: ��ʾ����״ͼ�� 
				// 2: ��ʾ����״ͼ���Ϸ� 3: ��ʾ����״ͼ���м� 4: ��ʾ����״ͼ���·� 
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
				// ���ݵ�������ʾ����ֵ��ģʽ(������״ͼ�͵���ͼ��Ч) 
				// 0: ����ʾ 1: ��ʾ����״ͼ�� 
				// 2: ��ʾ����״ͼ���Ϸ� 3: ��ʾ����״ͼ���м� 4: ��ʾ����״ͼ���·� 
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
				// ���ݵ�������ʾ����ֵ��ģʽ(������״ͼ�͵���ͼ��Ч) 
				// 0: ����ʾ 1: ��ʾ����״ͼ�� 
				// 2: ��ʾ����״ͼ���Ϸ� 3: ��ʾ����״ͼ���м� 4: ��ʾ����״ͼ���·� 
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
	m_MsChart.GetPlot().GetAxis(1,var).GetValueScale().SetMaximum(m_CurMaxValueShow); // Y�����̶� 
	m_MsChart.Refresh();	
}

void CMainComponentsAnaDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: �ڴ˴������Ϣ����������
	// ��Ϊ��ͼ��Ϣ���� CDialogEx::OnPaint()
	CRect rc;
	GetClientRect(&rc);
	CFont Font;
	LOGFONT LogFont;
	LogFont.lfWeight=0; //�������=10
	LogFont.lfHeight=0;	 //����߶�(��ת���������)=56
	LogFont.lfWidth=0;   //������(��ת�������߶�)=20
	LogFont.lfUnderline=FALSE; //���»���
	LogFont.lfStrikeOut=FALSE; //��ɾ����
	LogFont.lfItalic=FALSE; //��б��
	LogFont.lfCharSet=DEFAULT_CHARSET; //ʹ��ȱʡ�ַ���
	LogFont.lfEscapement=900; //������ʾ�Ƕ�=90��
	wcscpy(LogFont.lfFaceName,L"΢���ź�"); //������=@system_GB2312
	Font.CreateFontIndirect(&LogFont);
	CFont *oldFont = dc.SelectObject(&Font);
	COLORREF oldColor = dc.SetTextColor(RGB(0,0,0));
	CSize strsize = dc.GetTextExtent(L"����Ũ�ȣ�g/L��");
	dc.FillSolidRect(305,0,(strsize.cy+5),rc.Height(),RGB(255,255,255));
	dc.SetBkMode(TRANSPARENT);
	dc.TextOut(305,rc.Height()/2 + strsize.cx/2,L"����Ũ�ȣ�g/L��");
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

	// TODO: �ڴ˴������Ϣ����������
	Invalidate();
}


void CMainComponentsAnaDlg::OnNMClickListProspectrum(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	DWORD dwPos = GetMessagePos();  
	CPoint point( LOWORD(dwPos), HIWORD(dwPos) );  
	m_ListCtrlAnaSpec.ScreenToClient(&point);  
	LVHITTESTINFO lvinfo;  
	lvinfo.pt = point;  
	lvinfo.flags = LVHT_ABOVE;  

	UINT nFlag;  
	int nItem = m_ListCtrlAnaSpec.HitTest(point, &nFlag);  
	//�ж��Ƿ����checkbox��  
	if(nFlag == LVHT_ONITEMSTATEICON)  
	{
		//ִ��checkbox��Ӧ����
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

				for(int i = 0;i<tmp_Component.m_column;++i)
				{
					if(tmp_Component(1,i+1)<0)
					{
						if(abs(tmp_Component(1,i+1))<10e-3)
						{
							tmp_Component.SetItem(1,i+1,0);
						}		
					}
				}


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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	DWORD dwPos = GetMessagePos();  
	CPoint point( LOWORD(dwPos), HIWORD(dwPos) );  
	m_ListCtrlAnaSpec.ScreenToClient(&point);  
	LVHITTESTINFO lvinfo;  
	lvinfo.pt = point;  
	lvinfo.flags = LVHT_ABOVE;  

	UINT nFlag;  
	int nItem = m_ListCtrlAnaSpec.HitTest(point, &nFlag);  
	//�ж��Ƿ����checkbox��  
	if(nFlag == LVHT_ONITEMSTATEICON)  
	{
		//ִ��checkbox��Ӧ����
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

				for(int i = 0;i<tmp_Component.m_column;++i)
				{
					if(tmp_Component(1,i+1)<0)
					{
						if(abs(tmp_Component(1,i+1))<10e-3)
						{
							tmp_Component.SetItem(1,i+1,0);
						}		
					}
				}

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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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
		//ִ��checkbox��Ӧ����
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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
		//ִ��checkbox��Ӧ����
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
	//�ж��Ƿ����checkbox��  
	if(nFlag == LVHT_ONITEMSTATEICON)  
	{
		bool bcheck = 1;
		if(m_ListCtrlAnaComponents.GetCheck(nItem))
		{
			bcheck = 0;
		}
		DrawChart(nItem,bcheck,0);
	}	
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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
	//�ж��Ƿ����checkbox��  
	if(nFlag == LVHT_ONITEMSTATEICON)  
	{
		bool bcheck = 1;
		if(m_ListCtrlAnaComponents.GetCheck(nItem))
		{
			bcheck = 0;
		}
		DrawChart(nItem,bcheck,0);
	}	
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	*pResult = 0;
}


void CMainComponentsAnaDlg::OnPrint()
{
	// TODO: �ڴ���������������
	HANDLE hDib; 
	HPALETTE hPalette;
	m_MsChart.GetPlot().GetBackdrop()
		.GetFill().GetBrush().GetFillColor().Set(255, 255, 255);
	m_MsChart.GetPlot().GetBackdrop().GetShadow().SetStyle(FALSE);//��Ӱ

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
	m_MsChart.GetPlot().GetBackdrop().GetShadow().SetStyle(TRUE);//��Ӱ

	CPrintDialog mypd(FALSE);// �����ӡ���öԻ������
	mypd.GetDefaults();  // ��ȡȱʡ��ӡ�豸���豸����
	DEVMODE *dm = mypd.GetDevMode();// ��ȡȱʡ��ӡ����DEVMODE����
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
	int nHorRes = GetDeviceCaps(pd.hDC, HORZRES);//�����豸��������ʾ���mm 
	int nVerRes = GetDeviceCaps(pd.hDC, VERTRES);//�����豸��������ʾ�߶�mm

	if(pd.hDC != NULL)
	{
		DOCINFO di;   
		di.cbSize = sizeof(DOCINFO);  
		di.lpszDocName = L"ͼ���ӡ";
		di.lpszOutput = (LPTSTR)NULL;  
		di.lpszDatatype = (LPTSTR)NULL; 
		di.fwType = 0;
		pDC.StartDoc(&di); //֪ͨ��ӡ����������ִ��һ�µĴ�ӡ����  
		pDC.StartPage();   //֪ͨ��ӡ�����������ӡ��ҳ  
		SetMapMode(pd.hDC, MM_TEXT);//���õ�ǰӰ��ģʽΪ����λ0.001Ӣ��
		CRect rectPrint(0, 0, nHorRes, nVerRes);  

		//��ӡͼ��
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

		EndPage(pd.hDC); //֪ͨ��ӡ����������ҳ����  
		EndDoc(pd.hDC); 
		pDC.DeleteDC();
	}
}


void CMainComponentsAnaDlg::OnPrintpreview()
{
	// TODO: �ڴ���������������
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
	m_MsChart.GetPlot().GetBackdrop().GetShadow().SetStyle(FALSE);//��Ӱ

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
	m_MsChart.GetPlot().GetBackdrop().GetShadow().SetStyle(TRUE);//��Ӱ

	//��ӡͼ��
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
	// TODO: �ڴ���������������
	//m_bShowAnaComSetting = !m_bShowAnaComSetting;
	Invalidate();
}


void CMainComponentsAnaDlg::OnUpdateAnacomponentsetting(CCmdUI *pCmdUI)
{
	// TODO: �ڴ������������û����洦��������
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
	// TODO: �ڴ���������������
	OnBnClickedButtonImportspecfromlib();
}


void CMainComponentsAnaDlg::OnBnClickedButtonImportspecfromlib()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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
				//�����Ա��´�ʹ�ò������µ���
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
				MessageBox(pCMainComponentsAnaDlg->m_hWnd,L"���Ӽپƿ�ʧ�ܣ�",L"�پƾ���",MB_ICONERROR);
				return -1;
			}
			if(FakeWineLib.GetRecordCount() == 0)
			{
				MessageBox(pCMainComponentsAnaDlg->m_hWnd,L"�پƿ��޼�¼��",L"�پƾ���",MB_ICONWARNING);
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
			//ȡ�ô�������������
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
					//�����Ա��´�ʹ�ò������µ���
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
			m_ClusterDlg.m_ClusterAnalysis.Init(m_ClusterDlg.m_TotalDataSet,m_ClusterDlg.m_ClusterNum,200);    //��ʼ������
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

	//����PCA
	CMatrix signals(TotalSpecComponentConcentration.m_row,TotalSpecComponentConcentration.m_column);
	CMatrix PC(TotalSpecComponentConcentration.m_column,TotalSpecComponentConcentration.m_column);
	CMatrix V(1,TotalSpecComponentConcentration.m_column);

	int flag=TotalSpecComponentConcentration.PCA(signals,PC,V);
	//��ȡ���ɷָ���
	int MaxPCNum = 0;
	while(0!=V(1,MaxPCNum+1))++MaxPCNum;

	//��ʾ
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
	// TODO: �ڴ���������������
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
	// TODO: �ڴ���������������
	isPCA = false;
	TotalDataset.~CMatrix();

	CFakeWineLib FakeWineLib;
	FakeWineLib.m_strSort = L"WineNameFaked";
	if(!FakeWineLib.Open())
	{
		MessageBox(L"���Ӽپƿ�ʧ�ܣ�",L"�پƾ���",MB_ICONERROR);
		return;
	}
	if(FakeWineLib.GetRecordCount() == 0)
	{
		MessageBox(L"�پƿ��޼�¼��",L"�پƾ���",MB_ICONWARNING);
		return;
	}
	CClusterSettingDlg ClusterSettingDlg;
	if(IDOK == ClusterSettingDlg.DoModal())
	{
		m_RadioCluMethod = ClusterSettingDlg.m_RadioCluMethod;
		m_eps = ClusterSettingDlg.m_eps;
		m_Minpts = ClusterSettingDlg.m_Minpts;
		m_ClusterNum = ClusterSettingDlg.m_ClusterNum;

		//ȡ�ô�������������

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
	//����Ԥ����

	CMatrix Offset;
	for(int i = 0; i < TotalDataset.m_row;++i)
	{
		Offset.AddRows(m_Offset);
	}
	CMatrix TotalSpecComponentConcentration = TotalDataset*m_Weight+Offset;

	//����PCA
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

	m_ClusterDlg.m_RadioCluMethod = m_RadioCluMethod; //�������
	m_ClusterDlg.m_eps = m_eps;         //ɨ��뾶
	m_ClusterDlg.m_Minpts = m_Minpts;         //��С����
	m_ClusterDlg.m_ClusterNum = m_ClusterNum;     //�������
	PostMessage(WM_BEGINCLUSTER,0,0);
}

afx_msg LRESULT CMainComponentsAnaDlg::OnBegincluster(WPARAM wParam, LPARAM lParam)
{
	this->m_pShowInfoDlg = new CShowInfoDlg;
	m_pShowInfoDlg->Create(IDD_DIALOG_SHOWINFO,this);
	m_pShowInfoDlg->ShowWindow(SW_SHOW);
	m_pShowInfoDlg->m_StaticInfoShow = L"���ڽ��мپ��ѵ���࣬�����ĵȴ�";
	m_pShowInfoDlg->SetWindowText(L"�پ��ѵ����");
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


void CMainComponentsAnaDlg::OnClose()
{
	// TODO: �ڴ���������������
}
