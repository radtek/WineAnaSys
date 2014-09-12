// ClusterDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Client.h"
#include "ClusterDlg.h"
#include "afxdialogex.h"

// CClusterDlg �Ի���

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


// CClusterDlg ��Ϣ�������


BOOL CClusterDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��

	//��ȡ��������
	//CMatrix TotalDataSet;

	//��ȡ�پ�ͼ������
	m_FakeWineLib.m_strSort = L"WineNameFaked";
	m_FakeWineLib.Open();
	m_FakeWineLib.MoveFirst();
	//CMatrix temp(1,SPEC_LENTH);
	//CString2Matrix(temp,m_FakeWineLib.m_SpectrumData,L",");

	//m_TotalDataSet.AddRows(temp);
	m_ArraySpectrumID.Add(m_FakeWineLib.m_SpectrumID);
	m_ArrayWineNameFaked.Add(m_FakeWineLib.m_WineNameFaked);
	m_ArrayComfrom.Add(m_FakeWineLib.m_Comfrom);
	m_ArrayImportDate.Add(m_FakeWineLib.m_ImportDate);
	m_ArrayManager.Add(m_FakeWineLib.m_Manager);

	m_FakeWineLib.MoveNext();

	//CArray<CString,CString> m_ArraySpectrumID;
	//CArray<CString,CString> m_ArrayWineNameFaked;
	//CArray<CString,CString> m_ArrayComfrom;
	//CArray<CString,CString> m_ArrayImportDate;
	//CArray<CString,CString> m_ArrayManager;

	while(!m_FakeWineLib.IsEOF())
	{
		//CString2Matrix(temp,m_FakeWineLib.m_SpectrumData,L",");

		//m_TotalDataSet.AddRows(temp);
		m_ArraySpectrumID.Add(m_FakeWineLib.m_SpectrumID);
		m_ArrayWineNameFaked.Add(m_FakeWineLib.m_WineNameFaked);
		m_ArrayComfrom.Add(m_FakeWineLib.m_Comfrom);
		m_ArrayImportDate.Add(m_FakeWineLib.m_ImportDate);
		m_ArrayManager.Add(m_FakeWineLib.m_Manager);

		m_FakeWineLib.MoveNext();
	}

	//m_TotalDataSet.AddRows(m_AnaDataSet);
	if(m_RadioCluMethod == 0)
	{
		m_ClusterAnalysis.Init(m_TotalDataSet,m_eps,m_Minpts);
		m_ClusterAnalysis.DoDBSCANRecursive();
	}
	else
	{
		m_ClusterAnalysis.Init(m_TotalDataSet,m_ClusterNum,200);    //��ʼ������
	}

	int i=0;
	int typeNum = m_ClusterAnalysis.GetTypeNum();
	m_ClusterChart.CluserNum = m_ClusterAnalysis.GetTypeNum();
	//int id;
	for(;i<m_FakeWineLib.GetRecordCount();++i)
	{
		//id = myClusterAnalysis.GetPointID(i);
		m_ClusterChart.CreateNewPoint(m_ArraySpectrumID[i],0,true);
		m_ClusterChart.AppendPoint(m_ArraySpectrumID[i],i+1,m_ClusterAnalysis.GetPointID(i),TRUE);
	}
	
	for(;i<m_TotalDataSet.m_row;++i)
	{
		int ClusterID = m_ClusterAnalysis.GetPointID(i);
		if((ClusterID!=0)&&(m_AnaSpecClusterID.find(ClusterID)!= m_AnaSpecClusterID.end()))
		{
			m_AnaSpecClusterID.insert(ClusterID);
		}

		m_ClusterChart.CreateNewPoint(m_VectorCurveName[i-m_FakeWineLib.GetRecordCount()],0,false);
		m_ClusterChart.AppendPoint(m_VectorCurveName[i-m_FakeWineLib.GetRecordCount()],i+1,ClusterID,FALSE);
	}
	m_ClusterChart.ReCalculateAllCurveMaxMin();


	//�����б�
	m_ListTotalFake.InsertColumn(0,L"���",LVCFMT_LEFT,100);
	m_ListTotalFake.InsertColumn(1,L"����ID",LVCFMT_LEFT,100);
	m_ListTotalFake.InsertColumn(2,L"������",LVCFMT_LEFT,100);
	//m_ListTotalFake.InsertColumn(0,L"",LVCFMT_LEFT,20);

	DWORD dwStyle = m_ListTotalFake.GetExtendedStyle(); //��ȡ��ǰ��չ��ʽ
	dwStyle |= LVS_EX_FULLROWSELECT; //ѡ��ĳ��ʹ���и�����report���ʱ��
	dwStyle |= LVS_EX_GRIDLINES; //�����ߣ�report���ʱ��
	//dwStyle |= LVS_EX_FULLROWSELECT; //
	m_ListTotalFake.SetExtendedStyle(dwStyle); //������չ���

	m_ListMatchedFake.InsertColumn(0,L"���",LVCFMT_LEFT,100);
	m_ListMatchedFake.InsertColumn(1,L"����ID",LVCFMT_LEFT,100);
	m_ListMatchedFake.InsertColumn(2,L"������",LVCFMT_LEFT,100);

	dwStyle = m_ListMatchedFake.GetExtendedStyle(); //��ȡ��ǰ��չ��ʽ
	dwStyle |= LVS_EX_FULLROWSELECT; //ѡ��ĳ��ʹ���и�����report���ʱ��
	dwStyle |= LVS_EX_GRIDLINES; //�����ߣ�report���ʱ��
	//dwStyle |= LVS_EX_FULLROWSELECT; //
	m_ListMatchedFake.SetExtendedStyle(dwStyle); //������չ���


	//���Ȳ���ȫ��پ�����
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

	//����ͷ����������Ƶļپƹ���
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




	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}


void CClusterDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO: �ڴ˴������Ϣ����������

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
	// TODO: �ڴ˴������Ϣ����������
	// ��Ϊ��ͼ��Ϣ���� CDialogEx::OnPaint()
	m_ClusterChart.MoveWindow(0,0,m_rect.Width(),m_rect.Height()*0.7);
	m_StaticAll.MoveWindow(0,m_rect.Height()*0.7,m_rect.Width()/2-5,m_rect.Height()*0.3);
	m_ListTotalFake.MoveWindow(5,m_rect.Height()*0.7+20,m_rect.Width()/2-15,m_rect.Height()*0.3-40);
	m_StaticSimilarity.MoveWindow(m_rect.Width()/2+5,m_rect.Height()*0.7,m_rect.Width()/2-5,m_rect.Height()*0.3);
	m_ListMatchedFake.MoveWindow(m_rect.Width()/2+10,m_rect.Height()*0.7+20,m_rect.Width()/2-15,m_rect.Height()*0.3-40);
}


void CClusterDlg::OnNMDblclkListMatchedfake(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	*pResult = 0;
	if(0 == m_ListMatchedFake.GetSelectedCount())
		return;
	int index = m_ArraySimilaNo[m_ListMatchedFake.GetSelectionMark()];

	CString specInfo;
	specInfo.Format(L"���ױ�ţ�%s\n��ð�׾����ƣ�%s\n��Դ˵����%s\n���ʱ�䣺%s\n����Ա��%s",
		m_ArraySpectrumID[index],m_ArrayWineNameFaked[index],m_ArrayComfrom[index],m_ArrayImportDate[index],m_ArrayManager[index]);
	MessageBox(specInfo,L"������Ϣ",MB_ICONINFORMATION);
}


void CClusterDlg::OnNMDblclkListTotalfake(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	*pResult = 0;
	*pResult = 0;

	if(0 == m_ListTotalFake.GetSelectedCount())
		return;
	int index = m_ListTotalFake.GetSelectionMark();
	CString specInfo;
	specInfo.Format(L"���ױ�ţ�%s\n��ð�׾����ƣ�%s\n��Դ˵����%s\n���ʱ�䣺%s\n����Ա��%s",
		m_ArraySpectrumID[index],m_ArrayWineNameFaked[index],m_ArrayComfrom[index],m_ArrayImportDate[index],m_ArrayManager[index]);

	MessageBox(specInfo,L"������Ϣ",MB_ICONINFORMATION);



}

