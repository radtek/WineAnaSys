// DBManageTrueDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "DataServerCenter.h"
#include "DBManageTrueDlg.h"
#include "afxdialogex.h"
#include "ExcelOperator.h"
#include "InputSpecDatasetInfoDlg.h"
#include "AlcoholTypeIndex.h"
#include "FlavourTypeIndex.h"
#include "BrandTypeIndex.h"
#include "WineTypeIndex.h"
#include "PLS.h"
#include "PLSModel.h"
#include <map>
using std::map;
//#include "AddToLibDlg.h"
// CDBManageTrueDlg �Ի���

IMPLEMENT_DYNAMIC(CDBManageTrueDlg, CPropertyPage)
	
CDBManageTrueDlg::CDBManageTrueDlg(CWnd* pParent /*=NULL*/)
	: CPropertyPage(CDBManageTrueDlg::IDD, pParent)
	, m_Field1(_T(""))
	, m_AndOr1(_T(""))
	, m_Field2(_T(""))
	, m_AndOr2(_T(""))
	, m_Field3(_T(""))
	, m_Key1(_T(""))
	, m_Key2(_T(""))
	, m_Key3(_T(""))
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

CDBManageTrueDlg::~CDBManageTrueDlg()
{
}

void CDBManageTrueDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
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
}


BEGIN_MESSAGE_MAP(CDBManageTrueDlg, CDialogEx)
	ON_WM_SIZE()
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_BUTTON_OPENDATAPACKAGE_TRUE, &CDBManageTrueDlg::OnBnClickedButtonOpendatapackageTrue)
	ON_BN_CLICKED(IDC_BUTTON_LAST_TRUE, &CDBManageTrueDlg::OnBnClickedButtonLastTrue)
	ON_BN_CLICKED(IDC_BUTTON_NEXT_TRUE, &CDBManageTrueDlg::OnBnClickedButtonNextTrue)
	ON_BN_CLICKED(IDC_BUTTON_ADDTOLIB_TRUE, &CDBManageTrueDlg::OnBnClickedButtonAddtolibTrue)
	ON_BN_CLICKED(IDC_BUTTON_UPDATELIBINFO, &CDBManageTrueDlg::OnBnClickedButtonUpdatelibinfo)
	ON_BN_CLICKED(IDC_BUTTON_SHOWALL_TRUE, &CDBManageTrueDlg::OnBnClickedButtonShowallTrue)
	ON_BN_CLICKED(IDC_BUTTON_DELETE_TRUE, &CDBManageTrueDlg::OnBnClickedButtonDeleteTrue)
	ON_BN_CLICKED(IDC_BUTTON_CLEARSPECTOADD_TRUE, &CDBManageTrueDlg::OnBnClickedButtonClearspectoaddTrue)
	ON_BN_CLICKED(IDC_BUTTON_UPDATEMODEL_TRUE, &CDBManageTrueDlg::OnBnClickedButtonUpdatemodelTrue)
	ON_BN_CLICKED(IDC_BUTTON_SEARCHWITHKEYWORDS_TRUE, &CDBManageTrueDlg::OnBnClickedButtonSearchwithkeywordsTrue)
END_MESSAGE_MAP()


// CDBManageTrueDlg ��Ϣ�������


void CDBManageTrueDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO: �ڴ˴������Ϣ����������
	/*CWnd *pWnd[26]; 
	pWnd[0] = GetDlgItem(IDC_BUTTON_OPENDATAPACKAGE_TRUE);
	pWnd[1] = GetDlgItem(IDC_BUTTON_CLEARSPECTOADD_TRUE);
	pWnd[2] = GetDlgItem(IDC_BUTTON_ADDTOLIB_TRUE);
	pWnd[3] = GetDlgItem(IDC_LIST_TOADD_TRUE);
	pWnd[4] = GetDlgItem(IDC_STATIC_PICTUREOFSPECTOADD_TRUE);
	pWnd[5] = GetDlgItem(IDC_BUTTON_LAST_TRUE);
	pWnd[6] = GetDlgItem(IDC_BUTTON_NEXT_TRUE);
	pWnd[7] = GetDlgItem(IDC_STATIC1);
	pWnd[8] = GetDlgItem(IDC_COMBO_FIELD1_TRUE);
	pWnd[9] = GetDlgItem(IDC_EDIT_KEY1_TRUE);
	pWnd[10] = GetDlgItem(IDC_COMBO_ANDOR1_TRUE);
	pWnd[11] = GetDlgItem(IDC_COMBO_FIELD2_TRUE);
	pWnd[12] = GetDlgItem(IDC_EDIT_KEY2_TRUE);
	pWnd[13] = GetDlgItem(IDC_COMBO_ANDOR2_TRUE);
	pWnd[14] = GetDlgItem(IDC_COMBO_FIELD3_TRUE);
	pWnd[15] = GetDlgItem(IDC_EDIT_KEY3_TRUE);
	pWnd[16] = GetDlgItem(IDC_BUTTON_SEARCHWITHKEYWORDS_TRUE);
	pWnd[17] = GetDlgItem(IDC_BUTTON_SHOWALL_TRUE);
	pWnd[18] = GetDlgItem(IDC_BUTTON_DELETE_TRUE);
	pWnd[19] = GetDlgItem(IDC_BUTTON_UPDATEMODEL_TRUE);
	pWnd[20] = GetDlgItem(IDC_LIST_SPECLIB_TRUE);
	pWnd[21] = GetDlgItem(IDC_STATIC_PICTUREINLIB_TRUE);
	pWnd[22] = GetDlgItem(IDC_BUTTON_LASTLIB_TRUE);
	pWnd[23] = GetDlgItem(IDC_BUTTON_NEXTLIB_TRUE);
	pWnd[24] = GetDlgItem(IDC_STATIC2);
	pWnd[25] = GetDlgItem(IDC_BUTTON_UPDATELIBINFO);

	

	for(int i=0;i<26;i++)
	{
		//��ȡ�ؼ���� 
		if(pWnd[i])//�ж��Ƿ�Ϊ�գ���Ϊ�Ի��򴴽�ʱ����ô˺���������ʱ�ؼ���δ���� 
		{ 
			CRect rect; //��ȡ�ؼ��仯ǰ��С 
			pWnd[i]->GetWindowRect(&rect); 
			ScreenToClient(&rect);//���ؼ���Сת��Ϊ�ڶԻ����е��������� 
			//cx/m_rect.Width()Ϊ�Ի����ں���ı仯���� 
			rect.left=rect.left*cx/m_rect.Width();//////�����ؼ���С 
			rect.right=rect.right*cx/m_rect.Width(); 
			rect.top=rect.top*cy/m_rect.Height(); 
			rect.bottom=rect.bottom*cy/m_rect.Height(); 
			pWnd[i]->MoveWindow(rect);//���ÿؼ���С 
		}
	}

	GetClientRect(&m_rect);//���仯��ĶԻ����С��Ϊ�ɴ�С */

}


