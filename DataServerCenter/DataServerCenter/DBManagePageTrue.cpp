// DBManagePageTrue.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "DataServerCenter.h"
#include "DBManagePageTrue.h"
#include "afxdialogex.h"
#include "ExcelOperator.h"
#include "InputSpecDatasetInfoDlg.h"
#include "CompleteSpecInfoAddToLibDlg.h"
#include "AlcoholTypeIndex.h"
#include "FlavourTypeIndex.h"
#include "BrandTypeIndex.h"
#include "WineTypeIndex.h"
#include "PLS.h"
#include "PLSModel.h"
#include "ComponentPredictModel.h"
#include "ConcentrationTraceBackModel.h"
#include <map>
using std::map;
extern CString g_CurrentDir;
// CDBManagePageTrue �Ի���

IMPLEMENT_DYNAMIC(CDBManagePageTrue, CPropertyPage)

CDBManagePageTrue::CDBManagePageTrue()
	: CPropertyPage(CDBManagePageTrue::IDD)
	, m_Field1(_T(""))
	, m_AndOr1(_T(""))
	, m_Field2(_T(""))
	, m_AndOr2(_T(""))
	, m_Field3(_T(""))
	, m_Key1(_T(""))
	, m_Key2(_T(""))
	, m_Key3(_T(""))
	,m_pCUpdateModelProgressDlg(0)
{
	m_cxold = 0;
	m_cyold = 0;
	m_SpecDataFoldPath = _T("");
	m_PicIndex = 0;
	m_AlcoholContent = _T("");
	m_Flavour = _T("");
	m_Brand = _T("");
	m_WineName = _T("");
	m_ProductionDate = _T("");
	m_BatchNo = _T("");
	m_Comfrom = _T("");
	m_pAddToLibDlg=NULL;
}

CDBManagePageTrue::~CDBManagePageTrue()
{
	if(m_TrueWineLib.IsOpen())
	{
		m_TrueWineLib.Close();
	}
}

void CDBManagePageTrue::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_TOADD_TRUE, m_ListSpectrumToAdd);
	DDX_Control(pDX, IDC_LIST_SPECLIB_TRUE, m_ListSpectrunQueryed);
	DDX_CBString(pDX, IDC_COMBO_FIELD1_TRUE, m_Field1);
	DDX_CBString(pDX, IDC_COMBO_ANDOR1_TRUE, m_AndOr1);
	DDX_CBString(pDX, IDC_COMBO_FIELD2_TRUE, m_Field2);
	DDX_CBString(pDX, IDC_COMBO_ANDOR2_TRUE, m_AndOr2);
	DDX_CBString(pDX, IDC_COMBO_FIELD3_TRUE, m_Field3);
	DDX_Control(pDX, IDC_COMBO_FIELD1_TRUE, m_ComboField1);
	DDX_Control(pDX, IDC_COMBO_ANDOR1_TRUE, m_ComboAndOr1);
	DDX_Control(pDX, IDC_COMBO_FIELD2_TRUE, m_ComboField2);
	DDX_Control(pDX, IDC_COMBO_ANDOR2_TRUE, m_ComboAndOr2);
	DDX_Control(pDX, IDC_COMBO_FIELD3_TRUE, m_ComboField3);
	DDX_Text(pDX, IDC_EDIT_KEY1_TRUE, m_Key1);
	DDX_Text(pDX, IDC_EDIT_KEY2_TRUE, m_Key2);
	DDX_Text(pDX, IDC_EDIT_KEY3_TRUE, m_Key3);
	DDX_Control(pDX, IDC_BUTTON_LAST_TRUE, m_btnLastPic);
	DDX_Control(pDX, IDC_BUTTON_NEXT_TRUE, m_btnNextPic);
	DDX_Control(pDX, IDC_BUTTON_LASTLIB_TRUE, m_btnLastPicLib);
	DDX_Control(pDX, IDC_BUTTON_NEXTLIB_TRUE, m_btnNextPicLib);
}


BEGIN_MESSAGE_MAP(CDBManagePageTrue, CPropertyPage)
	ON_BN_CLICKED(IDC_BUTTON_OPENDATAPACKAGE_TRUE, &CDBManagePageTrue::OnBnClickedButtonOpendatapackageTrue)
	ON_BN_CLICKED(IDC_BUTTON_LAST_TRUE, &CDBManagePageTrue::OnBnClickedButtonLastTrue)
	ON_BN_CLICKED(IDC_BUTTON_NEXT_TRUE, &CDBManagePageTrue::OnBnClickedButtonNextTrue)
	ON_BN_CLICKED(IDC_BUTTON_ADDTOLIB_TRUE, &CDBManagePageTrue::OnBnClickedButtonAddtolibTrue)
	ON_BN_CLICKED(IDC_BUTTON_UPDATELIBINFO_TRUE, &CDBManagePageTrue::OnBnClickedButtonUpdatelibinfo)
	ON_BN_CLICKED(IDC_BUTTON_SHOWALL_TRUE, &CDBManagePageTrue::OnBnClickedButtonShowallTrue)
	ON_BN_CLICKED(IDC_BUTTON_DELETE_TRUE, &CDBManagePageTrue::OnBnClickedButtonDeleteTrue)
	ON_BN_CLICKED(IDC_BUTTON_CLEARSPECTOADD_TRUE, &CDBManagePageTrue::OnBnClickedButtonClearspectoaddTrue)
	ON_BN_CLICKED(IDC_BUTTON_UPDATEMODEL_TRUE, &CDBManagePageTrue::OnBnClickedButtonUpdatemodelTrue)
	ON_BN_CLICKED(IDC_BUTTON_SEARCHWITHKEYWORDS_TRUE, &CDBManagePageTrue::OnBnClickedButtonSearchwithkeywordsTrue)
	ON_WM_SHOWWINDOW()
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_BUTTON_SELECTALL_TRUE, &CDBManagePageTrue::OnBnClickedButtonSelectallTrue)
	ON_BN_CLICKED(IDC_BUTTON_INVERTSELECT_TRUE, &CDBManagePageTrue::OnBnClickedButtonInvertselectTrue)
	ON_BN_CLICKED(IDC_BUTTON_CLEARSEARCH_TRUE, &CDBManagePageTrue::OnBnClickedButtonClearsearch)
	ON_BN_CLICKED(IDC_BUTTON_LASTLIB_TRUE, &CDBManagePageTrue::OnBnClickedButtonLastlibTrue)
	ON_BN_CLICKED(IDC_BUTTON_NEXTLIB_TRUE, &CDBManagePageTrue::OnBnClickedButtonNextlibTrue)
	ON_NOTIFY(NM_CLICK, IDC_LIST_SPECLIB_TRUE, &CDBManagePageTrue::OnNMClickListSpeclibTrue)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_BUTTON_COMPONENTPREDICTMODELUPDATE, &CDBManagePageTrue::OnBnClickedButtonComponentpredictmodelupdate)
END_MESSAGE_MAP()


// CDBManagePageTrue ��Ϣ�������
BOOL CDBManagePageTrue::OnInitDialog()
{
	CPropertyPage::OnInitDialog();
	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	m_ComboField1.InsertString(0,L"������");
	m_ComboField1.InsertString(1,L"�׾�����");
	m_ComboField1.InsertString(2,L"�׾ƾƾ�����");
	m_ComboField1.InsertString(3,L"�׾�����");
	m_ComboField1.InsertString(4,L"�׾�Ʒ��");

	m_ComboField2.InsertString(0,L"������");
	m_ComboField2.InsertString(1,L"�׾�����");
	m_ComboField2.InsertString(2,L"�׾ƾƾ�����");
	m_ComboField2.InsertString(3,L"�׾�����");
	m_ComboField2.InsertString(4,L"�׾�Ʒ��");

	m_ComboField3.InsertString(0,L"������");
	m_ComboField3.InsertString(1,L"�׾�����");
	m_ComboField3.InsertString(2,L"�׾ƾƾ�����");
	m_ComboField3.InsertString(3,L"�׾�����");
	m_ComboField3.InsertString(4,L"�׾�Ʒ��");

	m_ComboField1.SetCurSel(0);
	m_ComboField2.SetCurSel(0);
	m_ComboField3.SetCurSel(0);

	m_ComboAndOr1.InsertString(0,L"AND");
	m_ComboAndOr1.InsertString(1,L"OR");
	m_ComboAndOr2.InsertString(0,L"AND");
	m_ComboAndOr2.InsertString(1,L"OR");

	m_ComboAndOr1.SetCurSel(0);
	m_ComboAndOr2.SetCurSel(0);
	///////////////////
	m_ListSpectrumToAdd.InsertColumn( 0, L"", LVCFMT_LEFT,30);//������ 
	m_ListSpectrumToAdd.InsertColumn( 1, L"�����ļ���", LVCFMT_LEFT,100 );//������ 
	m_ListSpectrumToAdd.InsertColumn( 2, L"�׾�����", LVCFMT_LEFT, 100 );//������ 
	m_ListSpectrumToAdd.InsertColumn( 3, L"�׾���Ϣ", LVCFMT_LEFT, 150 );//������ 
	m_ListSpectrumToAdd.InsertColumn( 4, L"��������", LVCFMT_LEFT, 100 );//������ 
	m_ListSpectrumToAdd.InsertColumn( 5, L"��������", LVCFMT_LEFT, 100 );//������ 
	m_ListSpectrumToAdd.InsertColumn( 6, L"��Դ˵��", LVCFMT_LEFT, 100 );//������ 
	m_ListSpectrumToAdd.InsertColumn( 7, L"��������", LVCFMT_LEFT, 100 );//������ 

	DWORD dwStyle = m_ListSpectrumToAdd.GetExtendedStyle(); //��ȡ��ǰ��չ��ʽ
	dwStyle |= LVS_EX_FULLROWSELECT; //ѡ��ĳ��ʹ���и�����report���ʱ��
	dwStyle |= LVS_EX_GRIDLINES; //�����ߣ�report���ʱ��
	m_ListSpectrumToAdd.SetExtendedStyle(dwStyle); //������չ���	
	m_ListSpectrumToAdd.SetHeaderBackColor(168,193,221,2);
	m_ListSpectrumToAdd.SetRowHeigt(28);


	m_ListSpectrunQueryed.InsertColumn( 0, L"", LVCFMT_LEFT,50);//������ 
	m_ListSpectrunQueryed.InsertColumn( 1, L"���ױ��", LVCFMT_LEFT, 100 );//������ 
	m_ListSpectrunQueryed.InsertColumn( 2, L"�׾�����", LVCFMT_LEFT, 100 );//������ 
	m_ListSpectrunQueryed.InsertColumn( 3, L"�׾���Ϣ", LVCFMT_LEFT, 150 );//������ 
	m_ListSpectrunQueryed.InsertColumn( 4, L"��������", LVCFMT_LEFT, 100 );//������
	m_ListSpectrunQueryed.InsertColumn( 5, L"��������", LVCFMT_LEFT, 100 );//������
	m_ListSpectrunQueryed.InsertColumn( 6, L"��Դ˵��", LVCFMT_LEFT, 100 );//������
	m_ListSpectrunQueryed.InsertColumn( 7, L"�������", LVCFMT_LEFT, 100 );//������
	m_ListSpectrunQueryed.InsertColumn( 8, L"������Ա", LVCFMT_LEFT, 100 );//������

	dwStyle = m_ListSpectrunQueryed.GetExtendedStyle(); //��ȡ��ǰ��չ��ʽ
	dwStyle |= LVS_EX_FULLROWSELECT; //ѡ��ĳ��ʹ���и�����report���ʱ��
	dwStyle |= LVS_EX_GRIDLINES; //�����ߣ�report���ʱ��
    dwStyle |= LVS_EX_CHECKBOXES; //itemǰ����checkbox�ؼ�

	m_ListSpectrunQueryed.SetExtendedStyle(dwStyle); 
	m_ListSpectrunQueryed.SetHeaderBackColor(168,193,221,2);
	m_ListSpectrunQueryed.SetRowHeigt(28);

	
	m_btnLastPic.SetIcon(IDI_ICON_PREV); 
	m_btnLastPic.DrawTransparent(TRUE);

	m_btnNextPic.SetIcon(IDI_ICON_NEXT); 
	m_btnNextPic.DrawTransparent(TRUE);
	m_btnNextPic.SetAlign(CButtonST::ST_ALIGN_HORIZ_RIGHT);

	m_btnLastPicLib.SetIcon(IDI_ICON_PREV); 
	m_btnLastPicLib.DrawTransparent(TRUE);

	m_btnNextPicLib.SetIcon(IDI_ICON_NEXT); 
	m_btnNextPicLib.DrawTransparent(TRUE);
	m_btnNextPicLib.SetAlign(CButtonST::ST_ALIGN_HORIZ_RIGHT);

	GetClientRect(&m_rect);
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}
BOOL CDBManagePageTrue::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: �ڴ����ר�ô����/����û���
	//m_btnLastPic.Create(_T("My button"), WS_CHILD|WS_VISIBLE|BS_ICON|BS_LEFT , CRect(10,10,60,50), this, 1);

	return CPropertyPage::PreCreateWindow(cs);
}
void CDBManagePageTrue::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: �ڴ˴������Ϣ����������
	// ��Ϊ��ͼ��Ϣ���� CPropertyPage::OnPaint()
	GetClientRect(&m_rect);//���仯��ĶԻ����С��Ϊ�ɴ�С 

	/*GetDlgItem(IDC_STATIC1)->MoveWindow(5,5,m_rect.Width()-10,m_rect.Height()*0.3);
	GetDlgItem(IDC_BUTTON_OPENDATAPACKAGE_TRUE)->MoveWindow(10,15,100,30);
	GetDlgItem(IDC_BUTTON_CLEARSPECTOADD_TRUE)->MoveWindow(10,55,100,30);
	GetDlgItem(IDC_BUTTON_ADDTOLIB_TRUE)->MoveWindow(10,95,100,30);
	GetDlgItem(IDC_LIST_TOADD_TRUE)->MoveWindow(110,15,(m_rect.Width()-10-135)*0.7,m_rect.Height()*0.3-60);
	GetDlgItem(IDC_STATIC_PICTUREOFSPECTOADD_TRUE)->MoveWindow((m_rect.Width()-10-135)*0.7+110+20,15,(m_rect.Width()-10-135)*0.3,m_rect.Height()*0.3-60);
	GetDlgItem(IDC_BUTTON_LAST_TRUE)->MoveWindow((m_rect.Width()-10-135)*0.7+110+20,m_rect.Height()*0.3-60+15+10,80,30);
	GetDlgItem(IDC_BUTTON_NEXT_TRUE)->MoveWindow((m_rect.Width()-10-135)+110+20-80,m_rect.Height()*0.3-60+15+10,80,30);
	
	GetDlgItem(IDC_STATIC2)->MoveWindow(5,m_rect.Height()*0.3+5,m_rect.Width()-10,m_rect.Height()*0.7-10);

	GetDlgItem(IDC_COMBO_FIELD1_TRUE);
	GetDlgItem(IDC_EDIT_KEY1_TRUE);
	GetDlgItem(IDC_COMBO_ANDOR1_TRUE);
	GetDlgItem(IDC_COMBO_FIELD2_TRUE);
	GetDlgItem(IDC_EDIT_KEY2_TRUE);
	GetDlgItem(IDC_COMBO_ANDOR2_TRUE);
	GetDlgItem(IDC_COMBO_FIELD3_TRUE);
	GetDlgItem(IDC_EDIT_KEY3_TRUE);


	GetDlgItem(IDC_BUTTON_SEARCHWITHKEYWORDS_TRUE);
	GetDlgItem(IDC_BUTTON_SHOWALL_TRUE);
	GetDlgItem(IDC_BUTTON_DELETE_TRUE);

	GetDlgItem(IDC_BUTTON_UPDATEMODEL_TRUE);
	GetDlgItem(IDC_LIST_SPECLIB_TRUE);
	GetDlgItem(IDC_STATIC_PICTUREINLIB_TRUE);

	GetDlgItem(IDC_BUTTON_LASTLIB_TRUE);
	GetDlgItem(IDC_BUTTON_NEXTLIB_TRUE);
	

	GetDlgItem(IDC_BUTTON_UPDATELIBINFO_TRUE);
	GetDlgItem(IDC_BUTTON_SELECTALL_TRUE);
	GetDlgItem(IDC_BUTTON_INVERTSELECT_TRUE);
	GetDlgItem(IDC_BUTTON_CLEARSEARCH_TRUE);
	GetDlgItem(IDC_BUTTON_COMPONENTPREDICTMODELUPDATE);*/

	if(!m_ary_PicturesPath.IsEmpty())
	{
		CRect rect;
		GetDlgItem(IDC_STATIC_PICTUREOFSPECTOADD_TRUE)->GetWindowRect(rect);
		DisplayPic(m_ary_PicturesPath[m_PicIndex],this->GetDlgItem(IDC_STATIC_PICTUREOFSPECTOADD_TRUE)->GetSafeHwnd(), rect.Width(), rect.Height());
	}
	else
	{	
		CRect rect;
		GetDlgItem(IDC_STATIC_PICTUREOFSPECTOADD_TRUE)->GetWindowRect(rect);
		CString path = g_CurrentDir;
		path+= L"\\Resource\\NOPic.jpg";
		DisplayPic(path,this->GetDlgItem(IDC_STATIC_PICTUREOFSPECTOADD_TRUE)->GetSafeHwnd(), rect.Width(), rect.Height());
	}
	if(!m_ary_PicturesQueryedPath.IsEmpty())
	{
		CRect rect;
		GetDlgItem(IDC_STATIC_PICTUREINLIB_TRUE)->GetWindowRect(rect);
		CString path;
		if(m_ary_PicturesQueryedPath[m_PicQueryedIndex].Find(L"DataServerCenter",0)!=-1)
		{
			path = m_ary_PicturesQueryedPath[m_PicQueryedIndex];
		}
		else
		{
			path = g_CurrentDir;
			path += m_ary_PicturesQueryedPath[m_PicQueryedIndex];
		}
		DisplayPic(path,this->GetDlgItem(IDC_STATIC_PICTUREINLIB_TRUE)->GetSafeHwnd(), rect.Width(), rect.Height());
	}
	else
	{
		CRect rect;
		GetDlgItem(IDC_STATIC_PICTUREINLIB_TRUE)->GetWindowRect(rect);
		CString path = g_CurrentDir;
		path += L"\\Resource\\NOPic.jpg";
		DisplayPic(path,this->GetDlgItem(IDC_STATIC_PICTUREINLIB_TRUE)->GetSafeHwnd(), rect.Width(), rect.Height());
	}
}


