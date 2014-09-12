// 这段 MFC 示例源代码演示如何使用 MFC Microsoft Office Fluent 用户界面 
// (“Fluent UI”)。该示例仅供参考，
// 用以补充《Microsoft 基础类参考》和 
// MFC C++ 库软件随附的相关电子文档。
// 复制、使用或分发 Fluent UI 的许可条款是单独提供的。
// 若要了解有关 Fluent UI 许可计划的详细信息，请访问  
// http://msdn.microsoft.com/officeui。
//
// 版权所有(C) Microsoft Corporation
// 保留所有权利。

// DataServerCenter.cpp : 定义应用程序的类行为。
//

#include "stdafx.h"
#include "afxwinappex.h"
#include "afxdialogex.h"
#include "DataServerCenter.h"
#include "MainFrm.h"
#include "SkinPPWTL.h"
#include "DataServerCenterDoc.h"
#include "DataServerCenterView.h"
#pragma comment(lib,"SkinPPWTL.lib")
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CDataServerCenterApp

BEGIN_MESSAGE_MAP(CDataServerCenterApp, CWinAppEx)
	ON_COMMAND(ID_APP_ABOUT, &CDataServerCenterApp::OnAppAbout)
	// 基于文件的标准文档命令
	ON_COMMAND(ID_FILE_NEW, &CWinAppEx::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, &CWinAppEx::OnFileOpen)
	// 标准打印设置命令
	ON_COMMAND(ID_FILE_PRINT_SETUP, &CWinAppEx::OnFilePrintSetup)
END_MESSAGE_MAP()


// CDataServerCenterApp 构造

CDataServerCenterApp::CDataServerCenterApp()
{
	m_bHiColorIcons = TRUE;

	// 支持重新启动管理器
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_ALL_ASPECTS;
#ifdef _MANAGED
	// 如果应用程序是利用公共语言运行时支持(/clr)构建的，则:
	//     1) 必须有此附加设置，“重新启动管理器”支持才能正常工作。
	//     2) 在您的项目中，您必须按照生成顺序向 System.Windows.Forms 添加引用。
	System::Windows::Forms::Application::SetUnhandledExceptionMode(System::Windows::Forms::UnhandledExceptionMode::ThrowException);
#endif

	// TODO: 将以下应用程序 ID 字符串替换为唯一的 ID 字符串；建议的字符串格式
	//为 CompanyName.ProductName.SubProduct.VersionInformation
	SetAppID(_T("DataServerCenter.AppID.NoVersion"));

	// TODO: 在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}

// 唯一的一个 CDataServerCenterApp 对象

CDataServerCenterApp theApp;


// CDataServerCenterApp 初始化

BOOL CDataServerCenterApp::InitInstance()
{
	skinppLoadSkin(skin_path); //加载皮肤
	// 如果一个运行在 Windows XP 上的应用程序清单指定要
	// 使用 ComCtl32.dll 版本 6 或更高版本来启用可视化方式，
	//则需要 InitCommonControlsEx()。否则，将无法创建窗口。
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// 将它设置为包括所有要在应用程序中使用的
	// 公共控件类。
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinAppEx::InitInstance();


	// 初始化 OLE 库
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}

	AfxEnableControlContainer();

	EnableTaskbarInteraction(FALSE);

	// 使用 RichEdit 控件需要  AfxInitRichEdit2()	
	// AfxInitRichEdit2();

	// 标准初始化
	// 如果未使用这些功能并希望减小
	// 最终可执行文件的大小，则应移除下列
	// 不需要的特定初始化例程
	// 更改用于存储设置的注册表项
	// TODO: 应适当修改该字符串，
	// 例如修改为公司或组织名
	SetRegistryKey(_T("应用程序向导生成的本地应用程序"));
	//LoadStdProfileSettings(4);  // 加载标准 INI 文件选项(包括 MRU)


	InitContextMenuManager();

	InitKeyboardManager();

	InitTooltipManager();
	CMFCToolTipInfo ttParams;
	ttParams.m_bVislManagerTheme = TRUE;
	theApp.GetTooltipManager()->SetTooltipParams(AFX_TOOLTIP_TYPE_ALL,
		RUNTIME_CLASS(CMFCToolTipCtrl), &ttParams);

	// 注册应用程序的文档模板。文档模板
	// 将用作文档、框架窗口和视图之间的连接
	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CDataServerCenterDoc),
		RUNTIME_CLASS(CMainFrame),       // 主 SDI 框架窗口
		RUNTIME_CLASS(CDataServerCenterView));
	if (!pDocTemplate)
		return FALSE;
	AddDocTemplate(pDocTemplate);


	// 分析标准 shell 命令、DDE、打开文件操作的命令行
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);



	// 调度在命令行中指定的命令。如果
	// 用 /RegServer、/Register、/Unregserver 或 /Unregister 启动应用程序，则返回 FALSE。
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

	// 唯一的一个窗口已初始化，因此显示它并对其进行更新
	int xSize=::GetSystemMetrics(SM_CXSCREEN);
	int ySize=::GetSystemMetrics(SM_CYSCREEN);
	int cx=xSize*9/10;
	int cy=ySize*9/10;
	int x=(xSize-cx)/2;
	int y=(ySize-cy)/2;
	//m_pMainWnd->MoveWindow(x,y,cx,cy);
	m_pMainWnd->SetWindowPos(m_pMainWnd,x,y,cx,cy,SWP_SHOWWINDOW);
	m_pMainWnd->ShowWindow(SW_SHOW);
	m_pMainWnd->SetWindowText(L"白酒鉴定与溯源平台-服务器");
	m_pMainWnd->UpdateWindow();
	// 仅当具有后缀时才调用 DragAcceptFiles
	//  在 SDI 应用程序中，这应在 ProcessShellCommand 之后发生
	return TRUE;
}