BOOL CDBManageTrueDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
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
	m_ListSpectrumToAdd.InsertColumn( 0, L"�����ļ���", LVCFMT_LEFT,100 );//������ 
	m_ListSpectrumToAdd.InsertColumn( 1, L"�׾�����", LVCFMT_LEFT, 100 );//������ 
	m_ListSpectrumToAdd.InsertColumn( 2, L"�׾���Ϣ", LVCFMT_LEFT, 150 );//������ 
	m_ListSpectrumToAdd.InsertColumn( 3, L"��������", LVCFMT_LEFT, 100 );//������ 
	m_ListSpectrumToAdd.InsertColumn( 4, L"��������", LVCFMT_LEFT, 100 );//������ 
	m_ListSpectrumToAdd.InsertColumn( 5, L"��Դ˵��", LVCFMT_LEFT, 100 );//������ 
	m_ListSpectrumToAdd.InsertColumn( 6, L"��������", LVCFMT_LEFT, 100 );//������ 

	DWORD dwStyle = m_ListSpectrumToAdd.GetExtendedStyle(); //��ȡ��ǰ��չ��ʽ
	dwStyle |= LVS_EX_FULLROWSELECT; //ѡ��ĳ��ʹ���и�����report���ʱ��
	dwStyle |= LVS_EX_GRIDLINES; //�����ߣ�report���ʱ��
	//  dwStyle |= LVS_EX_CHECKBOXES; //itemǰ����checkbox�ؼ�
	m_ListSpectrumToAdd.SetExtendedStyle(dwStyle); //������չ���	

	m_ListSpectrunQueryed.InsertColumn( 0, L"�׾�����", LVCFMT_LEFT, 100 );//������ 
	m_ListSpectrunQueryed.InsertColumn( 1, L"�׾���Ϣ", LVCFMT_LEFT, 150 );//������ 
	m_ListSpectrunQueryed.InsertColumn( 2, L"��������", LVCFMT_LEFT, 100 );//������
	m_ListSpectrunQueryed.InsertColumn( 3, L"��������", LVCFMT_LEFT, 100 );//������
	m_ListSpectrunQueryed.InsertColumn( 4, L"��Դ˵��", LVCFMT_LEFT, 100 );//������
	m_ListSpectrunQueryed.InsertColumn( 5, L"�������", LVCFMT_LEFT, 100 );//������

	dwStyle = m_ListSpectrunQueryed.GetExtendedStyle(); //��ȡ��ǰ��չ��ʽ
	dwStyle |= LVS_EX_FULLROWSELECT; //ѡ��ĳ��ʹ���и�����report���ʱ��
	dwStyle |= LVS_EX_GRIDLINES; //�����ߣ�report���ʱ��
	//  dwStyle |= LVS_EX_CHECKBOXES; //itemǰ����checkbox�ؼ�

	m_ListSpectrunQueryed.SetExtendedStyle(dwStyle); 

	GetClientRect(&m_rect);
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}


void CDBManageTrueDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: �ڴ˴������Ϣ����������
	if(!m_ary_PicturesPath.IsEmpty())
	{
		CRect rect;
		GetDlgItem(IDC_STATIC_PICTUREOFSPECTOADD_TRUE)->GetWindowRect(rect);
		DisplayPic(m_ary_PicturesPath[m_PicIndex],this->GetDlgItem(IDC_STATIC_PICTUREOFSPECTOADD_TRUE)->GetSafeHwnd(), rect.Width(), rect.Height());
	}
	// ��Ϊ��ͼ��Ϣ���� CDialogEx::OnPaint()
}


void CDBManageTrueDlg::OnBnClickedButtonOpendatapackageTrue()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	BROWSEINFO bi;
	char Buffer[MAX_PATH];
	//CString m_DatasetFoldPath ;//��·��������һ��CString������
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

	/*m_ary_PicturesPath.RemoveAll();
	m_ary_SpecFileName.RemoveAll();
	m_ary_SpecFilePath.RemoveAll();
	m_ListSpectrumToAdd.DeleteAllItems();
	//m_ary_ProjectPathNew.RemoveAll();*/
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
		if(str.Right(4)==L".jpg" ||str.Right(4)==L".bmp"||str.Right(4)==L".gif"&&L"." !=str &&L".." != str)m_ary_PicturesPath.Add(finder.GetFilePath());
		if(str==L"����˵��.xls"||str==L"����˵��.xlsx"&&L"." !=str &&L".." != str)m_ExcelInfoPath=finder.GetFilePath();
	}
	finder.Close();
	if(m_ary_SpecFileName.GetCount()==0)
	{
		MessageBox(L"���ļ�����û�й涨��ʽ�Ĺ������ݣ�������ѡ��",L"����",MB_ICONWARNING);
		return;
	}
	if(m_ary_SpecFileName.GetCount()<30)
	{
		MessageBox(L"��������̫��",L"����",MB_ICONWARNING);
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
		//strDirExcel.Replace(L"\\",L"\\\\");
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
		//Wineinfo+=Wineinfotemp;

		ExcelOperator.GetItem(6,1,L"String",m_BatchNo);
		//Wineinfo+=Wineinfotemp;

		ExcelOperator.GetItem(7,1,L"String",m_Comfrom);
		ExcelOperator.GetItem(8,1,L"Date",ReceiveDate);

		ExcelOperator.ReleaseExcel();
	}


	SortCArrayByFirst(m_ary_SpecFileName,m_ary_SpecFilePath);

	for(int i=0;i<m_ary_SpecFileName.GetCount();i++)
	{
		//CString Current=m_ary_SpecFilePath[i];

		m_ListSpectrumToAdd.InsertItem(i,m_ary_SpecFileName[i]);
		m_ListSpectrumToAdd.SetItemText(i,1,m_WineName);
		m_ListSpectrumToAdd.SetItemText(i,2,Wineinfo);
		m_ListSpectrumToAdd.SetItemText(i,3,m_ProductionDate);
		m_ListSpectrumToAdd.SetItemText(i,4,m_BatchNo);
		m_ListSpectrumToAdd.SetItemText(i,5,m_Comfrom);
		m_ListSpectrumToAdd.SetItemText(i,6,ReceiveDate);
	}

}