void CDBManagePageTrue::OnBnClickedButtonOpendatapackageTrue()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	BROWSEINFO bi;
	char Buffer[MAX_PATH];
	bi.hwndOwner = this->m_hWnd;
	bi.pidlRoot =NULL;//��ʼ���ƶ���rootĿ¼�ܲ�����
	bi.pszDisplayName =(LPWSTR)Buffer;//�˲�����ΪNULL������ʾ�Ի���
	bi.lpszTitle = L"ѡ��������ݼ��ļ���·��";
	bi.ulFlags =BIF_USENEWUI ; //BIF_EDITBOX;//���༭��ķ��
	bi.lpfn = NULL;
	bi.lParam = 0;
	bi.iImage=IDR_MAINFRAME;
	//��ʼ����ڲ���bi����
	LPITEMIDLIST pIDList = SHBrowseForFolder(&bi);//������ʾѡ��Ի���

	if(pIDList==NULL)
	{	
		return;
	}
	SHGetPathFromIDList(pIDList,(LPWSTR)Buffer);
	//ȡ���ļ���·����Buffer��
	m_SpecDataFoldPath.Format(L"%s",Buffer);
	// free memory used      
	IMalloc * imalloc = 0;
	if (SUCCEEDED(SHGetMalloc(&imalloc)))
	{
		imalloc->Free (pIDList);
		imalloc->Release();
	}
	//���ļ����е���������ݼ������Ϣ
	CString strDir=m_SpecDataFoldPath;
	strDir.Replace(L"\\",L"\\\\");
	strDir += "\\*.*";
	// �����õ��������ļ�����
	CFileFind finder; 
	BOOL bWorking = finder.FindFile(strDir);
	//���֮ǰ��������ݼ�
	OnBnClickedButtonClearspectoaddTrue();

	while (bWorking)  
	{
		bWorking = finder.FindNextFile();
		CString str=finder.GetFileName();
		if(str.Right(4)==L".CSV"||str.Right(4)==L".csv" &&L"." !=str &&L".." != str)//ע��þ���Ҫ�ų���.����..��
		{
			CString filename;
			CString fileTitle;
			//CString str=finder.GetFileName();
			m_ary_SpecFilePath.Add(finder.GetFilePath());
			//��ȡ�ļ���(��������׺)  
			//����CString��Left(int count)��ȡCString�д�����������count���ַ�  
			//fileName.GetLength()-4�е�4��ʾ".csv"�ĸ��ַ�  
			m_ary_SpecFileName.Add(finder.GetFileTitle());//���ļ���(��������׺)��ӵ�������  
		}
		if(str.Right(4)==L".jpg" ||str.Right(4)==L".bmp"||str.Right(4)==L".gif"&&L"." !=str &&L".." != str)
		{
			m_ary_PicturesName.Add(finder.GetFileName());
			m_ary_PicturesPath.Add(finder.GetFilePath());
		}
		if(str==L"����˵��.xls"||str==L"����˵��.xlsx"&&L"." !=str &&L".." != str)m_ExcelInfoPath=finder.GetFilePath();
	}
	finder.Close();
	if(m_ary_SpecFileName.GetCount()==0)
	{
		MessageBox(L"���ļ�����û�й涨��ʽ�Ĺ������ݣ�������ѡ��",L"����",MB_ICONWARNING);
		return;
	}
	if(m_ary_SpecFileName.GetCount()<20)
	{
		MessageBox(L"��������̫��",L"����",MB_ICONWARNING);
		return;
	}
	m_PicIndex=0;
	CString Winename;
	CString Wineinfo=L"";
	CString Comfrom;
	CString ReceiveDate;
	CString Wineinfotemp;
	if(m_ExcelInfoPath.IsEmpty())
	{
		if(IDOK==MessageBox(L"���ݰ���ȱ�ٱ�Ҫ�Ĺ���˵���ļ����Ƿ�������д��",L"��Ϣȱʧ",MB_OKCANCEL|MB_ICONQUESTION))
		{
			CInputSpecDatasetInfoDlg InputSpecDatasetInfoDlg;
			if(IDOK==InputSpecDatasetInfoDlg.DoModal())
			{
				m_WineName=InputSpecDatasetInfoDlg.m_WineName;
				m_AlcoholContent=InputSpecDatasetInfoDlg.m_AlcoholContent;
				Wineinfo=m_AlcoholContent+L"��,";
				m_Flavour=InputSpecDatasetInfoDlg.m_Flavour;
				Wineinfo+=m_Flavour;
				Wineinfo+=L",";
				m_Brand=InputSpecDatasetInfoDlg.m_Brand;
				Wineinfo+=m_Brand;
				m_Comfrom=L"����Ա";
				SYSTEMTIME tm;		
				GetDirTime(m_SpecDataFoldPath,tm);			
				ReceiveDate.Format(L"%d-%d-%d",tm.wYear,tm.wMonth,tm.wDay);			
			}
			else
			{
				m_ary_SpecFilePath.RemoveAll();
				m_ary_SpecFileName.RemoveAll();
				m_ary_PicturesPath.RemoveAll();
				return;
			}
		}
		else
		{
			m_ary_SpecFilePath.RemoveAll();
			m_ary_SpecFileName.RemoveAll();
			m_ary_PicturesPath.RemoveAll();
			return;
		}

	}
	else
	{
		CString strDirExcel=m_ExcelInfoPath;
		CExcelOperator ExcelOperator;
		ExcelOperator.LoadExcel(strDirExcel);
		ExcelOperator.GetItem(1,1,L"String",m_WineName);
		ExcelOperator.GetItem(2,1,L"int",m_AlcoholContent);
		Wineinfo+=m_AlcoholContent;
		Wineinfo+=L"��,";
		ExcelOperator.GetItem(3,1,L"String",m_Flavour);
		Wineinfo+=m_Flavour;
		Wineinfo+=L",";
		ExcelOperator.GetItem(4,1,L"String",m_Brand);
		Wineinfo+=m_Brand;
		ExcelOperator.GetItem(5,1,L"Date",m_ProductionDate);
		ExcelOperator.GetItem(6,1,L"String",m_BatchNo);
		ExcelOperator.GetItem(7,1,L"String",m_Comfrom);
		ExcelOperator.GetItem(8,1,L"Date",ReceiveDate);
		ExcelOperator.ReleaseExcel();
	}
	SortCArrayByFirst(m_ary_SpecFileName,m_ary_SpecFilePath);

	for(int i=0;i<m_ary_SpecFileName.GetCount();i++)
	{
		CString NuMber;
		NuMber.Format(L"%d",i+1);
		m_ListSpectrumToAdd.InsertItem(i,NuMber);
		m_ListSpectrumToAdd.SetItemText(i,1,m_ary_SpecFileName[i]);
		m_ListSpectrumToAdd.SetItemText(i,2,m_WineName);
		m_ListSpectrumToAdd.SetItemText(i,3,Wineinfo);
		m_ListSpectrumToAdd.SetItemText(i,4,m_ProductionDate);
		m_ListSpectrumToAdd.SetItemText(i,5,m_BatchNo);
		m_ListSpectrumToAdd.SetItemText(i,6,m_Comfrom);
		m_ListSpectrumToAdd.SetItemText(i,7,ReceiveDate);
	}
	OnPaint();
}

