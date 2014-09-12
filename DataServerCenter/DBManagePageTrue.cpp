// DBManagePageTrue.cpp : 实现文件
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
#include <map>
using std::map;
extern CString g_CurrentDir;
// CDBManagePageTrue 对话框

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
	//ON_BN_CLICKED(IDC_BUTTON1, &CDBManagePageTrue::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON_LASTLIB_TRUE, &CDBManagePageTrue::OnBnClickedButtonLastlibTrue)
	ON_BN_CLICKED(IDC_BUTTON_NEXTLIB_TRUE, &CDBManagePageTrue::OnBnClickedButtonNextlibTrue)
	ON_NOTIFY(NM_CLICK, IDC_LIST_SPECLIB_TRUE, &CDBManagePageTrue::OnNMClickListSpeclibTrue)
	ON_WM_PAINT()
END_MESSAGE_MAP()


// CDBManagePageTrue 消息处理程序
BOOL CDBManagePageTrue::OnInitDialog()
{
	CPropertyPage::OnInitDialog();
	// TODO:  在此添加额外的初始化
	m_ComboField1.InsertString(0,L"所有项");
	m_ComboField1.InsertString(1,L"白酒名称");
	m_ComboField1.InsertString(2,L"白酒酒精度数");
	m_ComboField1.InsertString(3,L"白酒香型");
	m_ComboField1.InsertString(4,L"白酒品牌");

	m_ComboField2.InsertString(0,L"所有项");
	m_ComboField2.InsertString(1,L"白酒名称");
	m_ComboField2.InsertString(2,L"白酒酒精度数");
	m_ComboField2.InsertString(3,L"白酒香型");
	m_ComboField2.InsertString(4,L"白酒品牌");

	m_ComboField3.InsertString(0,L"所有项");
	m_ComboField3.InsertString(1,L"白酒名称");
	m_ComboField3.InsertString(2,L"白酒酒精度数");
	m_ComboField3.InsertString(3,L"白酒香型");
	m_ComboField3.InsertString(4,L"白酒品牌");

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
	m_ListSpectrumToAdd.InsertColumn( 0, L"", LVCFMT_LEFT,50 );//插入列 
	m_ListSpectrumToAdd.InsertColumn( 1, L"光谱文件名", LVCFMT_LEFT,100 );//插入列 
	m_ListSpectrumToAdd.InsertColumn( 2, L"白酒名称", LVCFMT_LEFT, 100 );//插入列 
	m_ListSpectrumToAdd.InsertColumn( 3, L"白酒信息", LVCFMT_LEFT, 150 );//插入列 
	m_ListSpectrumToAdd.InsertColumn( 4, L"生产日期", LVCFMT_LEFT, 100 );//插入列 
	m_ListSpectrumToAdd.InsertColumn( 5, L"生产批号", LVCFMT_LEFT, 100 );//插入列 
	m_ListSpectrumToAdd.InsertColumn( 6, L"来源说明", LVCFMT_LEFT, 100 );//插入列 
	m_ListSpectrumToAdd.InsertColumn( 7, L"接收日期", LVCFMT_LEFT, 100 );//插入列 

	DWORD dwStyle = m_ListSpectrumToAdd.GetExtendedStyle(); //获取当前扩展样式
	dwStyle |= LVS_EX_FULLROWSELECT; //选中某行使整行高亮（report风格时）
	dwStyle |= LVS_EX_GRIDLINES; //网格线（report风格时）
	m_ListSpectrumToAdd.SetExtendedStyle(dwStyle); //设置扩展风格	

	m_ListSpectrunQueryed.InsertColumn( 0, L"", LVCFMT_LEFT, 50 );//插入列 
	m_ListSpectrunQueryed.InsertColumn( 1, L"光谱编号", LVCFMT_LEFT, 100 );//插入列 
	m_ListSpectrunQueryed.InsertColumn( 2, L"白酒名称", LVCFMT_LEFT, 100 );//插入列 
	m_ListSpectrunQueryed.InsertColumn( 3, L"白酒信息", LVCFMT_LEFT, 150 );//插入列 
	m_ListSpectrunQueryed.InsertColumn( 4, L"生产日期", LVCFMT_LEFT, 100 );//插入列
	m_ListSpectrunQueryed.InsertColumn( 5, L"生产批号", LVCFMT_LEFT, 100 );//插入列
	m_ListSpectrunQueryed.InsertColumn( 6, L"来源说明", LVCFMT_LEFT, 100 );//插入列
	m_ListSpectrunQueryed.InsertColumn( 7, L"入库日期", LVCFMT_LEFT, 100 );//插入列
	m_ListSpectrunQueryed.InsertColumn( 8, L"操作人员", LVCFMT_LEFT, 100 );//插入列


	dwStyle = m_ListSpectrunQueryed.GetExtendedStyle(); //获取当前扩展样式
	dwStyle |= LVS_EX_FULLROWSELECT; //选中某行使整行高亮（report风格时）
	dwStyle |= LVS_EX_GRIDLINES; //网格线（report风格时）
    dwStyle |= LVS_EX_CHECKBOXES; //item前生成checkbox控件

	m_ListSpectrunQueryed.SetExtendedStyle(dwStyle); 

	GetClientRect(&m_rect);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CDBManagePageTrue::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 在此处添加消息处理程序代码
	// 不为绘图消息调用 CPropertyPage::OnPaint()
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
		path+= L"\\Resource\\NOPic.jpg";
		DisplayPic(path,this->GetDlgItem(IDC_STATIC_PICTUREINLIB_TRUE)->GetSafeHwnd(), rect.Width(), rect.Height());
	}
}