void CDBManageTrueDlg::OnBnClickedButtonLastTrue()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if(m_PicIndex==0)
	{
		m_PicIndex=m_ary_PicturesPath.GetCount()-1;
	}
	else
	{
		m_PicIndex--;
	}
	OnPaint();
}


void CDBManageTrueDlg::OnBnClickedButtonNextTrue()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if(m_PicIndex==m_ary_PicturesPath.GetCount()-1)
	{
		m_PicIndex=0;
	}
	else
	{
		m_PicIndex++;
	}
	OnPaint();
}


void CDBManageTrueDlg::OnBnClickedButtonAddtolibTrue()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if(NULL!=m_pAddToLibDlg)
	{
		m_pAddToLibDlg->DestroyWindow();
		m_pAddToLibDlg=NULL;
	}
	m_pAddToLibDlg=new CAddToLibDlg(m_ary_SpecFilePath.GetCount(),1);
	m_pAddToLibDlg->Create(IDD_DIALOG_ADDTOLIBPROGRESS,this);
	m_pAddToLibDlg->ShowWindow(SW_SHOW);
	pThreadAddSpecToLib=AfxBeginThread(ThreadAddSpecToLib,this);


}
UINT ThreadAddSpecToLib(LPVOID lpParam)
{

	CDBManageTrueDlg* pDBManageTrueDlg=(CDBManageTrueDlg*)lpParam;
	CTrueWineLib TrueWineLib;
	CCSVOperator CSVOperator;
	CTime tm=CTime::GetCurrentTime();
	CString Currenttime;
	Currenttime=tm.Format("%Y-%m-%d");
	//Currenttime.Format(L"%d-%d-%d",tm.GetYear(),tm.GetMonth(),tm.GetDay());

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

		double tempSpecdata[869];
		CString temparray;
		CString temp;
		int j=0;
		for(;j<868;j++)
		{
			CSVOperator.GetDouble(j+1,2,tempSpecdata[j]);
			temp.Format(_T("%lf"),tempSpecdata[j]);
			temparray+=temp;
			temparray+=",";
		}
		CSVOperator.GetDouble(j+1,2,tempSpecdata[j]);
		temp.Format(_T("%lf"),tempSpecdata[j]);
		temparray+=temp;
		TrueWineLib.AddNew();

		TrueWineLib.m_WineName=pDBManageTrueDlg->m_WineName;
		TrueWineLib.m_AlcoholContent=pDBManageTrueDlg->m_AlcoholContent;
		TrueWineLib.m_Flavour=pDBManageTrueDlg->m_Flavour;
		TrueWineLib.m_Brand=pDBManageTrueDlg->m_Brand;
		TrueWineLib.m_ProductionBatchNo=pDBManageTrueDlg->m_BatchNo;
		TrueWineLib.m_ProductionDate=pDBManageTrueDlg->m_ProductionDate;
		TrueWineLib.m_SpectrumData=temparray;
		TrueWineLib.m_ComFrom=pDBManageTrueDlg->m_Comfrom;
		TrueWineLib.m_ImportDate=Currenttime;

		TrueWineLib.Update();

	}

	pDBManageTrueDlg->UpdateIndexTable();

	pDBManageTrueDlg->m_pAddToLibDlg->ShowWindow(SW_HIDE);
	//pDBManageTrueDlg->m_pAddToLibDlg=NULL;
	pDBManageTrueDlg->MessageBox(L"���ݵ���ɹ�",L"�������",MB_ICONINFORMATION);
	return 0;
}