void CDBManagePageTrue::OnBnClickedButtonLastTrue()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if(m_ary_PicturesPath.GetCount()!=0)
	{
		if(m_PicIndex==0)
		{
			m_PicIndex=m_ary_PicturesPath.GetCount()-1;
		}
		else
		{
			m_PicIndex--;
		}
	}
	OnPaint();
}


void CDBManagePageTrue::OnBnClickedButtonNextTrue()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if(m_ary_PicturesPath.GetCount()!=0)
	{
		if(m_PicIndex==m_ary_PicturesPath.GetCount()-1)
		{
			m_PicIndex=0;
		}
		else
		{
			m_PicIndex++;
		}
	}
	OnPaint();
}


void CDBManagePageTrue::OnBnClickedButtonAddtolibTrue()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if(m_ary_SpecFileName.IsEmpty())
	{
		MessageBox(L"û��Ҫ���Ĺ�������,����ӹ�������",L"�������",MB_ICONWARNING);
		return ;
	}
	CCompleteSpecInfoAddToLibDlg CompleteSpecInfoAddToLibDlg;
	CompleteSpecInfoAddToLibDlg.m_WineName=m_WineName;
	CompleteSpecInfoAddToLibDlg.m_AlcoholContent=m_AlcoholContent;
    CompleteSpecInfoAddToLibDlg.m_Flavour=m_Flavour;
	CompleteSpecInfoAddToLibDlg.m_Brand=m_Brand;
	CompleteSpecInfoAddToLibDlg.m_ProductionDate=m_ProductionDate;
	CompleteSpecInfoAddToLibDlg.m_BatchNo=m_BatchNo;
	CompleteSpecInfoAddToLibDlg.m_ComFrom=m_Comfrom;
	CompleteSpecInfoAddToLibDlg.m_SpecIDTitle=m_SpecIDTitle;
	CompleteSpecInfoAddToLibDlg.m_Manager=m_Manager;
	if(IDOK == CompleteSpecInfoAddToLibDlg.DoModal())
	{
		if(NULL!=m_pAddToLibDlg)
		{
			m_pAddToLibDlg->DestroyWindow();
			m_pAddToLibDlg=NULL;
		}
		m_Comfrom=CompleteSpecInfoAddToLibDlg.m_ComFrom;
		m_SpecIDTitle=CompleteSpecInfoAddToLibDlg.m_SpecIDTitle;
		m_Manager=CompleteSpecInfoAddToLibDlg.m_Manager;

		//����ͼƬ
		CWinePictures WinePictures;
		CTime tm=CTime::GetCurrentTime();
		CString Currenttime;
		Currenttime=tm.Format("%Y-%m-%d");
		if(!WinePictures.Open())
		{
			MessageBox(L"��ͼƬ��ʧ�ܣ�",L"����ͼƬ",MB_ICONERROR);
			return;
		}

		CString m_strFolderPath=g_CurrentDir;
		m_strFolderPath+=L"\\Lib\\Pictures\\";
		m_strFolderPath+=m_SpecIDTitle;
		//���ɱ���ͼƬ���ļ���
		CreateDirectory(m_strFolderPath, NULL );
		for(int i=0;i<m_ary_PicturesPath.GetCount();i++)
		{	
			//ֻ����ͼƬ�����·��
			CString newpicpath;
			newpicpath.Format(L"\\Lib\\Pictures\\%s\\%s",m_SpecIDTitle,m_ary_PicturesName[i]);
			CopyFile(m_ary_PicturesPath[i],g_CurrentDir+newpicpath,0);
			WinePictures.AddNew();
			WinePictures.m_WineName=m_WineName;
			WinePictures.m_AlcoholContent=m_AlcoholContent;
			WinePictures.m_Flavour=m_Flavour;
			WinePictures.m_Brand=m_Brand;
			WinePictures.m_ProductionBatchNo=m_BatchNo;
			WinePictures.m_ImportDate=Currenttime;
			WinePictures.m_Manager=m_Manager;
			WinePictures.m_PicturesDir=newpicpath;
			WinePictures.m_SpecIDTitle = m_SpecIDTitle;
			WinePictures.Update();
		}
		//��ʼ�������
		m_pAddToLibDlg=new CAddToLibDlg(m_ary_SpecFilePath.GetCount(),1);
		m_pAddToLibDlg->Create(IDD_DIALOG_ADDTOLIBPROGRESS,this);
		m_pAddToLibDlg->ShowWindow(SW_SHOW);
		pThreadAddSpecToLib=AfxBeginThread(ThreadAddSpecToLib,this);
	}
}

UINT ThreadAddSpecToLib(LPVOID lpParam)
{
	CDBManagePageTrue* pDBManageTrueDlg=(CDBManagePageTrue*)lpParam;
	CTrueWineLib TrueWineLib;
	CCSVOperator CSVOperator;
	CTime tm=CTime::GetCurrentTime();
	CString Currenttime;
	Currenttime=tm.Format("%Y-%m-%d");
	if(!TrueWineLib.Open())
	{
		pDBManageTrueDlg->MessageBox(L"�򿪹��׿�ʧ�ܣ�",L"�������",MB_ICONERROR);
	}

	for(int i=0;i<pDBManageTrueDlg->m_ary_SpecFilePath.GetCount();i++)
	{
		pDBManageTrueDlg->m_pAddToLibDlg->m_CurrentProgress=i+1;
		::PostMessage((HWND)(pDBManageTrueDlg->m_pAddToLibDlg->m_hWnd),WM_DISPLAY,0,0);

		CString Current=pDBManageTrueDlg->m_ary_SpecFilePath[i];

		CSVOperator.LoadCSV(pDBManageTrueDlg->m_ary_SpecFilePath[i]);

		//double tempSpecdata[869];
		CString Specdata;

		CString WaveNumber;
		CString temp;
		int j=0;
		double Specdatadb;
		double WaveNumberdb;
		for(;j<868;j++)
		{
			CSVOperator.GetDouble(j+1,2,Specdatadb);
			temp.Format(_T("%lf"),Specdatadb);
			Specdata+=temp;
			Specdata+=",";

			
			CSVOperator.GetDouble(j+1,1,WaveNumberdb);
			temp.Format(_T("%lf"),WaveNumberdb);
			WaveNumber+=temp;
			WaveNumber+=",";
		}


		CSVOperator.GetDouble(j+1,2,Specdatadb);
		temp.Format(_T("%lf"),Specdatadb);
		Specdata+=temp;

		CSVOperator.GetDouble(j+1,1,WaveNumberdb);
		temp.Format(_T("%lf"),WaveNumberdb);
		WaveNumber+=temp;

		//���ɹ���ID
		CString SpecID;
		SpecID.Format(L"%s-%d",pDBManageTrueDlg->m_SpecIDTitle,i+1);

		TrueWineLib.AddNew();

		TrueWineLib.m_WineName=pDBManageTrueDlg->m_WineName;
		TrueWineLib.m_AlcoholContent=pDBManageTrueDlg->m_AlcoholContent;
		TrueWineLib.m_Flavour=pDBManageTrueDlg->m_Flavour;
		TrueWineLib.m_Brand=pDBManageTrueDlg->m_Brand;
		TrueWineLib.m_ProductionBatchNo=pDBManageTrueDlg->m_BatchNo;
		TrueWineLib.m_ProductionDate=pDBManageTrueDlg->m_ProductionDate;
		TrueWineLib.m_SpectrumData=Specdata;
		TrueWineLib.m_ComFrom=pDBManageTrueDlg->m_Comfrom;
		TrueWineLib.m_SpecID=SpecID;
		TrueWineLib.m_ImportDate=Currenttime;
		TrueWineLib.m_Manager=pDBManageTrueDlg->m_Manager;
		TrueWineLib.m_WaveNumber=WaveNumber;

		TrueWineLib.Update();

	}
	pDBManageTrueDlg->UpdateIndexTable();
	pDBManageTrueDlg->m_pAddToLibDlg->ShowWindow(SW_HIDE);
	pDBManageTrueDlg->MessageBox(L"�������ݵ���ɹ�",L"�����������",MB_ICONINFORMATION);
	return 0;
}



void CDBManagePageTrue::UpdateIndexTable(void)
{
	CDatabase m_db;
	CRecordset rs;
	m_db.OpenEx(_T("DSN=�׾Ƽ�������Դ���ݿ�;"),CDatabase::noOdbcDialog);
	rs.m_pDatabase = &m_db;
	CString WineName;
	CString AlcoholContent;
	CString Flavour;
	CString Brand;
	CString ProductionBatchNo;
	//���°׾���������
	CWineTypeIndex WineTypeIndex;

	if(!WineTypeIndex.Open())
	{
		MessageBox(L"�򿪹���������ʧ��",L"����ʧ��",MB_ICONERROR);
		return;
	}
	if(!WineTypeIndex.IsEOF())
	{
		WineTypeIndex.MoveFirst();
		while(!WineTypeIndex.IsEOF())
		{
			WineTypeIndex.Delete();
			WineTypeIndex.MoveNext();
		}
	}
	CString command=_T("SELECT DISTINCT WineName,AlcoholContent,Flavour,Brand,ProductionBatchNo FROM TrueWineLib");
	if(!rs.Open(AFX_DB_USE_DEFAULT_TYPE,command))
	{
		MessageBox(L"����ƿ�ʧ��",L"����ʧ��",MB_ICONERROR);
		return;
	}
	if(!rs.IsEOF())
	{
		int count=1;
		rs.MoveFirst();
		while(!rs.IsEOF())
		{
			rs.GetFieldValue(L"WineName",WineName);
			rs.GetFieldValue(L"AlcoholContent",AlcoholContent);
			rs.GetFieldValue(L"Flavour",Flavour);
			rs.GetFieldValue(L"Brand",Brand);
			rs.GetFieldValue(L"ProductionBatchNo",ProductionBatchNo);

			WineTypeIndex.AddNew();
			WineTypeIndex.m_WineName=WineName;   //��֪��Ϊʲôʹ����select distinct �õ�ĳ���м�¼ʱ���Զ�����һ�з������ǰ���������ͬ����
			WineTypeIndex.m_AlcoholContent=AlcoholContent;
			WineTypeIndex.m_Flavour=Flavour;
			WineTypeIndex.m_Brand=Brand;
			WineTypeIndex.m_ProductionBatchNo=ProductionBatchNo;
			WineTypeIndex.m_TypeIndex=count;
			count++;
			rs.MoveNext();
			WineTypeIndex.Update();
		}
	}
	rs.Close();
	//���¾ƾ�������
	CAlcoholTypeIndex AlcoholTypeIndex;

	if(!AlcoholTypeIndex.Open())
	{
		MessageBox(L"�򿪹���������ʧ��",L"����ʧ��",MB_ICONERROR);
		return;
	}
	if(!AlcoholTypeIndex.IsEOF())
	{
		AlcoholTypeIndex.MoveFirst();
		while(!AlcoholTypeIndex.IsEOF())
		{
			AlcoholTypeIndex.Delete();
			AlcoholTypeIndex.MoveNext();
		}
	}
	command=_T("SELECT DISTINCT AlcoholContent FROM TrueWineLib");
	if(!rs.Open(AFX_DB_USE_DEFAULT_TYPE,command))
	{
		MessageBox(L"����ƿ�ʧ��",L"����ʧ��",MB_ICONERROR);
		return;
	}
	CString alcohol;


	if(!rs.IsEOF())
	{
		rs.MoveFirst();
		int i=1;
		while(!rs.IsEOF())
		{		
			rs.GetFieldValue(_T("AlcoholContent"),alcohol); 
			AlcoholTypeIndex.AddNew();
			AlcoholTypeIndex.m_AlcoholContent=alcohol;
			AlcoholTypeIndex.m_AlcoholIndex=i;
			i++;
			AlcoholTypeIndex.Update();
			rs.MoveNext();

		}
	}
	AlcoholTypeIndex.Close();
	rs.Close();

	//������������
	CFlavourTypeIndex FlavourTypeIndex;

	if(!FlavourTypeIndex.Open())
	{
		MessageBox(L"�򿪹���������ʧ��",L"����ʧ��",MB_ICONERROR);
		return;
	}
	if(!FlavourTypeIndex.IsEOF())
	{
		FlavourTypeIndex.MoveFirst();
		while(!FlavourTypeIndex.IsEOF())
		{
			FlavourTypeIndex.Delete();
			FlavourTypeIndex.MoveNext();
		}
	}
	command=_T("SELECT DISTINCT Flavour FROM TrueWineLib");
	if(!rs.Open(AFX_DB_USE_DEFAULT_TYPE,command))
	{
		MessageBox(L"����ƿ�ʧ��",L"����ʧ��",MB_ICONERROR);
		return;
	}
	if(!rs.IsEOF())
	{
		int count=1;
		rs.MoveFirst();
		while(!rs.IsEOF())
		{
			rs.GetFieldValue(L"Flavour",Flavour);
			FlavourTypeIndex.AddNew();
			FlavourTypeIndex.m_Flavour=Flavour;
			FlavourTypeIndex.m_FlavourIndex=count;
			count++;
			rs.MoveNext();
			FlavourTypeIndex.Update();
		}
	}
	rs.Close();
	//����Ʒ������
	CBrandTypeIndex BrandTypeIndex;
	if(!BrandTypeIndex.Open())
	{
		MessageBox(L"�򿪹���������ʧ��",L"����ʧ��",MB_ICONERROR);
		return;
	}
	if(!BrandTypeIndex.IsEOF())
	{
		BrandTypeIndex.MoveFirst();
		while(!BrandTypeIndex.IsEOF())
		{
			BrandTypeIndex.Delete();
			BrandTypeIndex.MoveNext();
		}
	}
	command=_T("SELECT DISTINCT Brand FROM TrueWineLib");
	if(!rs.Open(AFX_DB_USE_DEFAULT_TYPE,command))
	{
		MessageBox(L"����ƿ�ʧ��",L"����ʧ��",MB_ICONERROR);
		return;
	}
	if(!rs.IsEOF())
	{
		int count=1;
		rs.MoveFirst();
		while(!rs.IsEOF())
		{
			rs.GetFieldValue(L"Brand",Brand);
			BrandTypeIndex.AddNew();
			BrandTypeIndex.m_Brand=Brand;
			BrandTypeIndex.m_BrandIndex=count;
			count++;
			rs.MoveNext();
			BrandTypeIndex.Update();
		}
	}
	rs.Close();
}