int CDataServerCenterApp::ExitInstance()
{
	//TODO: 处理可能已添加的附加资源
	AfxOleTerm(FALSE);



	return CWinAppEx::ExitInstance();
}

// CDataServerCenterApp 消息处理程序


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

protected:
//	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
public:
//	CRect rc;
//	virtual BOOL OnInitDialog();
//	afx_msg void OnSize(UINT nType, int cx, int cy);
//	virtual BOOL OnInitDialog();
//	afx_msg void OnPaint();
//	void OnDisplay(void);
//	afx_msg void OnPaint();
protected:
//	afx_msg LRESULT OnUpdatelist(WPARAM wParam, LPARAM lParam);
public:
//	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
//	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
//	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
//	afx_msg void OnChangetotrans();
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

//void CAboutDlg::DoDataExchange(CDataExchange* pDX)
//{
//	CDialogEx::DoDataExchange(pDX);
//}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_SHOWWINDOW()
	ON_WM_CREATE()
END_MESSAGE_MAP()

// 用于运行对话框的应用程序命令
void CDataServerCenterApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

// CDataServerCenterApp 自定义加载/保存方法

void CDataServerCenterApp::PreLoadState()
{
	BOOL bNameValid;
	CString strName;
	bNameValid = strName.LoadString(IDS_EDIT_MENU);
	ASSERT(bNameValid);
	GetContextMenuManager()->AddMenu(strName, IDR_POPUP_EDIT);
}

void CDataServerCenterApp::LoadCustomState()
{
}

void CDataServerCenterApp::SaveCustomState()
{
}

// CDataServerCenterApp 消息处理程序