void CDBManageTrueDlg::UpdateIndexTable(void)
{
	//CTrueWineLib TrueWineLib;

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


void CDBManageTrueDlg::OnBnClickedButtonUpdatelibinfo()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateIndexTable();

	MessageBox(L"���ݿ���Ϣ���³ɹ�!",L"��Ϣ����",MB_ICONINFORMATION);
}
void CDBManageTrueDlg::OnBnClickedButtonUpdatemodelTrue()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdatePLSModel();
}

void CDBManageTrueDlg::UpdatePLSModel(void)
{
	pThreadUpdatePLSModel=AfxBeginThread(ThreadUpdatePLSModel,this);
}
UINT ThreadUpdatePLSModel(LPVOID lpParam)
{
	double coeff=2;//ϵ��

	CDBManageTrueDlg* pCDBManageTrueDlg=(CDBManageTrueDlg*)lpParam;
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
		//CSVMobj tempsvmobj1(869);
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
				//TrueWineLib.Close();
				AlcoholTypeCount++;
				Num_inX_1+=Num_of_EachAlcoholContent;
				MLabels.SetItem(1,AlcoholTypeCount,map_AlcoholContent[TrueWineLib.m_AlcoholContent]);	
				MNum_EachLabel.SetItem(1,AlcoholTypeCount,Num_of_EachAlcoholContent);
				AlcoholTypeIndex.MoveNext();

			}
		}		
		//ѵ���ƾ��ȷ���ģ��
		CMatrix Y_1map=BuildMapLabel(Y_1);

		//CMatrix X_1_mean=X_1.Mean();

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

		//map<int,int> Label_map;



		CMatrix para2_Y(1,Y_1map.m_column);
		CMatrix para1_Y(1,Y_1map.m_column);
		Pretreat(Y_1map,"center",para1_Y,para2_Y);
		CMatrix Y_1_mean=para1_Y;

		CMatrix B=Pls_nipals(X_1,Y_1map,W,T,P,Q,bestPC);

		CMatrix StandardPC(AlcoholTypeCount,bestPC);



		int begin=1;
		for(int i=1;i<=AlcoholTypeCount;i++)
		{
			StandardPC.SetRow(i,(T.GetRows(begin,begin+MNum_EachLabel(1,i)-1)).Mean());
			begin+=MNum_EachLabel(1,i);
		}
		//������ֵ
		CMatrix Threshold(1,AlcoholTypeCount);//�����������׼ֵ֮���������,�������׼ֵ֮���ƽ�������coeff����ȡ�ϴ���Ǹ�

		//CMatrix Threshold2(1,AlcoholTypeCount);//����
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
		CDatabase m_db;
		CRecordset rsforFlavour;
		rsforFlavour.m_strFilter.Format(L"AlcoholContent='%s'",AlcoholTypeIndex.m_AlcoholContent);
		m_db.OpenEx(_T("DSN=�׾Ƽ�������Դ���ݿ�;"),CDatabase::noOdbcDialog);
		rsforFlavour.m_pDatabase = &m_db;	
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
			//CMatrix X_2_mean=X_2.Mean();
			CMatrix Y_2map=BuildMapLabel(Y_2);
			CMatrix CV=Plscvfold(X_2,Y_2map,X_2.m_row,"center");
			CMatrixIndex CV_indexmin;
			CV.FindMin(CV_indexmin);
			int bestPC=CV_indexmin.column;

			CMatrix W(X_2.m_column,bestPC);
			CMatrix T(X_2.m_row,bestPC);
			CMatrix P(X_2.m_column,bestPC);
			CMatrix Q(bestPC,1);

			CMatrix para2_X(1,X_2.m_column);//=X_1.Mean();
			CMatrix para1_X(1,X_2.m_column);//=X_1.Mean();	
			Pretreat(X_2,"center",para1_X,para2_X);
			CMatrix X_2_mean=para1_X;		




			CMatrix para2_Y(1,Y_2map.m_column);//=X_1.Mean();
			CMatrix para1_Y(1,Y_2map.m_column);//=X_1.Mean();	
			Pretreat(Y_2map,"center",para1_Y,para2_Y);
			CMatrix Y_2_mean=para1_Y;

			CMatrix B=Pls_nipals(X_2,Y_2map,W,T,P,Q,bestPC);

			CMatrix StandardPC(FlavourTypeCount,bestPC);


			int begin=1;
			for(int i=1;i<=FlavourTypeCount;i++)
			{
				StandardPC.SetRow(i,(T.GetRows(begin,begin+MNum_EachLabel(1,i)-1)).Mean());
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
			CDatabase m_db;
			CRecordset rsforBrand;

			rsforBrand.m_strFilter.Format(L"AlcoholContent='%s' AND Flavour='%s'",AlcoholTypeIndex.m_AlcoholContent,tempFlavour);

			m_db.OpenEx(_T("DSN=�׾Ƽ�������Դ���ݿ�;"),CDatabase::noOdbcDialog);
			rsforBrand.m_pDatabase = &m_db;	
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
					//TrueWineLib.MoveFirst();
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
				//
				//	CMatrix X_3_mean=X_3.Mean();
				CMatrix Y_3map=BuildMapLabel(Y_3);

				CMatrix CV=Plscvfold(X_3,Y_3map,X_3.m_row,"center");
				CMatrixIndex CV_indexmin;
				CV.FindMin(CV_indexmin);
				int bestPC=CV_indexmin.column;

				CMatrix W(X_3.m_column,bestPC);
				CMatrix T(X_3.m_row,bestPC);
				CMatrix P(X_3.m_column,bestPC);
				CMatrix Q(bestPC,1);

				CMatrix para2_X(1,X_3.m_column);//=X_1.Mean();
				CMatrix para1_X(1,X_3.m_column);//=X_1.Mean();	
				Pretreat(X_3,"center",para1_X,para2_X);
				CMatrix X_3_mean=para1_X;



				CMatrix para2_Y(1,Y_3map.m_column);//=X_1.Mean();
				CMatrix para1_Y(1,Y_3map.m_column);//=X_1.Mean();	
				Pretreat(Y_3map,"center",para1_Y,para2_Y);
				CMatrix Y_3_mean=para1_Y;

				CMatrix B=Pls_nipals(X_3,Y_3map,W,T,P,Q,bestPC);
				CMatrix StandardPC(BrandTypeCount,bestPC);

				int begin=1;
				for(int i=1;i<=BrandTypeCount;i++)
				{
					StandardPC.SetRow(i,(T.GetRows(begin,begin+MNum_EachLabel(1,i)-1)).Mean());
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
				PLSModel.Update();
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
				CDatabase m_db;
				CRecordset rsforInBrand;

				rsforInBrand.m_strFilter.Format(L"AlcoholContent='%s' AND Flavour='%s' AND Brand='%s'",AlcoholTypeIndex.m_AlcoholContent,tempFlavour,tempBrand);

				m_db.OpenEx(_T("DSN=�׾Ƽ�������Դ���ݿ�;"),CDatabase::noOdbcDialog);
				rsforInBrand.m_pDatabase = &m_db;	
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
						//TrueWineLib.MoveFirst();
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
					//
					//	CMatrix X_4_mean=X_4.Mean();
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

					//map<int,int> Label_map;

					CMatrix para2_Y(1,Y_4map.m_column);//=X_1.Mean();
					CMatrix para1_Y(1,Y_4map.m_column);//=X_1.Mean();	
					Pretreat(Y_4map,"center",para1_Y,para2_Y);
					CMatrix Y_4_mean=para1_Y;

					CMatrix B=Pls_nipals(X_4,Y_4map,W,T,P,Q,bestPC);

					CMatrix StandardPC(InBrandTypeCount,bestPC);


					int begin=1;
					for(int i=1;i<=InBrandTypeCount;i++)
					{
						StandardPC.SetRow(i,(T.GetRows(begin,begin+MNum_EachLabel(1,i)-1)).Mean());
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
			}
			///////////////////////////////
			rsforFlavour.MoveNext();
		}
		AlcoholTypeIndex.MoveNext();
	}

	TrueWineLib.Close();
	WineTypeIndex.Close();
	AlcoholTypeIndex.Close();
	FlavourTypeIndex.Close();
	BrandTypeIndex.Close();
	pCDBManageTrueDlg->MessageBox(L"ģ�͸��³ɹ���",L"ģ�͸���",MB_ICONINFORMATION);
	//ɾ������Ҫ��ģ���ļ�

}