void CDBManagePageTrue::OnBnClickedButtonUpdatelibinfo()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateIndexTable();

	MessageBox(L"���ݿ���Ϣ���³ɹ�!",L"��Ϣ����",MB_ICONINFORMATION);
}

void CDBManagePageTrue::OnBnClickedButtonUpdatemodelTrue()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//UpdatePLSModel();
	UpdateConcentrationTraceBackModel();
}

void CDBManagePageTrue::UpdateConcentrationTraceBackModel()
{
	if(m_pCUpdateModelProgressDlg)
	{
		m_pCUpdateModelProgressDlg->DestroyWindow();
		m_pCUpdateModelProgressDlg = NULL;
	}
	m_pCUpdateModelProgressDlg=new CUpdateModelProgressDlg();
	m_pCUpdateModelProgressDlg->Create(IDD_DIALOG_UPDATEMODELPROGRESS,this);
	m_pCUpdateModelProgressDlg->ShowWindow(SW_SHOW);
	pThreadUpdatePLSModel=AfxBeginThread(ThreadUpdateConcentrationTraceBackModel,this);

}

UINT ThreadUpdateConcentrationTraceBackModel(LPVOID lpParam)
{
	double coeff=2;//ϵ��

	CDBManagePageTrue* pCDBManageTrueDlg=(CDBManagePageTrue*)lpParam;

	CTrueWineLib TrueWineLib;

	CWineTypeIndex WineTypeIndex;

	CConcentrationTraceBackModel ConcentrationTraceBackModel;

	if(!ConcentrationTraceBackModel.Open())
	{
		pCDBManageTrueDlg->m_pCUpdateModelProgressDlg->ShowWindow(SW_HIDE);
		pCDBManageTrueDlg->MessageBox(L"����Դģ�Ϳ�ʧ��",L"����ģ��ʧ��",MB_ICONERROR);
		return -1;
	}
	if(!ConcentrationTraceBackModel.IsEOF())
	{
		ConcentrationTraceBackModel.MoveFirst();
	}

	while(!ConcentrationTraceBackModel.IsEOF())
	{
		ConcentrationTraceBackModel.Delete();
		ConcentrationTraceBackModel.MoveNext();
	}

	CComponentPredictModel ComponentPredictModel;
	ComponentPredictModel.Open();

	CMatrix Weight;
	CMatrix offset(1,COMPONENT_COUNT);
	for(int i = 0;i<g_ArrayComponnetName.GetCount();++i)
	{
		CMatrix tmp_w(1,SPEC_LENTH);
		ComponentPredictModel.m_strFilter.Format(L"CAST ([Component] as varchar(1000))= '%s'",g_ArrayComponnetName[i]);
		ComponentPredictModel.Requery();
		CString2Matrix(tmp_w,ComponentPredictModel.m_Weight,L",");
		Weight.AddRows(tmp_w);
		offset.SetItem(1,i+1,ComponentPredictModel.m_Offset);
	}
	Weight = Weight.Transposition();
	//��������
	if(!WineTypeIndex.Open())
	{
		pCDBManageTrueDlg->m_pCUpdateModelProgressDlg->ShowWindow(SW_HIDE);
		pCDBManageTrueDlg->MessageBox(L"�����������ʧ��",L"����ģ��ʧ��",MB_ICONERROR);
		return -1;
	}
	if(0==WineTypeIndex.GetRecordCount())
	{
		pCDBManageTrueDlg->m_pCUpdateModelProgressDlg->ShowWindow(SW_HIDE);
		pCDBManageTrueDlg->MessageBox(L"���׹�����Ϣ���ִ����볢�Ը��£�",L"����ģ��ʧ��",MB_ICONERROR);
		return -1;
	}
	if(!TrueWineLib.Open())
	{
		pCDBManageTrueDlg->m_pCUpdateModelProgressDlg->ShowWindow(SW_HIDE);
		pCDBManageTrueDlg->MessageBox(L"����ƿ�ʧ��",L"����ģ��ʧ��",MB_ICONERROR);
		return -1;
	}
	/////////////////////////
	WineTypeIndex.MoveFirst();
	while(!WineTypeIndex.IsEOF())
	{
		TrueWineLib.m_strFilter.Format(L"AlcoholContent='%s'AND Flavour='%s' AND Brand='%s' AND WineName='%s'"
			                           ,WineTypeIndex.m_AlcoholContent
									   ,WineTypeIndex.m_Flavour
									   ,WineTypeIndex.m_Brand
									   ,WineTypeIndex.m_WineName);
		TrueWineLib.Requery();
		CMatrix SpecData;
		CMatrix Offset;
		if(!TrueWineLib.IsEOF())
		{
			TrueWineLib.MoveFirst();
			CMatrix tmp(1,SPEC_LENTH);
			while(!TrueWineLib.IsEOF())
			{
				CString2Matrix(tmp,TrueWineLib.m_SpectrumData,L",");
				SpecData.AddRows(tmp);
				Offset.AddRows(offset);
				TrueWineLib.MoveNext();
			}
		}
		CMatrix Component = SpecData*Weight + Offset;
		CMatrix stdComponent = Component.Mean(1);

		double Threshold;
		CMatrix tempdist(SpecData.m_row,1);
		for(int j= 1;j<=Component.m_row;++j)
		{
			double dist=(Component.GetRow(j)-stdComponent).Norm();
			tempdist.SetItem(j,1,dist);
		}
		Threshold = Max(tempdist.GetMax(),tempdist.GetMean()*coeff);
		
		ConcentrationTraceBackModel.AddNew();
		ConcentrationTraceBackModel.m_TypeIndex = WineTypeIndex.m_TypeIndex;
		stdComponent.Matrix2CString(ConcentrationTraceBackModel.m_StandardComponentConcentration,L",");
		ConcentrationTraceBackModel.m_Threshold = Threshold;
		ConcentrationTraceBackModel.Update();
		WineTypeIndex.MoveNext();
	}
	TrueWineLib.Close();
	WineTypeIndex.Close();
	pCDBManageTrueDlg->m_pCUpdateModelProgressDlg->ShowWindow(SW_HIDE);
	pCDBManageTrueDlg->MessageBox(L"ģ�͸��³ɹ���",L"ģ�͸���",MB_ICONINFORMATION);
	//ɾ������Ҫ��ģ���ļ�
	return 0;
}


void CDBManagePageTrue::UpdatePLSModel(void)
{
	if(m_pCUpdateModelProgressDlg)
	{
		m_pCUpdateModelProgressDlg->DestroyWindow();
		m_pCUpdateModelProgressDlg = NULL;
	}
	m_pCUpdateModelProgressDlg=new CUpdateModelProgressDlg();
	m_pCUpdateModelProgressDlg->Create(IDD_DIALOG_UPDATEMODELPROGRESS,this);
	m_pCUpdateModelProgressDlg->ShowWindow(SW_SHOW);
	pThreadUpdatePLSModel=AfxBeginThread(ThreadUpdatePLSModel,this);
}