void CDBManagePageTrue::OnBnClickedButtonOpendatapackageTrue()
{
	// TODO: 在此添加控件通知处理程序代码
	BROWSEINFO bi;
	char Buffer[MAX_PATH];
	bi.hwndOwner = this->m_hWnd;
	bi.pidlRoot =NULL;//初始化制定的root目录很不容易
	bi.pszDisplayName =(LPWSTR)Buffer;//此参数如为NULL则不能显示对话框
	bi.lpszTitle = L"选择光谱数据集文件夹路径";
	bi.ulFlags =BIF_USENEWUI ; //BIF_EDITBOX;//带编辑框的风格
	bi.lpfn = NULL;
	bi.lParam = 0;
	bi.iImage=IDR_MAINFRAME;
	//初始化入口参数bi结束
	LPITEMIDLIST pIDList = SHBrowseForFolder(&bi);//调用显示选择对话框

	if(pIDList==NULL)
	{	
		return;
	}
	SHGetPathFromIDList(pIDList,(LPWSTR)Buffer);
	//取得文件夹路径到Buffer里
	m_SpecDataFoldPath.Format(L"%s",Buffer);
	// free memory used      
	IMalloc * imalloc = 0;
	if (SUCCEEDED(SHGetMalloc(&imalloc)))
	{
		imalloc->Free (pIDList);
		imalloc->Release();
	}
	//从文件夹中导入光谱数据及相关信息
	CString strDir=m_SpecDataFoldPath;
	strDir.Replace(L"\\",L"\\\\");
	strDir += "\\*.*";

	// 遍历得到所有子文件夹名
	CFileFind finder; 
	BOOL bWorking = finder.FindFile(strDir);
	//清空之前导入的数据集
	OnBnClickedButtonClearspectoaddTrue();

	while (bWorking)  
	{
		bWorking = finder.FindNextFile();
		CString str=finder.GetFileName();
		if(str.Right(4)==L".CSV"||str.Right(4)==L".csv" &&L"." !=str &&L".." != str)//注意该句需要排除“.”“..”
		{
			CString filename;
			CString fileTitle;
			//CString str=finder.GetFileName();
			m_ary_SpecFilePath.Add(finder.GetFilePath());
			//获取文件名(不包含后缀)  
			//采用CString的Left(int count)截取CString中从左往右数的count个字符  
			//fileName.GetLength()-4中的4表示".csv"四个字符  
			m_ary_SpecFileName.Add(finder.GetFileTitle());//将文件名(不包含后缀)添加到数组中  
		}
		if(str.Right(4)==L".jpg" ||str.Right(4)==L".bmp"||str.Right(4)==L".gif"&&L"." !=str &&L".." != str)
		{
			m_ary_PicturesName.Add(finder.GetFileName());
			m_ary_PicturesPath.Add(finder.GetFilePath());
		}
		if(str==L"数据说明.xls"||str==L"数据说明.xlsx"&&L"." !=str &&L".." != str)m_ExcelInfoPath=finder.GetFilePath();
	}
	finder.Close();
	if(m_ary_SpecFileName.GetCount()==0)
	{
		MessageBox(L"该文件夹中没有规定格式的光谱数据，请重新选择！",L"警告",MB_ICONWARNING);
		return;
	}
	if(m_ary_SpecFileName.GetCount()<30)
	{
		MessageBox(L"光谱数量太少",L"警告",MB_ICONWARNING);
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
		if(IDOK==MessageBox(L"数据包中缺少必要的光谱说明文件，是否自行填写？",L"信息缺失",MB_OKCANCEL|MB_ICONQUESTION))
		{
			CInputSpecDatasetInfoDlg InputSpecDatasetInfoDlg;
			if(IDOK==InputSpecDatasetInfoDlg.DoModal())
			{
				m_WineName=InputSpecDatasetInfoDlg.m_WineName;
				m_AlcoholContent=InputSpecDatasetInfoDlg.m_AlcoholContent;
				Wineinfo=m_AlcoholContent+L"度,";
				m_Flavour=InputSpecDatasetInfoDlg.m_Flavour;
				Wineinfo+=m_Flavour;
				Wineinfo+=L",";
				m_Brand=InputSpecDatasetInfoDlg.m_Brand;
				Wineinfo+=m_Brand;
				m_Comfrom=L"管理员";
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
		Wineinfo+=L"度,";
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
		//CString Current=m_ary_SpecFilePath[i];
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
	// TODO: 在此添加控件通知处理程序代码
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
	// TODO: 在此添加控件通知处理程序代码
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
	// TODO: 在此添加控件通知处理程序代码

	if(m_ary_SpecFileName.IsEmpty())
	{
		MessageBox(L"没有要入库的光谱数据,请添加光谱数据",L"光谱入库",MB_ICONWARNING);
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
	CompleteSpecInfoAddToLibDlg.DoModal();

	if(NULL!=m_pAddToLibDlg)
	{
		m_pAddToLibDlg->DestroyWindow();
		m_pAddToLibDlg=NULL;
	}
	m_Comfrom=CompleteSpecInfoAddToLibDlg.m_ComFrom;
	m_SpecIDTitle=CompleteSpecInfoAddToLibDlg.m_SpecIDTitle;
	m_Manager=CompleteSpecInfoAddToLibDlg.m_Manager;

	//保存图片
	CWinePictures WinePictures;
	CTime tm=CTime::GetCurrentTime();
	CString Currenttime;
	Currenttime=tm.Format("%Y-%m-%d");
	//Currenttime.Format(L"%d-%d-%d",tm.GetYear(),tm.GetMonth(),tm.GetDay());

	if(!WinePictures.Open())
	{
		MessageBox(L"打开图片库失败！",L"导入图片",MB_ICONERROR);
		return;
	}

	CString m_strFolderPath=g_CurrentDir;
	m_strFolderPath+=L"\\Lib\\Pictures\\";
	m_strFolderPath+=m_SpecIDTitle;
	//生成保存图片的文件夹
	CreateDirectory(m_strFolderPath, NULL );
	for(int i=0;i<m_ary_PicturesPath.GetCount();i++)
	{	
		//只保存图片的相对路径
		CString newpicpath=L"\\Lib\\Pictures\\";
		newpicpath+=m_SpecIDTitle;
		newpicpath+=m_ary_PicturesName[i];
		CopyFile(m_ary_PicturesPath[i],newpicpath,0);
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
	//开始导入光谱
	m_pAddToLibDlg=new CAddToLibDlg(m_ary_SpecFilePath.GetCount(),1);
	m_pAddToLibDlg->Create(IDD_DIALOG_ADDTOLIBPROGRESS,this);
	m_pAddToLibDlg->ShowWindow(SW_SHOW);
	pThreadAddSpecToLib=AfxBeginThread(ThreadAddSpecToLib,this);


}
UINT ThreadAddSpecToLib(LPVOID lpParam)
{

	CDBManagePageTrue* pDBManageTrueDlg=(CDBManagePageTrue*)lpParam;
	CTrueWineLib TrueWineLib;
	CCSVOperator CSVOperator;
	CTime tm=CTime::GetCurrentTime();
	CString Currenttime;
	Currenttime=tm.Format("%Y-%m-%d");
	//Currenttime.Format(L"%d-%d-%d",tm.GetYear(),tm.GetMonth(),tm.GetDay());

	if(!TrueWineLib.Open())
	{
		pDBManageTrueDlg->MessageBox(L"打开光谱库失败！",L"导入光谱",MB_ICONERROR);
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



		//生成光谱ID
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
	//pDBManageTrueDlg->m_pAddToLibDlg=NULL;
	pDBManageTrueDlg->MessageBox(L"光谱数据导入成功",L"光谱数据入库",MB_ICONINFORMATION);
	return 0;
}



void CDBManagePageTrue::UpdateIndexTable(void)
{
	//CTrueWineLib TrueWineLib;

	CDatabase m_db;
	CRecordset rs;
	m_db.OpenEx(_T("DSN=白酒鉴定与溯源数据库;"),CDatabase::noOdbcDialog);
	rs.m_pDatabase = &m_db;

	CString WineName;
	CString AlcoholContent;
	CString Flavour;
	CString Brand;
	CString ProductionBatchNo;

	//更新白酒类型索引
	CWineTypeIndex WineTypeIndex;

	if(!WineTypeIndex.Open())
	{
		MessageBox(L"打开光谱索引库失败",L"更新失败",MB_ICONERROR);
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
		MessageBox(L"打开真酒库失败",L"更新失败",MB_ICONERROR);
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
			WineTypeIndex.m_WineName=WineName;   //不知道为什么使用了select distinct 得到某几列记录时，自动把这一列分配给最前面的类型相同的列
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
	//更新酒精度索引
	CAlcoholTypeIndex AlcoholTypeIndex;

	if(!AlcoholTypeIndex.Open())
	{
		MessageBox(L"打开光谱索引库失败",L"更新失败",MB_ICONERROR);
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
		MessageBox(L"打开真酒库失败",L"更新失败",MB_ICONERROR);
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

	//更新香型索引
	CFlavourTypeIndex FlavourTypeIndex;

	if(!FlavourTypeIndex.Open())
	{
		MessageBox(L"打开光谱索引库失败",L"更新失败",MB_ICONERROR);
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
		MessageBox(L"打开真酒库失败",L"更新失败",MB_ICONERROR);
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

	//更新品牌索引
	CBrandTypeIndex BrandTypeIndex;

	if(!BrandTypeIndex.Open())
	{
		MessageBox(L"打开光谱索引库失败",L"更新失败",MB_ICONERROR);
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
		MessageBox(L"打开真酒库失败",L"更新失败",MB_ICONERROR);
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
	// TODO: 在此添加控件通知处理程序代码
	UpdateIndexTable();

	MessageBox(L"数据库信息更新成功!",L"信息更新",MB_ICONINFORMATION);
}
void CDBManagePageTrue::OnBnClickedButtonUpdatemodelTrue()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdatePLSModel();
}

void CDBManagePageTrue::UpdatePLSModel(void)
{
	pThreadUpdatePLSModel=AfxBeginThread(ThreadUpdatePLSModel,this);
}
UINT ThreadUpdatePLSModel(LPVOID lpParam)
{
	double coeff=2;//系数

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
		pCDBManageTrueDlg->MessageBox(L"打开PLS模型库失败",L"更新模型失败",MB_ICONERROR);
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

	//打开索引库
	if(!WineTypeIndex.Open())
	{
		pCDBManageTrueDlg->MessageBox(L"打开真酒索引库失败",L"更新模型失败",MB_ICONERROR);
		return -1;
	}
	if(0==WineTypeIndex.GetRecordCount())
	{
		pCDBManageTrueDlg->MessageBox(L"光谱管理信息出现错误，请尝试更新！",L"更新模型失败",MB_ICONERROR);
		return -1;
	}

	if(!AlcoholTypeIndex.Open())
	{
		pCDBManageTrueDlg->MessageBox(L"打开酒精度索引库失败",L"更新模型失败",MB_ICONERROR);
		return -1;
	}
	if(0==AlcoholTypeIndex.GetRecordCount())
	{
		pCDBManageTrueDlg->MessageBox(L"光谱管理信息出现错误，请尝试更新！",L"更新模型失败",MB_ICONERROR);
		return -1;
	}

	if(!FlavourTypeIndex.Open())
	{
		pCDBManageTrueDlg->MessageBox(L"打开香型索引库失败",L"更新模型失败",MB_ICONERROR);
		return -1;
	}
	if(0==FlavourTypeIndex.GetRecordCount())
	{
		pCDBManageTrueDlg->MessageBox(L"光谱管理信息出现错误，请尝试更新！",L"更新模型失败",MB_ICONERROR);
		return -1;
	}

	if(!BrandTypeIndex.Open())
	{
		pCDBManageTrueDlg->MessageBox(L"打开品牌索引库失败",L"更新模型失败",MB_ICONERROR);
		return -1;
	}
	if(0==BrandTypeIndex.GetRecordCount())
	{
		pCDBManageTrueDlg->MessageBox(L"光谱管理信息出现错误，请尝试更新！",L"更新模型失败",MB_ICONERROR);
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
		pCDBManageTrueDlg->MessageBox(L"打开真酒库失败",L"更新模型失败",MB_ICONERROR);
		return -1;
	}
	if(TrueWineLib.IsEOF())
	{
		pCDBManageTrueDlg->MessageBox(L"真酒库中无光谱记录",L"更新模型失败",MB_ICONERROR);
		return -1;
	}
	TrueWineLib.MoveFirst();
	while(!TrueWineLib.IsEOF())
	{
		TrueWineLib.MoveNext();
	}
	TrueWineLib.MoveFirst();

	//酒精度分类模型
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
					pCDBManageTrueDlg->MessageBox(L"查询真酒库失败",L"更新酒精度模型失败",MB_ICONERROR);
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
		//训练酒精度分类模型
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
			StandardPC.SetRow(i,(T.GetRows(begin,begin+MNum_EachLabel(1,i)-1)).Mean(1));
			begin+=MNum_EachLabel(1,i);
		}
		//计算阈值
		CMatrix Threshold(1,AlcoholTypeCount);//计算样本与标准值之间的最大距离,样本与标准值之间的平均距离的coeff倍，取较大的那个

		//CMatrix Threshold2(1,AlcoholTypeCount);//计算
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
		//阈值
		Threshold.Matrix2CString(threshold,L",");


		//保存模型
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
	//开始香型
	AlcoholTypeIndex.MoveFirst();
	while(!AlcoholTypeIndex.IsEOF())
	{


		CString command=_T("SELECT DISTINCT Flavour FROM TrueWineLib");

		CString tempFlavourInAlconhol;
		CDatabase m_db;
		CRecordset rsforFlavour;
		rsforFlavour.m_strFilter.Format(L"AlcoholContent='%s'",AlcoholTypeIndex.m_AlcoholContent);
		m_db.OpenEx(_T("DSN=白酒鉴定与溯源数据库;"),CDatabase::noOdbcDialog);
		rsforFlavour.m_pDatabase = &m_db;	
		if (!rsforFlavour.Open(AFX_DB_USE_DEFAULT_TYPE,command))
		{
			CString str;
			str.Format(L"更新%s度香型分类模型失败！",AlcoholTypeIndex.m_AlcoholContent);
			pCDBManageTrueDlg->MessageBox(str,L"打开数据库失败",MB_ICONERROR);
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
					pCDBManageTrueDlg->MessageBox(L"查询真酒库失败",L"更新香型模型失败",MB_ICONERROR);
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
			//训练香型分类模型
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
				StandardPC.SetRow(i,(T.GetRows(begin,begin+MNum_EachLabel(1,i)-1)).Mean(1));
				begin+=MNum_EachLabel(1,i);
			}
			//计算阈值
			CMatrix Threshold(1,FlavourTypeCount);//计算样本与标准值之间的最大距离,样本与标准值之间的平均距离的coeff倍，取较大的那个
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
			//阈值
			Threshold.Matrix2CString(threshold,L",");


			//保存模型
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
		//开始品牌
		rsforFlavour.MoveFirst();
		while(!rsforFlavour.IsEOF())
		{
			CString tempFlavour;
			rsforFlavour.GetFieldValue(L"Flavour",tempFlavour);


			CString command=_T("SELECT DISTINCT Brand FROM TrueWineLib");
			CDatabase m_db;
			CRecordset rsforBrand;

			rsforBrand.m_strFilter.Format(L"AlcoholContent='%s' AND Flavour='%s'",AlcoholTypeIndex.m_AlcoholContent,tempFlavour);

			m_db.OpenEx(_T("DSN=白酒鉴定与溯源数据库;"),CDatabase::noOdbcDialog);
			rsforBrand.m_pDatabase = &m_db;	
			if (!rsforBrand.Open(AFX_DB_USE_DEFAULT_TYPE,command))
			{
				CString str;
				str.Format(L"更新%s度%s分类模型失败！",AlcoholTypeIndex.m_AlcoholContent,tempFlavour);
				pCDBManageTrueDlg->MessageBox(str,L"打开数据库失败",MB_ICONERROR);
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
						pCDBManageTrueDlg->MessageBox(L"打开真酒库失败",L"更新酒精度模型失败",MB_ICONERROR);
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
					StandardPC.SetRow(i,(T.GetRows(begin,begin+MNum_EachLabel(1,i)-1)).Mean(1));
					begin+=MNum_EachLabel(1,i);
				}
				//计算阈值
				CMatrix Threshold(1,BrandTypeCount);//计算样本与标准值之间的最大距离,样本与标准值之间的平均距离的coeff倍，取较大的那个
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
				//阈值
				Threshold.Matrix2CString(threshold,L",");


				//保存模型
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
			//开始品牌内
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

				m_db.OpenEx(_T("DSN=白酒鉴定与溯源数据库;"),CDatabase::noOdbcDialog);
				rsforInBrand.m_pDatabase = &m_db;	
				if (!rsforInBrand.Open(AFX_DB_USE_DEFAULT_TYPE,command))
				{
					CString str;
					str.Format(L"更新%s度%s%s分类模型失败！",AlcoholTypeIndex.m_AlcoholContent,tempFlavour,tempBrand);
					pCDBManageTrueDlg->MessageBox(str,L"打开数据库失败",MB_ICONERROR);
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
							pCDBManageTrueDlg->MessageBox(L"打开真酒库失败",L"更新品牌内模型失败",MB_ICONERROR);
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
						StandardPC.SetRow(i,(T.GetRows(begin,begin+MNum_EachLabel(1,i)-1)).Mean(1));
						begin+=MNum_EachLabel(1,i);
					}

					CMatrix Threshold(1,InBrandTypeCount);//计算样本与标准值之间的最大距离,样本与标准值之间的平均距离的coeff倍，取较大的那个
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
					//阈值
					Threshold.Matrix2CString(threshold,L",");

					//保存模型
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
	pCDBManageTrueDlg->MessageBox(L"模型更新成功！",L"模型更新",MB_ICONINFORMATION);
	//删除不需要的模型文件
	return 0;
}

void CDBManagePageTrue::OnBnClickedButtonShowallTrue()
{
	// TODO: 在此添加控件通知处理程序代码
	m_TrueWineLib.m_strSort=L"WineName";
	if(m_TrueWineLib.IsOpen())
	{
		m_TrueWineLib.Close();
	}
	if(!m_TrueWineLib.Open())
	{
		MessageBox(L"打开真酒库失败!",L"打开失败",MB_ICONERROR);
		return;
	}
	m_TrueWineLib.m_strSort=L"WineName";
	ShowQuerySpecRecord();
}


void CDBManagePageTrue::OnBnClickedButtonDeleteTrue()
{
	// TODO: 在此添加控件通知处理程序代码
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
	m_ListSpectrunQueryed.SetRedraw(FALSE);

	m_ListSpectrunQueryed.DeleteAllItems();
	if(m_TrueWineLib.IsEOF()||!m_TrueWineLib.IsOpen())
	{
		MessageBox(L"没有相关记录!",L"无记录",MB_ICONWARNING);
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
	m_ListSpectrunQueryed.SetRedraw(TRUE);
//	m_ListSpectrunQueryed.Invalidate();
	m_ListSpectrunQueryed.UpdateWindow();
	m_TrueWineLib.MoveFirst();
}


void CDBManagePageTrue::OnBnClickedButtonClearspectoaddTrue()
{
	// TODO: 在此添加控件通知处理程序代码
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
	// TODO: 在此添加控件通知处理程序代码
	m_TrueWineLib.m_strFilter=L"";
	m_TrueWineLib.m_strSort=L"WineName";
	if(!m_TrueWineLib.IsOpen())
	{
		m_TrueWineLib.Open();
	}
	UpdateData(TRUE);
	if(m_Key1.IsEmpty()&&m_Key2.IsEmpty()&&m_Key3.IsEmpty())
	{
		MessageBox(L"请填写搜索关键词！",L"搜索出错",MB_ICONWARNING);
		return ;
	}

	if(!m_Key1.IsEmpty())
	{
		CString ComboField1;
		CString strFilter1;

		m_ComboField1.GetLBText(m_ComboField1.GetCurSel(),ComboField1);
		if(ComboField1 == L"所有项")
		{
			strFilter1.Format(L" (WineName = '%s' OR AlcoholContent = '%s' OR Flavour = '%s' OR Brand = '%s') ",
				m_Key1,m_Key1,m_Key1,m_Key1);
		}
		else if(ComboField1 == L"白酒名称")
		{
			strFilter1.Format(L" (WineName = '%s') ",m_Key1);
		}
		else if(ComboField1 == L"白酒酒精度数")
		{
			strFilter1.Format(L" (AlcoholContent = '%s') ",m_Key1);
		}
		else if(ComboField1 == L"白酒香型")
		{
			strFilter1.Format(L" (Flavour = '%s') ",m_Key1);
		}
		else if(ComboField1 == L"白酒品牌")
		{
			strFilter1.Format(L" (Brand = '%s') ",m_Key1);
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
		if(ComboField2 == L"所有项")
		{

			strFilter2.Format(L" (WineName = '%s' OR AlcoholContent = '%s' OR Flavour = '%s' OR Brand = '%s') ",
				m_Key2,m_Key2,m_Key2,m_Key2);
		}
		else if(ComboField2 == L"白酒名称")
		{
			strFilter2.Format(L" (WineName = '%s') ",m_Key2);
		}
		else if(ComboField2 == L"白酒酒精度数")
		{
			strFilter2.Format(L" (AlcoholContent = '%s') ",m_Key2);
		}
		else if(ComboField2 == L"白酒香型")
		{
			strFilter2.Format(L" (Flavour = '%s')",m_Key2);
		}
		else if(ComboField2 == L"白酒品牌")
		{
			strFilter2.Format(L" (Brand = '%s') ",m_Key2);
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
		if(ComboField3 == L"所有项")
		{

			strFilter3.Format(L" (WineName = '%s' OR AlcoholContent = '%s' OR Flavour = '%s' OR Brand = '%s') ",
				m_Key3,m_Key3,m_Key3,m_Key3);
		}
		else if(ComboField3 == L"白酒名称")
		{
			strFilter3.Format(L" (WineName = '%s') ",m_Key3);
		}
		else if(ComboField3 == L"白酒酒精度数")
		{
			strFilter3.Format(L" (AlcoholContent = '%s') ",m_Key3);
		}
		else if(ComboField3 == L"白酒香型")
		{
			strFilter3.Format(L" (Flavour = '%s') ",m_Key3);
		}
		else if(ComboField3 == L"白酒品牌")
		{
			strFilter3.Format(L" (Brand = '%s') ",m_Key3);
		}
		m_TrueWineLib.m_strFilter+=strFilter3;
	}
	m_TrueWineLib.Requery();
	ShowQuerySpecRecord();
}


void CDBManagePageTrue::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CPropertyPage::OnShowWindow(bShow, nStatus);

	// TODO: 在此处添加消息处理程序代码
	CRect rect;
	GetParent()->GetClientRect(&rect);
	int nWidth =rect.right-rect.left;
	int nHeight =rect.bottom-rect.top;

	if(bShow)
	{
		GetParent()->ShowWindow(SW_HIDE);
		GetParent()->SetWindowPos(NULL,0,0,nWidth,nHeight,SWP_NOMOVE|SWP_NOZORDER|SWP_NOACTIVATE);
		GetParent()->ShowWindow(SW_SHOW);
	}
}


void CDBManagePageTrue::OnSize(UINT nType, int cx, int cy)
{
	CPropertyPage::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
	CWnd *pWnd[29]; 
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

	pWnd[25] = GetDlgItem(IDC_BUTTON_UPDATELIBINFO_TRUE);
	pWnd[26] = GetDlgItem(IDC_BUTTON_SELECTALL_TRUE);
	pWnd[27] = GetDlgItem(IDC_BUTTON_INVERTSELECT_TRUE);
	pWnd[28] = GetDlgItem(IDC_BUTTON_CLEARSEARCH_TRUE);
	//IDC_BUTTON_CLEARSEARCH
	for(int i=0;i<29;i++)
	{
		//获取控件句柄 
		if(pWnd[i])//判断是否为空，因为对话框创建时会调用此函数，而当时控件还未创建 
		{ 
			CRect rect; //获取控件变化前大小 
			pWnd[i]->GetWindowRect(&rect); 
			ScreenToClient(&rect);//将控件大小转换为在对话框中的区域坐标 
			//cx/m_rect.Width()为对话框在横向的变化比例 
			rect.left=rect.left*cx/m_rect.Width();//////调整控件大小 
			rect.right=rect.right*cx/m_rect.Width(); 
			rect.top=rect.top*cy/m_rect.Height(); 
			rect.bottom=rect.bottom*cy/m_rect.Height(); 
			pWnd[i]->MoveWindow(rect);//设置控件大小 
		}
	}

	GetClientRect(&m_rect);//将变化后的对话框大小设为旧大小 
}


void CDBManagePageTrue::OnBnClickedButtonSelectallTrue()
{
	// TODO: 在此添加控件通知处理程序代码
    int nCount=m_ListSpectrunQueryed.GetItemCount();
    for(int i=0;i<nCount;i++)
    {
        m_ListSpectrunQueryed.SetCheck(i,TRUE);
    }
}


void CDBManagePageTrue::OnBnClickedButtonInvertselectTrue()
{
	// TODO: 在此添加控件通知处理程序代码
	int nCount=m_ListSpectrunQueryed.GetItemCount();
	for(int i=0;i<nCount;i++)
	{
		m_ListSpectrunQueryed.SetCheck(i,m_ListSpectrunQueryed.GetCheck(i)?FALSE:TRUE);
	}

}


void CDBManagePageTrue::OnBnClickedButtonClearsearch()
{
	// TODO: 在此添加控件通知处理程序代码
	if(m_TrueWineLib.IsOpen())
		m_TrueWineLib.Close();
	m_ListSpectrunQueryed.DeleteAllItems();
	m_PicQueryedIndex=0;
	m_ary_PicturesQueryedPath.RemoveAll();
}

void CDBManagePageTrue::OnBnClickedButtonLastlibTrue()
{
	// TODO: 在此添加控件通知处理程序代码
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
	// TODO: 在此添加控件通知处理程序代码
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
	// TODO: 在此添加控件通知处理程序代码
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