BOOL DisplayPic(CString strFileName, HWND hWnd, int nScrWidth, int nScrHeight)
{
	HDC hDC=::GetDC(hWnd);

	IPicture *pPicture;
	IStream *pStream;

	HRESULT hResult;

	HANDLE hFile = NULL;
	DWORD dwFileSize,dwByteRead;

	hFile = CreateFile(strFileName, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	if (hFile == INVALID_HANDLE_VALUE)
	{
		return E_FAIL;
	}

	dwFileSize = GetFileSize(hFile, NULL);

	HGLOBAL hGlobal = ::GlobalAlloc(GHND, dwFileSize);
	if(hGlobal == NULL)
	{
		AfxMessageBox(L"图形文件装载出错！");
		return E_FAIL;
	}

	LPVOID lpData = NULL;

	lpData = ::GlobalLock(hGlobal);

	ReadFile(hFile, lpData, dwFileSize, &dwByteRead, NULL);
	::GlobalUnlock(hGlobal);

	if(CreateStreamOnHGlobal(hGlobal, TRUE, &pStream) != S_OK)
	{
		AfxMessageBox(L"流初始化失败！");
		return E_FAIL;
	}

	hResult = OleLoadPicture(pStream, dwFileSize, TRUE, IID_IPicture, (LPVOID*) &pPicture);

	if(FAILED(hResult))
	{
		AfxMessageBox(L"图形文件装载出错！");
		return E_FAIL;
	}

	OLE_XSIZE_HIMETRIC hmWidth;//图片的真实宽度
	OLE_YSIZE_HIMETRIC hmHeight;//图片的真实高度
	pPicture->get_Width(&hmWidth);
	pPicture->get_Height(&hmHeight);

	//将图形输出到屏幕上
	hResult = pPicture->Render(hDC, 0, 0, nScrWidth, nScrHeight, 0, hmHeight, hmWidth, -hmHeight, NULL);

	CloseHandle(hFile);//关闭打开的文件

	pPicture->Release();
	// Free memory.
	::GlobalFree(hGlobal);

	if (SUCCEEDED(hResult))
	{
		return S_OK;
	}
	else
	{
		AfxMessageBox(L"图形文件装载出错！");
		return E_FAIL;
	}
}
BOOL GetDirTime(CString DirName, SYSTEMTIME &stime)
{

	// 打开文件夹
	HANDLE hDir = CreateFile (DirName, GENERIC_READ,FILE_SHARE_READ|FILE_SHARE_DELETE, 
		  NULL, OPEN_EXISTING,FILE_FLAG_BACKUP_SEMANTICS, NULL); 
	FILETIME lpCreationTime; // 文件夹的创建时间
	FILETIME lpLastAccessTime; // 对文件夹的最近访问时间 
	FILETIME lpLastWriteTime; // 文件夹的最近修改时间 
	// 获取文件夹时间属性信息
	BOOL retval=GetFileTime(hDir, &lpCreationTime, &lpLastAccessTime, &lpLastWriteTime);
	if (retval)
	{ 
		FILETIME ftime; 
		FileTimeToLocalFileTime(&lpLastWriteTime, &ftime); // 转换成本地时间 
		FileTimeToSystemTime(&ftime, &stime); // 转换成系统时间格式
	} 
	CloseHandle(hDir); // 关闭打开过的文件夹
	return retval;
}


//对
BOOL SortCArray(CArray<CString,CString> &A)
{
	CArray<CString,CString> B;
	CArray<CString,CString> C;
	CArray<int,int> D;

	for(int i=0;i<A.GetCount();i++)
	{
		CString temp=A[i];
		int n=_ttoi(temp);
		int m=_ttoi(A[i]);

		if(A[i].GetAt(0)<'0'||A[i].GetAt(0)>'9')
		{
			B.Add(A[i]);
		}
		else
		{
			D.Add(_ttoi(A[i]));
			C.Add(A[i]);
		}
	}
	for(int i=0;i<D.GetCount();i++)
	{
		int minIndex=i;
		for(int j=i+1;j<D.GetCount();j++)
		{
			if(D[minIndex]>D[j])
			{
				minIndex=j;
			}
		}
		A.SetAt(i,C[minIndex]);
		

		if(minIndex!=i)
		{
			int tempD=D[i];
			D.SetAt(i,D[minIndex]);
			D.SetAt(minIndex,tempD);

			CString tempC=C[i];
			C.SetAt(i,C[minIndex]);
			C.SetAt(minIndex,tempC);
		}

	}
	for(int i=D.GetCount();i<A.GetCount();i++)
	{
		A[i]=B[i-D.GetCount()];
	}
	return 1;
}

BOOL SortCArrayByFirst(CArray<CString,CString> &A,CArray<CString,CString> &B)
{
	CArray<CString,CString> A1;
	CArray<CString,CString> A2;
	CArray<CString,CString> B1;
	CArray<CString,CString> B2;
	CArray<int,int> D;

	for(int i=0;i<A.GetCount();i++)
	{
		CString temp=A[i];
		int n=_ttoi(temp);
		int m=_ttoi(A[i]);

		if(A[i].GetAt(0)<'0'||A[i].GetAt(0)>'9')
		{
			A2.Add(A[i]);
			B2.Add(B[i]);
		}
		else
		{
			D.Add(_ttoi(A[i]));
			A1.Add(A[i]);
			B1.Add(B[i]);
		}
	}
	for(int i=0;i<D.GetCount();i++)
	{
		int minIndex=i;
		for(int j=i+1;j<D.GetCount();j++)
		{
			if(D[minIndex]>D[j])
			{
				minIndex=j;
			}
		}
		A[i]=A1[minIndex];
		B[i]=B1[minIndex];

		if(minIndex!=i)
		{
			int tempD=D[i];
			D.SetAt(i,D[minIndex]);
			D.SetAt(minIndex,tempD);

			CString tempA1=A1[i];
			A1.SetAt(i,A1[minIndex]);
			A1.SetAt(minIndex,tempA1);

			CString tempB1=B1[i];
			B1.SetAt(i,B1[minIndex]);
			B1.SetAt(minIndex,tempB1);
		}

	}
	for(int i=D.GetCount();i<A.GetCount();i++)
	{
		A[i]=A2[i-D.GetCount()];
		B[i]=B2[i-D.GetCount()];

	}
	return 1;
}


int CString2Matrix(CMatrix &A,const CString &cstr,const CString &Sep)
{
	int DataStrlenth=cstr.GetLength();
	wchar_t *str1=(wchar_t *)malloc(sizeof(wchar_t)*(DataStrlenth*2));
	wcscpy(str1,CT2CW(cstr));
	wchar_t *token; 
	token=wcstok(str1,Sep);
	int row=1;
	int column=0;
	while(token!=NULL&&row<=A.m_row)
	{
		double a=_wtof(token);
		if(column<A.m_column)
		{
			column++;
			A.SetItem(row,column,a);
			
		}
		else
		{
			column=1;
			row++;
			A.SetItem(row,column,a);	
		}
		token=wcstok(NULL,Sep);
	}
	free(str1);
	str1=NULL;
	if(column<A.m_column||row<A.m_row)
	{
		return -1;
	}
	if(token!=NULL)
	{
		return 0;
	}
	return 1;	
}

double PredictProject(const CString &projectpath,const CString &specName)
{
	double result;
	CString SpecPath=projectpath;
	SpecPath+=L"\\";
	SpecPath+=specName;
	SpecPath+=L".CSV";

	CWineTypeIndex WineTypeIndex;
	CAlcoholTypeIndex AlcoholTypeIndex;
	CFlavourTypeIndex FlavourTypeIndex;
	CBrandTypeIndex BrandTypeIndex;

	CPLSModel PLSModel;

	CString Alcohol;
	CString Flavour;
	CString Brand;

	map<int,CString> map_WineName;
	map<int,CString> map_AlcoholContent;
	map<int,CString> map_Flavour;
	map<int,CString> map_Brand;
	//打开索引库
	if(!WineTypeIndex.Open())
	{
		return -1;
	}
	if(!AlcoholTypeIndex.Open())
	{
		return -1;
	}
	if(!FlavourTypeIndex.Open())
	{
		return -1;
	}
	if(!BrandTypeIndex.Open())
	{
		return -1;
	}
	//
	PLSModel.Open();

	WineTypeIndex.MoveFirst();
	while(!WineTypeIndex.IsEOF())
	{
		map_WineName[WineTypeIndex.m_TypeIndex]= WineTypeIndex.m_WineName;
		WineTypeIndex.MoveNext();
	}
	AlcoholTypeIndex.MoveFirst();
	while(!AlcoholTypeIndex.IsEOF())
	{
		map_AlcoholContent[AlcoholTypeIndex.m_AlcoholIndex]=AlcoholTypeIndex.m_AlcoholContent;
		AlcoholTypeIndex.MoveNext();
	}
	AlcoholTypeIndex.MoveFirst();

	FlavourTypeIndex.MoveFirst();
	while(!FlavourTypeIndex.IsEOF())
	{
		map_Flavour[FlavourTypeIndex.m_FlavourIndex]=FlavourTypeIndex.m_Flavour;
		FlavourTypeIndex.MoveNext();
	}
	FlavourTypeIndex.MoveFirst();

	BrandTypeIndex.MoveFirst();
	while(!BrandTypeIndex.IsEOF())
	{
		map_Brand[BrandTypeIndex.m_BrandIndex]= BrandTypeIndex.m_Brand;
		BrandTypeIndex.MoveNext();
	}
	BrandTypeIndex.MoveFirst();

	CMatrix m_SpecData(1,869);


	CCSVOperator CSVOperator;
	CSVOperator.LoadCSV(SpecPath);

	for(int j=1;j<=869;j++)
	{
		double tempDouble;
		CSVOperator.GetDouble(j,2,tempDouble);
		m_SpecData.SetItem(1,j,tempDouble);
	}

	int AlcoholFlag;
	int FlavourFlag;
	int BrandFlag;
	int WineNameFlag;

	bool FlavourNeed;
	bool BrandNeed;
	//	bool InBrandNeed;
	//酒精度
	PLSModel.m_strFilter=L"AlcoholContent is null";
	PLSModel.Requery();
	//获取模型数据
	CMatrix WeightAlcohol(869,PLSModel.m_FactorNum);
	CMatrix LabelsAlcohol(1,PLSModel.m_ClassCount);
	CMatrix Num_EachLabelAlcohol(1,PLSModel.m_ClassCount);
	CMatrix StandardPCAlcohol(PLSModel.m_ClassCount,PLSModel.m_FactorNum);
	CMatrix TrainsetMean(1,869);

	int flag=CString2Matrix(WeightAlcohol,PLSModel.m_Weight,L",");

	flag=CString2Matrix(LabelsAlcohol,PLSModel.m_Labels,L",");

	flag=CString2Matrix(Num_EachLabelAlcohol,PLSModel.m_Num_EachLabel,L",");

	flag=CString2Matrix(StandardPCAlcohol,PLSModel.m_StandardPC,L",");

	flag=CString2Matrix(TrainsetMean,PLSModel.m_TrainsetMean,L",");

	int totalnum=Num_EachLabelAlcohol.Sum();

	CMatrix TrainsetPCAlcohol(totalnum,PLSModel.m_FactorNum);

	flag=CString2Matrix(TrainsetPCAlcohol,PLSModel.m_TrainsetPC,L",");

	CMatrix Dist(1,PLSModel.m_ClassCount);

	CMatrix testSamplePCAlcohol=(m_SpecData-TrainsetMean)*WeightAlcohol;

	int begin=1;
	for(int i=1;i<=PLSModel.m_ClassCount;i++)
	{
		CMatrix tempPC=TrainsetPCAlcohol.GetRows(begin,begin+Num_EachLabelAlcohol(1,i)-1);
		begin=begin+Num_EachLabelAlcohol(1,i);
		CMatrix covPC=tempPC.covariance();		
		CMatrix t_M=testSamplePCAlcohol-StandardPCAlcohol.GetRow(i);
		covPC.Inv();
		Dist.SetItem(1,i,(t_M*covPC*t_M.Transposition())(1,1));
	}
	CMatrixIndex  Index;
	Dist.FindMin(Index);


	AlcoholFlag=Index.column;

	Alcohol=map_AlcoholContent[LabelsAlcohol(1,AlcoholFlag)];


	WineTypeIndex.m_strFilter.Format(L"AlcoholContent='%s'",Alcohol);
	WineTypeIndex.Requery();
	if(!WineTypeIndex.IsEOF())
	{
		WineTypeIndex.MoveFirst();
		while(!WineTypeIndex.IsEOF())
		{
			WineTypeIndex.MoveNext();
		}
	}

	if(1==WineTypeIndex.GetRecordCount())
	{
		result=WineTypeIndex.m_TypeIndex;
		return result;
	}
	//是否需要香型分类
	PLSModel.m_strFilter.Format(L"AlcoholContent='%s' AND Flavour is null",Alcohol);
	PLSModel.Requery();
	FlavourNeed=0;
	if(0!=PLSModel.GetRecordCount())
	{
		//需要进行香型分类
		FlavourNeed=1;
		CMatrix WeightFlavour(869,PLSModel.m_FactorNum);
		CMatrix LabelsFlavour(1,PLSModel.m_ClassCount);
		CMatrix Num_EachLabelFlavour(1,PLSModel.m_ClassCount);
		CMatrix StandardPCFlavour(PLSModel.m_ClassCount,PLSModel.m_FactorNum);
		TrainsetMean(1,869);

		flag=CString2Matrix(WeightFlavour,PLSModel.m_Weight,L",");

		flag=CString2Matrix(LabelsFlavour,PLSModel.m_Labels,L",");

		flag=CString2Matrix(Num_EachLabelFlavour,PLSModel.m_Num_EachLabel,L",");

		flag=CString2Matrix(StandardPCFlavour,PLSModel.m_StandardPC,L",");

		flag=CString2Matrix(TrainsetMean,PLSModel.m_TrainsetMean,L",");

		totalnum=Num_EachLabelFlavour.Sum();

		CMatrix TrainsetPCFlavour(totalnum,PLSModel.m_FactorNum);
		TrainsetPCFlavour(totalnum,PLSModel.m_FactorNum);

		flag=CString2Matrix(TrainsetPCFlavour,PLSModel.m_TrainsetPC,L",");

		Dist(1,PLSModel.m_ClassCount);

		CMatrix testSamplePCFlavour=(m_SpecData-TrainsetMean)*WeightFlavour;

		int begin=1;
		for(int i=1;i<=PLSModel.m_ClassCount;i++)
		{
			CMatrix tempPC=TrainsetPCFlavour.GetRows(begin,begin+Num_EachLabelFlavour(1,i)-1);
			begin=begin+Num_EachLabelFlavour(1,i);
			CMatrix covPC=tempPC.covariance();		
			CMatrix t_M=testSamplePCFlavour-StandardPCFlavour.GetRow(i);
			covPC.Inv();
			Dist.SetItem(1,i,(t_M*covPC*t_M.Transposition())(1,1));
		}
		CMatrixIndex  Index;
		Dist.FindMin(Index);

		FlavourFlag=Index.column;
		Flavour=map_Flavour[LabelsFlavour(1,FlavourFlag)];

		WineTypeIndex.m_strFilter.Format(L"AlcoholContent='%s' AND Flavour='%s'",Alcohol,Flavour);
		WineTypeIndex.Requery();
		if(!WineTypeIndex.IsEOF())
		{
			WineTypeIndex.MoveFirst();
			while(!WineTypeIndex.IsEOF())
			{
				WineTypeIndex.MoveNext();
			}
		}

		if(1==WineTypeIndex.GetRecordCount())
		{
			result=WineTypeIndex.m_TypeIndex;
			WineTypeIndex.Close();
			return result;
		}
	}
	//是否需要进行品牌分类
	PLSModel.m_strFilter.Format(L"AlcoholContent='%s' AND Brand is null ",Alcohol);

	if(FlavourNeed)
	{
		CString temp;
		temp.Format(L"AND Flavour ='%s' ",Flavour);
		PLSModel.m_strFilter+=temp;
	}
	PLSModel.Requery();
	BrandNeed=0;
	if(0!=PLSModel.GetRecordCount())
	{
		//需要进行品牌分类
		BrandNeed=1;
		CMatrix WeightBrand(869,PLSModel.m_FactorNum);
		CMatrix LabelsBrand(1,PLSModel.m_ClassCount);
		CMatrix Num_EachLabelBrand(1,PLSModel.m_ClassCount);
		CMatrix StandardPCBrand(PLSModel.m_ClassCount,PLSModel.m_FactorNum);
		TrainsetMean(1,869);

		flag=CString2Matrix(WeightBrand,PLSModel.m_Weight,L",");

		flag=CString2Matrix(LabelsBrand,PLSModel.m_Labels,L",");

		flag=CString2Matrix(Num_EachLabelBrand,PLSModel.m_Num_EachLabel,L",");

		flag=CString2Matrix(StandardPCBrand,PLSModel.m_StandardPC,L",");

		flag=CString2Matrix(TrainsetMean,PLSModel.m_TrainsetMean,L",");

		totalnum=Num_EachLabelBrand.Sum();

		CMatrix TrainsetPCBrand(totalnum,PLSModel.m_FactorNum);

		flag=CString2Matrix(TrainsetPCBrand,PLSModel.m_TrainsetPC,L",");

		Dist(1,PLSModel.m_ClassCount);

		CMatrix testSamplePCBrand=(m_SpecData-TrainsetMean)*WeightBrand;

		int begin=1;
		for(int i=1;i<=PLSModel.m_ClassCount;i++)
		{
			CMatrix tempPC=TrainsetPCBrand.GetRows(begin,begin+Num_EachLabelBrand(1,i)-1);
			begin=begin+Num_EachLabelBrand(1,i);
			CMatrix covPC=tempPC.covariance();		
			CMatrix t_M=testSamplePCBrand-StandardPCBrand.GetRow(i);
			covPC.Inv();
			Dist.SetItem(1,i,(t_M*covPC*t_M.Transposition())(1,1));
		}
		CMatrixIndex  Index;
		Dist.FindMin(Index);

		BrandFlag=Index.column;

		Brand=map_Brand[LabelsBrand(1,BrandFlag)];
		WineTypeIndex.m_strFilter.Format(L"AlcoholContent='%s' AND Brand ='%s' ",Alcohol,Brand);

		if(FlavourNeed)
		{
			CString temp;
			temp.Format(L"AND Flavour ='%s' ",Flavour);
			WineTypeIndex.m_strFilter+=temp;
		}

		WineTypeIndex.Requery();
		if(!WineTypeIndex.IsEOF())
		{
			WineTypeIndex.MoveFirst();
			while(!WineTypeIndex.IsEOF())
			{
				WineTypeIndex.MoveNext();
			}
		}
		if(1==WineTypeIndex.GetRecordCount())
		{
			result=WineTypeIndex.m_TypeIndex;
			WineTypeIndex.Close();
			return result;
		}
	}
	//进行品牌内分类
	PLSModel.m_strFilter.Format(L"AlcoholContent='%s' ",Alcohol);

	if(FlavourNeed)
	{
		CString temp;
		temp.Format(L"AND Flavour ='%s' ",Flavour);
		PLSModel.m_strFilter+=temp;
	}
	if(BrandNeed)
	{
		CString temp;
		temp.Format(L"AND Brand ='%s' ",Brand);
		PLSModel.m_strFilter+=temp;
	}

	PLSModel.Requery();   
	CMatrix WeightInBrand(869,PLSModel.m_FactorNum);
	CMatrix LabelsInBrand(1,PLSModel.m_ClassCount);
	CMatrix Num_EachLabelInBrand(1,PLSModel.m_ClassCount);
	CMatrix StandardPCInBrand(PLSModel.m_ClassCount,PLSModel.m_FactorNum);
	TrainsetMean(1,869);

	flag=CString2Matrix(WeightInBrand,PLSModel.m_Weight,L",");

	flag=CString2Matrix(LabelsInBrand,PLSModel.m_Labels,L",");

	flag=CString2Matrix(Num_EachLabelInBrand,PLSModel.m_Num_EachLabel,L",");

	flag=CString2Matrix(StandardPCInBrand,PLSModel.m_StandardPC,L",");

	flag=CString2Matrix(TrainsetMean,PLSModel.m_TrainsetMean,L",");

	totalnum=Num_EachLabelInBrand.Sum();

	CMatrix TrainsetPCInBrand(totalnum,PLSModel.m_FactorNum);

	flag=CString2Matrix(TrainsetPCInBrand,PLSModel.m_TrainsetPC,L",");

	Dist(1,PLSModel.m_ClassCount);

	CMatrix testSamplePCInBrand=(m_SpecData-TrainsetMean)*WeightInBrand;
	begin=1;
	for(int i=1;i<=PLSModel.m_ClassCount;i++)
	{
		CMatrix tempPC=TrainsetPCInBrand.GetRows(begin,begin+Num_EachLabelInBrand(1,i)-1);
		begin=begin+Num_EachLabelInBrand(1,i);
		CMatrix covPC=tempPC.covariance();	
		CMatrix t_M=testSamplePCInBrand-StandardPCInBrand.GetRow(i);
		covPC.Inv();
		Dist.SetItem(1,i,(t_M*covPC*t_M.Transposition())(1,1));
	}
	Dist.FindMin(Index);

	WineNameFlag=Index.column;

	WineTypeIndex.m_strFilter.Format(L"AlcoholContent='%s' AND Flavour ='%s' AND Brand ='%s' AND WineName='%s'",Alcohol,Flavour,Brand,map_WineName[LabelsInBrand(1,WineNameFlag)]);
	WineTypeIndex.Requery();
	result=WineTypeIndex.m_TypeIndex;
	WineTypeIndex.Close();
	return result;
}

CString Char2CString(const char *buf)
{
	//计算char *数组大小，以字节为单位，一个汉字占两个字节
	int bufLen = strlen(buf);
	//计算多字节字符的大小，按字符计算。
	int len = MultiByteToWideChar(CP_ACP,0,buf,bufLen,NULL,0);
	TCHAR *buftemp = new TCHAR[len + 1];
	MultiByteToWideChar(CP_ACP,0,buf,bufLen,buftemp,len);
	buftemp[len] = '\0';  //添加字符串结尾，注意不是len+1
	//将TCHAR数组转换为CString
	CString pWideChar;
	pWideChar.Append(buftemp);
	//删除缓冲区
	delete []buftemp;
	return pWideChar;
}

char * CString2Char(const CString cString) 
{
	//获取宽字节字符的大小，大小是按字节计算的
	int len = WideCharToMultiByte(CP_ACP,0,cString,cString.GetLength(),NULL,0,NULL,NULL);
	//为多字节字符数组申请空间，数组大小为按字节计算的宽字节字节大小
	char * pchar = new char[len+2];   //以字节为单位
	//char * pchar = (char*) malloc(sizeof(char)*(len+1));   //以字节为单位

	//宽字节编码转换成多字节编码
	WideCharToMultiByte(CP_ACP,0,cString,cString.GetLength(),pchar,len,NULL,NULL);
	WideCharToMultiByte(CP_ACP,0,cString,cString.GetLength() + 1 ,pchar,len + 1 ,NULL,NULL);
	pchar[len+1] = '/0';   //多字节字符以'/0'结束

	return pchar;  //此函数申请了内存空间，需要在函数外delete
}
CString GetDate()
{
	SYSTEMTIME sys; 
	GetLocalTime(&sys); 
	CString sTime;
	sTime.Format(L"%d-%d-%d",sys.wYear,sys.wMonth,sys.wDay);
	return sTime;
}


BOOL MoveFolder(LPCTSTR lpszFromPath,LPCTSTR lpszToPath)
{
	//文件夹旧位置
	int iPathLen = _tcslen(lpszFromPath);
	TCHAR tczFromFolder[MAX_PATH+1];
	ZeroMemory(tczFromFolder, (MAX_PATH+1)*sizeof(TCHAR));
	_tcscpy(tczFromFolder, lpszFromPath);
	tczFromFolder[iPathLen] = _T('\0');
	tczFromFolder[iPathLen+1] = _T('\0');

	//文件夹新位置
	iPathLen = _tcslen(lpszToPath);
	TCHAR tczToFolder[MAX_PATH+1];
	ZeroMemory(tczToFolder, (MAX_PATH+1)*sizeof(TCHAR));
	_tcscpy(tczToFolder, lpszToPath);
	tczToFolder[iPathLen] = _T('\0');
	tczToFolder[iPathLen+1] = _T('\0');

	SHFILEOPSTRUCT FileOp;
	ZeroMemory((void*)&FileOp,sizeof(SHFILEOPSTRUCT));
	FileOp.fFlags = FOF_NOCONFIRMATION ;
	FileOp.hNameMappings = NULL;
	FileOp.hwnd = NULL;
	FileOp.lpszProgressTitle = NULL;
	FileOp.pFrom = tczFromFolder;
	FileOp.pTo = tczToFolder;
	FileOp.wFunc = FO_MOVE;
	return SHFileOperation(&FileOp) == 0;
}

BOOL DeleteFolder(LPCTSTR lpszPath)
{
	int iPathLen = _tcslen(lpszPath);

	TCHAR tczFolder[MAX_PATH+1];
	ZeroMemory(tczFolder, (MAX_PATH+1)*sizeof(TCHAR));
	_tcscpy(tczFolder, lpszPath);
	tczFolder[iPathLen] = _T('\0');
	tczFolder[iPathLen+1] = _T('\0');

	SHFILEOPSTRUCT FileOp;
	ZeroMemory((void*)&FileOp,sizeof(SHFILEOPSTRUCT));
	FileOp.fFlags = FOF_NOCONFIRMATION;
	FileOp.hNameMappings = NULL;
	FileOp.hwnd = NULL;
	FileOp.lpszProgressTitle = NULL;
	FileOp.pFrom = tczFolder;
	FileOp.pTo = NULL;
	FileOp.wFunc = FO_DELETE;
	return SHFileOperation(&FileOp) == 0;
}


BOOL CopyFolder(LPCTSTR lpszFromPath,LPCTSTR lpszToPath)
{
	SHFILEOPSTRUCT FileOp;
	ZeroMemory((void*)&FileOp,sizeof(SHFILEOPSTRUCT));
	FileOp.fFlags = FOF_NOCONFIRMATION ;
	FileOp.hNameMappings = NULL;
	FileOp.hwnd = NULL;
	FileOp.lpszProgressTitle = NULL;
	FileOp.pFrom = lpszFromPath;
	FileOp.pTo = lpszToPath;
	FileOp.wFunc = FO_COPY;
	return SHFileOperation(&FileOp) == 0;
}

void ClipChildren(HWND hwnd, HRGN parent)
{
	HWND child = ::GetWindow(hwnd, GW_CHILD);
	while(child)
	{
		RECT rect;
		GetWindowRect(child, &rect);
		ScreenToClient(hwnd, (LPPOINT)&rect.left);
		ScreenToClient(hwnd, (LPPOINT)&rect.right);
		HRGN rgn = CreateRectRgnIndirect(&rect);
		CombineRgn(parent, parent, rgn, RGN_DIFF);
		DeleteObject(rgn);
		child = GetWindow(child, GW_HWNDNEXT);
	}
}