UINT ThreadUpdatePLSModel(LPVOID lpParam)
{
	double coeff=2;//ϵ��

	CDBManagePageTrue* pCDBManageTrueDlg=(CDBManagePageTrue*)lpParam;
	CTrueWineLib TrueWineLib;

	CWineTypeIndex WineTypeIndex;
	CAlcoholTypeIndex AlcoholTypeIndex;
	CFlavourTypeIndex FlavourTypeIndex;
	CBrandTypeIndex BrandTypeIndex;

	CPLSModel PLSModel;

	map<CString,int> map_TrueWineIndex;
	map<CString, int> map_AlcoholContent;
	map<CString,int> map_Flavour;
	map<CString,int> map_Brand;

	if(!PLSModel.Open())
	{
		pCDBManageTrueDlg->MessageBox(L"��PLSģ�Ϳ�ʧ��",L"����ģ��ʧ��",MB_ICONERROR);
		return -1;
	}
	if(!PLSModel.IsEOF())
	{
		PLSModel.MoveFirst();
	}

	while(!PLSModel.IsEOF())
	{
		PLSModel.Delete();
		PLSModel.MoveNext();
	}

	//��������
	if(!WineTypeIndex.Open())
	{
		pCDBManageTrueDlg->MessageBox(L"�����������ʧ��",L"����ģ��ʧ��",MB_ICONERROR);
		return -1;
	}
	if(0==WineTypeIndex.GetRecordCount())
	{
		pCDBManageTrueDlg->MessageBox(L"���׹�����Ϣ���ִ����볢�Ը��£�",L"����ģ��ʧ��",MB_ICONERROR);
		return -1;
	}

	if(!AlcoholTypeIndex.Open())
	{
		pCDBManageTrueDlg->MessageBox(L"�򿪾ƾ���������ʧ��",L"����ģ��ʧ��",MB_ICONERROR);
		return -1;
	}
	if(0==AlcoholTypeIndex.GetRecordCount())
	{
		pCDBManageTrueDlg->MessageBox(L"���׹�����Ϣ���ִ����볢�Ը��£�",L"����ģ��ʧ��",MB_ICONERROR);
		return -1;
	}

	if(!FlavourTypeIndex.Open())
	{
		pCDBManageTrueDlg->MessageBox(L"������������ʧ��",L"����ģ��ʧ��",MB_ICONERROR);
		return -1;
	}
	if(0==FlavourTypeIndex.GetRecordCount())
	{
		pCDBManageTrueDlg->MessageBox(L"���׹�����Ϣ���ִ����볢�Ը��£�",L"����ģ��ʧ��",MB_ICONERROR);
		return -1;
	}

	if(!BrandTypeIndex.Open())
	{
		pCDBManageTrueDlg->MessageBox(L"��Ʒ��������ʧ��",L"����ģ��ʧ��",MB_ICONERROR);
		return -1;
	}
	if(0==BrandTypeIndex.GetRecordCount())
	{
		pCDBManageTrueDlg->MessageBox(L"���׹�����Ϣ���ִ����볢�Ը��£�",L"����ģ��ʧ��",MB_ICONERROR);
		return -1;
	}
	/////////////////////////
	WineTypeIndex.MoveFirst();
	while(!WineTypeIndex.IsEOF())
	{
		map_TrueWineIndex[WineTypeIndex.m_WineName]=WineTypeIndex.m_TypeIndex;
		WineTypeIndex.MoveNext();
	}

	AlcoholTypeIndex.MoveFirst();
	while(!AlcoholTypeIndex.IsEOF())
	{
		map_AlcoholContent[AlcoholTypeIndex.m_AlcoholContent]=AlcoholTypeIndex.m_AlcoholIndex;
		AlcoholTypeIndex.MoveNext();
	}
	AlcoholTypeIndex.MoveFirst();

	FlavourTypeIndex.MoveFirst();
	while(!FlavourTypeIndex.IsEOF())
	{
		map_Flavour[FlavourTypeIndex.m_Flavour]=FlavourTypeIndex.m_FlavourIndex;
		FlavourTypeIndex.MoveNext();
	}
	FlavourTypeIndex.MoveFirst();

	BrandTypeIndex.MoveFirst();
	while(!BrandTypeIndex.IsEOF())
	{
		map_Brand[BrandTypeIndex.m_Brand]=BrandTypeIndex.m_BrandIndex;
		BrandTypeIndex.MoveNext();
	}
	BrandTypeIndex.MoveFirst();

	if(!TrueWineLib.Open())
	{
		pCDBManageTrueDlg->MessageBox(L"����ƿ�ʧ��",L"����ģ��ʧ��",MB_ICONERROR);
		return -1;
	}
	if(TrueWineLib.IsEOF())
	{
		pCDBManageTrueDlg->MessageBox(L"��ƿ����޹��׼�¼",L"����ģ��ʧ��",MB_ICONERROR);
		return -1;
	}
	TrueWineLib.MoveFirst();
	while(!TrueWineLib.IsEOF())
	{
		TrueWineLib.MoveNext();
	}
	TrueWineLib.MoveFirst();

	//�ƾ��ȷ���ģ��
	int Num_of_AlcoholContent=AlcoholTypeIndex.GetRecordCount();
	if(1!=Num_of_AlcoholContent)
	{
		CMatrix MNum_EachLabel(1,Num_of_AlcoholContent);
		CMatrix MLabels(1,Num_of_AlcoholContent);

		CMatrix X_1(TrueWineLib.GetRecordCount(),869);
		CMatrix Y_1(TrueWineLib.GetRecordCount(),1);

		int AlcoholTypeCount=0;
		if(!AlcoholTypeIndex.IsEOF())
		{
			TrueWineLib.MoveFirst();
			while(!TrueWineLib.IsEOF())
			{
				TrueWineLib.MoveNext();
			}
			int Num_inX_1=0;

			AlcoholTypeIndex.MoveFirst();	
			while(!AlcoholTypeIndex.IsEOF())
			{


				TrueWineLib.m_strFilter.Format(L"AlcoholContent='%s'",AlcoholTypeIndex.m_AlcoholContent);
				TrueWineLib.m_strSort=L"WineName";
				if(!TrueWineLib.Requery())
				{
					pCDBManageTrueDlg->MessageBox(L"��ѯ��ƿ�ʧ��",L"���¾ƾ���ģ��ʧ��",MB_ICONERROR);
					return -1;
				}
				TrueWineLib.MoveFirst();
				while(!TrueWineLib.IsEOF())
				{
					TrueWineLib.MoveNext();
				}			
				int Num_of_EachAlcoholContent=TrueWineLib.GetRecordCount();

				TrueWineLib.MoveFirst();
				CMatrix tempMatrix(1,869);
				int m=1;
				TrueWineLib.MoveFirst();

				while(!TrueWineLib.IsEOF())
				{
					CString2Matrix(tempMatrix,TrueWineLib.m_SpectrumData,L",");
					X_1.SetRow(Num_inX_1+m,tempMatrix);
					Y_1.SetItem(Num_inX_1+m,1,map_AlcoholContent[TrueWineLib.m_AlcoholContent]);
					TrueWineLib.MoveNext();
					m++;
				}
				AlcoholTypeCount++;
				Num_inX_1+=Num_of_EachAlcoholContent;
				MLabels.SetItem(1,AlcoholTypeCount,map_AlcoholContent[TrueWineLib.m_AlcoholContent]);	
				MNum_EachLabel.SetItem(1,AlcoholTypeCount,Num_of_EachAlcoholContent);
				AlcoholTypeIndex.MoveNext();

			}
		}		
		//ѵ���ƾ��ȷ���ģ��
		CMatrix Y_1map=BuildMapLabel(Y_1);

		CMatrix CV=Plscvfold(X_1,Y_1map,X_1.m_row,"center");	
		CMatrixIndex CV_indexmin;
		CV.FindMin(CV_indexmin);
		int bestPC=CV_indexmin.column;

		CMatrix W(X_1.m_column,bestPC);
		CMatrix T(X_1.m_row,bestPC);
		CMatrix P(X_1.m_column,bestPC);
		CMatrix Q(bestPC,1);

		CMatrix para2(1,X_1.m_column);//=X_1.Mean();
		CMatrix para1(1,X_1.m_column);//=X_1.Mean();
		CMatrix para2_X(1,X_1.m_column);//=X_1.Mean();
		CMatrix para1_X(1,X_1.m_column);//=X_1.Mean();	
		Pretreat(X_1,"center",para1_X,para2_X);
		
		CMatrix X_1_mean=para1_X;		
	    CMatrix para2_Y(1,Y_1map.m_column);
		CMatrix para1_Y(1,Y_1map.m_column);
		Pretreat(Y_1map,"center",para1_Y,para2_Y);
		CMatrix Y_1_mean=para1_Y;
		CMatrix B=Pls_nipals(X_1,Y_1map,W,T,P,Q,bestPC);
		CMatrix StandardPC(AlcoholTypeCount,bestPC);

		int begin=1;
		for(int i=1;i<=AlcoholTypeCount;i++)
		{
			StandardPC.SetRow(i,(T.GetRows(begin,begin+MNum_EachLabel(1,i)-1)).Mean(1));
			begin+=MNum_EachLabel(1,i);
		}
		//������ֵ
		CMatrix Threshold(1,AlcoholTypeCount);//�����������׼ֵ֮���������,�������׼ֵ֮���ƽ�������coeff����ȡ�ϴ���Ǹ�

		begin=1;
		for(int i=1;i<=AlcoholTypeCount;i++)
		{
			CMatrix tempdist(MNum_EachLabel(1,i),1);
			CMatrix tempPC=T.GetRows(begin,begin+MNum_EachLabel(1,i)-1);
			CMatrix covPC=tempPC.covariance();
			covPC.Inv();
			int k=1;
			for(int j=begin;j<=begin+MNum_EachLabel(1,i)-1;j++)
			{

				CMatrix diff=T.GetRow(j)-StandardPC.GetRow(i);
				tempdist.SetItem(k,1,(diff*covPC*diff.Transposition())(1,1));
				k++;
			}
			Threshold.SetItem(1,i,Max(tempdist.GetMax(),tempdist.GetMean()*coeff));
			begin=begin+MNum_EachLabel(1,i);
		}
		CString Weight;
		CString Labels;
		CString Num_EachLabel;
		CString standardPC;
		CString TrainsetPC;

		CString TrainsetMean;
		CString threshold;
		//Weight
		W.Matrix2CString(Weight,L",");
		//Labels
		MLabels.Matrix2CString(Labels,L",");
		//Num_EachLabel
		MNum_EachLabel.Matrix2CString(Num_EachLabel,L",");
		//standardPC
		StandardPC.Matrix2CString(standardPC,L",");
		//TrainsetPC
		T.Matrix2CString(TrainsetPC,L",");
		//TrainsetMean
		X_1_mean.Matrix2CString(TrainsetMean,L",");
		//��ֵ
		Threshold.Matrix2CString(threshold,L",");

		//����ģ��
		PLSModel.AddNew();
		PLSModel.m_FactorNum=bestPC;
		PLSModel.m_ClassCount=AlcoholTypeCount;
		PLSModel.m_Weight=Weight;
		PLSModel.m_Labels=Labels;
		PLSModel.m_Num_EachLabel=Num_EachLabel;
		PLSModel.m_StandardPC=standardPC;
		PLSModel.m_TrainsetPC=TrainsetPC;
		PLSModel.m_TrainsetMean=TrainsetMean;	
		PLSModel.m_Threshold=threshold;
		PLSModel.Update();
	}	
	//��ʼ����
	AlcoholTypeIndex.MoveFirst();
	while(!AlcoholTypeIndex.IsEOF())
	{
		CString command=_T("SELECT DISTINCT Flavour FROM TrueWineLib");
		CString tempFlavourInAlconhol;
		CDatabase m_db2;
		CRecordset rsforFlavour;
		rsforFlavour.m_strFilter.Format(L"AlcoholContent='%s'",AlcoholTypeIndex.m_AlcoholContent);
		m_db2.OpenEx(_T("DSN=�׾Ƽ�������Դ���ݿ�;"),CDatabase::noOdbcDialog);
		rsforFlavour.m_pDatabase = &m_db2;	
		if (!rsforFlavour.Open(AFX_DB_USE_DEFAULT_TYPE,command))
		{
			CString str;
			str.Format(L"����%s�����ͷ���ģ��ʧ�ܣ�",AlcoholTypeIndex.m_AlcoholContent);
			pCDBManageTrueDlg->MessageBox(str,L"�����ݿ�ʧ��",MB_ICONERROR);
			return -1;
		}

		if(!rsforFlavour.IsEOF())
		{
			rsforFlavour.MoveFirst();

			while(!rsforFlavour.IsEOF())
			{
				rsforFlavour.MoveNext();
			}
		}
		int Num_of_FlavourInAlcoholContent=rsforFlavour.GetRecordCount();
		if(1!=Num_of_FlavourInAlcoholContent)
		{
			TrueWineLib.m_strFilter.Format(L"AlcoholContent='%s'",AlcoholTypeIndex.m_AlcoholContent);
			TrueWineLib.Requery();
			TrueWineLib.MoveFirst();
			while(!TrueWineLib.IsEOF())
			{
				TrueWineLib.MoveNext();
			}
			TrueWineLib.MoveFirst();
			CMatrix MNum_EachLabel(1,Num_of_FlavourInAlcoholContent);
			CMatrix MLabels(1,Num_of_FlavourInAlcoholContent);

			CMatrix X_2(TrueWineLib.GetRecordCount(),869);
			CMatrix Y_2(TrueWineLib.GetRecordCount(),1);

			int FlavourTypeCount=0;
			int Num_inX_2=0;

			rsforFlavour.MoveFirst();			
			while(!rsforFlavour.IsEOF())
			{

				rsforFlavour.GetFieldValue(L"Flavour",tempFlavourInAlconhol);

				TrueWineLib.m_strFilter.Format(L"AlcoholContent='%s'AND Flavour='%s'",AlcoholTypeIndex.m_AlcoholContent,tempFlavourInAlconhol);

				if(!TrueWineLib.Requery())
				{
					pCDBManageTrueDlg->MessageBox(L"��ѯ��ƿ�ʧ��",L"��������ģ��ʧ��",MB_ICONERROR);
					return -1;
				}
				if(!TrueWineLib.IsEOF())
				{
					TrueWineLib.MoveFirst();
					while(!TrueWineLib.IsEOF())
					{
						TrueWineLib.MoveNext();
					}
					TrueWineLib.MoveFirst();
				}

				int Num_of_EachFlavorInAlcoholContent=TrueWineLib.GetRecordCount();


				CMatrix tempMatrix(1,869);
				int m=1;
				while(!TrueWineLib.IsEOF())
				{
					CString2Matrix(tempMatrix,TrueWineLib.m_SpectrumData,L",");
					X_2.SetRow(Num_inX_2+m,tempMatrix);
					Y_2.SetItem(Num_inX_2+m,1,map_Flavour[TrueWineLib.m_Flavour]);
					TrueWineLib.MoveNext();
					m++;
				}
				FlavourTypeCount++;
				Num_inX_2+=Num_of_EachFlavorInAlcoholContent;
				MLabels.SetItem(1,FlavourTypeCount,map_Flavour[TrueWineLib.m_Flavour]);		
				MNum_EachLabel.SetItem(1,FlavourTypeCount,Num_of_EachFlavorInAlcoholContent);
				rsforFlavour.MoveNext();
			}
			//ѵ�����ͷ���ģ��
			CMatrix Y_2map=BuildMapLabel(Y_2);
			CMatrix CV=Plscvfold(X_2,Y_2map,X_2.m_row,"center");
			CMatrixIndex CV_indexmin;
			CV.FindMin(CV_indexmin);
			int bestPC=CV_indexmin.column;


			CMatrix W(X_2.m_column,bestPC);
			CMatrix T(X_2.m_row,bestPC);
			CMatrix P(X_2.m_column,bestPC);
			CMatrix Q(bestPC,1);

			CMatrix para2_X(1,X_2.m_column);
			CMatrix para1_X(1,X_2.m_column);
			Pretreat(X_2,"center",para1_X,para2_X);
			CMatrix X_2_mean=para1_X;		

			CMatrix para2_Y(1,Y_2map.m_column);
			CMatrix para1_Y(1,Y_2map.m_column);
			Pretreat(Y_2map,"center",para1_Y,para2_Y);
			CMatrix Y_2_mean=para1_Y;

			CMatrix B=Pls_nipals(X_2,Y_2map,W,T,P,Q,bestPC);

			CMatrix StandardPC(FlavourTypeCount,bestPC);


			int begin=1;
			for(int i=1;i<=FlavourTypeCount;i++)
			{
				StandardPC.SetRow(i,(T.GetRows(begin,begin+MNum_EachLabel(1,i)-1)).Mean(1));
				begin+=MNum_EachLabel(1,i);
			}
			//������ֵ
			CMatrix Threshold(1,FlavourTypeCount);//�����������׼ֵ֮���������,�������׼ֵ֮���ƽ�������coeff����ȡ�ϴ���Ǹ�
			begin=1;
			for(int i=1;i<=FlavourTypeCount;i++)
			{
				CMatrix tempdist(MNum_EachLabel(1,i),1);
				CMatrix tempPC=T.GetRows(begin,begin+MNum_EachLabel(1,i)-1);
				CMatrix covPC=tempPC.covariance();
				covPC.Inv();
				int k=1;
				for(int j=begin;j<=begin+MNum_EachLabel(1,i)-1;j++)
				{

					CMatrix diff=T.GetRow(j)-StandardPC.GetRow(i);
					tempdist.SetItem(k,1,(diff*covPC*diff.Transposition())(1,1));
					k++;
				}
				Threshold.SetItem(1,i,Max(tempdist.GetMax(),tempdist.GetMean()*coeff));
				begin=begin+MNum_EachLabel(1,i);
			}

			CString Weight;
			CString Labels;
			CString Num_EachLabel;
			CString standardPC;
			CString TrainsetPC;
			CString TrainsetMean;
			CString threshold;

			//Weight
			W.Matrix2CString(Weight,L",");
			//Labels
			MLabels.Matrix2CString(Labels,L",");
			//Num_EachLabel
			MNum_EachLabel.Matrix2CString(Num_EachLabel,L",");
			//standardPC
			StandardPC.Matrix2CString(standardPC,L",");
			//TrainsetPC
			T.Matrix2CString(TrainsetPC,L",");
			//TrainsetMean
			X_2_mean.Matrix2CString(TrainsetMean,L",");
			//��ֵ
			Threshold.Matrix2CString(threshold,L",");
			//����ģ��
			PLSModel.AddNew();
			PLSModel.m_AlcoholContent=AlcoholTypeIndex.m_AlcoholContent;
			PLSModel.m_FactorNum=bestPC;
			PLSModel.m_ClassCount=FlavourTypeCount;
			PLSModel.m_Weight=Weight;
			PLSModel.m_Labels=Labels;
			PLSModel.m_Num_EachLabel=Num_EachLabel;
			PLSModel.m_StandardPC=standardPC;
			PLSModel.m_TrainsetPC=TrainsetPC;
			PLSModel.m_TrainsetMean=TrainsetMean;	
			PLSModel.m_Threshold=threshold;
			PLSModel.Update();
		}
		/////////////////////////////////////////////
		//��ʼƷ��
		rsforFlavour.MoveFirst();
		while(!rsforFlavour.IsEOF())
		{
			CString tempFlavour;
			rsforFlavour.GetFieldValue(L"Flavour",tempFlavour);


			CString command=_T("SELECT DISTINCT Brand FROM TrueWineLib");
			CDatabase m_db3;
			CRecordset rsforBrand;

			rsforBrand.m_strFilter.Format(L"AlcoholContent='%s' AND Flavour='%s'",AlcoholTypeIndex.m_AlcoholContent,tempFlavour);

			m_db3.OpenEx(_T("DSN=�׾Ƽ�������Դ���ݿ�;"),CDatabase::noOdbcDialog);
			rsforBrand.m_pDatabase = &m_db3;	
			if (!rsforBrand.Open(AFX_DB_USE_DEFAULT_TYPE,command))
			{
				CString str;
				str.Format(L"����%s��%s����ģ��ʧ�ܣ�",AlcoholTypeIndex.m_AlcoholContent,tempFlavour);
				pCDBManageTrueDlg->MessageBox(str,L"�����ݿ�ʧ��",MB_ICONERROR);
				return -1;
			}

			if(!rsforBrand.IsEOF())
			{
				rsforBrand.MoveFirst();
			}
			while(!rsforBrand.IsEOF())
			{
				rsforBrand.MoveNext();
			}

			int Num_of_BrandInFlavour=rsforBrand.GetRecordCount();
			if(1!=Num_of_BrandInFlavour)
			{			
				TrueWineLib.m_strFilter.Format(L"AlcoholContent='%s' AND Flavour='%s'",AlcoholTypeIndex.m_AlcoholContent,tempFlavour);
				TrueWineLib.Requery();
				TrueWineLib.MoveFirst();
				while(!TrueWineLib.IsEOF())
				{
					TrueWineLib.MoveNext();
				}
				TrueWineLib.MoveFirst();
				CMatrix MNum_EachLabel(1,Num_of_BrandInFlavour);
				CMatrix MLabels(1,Num_of_BrandInFlavour);

				CMatrix X_3(TrueWineLib.GetRecordCount(),869);
				CMatrix Y_3(TrueWineLib.GetRecordCount(),1);

				int BrandTypeCount=0;
				int Num_inX_3=0;
				rsforBrand.MoveFirst();			
				while(!rsforBrand.IsEOF())
				{
					CString tempBrandInFlavour;

					rsforBrand.GetFieldValue(L"Brand",tempBrandInFlavour);

					TrueWineLib.m_strFilter.Format(L"AlcoholContent='%s'AND Flavour='%s' AND Brand='%s'",AlcoholTypeIndex.m_AlcoholContent,tempFlavour,tempBrandInFlavour);

					if(!TrueWineLib.Requery())
					{
						pCDBManageTrueDlg->MessageBox(L"����ƿ�ʧ��",L"���¾ƾ���ģ��ʧ��",MB_ICONERROR);
						return -1;
					}
					if(!TrueWineLib.IsEOF())
					{
						TrueWineLib.MoveFirst();
						while(!TrueWineLib.IsEOF())
						{
							TrueWineLib.MoveNext();
						}	
						TrueWineLib.MoveFirst();
					}

					int Num_of_EachBrandInFlavour=TrueWineLib.GetRecordCount();

					CMatrix tempMatrix(1,869);
					int m=1;
					while(!TrueWineLib.IsEOF())
					{
						CString2Matrix(tempMatrix,TrueWineLib.m_SpectrumData,L",");
						X_3.SetRow(Num_inX_3+m,tempMatrix);
						Y_3.SetItem(Num_inX_3+m,1,map_Brand[TrueWineLib.m_Brand]);
						TrueWineLib.MoveNext();
						m++;
					}
					BrandTypeCount++;
					Num_inX_3+=Num_of_EachBrandInFlavour;
					MLabels.SetItem(1,BrandTypeCount,map_Brand[TrueWineLib.m_Brand]);	
					MNum_EachLabel.SetItem(1,BrandTypeCount,Num_of_EachBrandInFlavour);
					rsforBrand.MoveNext();
				}

				CMatrix Y_3map=BuildMapLabel(Y_3);
				CMatrix CV=Plscvfold(X_3,Y_3map,X_3.m_row,"center");
				CMatrixIndex CV_indexmin;
				CV.FindMin(CV_indexmin);
				int bestPC=CV_indexmin.column;

				CMatrix W(X_3.m_column,bestPC);
				CMatrix T(X_3.m_row,bestPC);
				CMatrix P(X_3.m_column,bestPC);
				CMatrix Q(bestPC,1);

				CMatrix para2_X(1,X_3.m_column);
				CMatrix para1_X(1,X_3.m_column);
				Pretreat(X_3,"center",para1_X,para2_X);
				CMatrix X_3_mean=para1_X;

				CMatrix para2_Y(1,Y_3map.m_column);
				CMatrix para1_Y(1,Y_3map.m_column);	
				Pretreat(Y_3map,"center",para1_Y,para2_Y);
				CMatrix Y_3_mean=para1_Y;

				CMatrix B=Pls_nipals(X_3,Y_3map,W,T,P,Q,bestPC);
				CMatrix StandardPC(BrandTypeCount,bestPC);

				int begin=1;
				for(int i=1;i<=BrandTypeCount;i++)
				{
					StandardPC.SetRow(i,(T.GetRows(begin,begin+MNum_EachLabel(1,i)-1)).Mean(1));
					begin+=MNum_EachLabel(1,i);
				}
				//������ֵ
				CMatrix Threshold(1,BrandTypeCount);//�����������׼ֵ֮���������,�������׼ֵ֮���ƽ�������coeff����ȡ�ϴ���Ǹ�
				begin=1;
				for(int i=1;i<=BrandTypeCount;i++)
				{
					CMatrix tempdist(MNum_EachLabel(1,i),1);
					CMatrix tempPC=T.GetRows(begin,begin+MNum_EachLabel(1,i)-1);
					CMatrix covPC=tempPC.covariance();
					covPC.Inv();
					int k=1;
					for(int j=begin;j<=begin+MNum_EachLabel(1,i)-1;j++)
					{

						CMatrix diff=T.GetRow(j)-StandardPC.GetRow(i);
						tempdist.SetItem(k,1,(diff*covPC*diff.Transposition())(1,1));
						k++;
					}
					Threshold.SetItem(1,i,Max(tempdist.GetMax(),tempdist.GetMean()*coeff));
					begin=begin+MNum_EachLabel(1,i);
				}

				CString Weight;
				CString Labels;
				CString Num_EachLabel;
				CString standardPC;
				CString TrainsetPC;

				CString TrainsetMean;
				CString threshold;

				//Weight
				W.Matrix2CString(Weight,L",");
				//Labels
				MLabels.Matrix2CString(Labels,L",");
				//Num_EachLabel
				MNum_EachLabel.Matrix2CString(Num_EachLabel,L",");
				//standardPC
				StandardPC.Matrix2CString(standardPC,L",");
				//TrainsetPC
				T.Matrix2CString(TrainsetPC,L",");
				//TrainsetMean
				X_3_mean.Matrix2CString(TrainsetMean,L",");
				//��ֵ
				Threshold.Matrix2CString(threshold,L",");
				//����ģ��
				PLSModel.AddNew();
				PLSModel.m_AlcoholContent=AlcoholTypeIndex.m_AlcoholContent;
				PLSModel.m_Flavour=tempFlavour;
				PLSModel.m_FactorNum=bestPC;
				PLSModel.m_ClassCount=BrandTypeCount;
				PLSModel.m_Weight=Weight;
				PLSModel.m_Labels=Labels;
				PLSModel.m_Num_EachLabel=Num_EachLabel;
				PLSModel.m_StandardPC=standardPC;
				PLSModel.m_TrainsetPC=TrainsetPC;
				PLSModel.m_TrainsetMean=TrainsetMean;	
				PLSModel.m_Threshold=threshold;
				try
				{
					PLSModel.Update();
				}				
				catch(CDBException *e)
				{
					int nRet = GetLastError();
				}
			}
			/////////////////////////////////////////
			//��ʼƷ����
			rsforBrand.MoveFirst();
			while(!rsforBrand.IsEOF())
			{
				CString tempBrand;
				rsforBrand.GetFieldValue(L"Brand",tempBrand);


				CString command=_T("SELECT DISTINCT WineName FROM TrueWineLib");

				CString tempInBrand;
				CDatabase m_db4;
				CRecordset rsforInBrand;

				rsforInBrand.m_strFilter.Format(L"AlcoholContent='%s' AND Flavour='%s' AND Brand='%s'",AlcoholTypeIndex.m_AlcoholContent,tempFlavour,tempBrand);

				m_db4.OpenEx(_T("DSN=�׾Ƽ�������Դ���ݿ�;"),CDatabase::noOdbcDialog);
				rsforInBrand.m_pDatabase = &m_db4;	
				if (!rsforInBrand.Open(AFX_DB_USE_DEFAULT_TYPE,command))
				{
					CString str;
					str.Format(L"����%s��%s%s����ģ��ʧ�ܣ�",AlcoholTypeIndex.m_AlcoholContent,tempFlavour,tempBrand);
					pCDBManageTrueDlg->MessageBox(str,L"�����ݿ�ʧ��",MB_ICONERROR);
					return -1;
				}

				if(!rsforInBrand.IsEOF())
				{
					rsforInBrand.MoveFirst();

				}
				while(!rsforInBrand.IsEOF())
				{
					rsforInBrand.MoveNext();
				}

				int Num_of_InBrand=rsforInBrand.GetRecordCount();
				if(1!=Num_of_InBrand)
				{				
					TrueWineLib.m_strFilter.Format(L"AlcoholContent='%s' AND Flavour='%s' AND Brand='%s'",AlcoholTypeIndex.m_AlcoholContent,tempFlavour,tempBrand);
					TrueWineLib.Requery();
					TrueWineLib.MoveFirst();
					while(!TrueWineLib.IsEOF())
					{
						TrueWineLib.MoveNext();
					}
					TrueWineLib.MoveFirst();
					CMatrix MNum_EachLabel(1,Num_of_InBrand);
					CMatrix MLabels(1,Num_of_InBrand);

					CMatrix X_4(TrueWineLib.GetRecordCount(),869);
					CMatrix Y_4(TrueWineLib.GetRecordCount(),1);

					int InBrandTypeCount=0;
					int Num_inX_4=0;
					rsforInBrand.MoveFirst();			
					while(!rsforInBrand.IsEOF())
					{
						rsforInBrand.GetFieldValue(L"WineName",tempInBrand);
						TrueWineLib.m_strFilter.Format(L"AlcoholContent='%s'AND Flavour='%s' AND Brand='%s' AND WineName='%s'",
							AlcoholTypeIndex.m_AlcoholContent,tempFlavour,tempBrand,tempInBrand);

						if(!TrueWineLib.Requery())
						{
							pCDBManageTrueDlg->MessageBox(L"����ƿ�ʧ��",L"����Ʒ����ģ��ʧ��",MB_ICONERROR);
							return -1;
						}
						if(!TrueWineLib.IsEOF())
						{
							TrueWineLib.MoveFirst();
							while(!TrueWineLib.IsEOF())
							{
								TrueWineLib.MoveNext();
							}	
							TrueWineLib.MoveFirst();
						}

						int Num_of_EachTypeInBrand=TrueWineLib.GetRecordCount();
						CMatrix tempMatrix(1,869);
						int m=1;
						int DataStrlenth=TrueWineLib.m_SpectrumData.GetLength();
						while(!TrueWineLib.IsEOF())
						{
							CString2Matrix(tempMatrix,TrueWineLib.m_SpectrumData,L",");
							X_4.SetRow(Num_inX_4+m,tempMatrix);
							Y_4.SetItem(Num_inX_4+m,1,map_TrueWineIndex[TrueWineLib.m_WineName]);
							TrueWineLib.MoveNext();
							m++;
						}
						InBrandTypeCount++;
						Num_inX_4+=Num_of_EachTypeInBrand;					
						MLabels.SetItem(1,InBrandTypeCount,map_TrueWineIndex[TrueWineLib.m_WineName]);
						MNum_EachLabel.SetItem(1,InBrandTypeCount,Num_of_EachTypeInBrand);
						rsforInBrand.MoveNext();
					}
					CMatrix Y_4map=BuildMapLabel(Y_4);
					CMatrix CV=Plscvfold(X_4,Y_4map,X_4.m_row,"center");
					CMatrixIndex CV_indexmin;
					CV.FindMin(CV_indexmin);
					int bestPC=CV_indexmin.column;

					CMatrix W(X_4.m_column,bestPC);
					CMatrix T(X_4.m_row,bestPC);
					CMatrix P(X_4.m_column,bestPC);
					CMatrix Q(bestPC,1);

					CMatrix para2_X(1,X_4.m_column);//=X_1.Mean();
					CMatrix para1_X(1,X_4.m_column);//=X_1.Mean();	
					Pretreat(X_4,"center",para1_X,para2_X);
					CMatrix X_4_mean=para1_X;	

					CMatrix para2_Y(1,Y_4map.m_column);
					CMatrix para1_Y(1,Y_4map.m_column);	
					Pretreat(Y_4map,"center",para1_Y,para2_Y);
					CMatrix Y_4_mean=para1_Y;

					CMatrix B=Pls_nipals(X_4,Y_4map,W,T,P,Q,bestPC);

					CMatrix StandardPC(InBrandTypeCount,bestPC);


					int begin=1;
					for(int i=1;i<=InBrandTypeCount;i++)
					{
						StandardPC.SetRow(i,(T.GetRows(begin,begin+MNum_EachLabel(1,i)-1)).Mean(1));
						begin+=MNum_EachLabel(1,i);
					}

					CMatrix Threshold(1,InBrandTypeCount);//�����������׼ֵ֮���������,�������׼ֵ֮���ƽ�������coeff����ȡ�ϴ���Ǹ�
					begin=1;
					for(int i=1;i<=InBrandTypeCount;i++)
					{
						CMatrix tempdist(MNum_EachLabel(1,i),1);
						CMatrix tempPC=T.GetRows(begin,begin+MNum_EachLabel(1,i)-1);
						CMatrix covPC=tempPC.covariance();
						covPC.Inv();
						int k=1;
						for(int j=begin;j<=begin+MNum_EachLabel(1,i)-1;j++)
						{

							CMatrix diff=T.GetRow(j)-StandardPC.GetRow(i);
							tempdist.SetItem(k,1,(diff*covPC*diff.Transposition())(1,1));
							k++;
						}
						Threshold.SetItem(1,i,Max(tempdist.GetMax(),tempdist.GetMean()*coeff));
						begin=begin+MNum_EachLabel(1,i);
					}

					CString Weight;
					CString Labels;
					CString Num_EachLabel;
					CString standardPC;
					CString TrainsetPC;
					CString TrainsetMean;
					CString threshold;
					//Weight
					W.Matrix2CString(Weight,L",");
					//Labels
					MLabels.Matrix2CString(Labels,L",");
					//Num_EachLabel
					MNum_EachLabel.Matrix2CString(Num_EachLabel,L",");
					//standardPC
					StandardPC.Matrix2CString(standardPC,L",");
					//TrainsetPC
					T.Matrix2CString(TrainsetPC,L",");
					//TrainsetMean
					X_4_mean.Matrix2CString(TrainsetMean,L",");
					//��ֵ
					Threshold.Matrix2CString(threshold,L",");
					//����ģ��
					PLSModel.AddNew();
					PLSModel.m_AlcoholContent=AlcoholTypeIndex.m_AlcoholContent;
					PLSModel.m_Flavour=tempFlavour;
					PLSModel.m_Brand=tempBrand;
					PLSModel.m_FactorNum=bestPC;
					PLSModel.m_ClassCount=InBrandTypeCount;
					PLSModel.m_Weight=Weight;
					PLSModel.m_Labels=Labels;
					PLSModel.m_Num_EachLabel=Num_EachLabel;
					PLSModel.m_StandardPC=standardPC;
					PLSModel.m_TrainsetPC=TrainsetPC;
					PLSModel.m_TrainsetMean=TrainsetMean;	
					PLSModel.m_Threshold=threshold;
					PLSModel.Update();
				}
				rsforBrand.MoveNext();
				m_db4.Close();
			}
			rsforBrand.Close();
			///////////////////////////////
			m_db3.Close();
			rsforFlavour.MoveNext();
		}
		rsforFlavour.Close();
		//////////////
		m_db2.Close();
		AlcoholTypeIndex.MoveNext();
	}
	AlcoholTypeIndex.Close();

	PLSModel.Close();
	TrueWineLib.Close();
	WineTypeIndex.Close();
	AlcoholTypeIndex.Close();
	FlavourTypeIndex.Close();
	BrandTypeIndex.Close();
	pCDBManageTrueDlg->m_pCUpdateModelProgressDlg->ShowWindow(SW_HIDE);
	pCDBManageTrueDlg->MessageBox(L"ģ�͸��³ɹ���",L"ģ�͸���",MB_ICONINFORMATION);
	//ɾ������Ҫ��ģ���ļ�
	return 0;
}