void CDBManageTrueDlg::OnBnClickedButtonShowallTrue()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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


void CDBManageTrueDlg::OnBnClickedButtonDeleteTrue()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	POSITION pos=m_ListSpectrunQueryed.GetFirstSelectedItemPosition(); 

	int nItem;
	while(pos)
	{
		nItem = m_ListSpectrunQueryed.GetNextSelectedItem(pos);
		m_TrueWineLib.MoveFirst();
		for(int i=0;i!=nItem;i++)m_TrueWineLib.MoveNext();
		m_TrueWineLib.Delete();

	}

	POSITION  sSelPos = NULL;
	while(sSelPos = m_ListSpectrunQueryed.GetFirstSelectedItemPosition())
	{
		//int nItem = -1;
		nItem = m_ListSpectrunQueryed.GetNextSelectedItem(sSelPos);
		if(nItem >= 0 && nItem<m_ListSpectrunQueryed.GetItemCount())
		{
			m_ListSpectrunQueryed.DeleteItem(nItem);	        
		}
	}
	UpdateIndexTable();
	//m_TrueWineLib.Requery();
	//ShowQuerySpecRecord();
}


void CDBManageTrueDlg::ShowQuerySpecRecord(void)
{
	m_ListSpectrunQueryed.DeleteAllItems();
	if(m_TrueWineLib.IsEOF())
	{
		MessageBox(L"û����ؼ�¼!",L"�޼�¼",MB_ICONWARNING);
		return;
	}
	m_TrueWineLib.MoveFirst();

	int count=0;
	while(!m_TrueWineLib.IsEOF())
	{
		CString temp;
		temp.Format(L"%s,%s,%s",m_TrueWineLib.m_AlcoholContent,m_TrueWineLib.m_Flavour,m_TrueWineLib.m_Brand);
		m_ListSpectrunQueryed.InsertItem(count,m_TrueWineLib.m_WineName);
		m_ListSpectrunQueryed.SetItemText(count,1,temp);
		m_ListSpectrunQueryed.SetItemText(count,2,m_TrueWineLib.m_ProductionDate);
		m_ListSpectrunQueryed.SetItemText(count,3,m_TrueWineLib.m_ProductionBatchNo);
		m_ListSpectrunQueryed.SetItemText(count,4,m_TrueWineLib.m_ComFrom);
		m_ListSpectrunQueryed.SetItemText(count,5,m_TrueWineLib.m_ImportDate);


		m_TrueWineLib.MoveNext();
		count++;
	}
	m_TrueWineLib.MoveFirst();
}


