
// MSChartTestDlg.cpp : ʵ���ļ�
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


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
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


// CMSChartTestDlg �Ի���




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


// CMSChartTestDlg ��Ϣ�������

BOOL CMSChartTestDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
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

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	CRect rc;
	GetClientRect(&rc);
	if(!m_Chart.Create(L"�׾�������Ҫ��������Ũ��", WS_CHILD| WS_VISIBLE, rc, this, 10))
	{
		return -1;
	}
	InitChart();
	DrawChart();
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
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

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CMSChartTestDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
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

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CMSChartTestDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

//��Ҫ���ñ����ʽ�����XY�᳤��ɫ�ʵȵ�

void CMSChartTestDlg::InitChart()
{

	// ���ñ��� 
	m_Chart.SetTitleText(L"mschart ʾ�� by thinkry@263.net"); 

	// ��������ı䱳��ɫ 
	m_Chart.GetBackdrop().GetFill().SetStyle(1); 
	m_Chart.GetBackdrop().GetFill().GetBrush().GetFillColor().Set(255, 255, 255); 

	// ��ʾͼ�� 

	m_Chart.SetShowLegend(TRUE); 
	m_Chart.SetColumn(1); 
	m_Chart.SetColumnLabel(L"1�Ż�"); 
	m_Chart.SetColumn(2); 
	m_Chart.SetColumnLabel(L"2�Ż�"); 
	m_Chart.SetColumn(3); 
	m_Chart.SetColumnLabel(L"3�Ż�"); 

	// ջģʽ 
	//m_Chart.SetStacking(TRUE); 

	// Y������ 
	VARIANT var; 
	m_Chart.GetPlot().GetAxis(1,var).GetValueScale().SetAuto(TRUE); // ���Զ���עY��̶� 
	m_Chart.GetPlot().GetAxis(1,var).GetValueScale().SetMaximum(100); // Y�����̶� 
	m_Chart.GetPlot().GetAxis(1,var).GetValueScale().SetMinimum(0); // Y����С�̶� 
	m_Chart.GetPlot().GetAxis(1,var).GetValueScale().SetMajorDivision(5); // Y��̶�5�ȷ� 
	m_Chart.GetPlot().GetAxis(1,var).GetValueScale().SetMinorDivision(1); // ÿ�̶�һ���̶��� 
	m_Chart.GetPlot().GetAxis(1,var).GetAxisTitle().SetText(L"Сʱ"); // Y������ 

	//m_Chart.GetPlot().GetAxis(1,var).GetAxisTitle().GetVtFont().GetVtColor().Set(0,255,0);
	//m_Chart.GetPlot().GetAxis(1,var).GetAxisTitle().GetVtFont().SetName(L"Arial Unicode MS");
	// 3������ 
	m_Chart.SetColumnCount(3);  

	// ��ɫ 
	m_Chart.GetPlot().GetSeriesCollection().GetItem(1).GetPen().GetVtColor().Set(0, 0, 255); 
	m_Chart.GetPlot().GetSeriesCollection().GetItem(2).GetPen().GetVtColor().Set(255, 0, 0); 
	m_Chart.GetPlot().GetSeriesCollection().GetItem(3).GetPen().GetVtColor().Set(0, 255, 0); 

	// �߿�(�Ե���ͼ��Ч) 
	m_Chart.GetPlot().GetSeriesCollection().GetItem(1).GetPen().SetWidth(50); 
	m_Chart.GetPlot().GetSeriesCollection().GetItem(2).GetPen().SetWidth(100); 
	m_Chart.GetPlot().GetSeriesCollection().GetItem(3).GetPen().SetWidth(2); 

	// ���ݵ�������ʾ����ֵ��ģʽ(������״ͼ�͵���ͼ��Ч) 
	// 0: ����ʾ 1: ��ʾ����״ͼ�� 
	// 2: ��ʾ����״ͼ���Ϸ� 3: ��ʾ����״ͼ���м� 4: ��ʾ����״ͼ���·� 
	m_Chart.GetPlot().GetSeriesCollection().GetItem(1).GetDataPoints().GetItem(-1).GetDataPointLabel().SetLocationType(1); 
	m_Chart.GetPlot().GetSeriesCollection().GetItem(2).GetDataPoints().GetItem(-1).GetDataPointLabel().SetLocationType(1); 
	m_Chart.GetPlot().GetSeriesCollection().GetItem(3).GetDataPoints().GetItem(-1).GetDataPointLabel().SetLocationType(1);
	//�Ŵ���С
	
}

void CMSChartTestDlg::DrawChart()
{
	int nRowCount = 6;
	m_Chart.SetRowCount(nRowCount);

	VARIANT var;
	m_Chart.GetPlot().GetAxis(0,var).GetCategoryScale().SetAuto(FALSE);			// ���Զ���עX��̶�
	m_Chart.GetPlot().GetAxis(0,var).GetCategoryScale().SetDivisionsPerLabel(1);// ÿ�̶�һ����ע
	m_Chart.GetPlot().GetAxis(0,var).GetCategoryScale().SetDivisionsPerTick(1); // ÿ�̶�һ���̶���
	m_Chart.GetPlot().GetAxis(0,var).GetAxisTitle().SetText(L"����");			// X������

	//char buf[32];
	srand( (unsigned)time( NULL ) );
	CString Label;
	for(int row = 1; row <= nRowCount; ++row)
	{
		m_Chart.SetRow(row);
		Label.Format(L"%d��", row);

		m_Chart.SetRowLabel(Label);
		m_Chart.GetDataGrid().SetData(row, 1, rand() * 100 / RAND_MAX, 0);
		m_Chart.GetDataGrid().SetData(row, 2, rand() * 100 / RAND_MAX, 0);
		m_Chart.GetDataGrid().SetData(row, 3, rand() * 100 / RAND_MAX, 0);
	}

	m_Chart.Refresh();	
}