void CDBManagePageTrue::OnBnClickedButtonShowallTrue()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_TrueWineLib.m_strFilter = L"";
	m_TrueWineLib.m_strSort=L"WineName";
	if(m_TrueWineLib.IsOpen())
	{
		m_TrueWineLib.Close();
	}
	if(!m_TrueWineLib.Open())
	{
		MessageBox(L"����ƿ�ʧ��!",L"��ʧ��",MB_ICONERROR);
		return;
	}
	m_TrueWineLib.m_strSort=L"WineName";
	ShowQuerySpecRecord();
}


void CDBManagePageTrue::OnBnClickedButtonDeleteTrue()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	int iitem=m_ListSpectrunQueryed.GetItemCount();
	if(0 == iitem)
		return;
	for(int i=0; i<m_ListSpectrunQueryed.GetItemCount(); i++)
	{
		if(m_ListSpectrunQueryed.GetItemState(i, LVIS_SELECTED)==
			LVIS_SELECTED || m_ListSpectrunQueryed.GetCheck(i))
		{
			m_TrueWineLib.MoveFirst();
			for(int j=0;j!=i;j++)m_TrueWineLib.MoveNext();
			m_TrueWineLib.Delete();
		}

	}
	UpdateIndexTable();
	m_TrueWineLib.Requery();
	ShowQuerySpecRecord();
}