void CDBManageTrueDlg::OnBnClickedButtonClearspectoaddTrue()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	m_ListSpectrumToAdd.DeleteAllItems();
	m_ary_PicturesPath.RemoveAll();
	m_ary_SpecFileName.RemoveAll();
	m_ary_SpecFilePath.RemoveAll();
	m_ExcelInfoPath=L"";

}





void CDBManageTrueDlg::OnBnClickedButtonSearchwithkeywordsTrue()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if(!m_TrueWineLib.IsOpen())
	{
		m_TrueWineLib.Open();
	}
	UpdateData(TRUE);

	m_TrueWineLib.m_strFilter=L"";

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
			strFilter1.Format(L" (WineName = '%s' OR AlcoholContent = '%s' OR Flavour = '%s' OR Brand = '%s') ",
				m_Key1,m_Key1,m_Key1,m_Key1);
		}
		else if(ComboField1 == L"�׾�����")
		{
			strFilter1.Format(L" (WineName = '%s') ",m_Key1);
		}
		else if(ComboField1 == L"�׾ƾƾ�����")
		{
			strFilter1.Format(L" (AlcoholContent = '%s') ",m_Key1);
		}
		else if(ComboField1 == L"�׾�����")
		{
			strFilter1.Format(L" (Flavour = '%s') ",m_Key1);
		}
		else if(ComboField1 == L"�׾�Ʒ��")
		{
			strFilter1.Format(L" (Brand = '%s') ",m_Key1);
		}
		m_TrueWineLib.m_strFilter+=strFilter1;
	}

	if(!m_Key2.IsEmpty())
	{
		CString ComboAndOr1;
		m_ComboAndOr1.GetLBText(m_ComboAndOr1.GetCurSel(),ComboAndOr1);

		m_TrueWineLib.m_strFilter+=ComboAndOr1;
		m_TrueWineLib.m_strFilter+=L" ";

		CString ComboField2;
		m_ComboField2.GetLBText(m_ComboField2.GetCurSel(),ComboField2);

		CString strFilter2;
		if(ComboField2 == L"������")
		{

			strFilter2.Format(L" (WineName = '%s' OR AlcoholContent = '%s' OR Flavour = '%s' OR Brand = '%s') ",
				m_Key2,m_Key2,m_Key2,m_Key2);
		}
		else if(ComboField2 == L"�׾�����")
		{
			strFilter2.Format(L" (WineName = '%s') ",m_Key2);
		}
		else if(ComboField2 == L"�׾ƾƾ�����")
		{
			strFilter2.Format(L" (AlcoholContent = '%s') ",m_Key2);
		}
		else if(ComboField2 == L"�׾�����")
		{
			strFilter2.Format(L" (Flavour = '%s')",m_Key2);
		}
		else if(ComboField2 == L"�׾�Ʒ��")
		{
			strFilter2.Format(L" (Brand = '%s') ",m_Key2);
		}
		m_TrueWineLib.m_strFilter+=strFilter2;
	}
	///////
	if(!m_Key3.IsEmpty())
	{
		CString ComboAndOr2;
		m_ComboAndOr2.GetLBText(m_ComboAndOr2.GetCurSel(),ComboAndOr2);

		m_TrueWineLib.m_strFilter+=ComboAndOr2;
		m_TrueWineLib.m_strFilter+=L" ";

		CString ComboField3;
		m_ComboField3.GetLBText(m_ComboField3.GetCurSel(),ComboField3);

		CString strFilter3;
		if(ComboField3 == L"������")
		{

			strFilter3.Format(L" (WineName = '%s' OR AlcoholContent = '%s' OR Flavour = '%s' OR Brand = '%s') ",
				m_Key3,m_Key3,m_Key3,m_Key3);
		}
		else if(ComboField3 == L"�׾�����")
		{
			strFilter3.Format(L" (WineName = '%s') ",m_Key3);
		}
		else if(ComboField3 == L"�׾ƾƾ�����")
		{
			strFilter3.Format(L" (AlcoholContent = '%s') ",m_Key3);
		}
		else if(ComboField3 == L"�׾�����")
		{
			strFilter3.Format(L" (Flavour = '%s') ",m_Key3);
		}
		else if(ComboField3 == L"�׾�Ʒ��")
		{
			strFilter3.Format(L" (Brand = '%s') ",m_Key3);
		}
		m_TrueWineLib.m_strFilter+=strFilter3;
	}
	m_TrueWineLib.Requery();
	ShowQuerySpecRecord();
}