void CDBManagePageTrue::ShowQuerySpecRecord(void)
{
	m_ListSpectrunQueryed.DeleteAllItems();
	if(m_TrueWineLib.IsEOF()||!m_TrueWineLib.IsOpen())
	{
		MessageBox(L"û����ؼ�¼!",L"�޼�¼",MB_ICONWARNING);
		return;
	}
	m_TrueWineLib.MoveFirst();

	int count=0;
	while(!m_TrueWineLib.IsEOF())
	{
		CString Number;
		Number.Format(L"%d",count+1);
		CString temp;
		temp.Format(L"%s,%s,%s",m_TrueWineLib.m_AlcoholContent,m_TrueWineLib.m_Flavour,m_TrueWineLib.m_Brand);
		m_ListSpectrunQueryed.InsertItem(count,Number);
		m_ListSpectrunQueryed.SetItemText(count,1,m_TrueWineLib.m_SpecID);
		m_ListSpectrunQueryed.SetItemText(count,2,m_TrueWineLib.m_WineName);
		m_ListSpectrunQueryed.SetItemText(count,3,temp);
		m_ListSpectrunQueryed.SetItemText(count,4,m_TrueWineLib.m_ProductionDate);
		m_ListSpectrunQueryed.SetItemText(count,5,m_TrueWineLib.m_ProductionBatchNo);
		m_ListSpectrunQueryed.SetItemText(count,6,m_TrueWineLib.m_ComFrom);
		m_ListSpectrunQueryed.SetItemText(count,7,m_TrueWineLib.m_ImportDate);
		m_ListSpectrunQueryed.SetItemText(count,8,m_TrueWineLib.m_Manager);

		m_TrueWineLib.MoveNext();
		count++;
	}
	m_ListSpectrunQueryed.UpdateWindow();
	m_TrueWineLib.MoveFirst();
}


void CDBManagePageTrue::OnBnClickedButtonClearspectoaddTrue()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_ListSpectrumToAdd.DeleteAllItems();
	m_ary_PicturesPath.RemoveAll();
	m_ary_PicturesName.RemoveAll();

	m_ary_SpecFileName.RemoveAll();
	m_ary_SpecFilePath.RemoveAll();
	m_ExcelInfoPath=L"";

	m_WineName=L"";
	m_AlcoholContent=L"";
    m_Flavour=L"";
	m_Brand=L"";
	m_BatchNo=L"";
	m_Comfrom=L"";
	m_SpecIDTitle=L"";
	m_Manager=L"";
}
void CDBManagePageTrue::OnBnClickedButtonSearchwithkeywordsTrue()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_TrueWineLib.m_strFilter=L"";
	m_TrueWineLib.m_strSort=L"WineName";
	if(!m_TrueWineLib.IsOpen())
	{
		m_TrueWineLib.Open();
	}
	UpdateData(TRUE);
	if(m_Key1.IsEmpty()&&m_Key2.IsEmpty()&&m_Key3.IsEmpty())
	{
		MessageBox(L"����д�����ؼ��ʣ�",L"��������",MB_ICONWARNING);
		return ;
	}
	if(!m_Key1.IsEmpty())
	{
		CString ComboField1;
		CString strFilter1;

		m_ComboField1.GetLBText(m_ComboField1.GetCurSel(),ComboField1);
		if(ComboField1 == L"������")
		{
			strFilter1.Format(L" (WineName LIKE '%%%s%%' OR AlcoholContent LIKE '%%%s%%' OR Flavour LIKE '%%%s%%' OR Brand LIKE '%%%s%%') ",
				m_Key1,m_Key1,m_Key1,m_Key1);
		}
		else if(ComboField1 == L"�׾�����")
		{
			strFilter1.Format(L" (WineName LIKE '%%%s%%') ",m_Key1);
		}
		else if(ComboField1 == L"�׾ƾƾ�����")
		{
			strFilter1.Format(L" (AlcoholContent LIKE '%%%s%%') ",m_Key1);
		}
		else if(ComboField1 == L"�׾�����")
		{
			strFilter1.Format(L" (Flavour LIKE '%%%s%%') ",m_Key1);
		}
		else if(ComboField1 == L"�׾�Ʒ��")
		{
			strFilter1.Format(L" (Brand LIKE '%%%s%%') ",m_Key1);
		}
		m_TrueWineLib.m_strFilter+=strFilter1;
	}
	if(!m_Key2.IsEmpty())
	{

		if(!m_TrueWineLib.m_strFilter.IsEmpty())
		{
			CString ComboAndOr1;
			m_ComboAndOr1.GetLBText(m_ComboAndOr1.GetCurSel(),ComboAndOr1);
			m_TrueWineLib.m_strFilter+=ComboAndOr1;
			m_TrueWineLib.m_strFilter+=L" ";
		}
		CString ComboField2;
		m_ComboField2.GetLBText(m_ComboField2.GetCurSel(),ComboField2);
		CString strFilter2;
		if(ComboField2 == L"������")
		{

			strFilter2.Format(L" (WineName LIKE '%%%s%%' OR AlcoholContent LIKE '%%%s%%' OR Flavour LIKE '%%%s%%' OR Brand LIKE '%%%s%%') ",
				m_Key2,m_Key2,m_Key2,m_Key2);
		}
		else if(ComboField2 == L"�׾�����")
		{
			strFilter2.Format(L" (WineName LIKE '%%%s%%') ",m_Key2);
		}
		else if(ComboField2 == L"�׾ƾƾ�����")
		{
			strFilter2.Format(L" (AlcoholContent LIKE '%%%s%%') ",m_Key2);
		}
		else if(ComboField2 == L"�׾�����")
		{
			strFilter2.Format(L" (Flavour LIKE '%%%s%%')",m_Key2);
		}
		else if(ComboField2 == L"�׾�Ʒ��")
		{
			strFilter2.Format(L" (Brand LIKE '%%%s%%') ",m_Key2);
		}
		m_TrueWineLib.m_strFilter+=strFilter2;
	}
	if(!m_Key3.IsEmpty())
	{
		if(!m_TrueWineLib.m_strFilter.IsEmpty())
		{
			CString ComboAndOr2;
			m_ComboAndOr2.GetLBText(m_ComboAndOr2.GetCurSel(),ComboAndOr2);
			m_TrueWineLib.m_strFilter+=ComboAndOr2;
			m_TrueWineLib.m_strFilter+=L" ";
		}
		CString ComboField3;
		m_ComboField3.GetLBText(m_ComboField3.GetCurSel(),ComboField3);

		CString strFilter3;
		if(ComboField3 == L"������")
		{

			strFilter3.Format(L" (WineName LIKE '%%%s%%' OR AlcoholContent LIKE '%%%s%%' OR Flavour LIKE '%%%s%%' OR Brand LIKE '%%%s%%') ",
				m_Key3,m_Key3,m_Key3,m_Key3);
		}
		else if(ComboField3 == L"�׾�����")
		{
			strFilter3.Format(L" (WineName LIKE '%%%s%%') ",m_Key3);
		}
		else if(ComboField3 == L"�׾ƾƾ�����")
		{
			strFilter3.Format(L" (AlcoholContent LIKE '%%%s%%') ",m_Key3);
		}
		else if(ComboField3 == L"�׾�����")
		{
			strFilter3.Format(L" (Flavour LIKE '%%%s%%') ",m_Key3);
		}
		else if(ComboField3 == L"�׾�Ʒ��")
		{
			strFilter3.Format(L" (Brand LIKE '%%%s%%') ",m_Key3);
		}
		m_TrueWineLib.m_strFilter+=strFilter3;
	}
	m_TrueWineLib.Requery();
	ShowQuerySpecRecord();
}


void CDBManagePageTrue::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CPropertyPage::OnShowWindow(bShow, nStatus);

	// TODO: �ڴ˴������Ϣ����������
}


void CDBManagePageTrue::OnSize(UINT nType, int cx, int cy)
{
	CPropertyPage::OnSize(nType, cx, cy);

	// TODO: �ڴ˴������Ϣ����������
	CWnd *pWnd[30];

	pWnd[0] = GetDlgItem(IDC_STATIC1);
	pWnd[1] = GetDlgItem(IDC_BUTTON_OPENDATAPACKAGE_TRUE);
	pWnd[2] = GetDlgItem(IDC_BUTTON_CLEARSPECTOADD_TRUE);
	pWnd[3] = GetDlgItem(IDC_BUTTON_ADDTOLIB_TRUE);
	pWnd[4] = GetDlgItem(IDC_LIST_TOADD_TRUE);
	pWnd[5] = GetDlgItem(IDC_STATIC_PICTUREOFSPECTOADD_TRUE);
	pWnd[6] = GetDlgItem(IDC_BUTTON_LAST_TRUE);
	pWnd[7] = GetDlgItem(IDC_BUTTON_NEXT_TRUE);

	pWnd[8] = GetDlgItem(IDC_STATIC2);
	pWnd[9] = GetDlgItem(IDC_COMBO_FIELD1_TRUE);
	pWnd[10] = GetDlgItem(IDC_EDIT_KEY1_TRUE);
	pWnd[11] = GetDlgItem(IDC_COMBO_ANDOR1_TRUE);
	pWnd[12] = GetDlgItem(IDC_COMBO_FIELD2_TRUE);
	pWnd[13] = GetDlgItem(IDC_EDIT_KEY2_TRUE);
	pWnd[14] = GetDlgItem(IDC_COMBO_ANDOR2_TRUE);
	pWnd[15] = GetDlgItem(IDC_COMBO_FIELD3_TRUE);
	pWnd[16] = GetDlgItem(IDC_EDIT_KEY3_TRUE);


	pWnd[17] = GetDlgItem(IDC_BUTTON_SEARCHWITHKEYWORDS_TRUE);
	pWnd[18] = GetDlgItem(IDC_BUTTON_SHOWALL_TRUE);
	pWnd[19] = GetDlgItem(IDC_BUTTON_DELETE_TRUE);

	pWnd[20] = GetDlgItem(IDC_BUTTON_UPDATEMODEL_TRUE);
	pWnd[21] = GetDlgItem(IDC_LIST_SPECLIB_TRUE);
	pWnd[22] = GetDlgItem(IDC_STATIC_PICTUREINLIB_TRUE);

	pWnd[23] = GetDlgItem(IDC_BUTTON_LASTLIB_TRUE);
	pWnd[24] = GetDlgItem(IDC_BUTTON_NEXTLIB_TRUE);


	pWnd[25] = GetDlgItem(IDC_BUTTON_UPDATELIBINFO_TRUE);
	pWnd[26] = GetDlgItem(IDC_BUTTON_SELECTALL_TRUE);
	pWnd[27] = GetDlgItem(IDC_BUTTON_INVERTSELECT_TRUE);
	pWnd[28] = GetDlgItem(IDC_BUTTON_CLEARSEARCH_TRUE);
	pWnd[29] = GetDlgItem(IDC_BUTTON_COMPONENTPREDICTMODELUPDATE);

	for(int i=0;i<30;i++)
	{
		//��ȡ�ؼ���� 
		if(pWnd[i])                                //�ж��Ƿ�Ϊ�գ���Ϊ�Ի��򴴽�ʱ����ô˺���������ʱ�ؼ���δ���� 
		{ 
			CRect rect;                            //��ȡ�ؼ��仯ǰ��С 
			pWnd[i]->GetWindowRect(&rect); 
			ScreenToClient(&rect);                 //���ؼ���Сת��Ϊ�ڶԻ����е��������� 
			rect.left=rect.left*cx/m_rect.Width(); //�����ؼ���С 
			rect.right=rect.right*cx/m_rect.Width(); 
			rect.top=rect.top*cy/m_rect.Height(); 
			rect.bottom=rect.bottom*cy/m_rect.Height(); 
			pWnd[i]->MoveWindow(rect);             //���ÿؼ���С 
		}
	}
	GetClientRect(&m_rect);//���仯��ĶԻ����С��Ϊ�ɴ�С 
}


void CDBManagePageTrue::OnBnClickedButtonSelectallTrue()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
    int nCount=m_ListSpectrunQueryed.GetItemCount();
    for(int i=0;i<nCount;i++)
    {
        m_ListSpectrunQueryed.SetCheck(i,TRUE);
    }
}


void CDBManagePageTrue::OnBnClickedButtonInvertselectTrue()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	int nCount=m_ListSpectrunQueryed.GetItemCount();
	for(int i=0;i<nCount;i++)
	{
		m_ListSpectrunQueryed.SetCheck(i,m_ListSpectrunQueryed.GetCheck(i)?FALSE:TRUE);
	}
}


void CDBManagePageTrue::OnBnClickedButtonClearsearch()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if(m_TrueWineLib.IsOpen())
	{
		m_TrueWineLib.Close();
	}
	m_ListSpectrunQueryed.DeleteAllItems();
	m_PicQueryedIndex=0;
	m_ary_PicturesQueryedPath.RemoveAll();
}

void CDBManagePageTrue::OnBnClickedButtonLastlibTrue()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if(m_ary_PicturesQueryedPath.GetCount()!=0)
	{
		if(m_PicQueryedIndex==0)
		{
			m_PicQueryedIndex=m_ary_PicturesQueryedPath.GetCount()-1;
		}
		else
		{
			m_PicQueryedIndex--;
		}
	}
	OnPaint();
}


void CDBManagePageTrue::OnBnClickedButtonNextlibTrue()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if(m_ary_PicturesQueryedPath.GetCount()!=0)
	{
		if(m_PicQueryedIndex==m_ary_PicturesQueryedPath.GetCount()-1)
		{
			m_PicQueryedIndex=0;
		}
		else
		{
			m_PicQueryedIndex++;
		}
	}
	OnPaint();
}


void CDBManagePageTrue::OnNMClickListSpeclibTrue(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	*pResult = 0;
	if(m_ListSpectrunQueryed.GetSelectedCount()==0)
	{
		return ;
	}
	int nItem=m_ListSpectrunQueryed.GetSelectionMark();
	if(m_TrueWineLib.IsOpen())
	{
		if(!m_TrueWineLib.IsEOF())
		{
			m_TrueWineLib.MoveFirst();
		}
		int count=0;
		for(;count<nItem;count++)
		{
			m_TrueWineLib.MoveNext();
		}

		CWinePictures WinePictures;
		WinePictures.Open();
		WinePictures.m_strFilter.Format(L" WineName = '%s' AND AlcoholContent = '%s' AND Flavour =  '%s' AND Brand = '%s' AND ProductionBatchNo = '%s'",
			   m_TrueWineLib.m_WineName,m_TrueWineLib.m_AlcoholContent,m_TrueWineLib.m_Flavour,m_TrueWineLib.m_Brand,m_TrueWineLib.m_ProductionBatchNo);

		WinePictures.Requery();
		m_ary_PicturesQueryedPath.RemoveAll();
		int n = WinePictures.GetRecordCount();
		if(!WinePictures.IsEOF())
		{
			WinePictures.MoveFirst();

			while(!WinePictures.IsEOF())
			{
				m_ary_PicturesQueryedPath.Add(WinePictures.m_PicturesDir);
				WinePictures.MoveNext();
			}
		}
		m_PicQueryedIndex=0;
	}
	OnPaint();
}


void CDBManagePageTrue::OnBnClickedButtonComponentpredictmodelupdate()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������|All Files (*.*)|*.*
	CFileDialog fileDialog = CFileDialog(TRUE, L"����", L"ѡ��ɷ�Ԥ��ģ���ļ�", OFN_READONLY,
		L"Model Files (*.csv)|*.csv||",this);
	if (fileDialog.DoModal() == IDOK)
	{
		CString strPath = fileDialog.GetPathName();
		CCSVOperator CSVOperator;
		CSVOperator.LoadCSV(strPath);

		CArray<CString,CString> ComponentArray;
		ComponentArray.Add(L"��ȩ");
		ComponentArray.Add(L"��������");
		ComponentArray.Add(L"�״�");
		ComponentArray.Add(L"��������");
		ComponentArray.Add(L"������");
		ComponentArray.Add(L"�춡��");
		ComponentArray.Add(L"��������");
		ComponentArray.Add(L"������");
		ComponentArray.Add(L"���촼");
		ComponentArray.Add(L"��������");
		ComponentArray.Add(L"��������");
		ComponentArray.Add(L"��������");
		ComponentArray.Add(L"��ȩ");
		ComponentArray.Add(L"����");
		ComponentArray.Add(L"��-���Ҵ�");
		ComponentArray.Add(L"����");
		ComponentArray.Add(L"����");

		CComponentPredictModel ComponentPredictModel;

		if(!ComponentPredictModel.Open())
		{
			MessageBox(L"���³ɷ�Ԥ��ģ��ʧ�ܣ������ݿ�ʧ�ܣ�",L"�ɷ�Ԥ��ģ�͸���",MB_ICONERROR);
			return;
		}
		//������ݿ�

		if(!ComponentPredictModel.IsEOF())
		{
			ComponentPredictModel.MoveFirst();
			while(!ComponentPredictModel.IsEOF())
			{
				ComponentPredictModel.Delete();
				ComponentPredictModel.MoveNext();
			}
		}
		CMatrix tmp(1,SPEC_LENTH);
		double tmpItem;
		double offset;
		for(int i = 0; i < COMPONENT_COUNT; ++i)
		{

			for(int j = 0; j < SPEC_LENTH+1 ; ++j)
			{	
				if(j != 0)
				{
					CSVOperator.GetDouble(j+1,i+1,tmpItem);
					tmp.SetItem(1,j,tmpItem);				
				}
				else
				{
					CSVOperator.GetDouble(j+1,i+1,offset);
				}
			}
			CString weight;
			tmp.Matrix2CString(weight,L",");

			ComponentPredictModel.AddNew();
			ComponentPredictModel.m_Component = ComponentArray[i];
			ComponentPredictModel.m_Weight = weight ;
			ComponentPredictModel.m_Offset = offset;
			ComponentPredictModel.Update();
		}
		ComponentPredictModel.Close();
		MessageBox(L"�ɷ�Ԥ��ģ�͸��³ɹ���",L"�ɷ�Ԥ��ģ�͸���",MB_ICONINFORMATION);
	}
}
