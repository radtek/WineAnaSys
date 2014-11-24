// ��� MFC ʾ��Դ������ʾ���ʹ�� MFC Microsoft Office Fluent �û����� 
// (��Fluent UI��)����ʾ�������ο���
// ���Բ��䡶Microsoft ������ο����� 
// MFC C++ ������渽����ص����ĵ���
// ���ơ�ʹ�û�ַ� Fluent UI ����������ǵ����ṩ�ġ�
// ��Ҫ�˽��й� Fluent UI ��ɼƻ�����ϸ��Ϣ�������  
// http://msdn.microsoft.com/officeui��
//
// ��Ȩ����(C) Microsoft Corporation
// ��������Ȩ����

// DataServerCenter.cpp : ����Ӧ�ó��������Ϊ��
//

#include "stdafx.h"
#include "afxwinappex.h"
#include "afxdialogex.h"
#include "DataServerCenter.h"
#include "MainFrm.h"
//#include "SkinPPWTL.h"
#include "DataServerCenterDoc.h"
#include "DataServerCenterView.h"
#include "ComponentPredictModel.h"
#include "ConcentrationTraceBackModel.h"
//#pragma comment(lib,"SkinPPWTL.lib")
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CDataServerCenterApp

BEGIN_MESSAGE_MAP(CDataServerCenterApp, CWinAppEx)
	ON_COMMAND(ID_APP_ABOUT, &CDataServerCenterApp::OnAppAbout)
	// �����ļ��ı�׼�ĵ�����
	ON_COMMAND(ID_FILE_NEW, &CWinAppEx::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, &CWinAppEx::OnFileOpen)
	// ��׼��ӡ��������
	ON_COMMAND(ID_FILE_PRINT_SETUP, &CWinAppEx::OnFilePrintSetup)
END_MESSAGE_MAP()


// CDataServerCenterApp ����

CDataServerCenterApp::CDataServerCenterApp()
{
	m_bHiColorIcons = TRUE;

	// ֧����������������
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_ALL_ASPECTS;
#ifdef _MANAGED
	// ���Ӧ�ó��������ù�����������ʱ֧��(/clr)�����ģ���:
	//     1) �����д˸������ã�������������������֧�ֲ�������������
	//     2) ��������Ŀ�У������밴������˳���� System.Windows.Forms ������á�
	System::Windows::Forms::Application::SetUnhandledExceptionMode(System::Windows::Forms::UnhandledExceptionMode::ThrowException);
#endif

	// TODO: ������Ӧ�ó��� ID �ַ����滻ΪΨһ�� ID �ַ�����������ַ�����ʽ
	//Ϊ CompanyName.ProductName.SubProduct.VersionInformation
	SetAppID(_T("DataServerCenter.AppID.NoVersion"));

	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}

// Ψһ��һ�� CDataServerCenterApp ����

CDataServerCenterApp theApp;


// CDataServerCenterApp ��ʼ��

BOOL CDataServerCenterApp::InitInstance()
{
	//skinppLoadSkin(skin_path); //����Ƥ��
	// ���һ�������� Windows XP �ϵ�Ӧ�ó����嵥ָ��Ҫ
	// ʹ�� ComCtl32.dll �汾 6 ����߰汾�����ÿ��ӻ���ʽ��
	//����Ҫ InitCommonControlsEx()�����򣬽��޷��������ڡ�
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// ��������Ϊ��������Ҫ��Ӧ�ó�����ʹ�õ�
	// �����ؼ��ࡣ
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinAppEx::InitInstance();


	// ��ʼ�� OLE ��
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}

	AfxEnableControlContainer();

	EnableTaskbarInteraction(FALSE);

	// ʹ�� RichEdit �ؼ���Ҫ  AfxInitRichEdit2()	
	// AfxInitRichEdit2();

	// ��׼��ʼ��
	// ���δʹ����Щ���ܲ�ϣ����С
	// ���տ�ִ���ļ��Ĵ�С����Ӧ�Ƴ�����
	// ����Ҫ���ض���ʼ������
	// �������ڴ洢���õ�ע�����
	// TODO: Ӧ�ʵ��޸ĸ��ַ�����
	// �����޸�Ϊ��˾����֯��
	SetRegistryKey(_T("Ӧ�ó��������ɵı���Ӧ�ó���"));
	//LoadStdProfileSettings(4);  // ���ر�׼ INI �ļ�ѡ��(���� MRU)


	InitContextMenuManager();

	InitKeyboardManager();

	InitTooltipManager();
	CMFCToolTipInfo ttParams;
	ttParams.m_bVislManagerTheme = TRUE;
	theApp.GetTooltipManager()->SetTooltipParams(AFX_TOOLTIP_TYPE_ALL,
		RUNTIME_CLASS(CMFCToolTipCtrl), &ttParams);

	// ע��Ӧ�ó�����ĵ�ģ�塣�ĵ�ģ��
	// �������ĵ�����ܴ��ں���ͼ֮�������
	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CDataServerCenterDoc),
		RUNTIME_CLASS(CMainFrame),       // �� SDI ��ܴ���
		RUNTIME_CLASS(CDataServerCenterView));
	if (!pDocTemplate)
		return FALSE;
	AddDocTemplate(pDocTemplate);


	// ������׼ shell ���DDE�����ļ�������������
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	g_ArrayComponnetName.Add(L"��ȩ");
	g_ArrayComponnetName.Add(L"��������");
	g_ArrayComponnetName.Add(L"�״�");
	g_ArrayComponnetName.Add(L"��������");
	g_ArrayComponnetName.Add(L"������");
	g_ArrayComponnetName.Add(L"�춡��");
	g_ArrayComponnetName.Add(L"��������");
	g_ArrayComponnetName.Add(L"������");
	g_ArrayComponnetName.Add(L"���촼");
	g_ArrayComponnetName.Add(L"��������");
	g_ArrayComponnetName.Add(L"��������");
	g_ArrayComponnetName.Add(L"��������");
	g_ArrayComponnetName.Add(L"��ȩ");
	g_ArrayComponnetName.Add(L"����");
	g_ArrayComponnetName.Add(L"��-���Ҵ�");
	g_ArrayComponnetName.Add(L"����");
	g_ArrayComponnetName.Add(L"����");

	// ��������������ָ����������
	// �� /RegServer��/Register��/Unregserver �� /Unregister ����Ӧ�ó����򷵻� FALSE��
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

	// Ψһ��һ�������ѳ�ʼ���������ʾ����������и���
	int xSize=::GetSystemMetrics(SM_CXSCREEN);
	int ySize=::GetSystemMetrics(SM_CYSCREEN);
	int cx=720;
	int cy=720;
	int x=(xSize-cx)/2;
	int y=(ySize-cy)/2;
	//m_pMainWnd->MoveWindow(x,y,cx,cy);
	m_pMainWnd->SetWindowPos(m_pMainWnd,x,y,cx,cy,SWP_SHOWWINDOW);
	m_pMainWnd->ShowWindow(SW_SHOW);
	m_pMainWnd->SetWindowText(L"�׾Ƽ�������Դƽ̨-������");
	m_pMainWnd->UpdateWindow();
	// �������к�׺ʱ�ŵ��� DragAcceptFiles
	//  �� SDI Ӧ�ó����У���Ӧ�� ProcessShellCommand ֮����
	return TRUE;
}

int CDataServerCenterApp::ExitInstance()
{
	//TODO: �����������ӵĸ�����Դ
	AfxOleTerm(FALSE);
	return CWinAppEx::ExitInstance();
}

// CDataServerCenterApp ��Ϣ�������


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

protected:
//	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
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

// �������жԻ����Ӧ�ó�������
void CDataServerCenterApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

// CDataServerCenterApp �Զ������/���淽��

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

// CDataServerCenterApp ��Ϣ�������



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
		AfxMessageBox(L"ͼ���ļ�װ�س���");
		return E_FAIL;
	}

	LPVOID lpData = NULL;

	lpData = ::GlobalLock(hGlobal);

	ReadFile(hFile, lpData, dwFileSize, &dwByteRead, NULL);
	::GlobalUnlock(hGlobal);

	if(CreateStreamOnHGlobal(hGlobal, TRUE, &pStream) != S_OK)
	{
		AfxMessageBox(L"����ʼ��ʧ�ܣ�");
		return E_FAIL;
	}

	hResult = OleLoadPicture(pStream, dwFileSize, TRUE, IID_IPicture, (LPVOID*) &pPicture);

	if(FAILED(hResult))
	{
		AfxMessageBox(L"ͼ���ļ�װ�س���");
		return E_FAIL;
	}

	OLE_XSIZE_HIMETRIC hmWidth;//ͼƬ����ʵ���
	OLE_YSIZE_HIMETRIC hmHeight;//ͼƬ����ʵ�߶�
	pPicture->get_Width(&hmWidth);
	pPicture->get_Height(&hmHeight);

	//��ͼ���������Ļ��
	hResult = pPicture->Render(hDC, 0, 0, nScrWidth, nScrHeight, 0, hmHeight, hmWidth, -hmHeight, NULL);

	CloseHandle(hFile);//�رմ򿪵��ļ�

	pPicture->Release();
	// Free memory.
	::GlobalFree(hGlobal);

	if (SUCCEEDED(hResult))
	{
		return S_OK;
	}
	else
	{
		AfxMessageBox(L"ͼ���ļ�װ�س���");
		return E_FAIL;
	}
}
BOOL GetDirTime(CString DirName, SYSTEMTIME &stime)
{

	// ���ļ���
	HANDLE hDir = CreateFile (DirName, GENERIC_READ,FILE_SHARE_READ|FILE_SHARE_DELETE, 
		  NULL, OPEN_EXISTING,FILE_FLAG_BACKUP_SEMANTICS, NULL); 
	FILETIME lpCreationTime; // �ļ��еĴ���ʱ��
	FILETIME lpLastAccessTime; // ���ļ��е��������ʱ�� 
	FILETIME lpLastWriteTime; // �ļ��е�����޸�ʱ�� 
	// ��ȡ�ļ���ʱ��������Ϣ
	BOOL retval=GetFileTime(hDir, &lpCreationTime, &lpLastAccessTime, &lpLastWriteTime);
	if (retval)
	{ 
		FILETIME ftime; 
		FileTimeToLocalFileTime(&lpLastWriteTime, &ftime); // ת���ɱ���ʱ�� 
		FileTimeToSystemTime(&ftime, &stime); // ת����ϵͳʱ���ʽ
	} 
	CloseHandle(hDir); // �رմ򿪹����ļ���
	return retval;
}


//��
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

int PredictProject(const CString &SpecPath,CMatrix &ComponentConcentration)
{

	CMatrix m_SpecData(1,869);
	CCSVOperator CSVOperator;
	CSVOperator.LoadCSV(SpecPath);

	for(int j=1;j<=869;j++)
	{
		double tempDouble;
		CSVOperator.GetDouble(j,2,tempDouble);
		m_SpecData.SetItem(1,j,tempDouble);
	}

	CComponentPredictModel ComponentPredictModel;
	ComponentPredictModel.Open();

	CMatrix Weight;
	CMatrix Offset(1,COMPONENT_COUNT);
	for(int i = 0;i<g_ArrayComponnetName.GetCount();++i)
	{
		CMatrix tmp_w(1,SPEC_LENTH);
		ComponentPredictModel.m_strFilter.Format(L"CAST ([Component] as varchar(1000))= '%s'",g_ArrayComponnetName[i]);
		ComponentPredictModel.Requery();
		CString2Matrix(tmp_w,ComponentPredictModel.m_Weight,L",");
		Weight.AddRows(tmp_w);
		Offset.SetItem(1,i+1,ComponentPredictModel.m_Offset);
	}
	Weight = Weight.Transposition();
	ComponentConcentration = m_SpecData*Weight+Offset;

	for(int i = 0;i<ComponentConcentration.m_column;++i)
	{
		if(ComponentConcentration(1,i+1)<0)
		{
			return -1;
		}
	}

	CConcentrationTraceBackModel ConcentrationTraceBackModel;
	ConcentrationTraceBackModel.Open();

	double minDist;
	double Threshold;
	double curDist;
	int predict;
	CMatrix stdConcentration(1,COMPONENT_COUNT);
	if(!ConcentrationTraceBackModel.IsEOF())
	{
		ConcentrationTraceBackModel.MoveFirst();
		CString2Matrix(stdConcentration,ConcentrationTraceBackModel.m_StandardComponentConcentration,L",");
		minDist = (stdConcentration - ComponentConcentration).Norm();
		Threshold = ConcentrationTraceBackModel.m_Threshold;
		predict = ConcentrationTraceBackModel.m_TypeIndex;
		ConcentrationTraceBackModel.MoveNext();
		while(!ConcentrationTraceBackModel.IsEOF())
		{
			CString2Matrix(stdConcentration,ConcentrationTraceBackModel.m_StandardComponentConcentration,L",");
			curDist = (stdConcentration - ComponentConcentration).Norm();
			if(minDist > curDist)
			{
				minDist = curDist;
				Threshold = ConcentrationTraceBackModel.m_Threshold;
				predict = ConcentrationTraceBackModel.m_TypeIndex;
			}
			ConcentrationTraceBackModel.MoveNext();
		}
	}

	if(minDist < Threshold)
	{
		return predict;
	}
	else
	{
		return 0;
	}
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
	//��������
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
	//�ƾ���
	PLSModel.m_strFilter=L"AlcoholContent is null";
	PLSModel.Requery();
	//��ȡģ������
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
	//�Ƿ���Ҫ���ͷ���
	PLSModel.m_strFilter.Format(L"AlcoholContent='%s' AND Flavour is null",Alcohol);
	PLSModel.Requery();
	FlavourNeed=0;
	if(0!=PLSModel.GetRecordCount())
	{
		//��Ҫ�������ͷ���
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
	//�Ƿ���Ҫ����Ʒ�Ʒ���
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
		//��Ҫ����Ʒ�Ʒ���
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
	//����Ʒ���ڷ���
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

	totalnum=(int)Num_EachLabelInBrand.Sum();

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
	//����char *�����С�����ֽ�Ϊ��λ��һ������ռ�����ֽ�
	int bufLen = strlen(buf);
	//������ֽ��ַ��Ĵ�С�����ַ����㡣
	int len = MultiByteToWideChar(CP_ACP,0,buf,bufLen,NULL,0);
	TCHAR *buftemp = new TCHAR[len + 1];
	MultiByteToWideChar(CP_ACP,0,buf,bufLen,buftemp,len);
	buftemp[len] = '\0';  //����ַ�����β��ע�ⲻ��len+1
	//��TCHAR����ת��ΪCString
	CString pWideChar;
	pWideChar.Append(buftemp);
	//ɾ��������
	delete []buftemp;
	return pWideChar;
}

char * CString2Char(const CString cString) 
{
	//��ȡ���ֽ��ַ��Ĵ�С����С�ǰ��ֽڼ����
	int len = WideCharToMultiByte(CP_ACP,0,cString,cString.GetLength(),NULL,0,NULL,NULL);
	//Ϊ���ֽ��ַ���������ռ䣬�����СΪ���ֽڼ���Ŀ��ֽ��ֽڴ�С
	char * pchar = new char[len+2];   //���ֽ�Ϊ��λ
	//���ֽڱ���ת���ɶ��ֽڱ���
	WideCharToMultiByte(CP_ACP,0,cString,cString.GetLength(),pchar,len,NULL,NULL);
	WideCharToMultiByte(CP_ACP,0,cString,cString.GetLength() + 1 ,pchar,len + 1 ,NULL,NULL);
	pchar[len+1] = '/0';   //���ֽ��ַ���'/0'����

	return pchar;  //�˺����������ڴ�ռ䣬��Ҫ�ں�����delete
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
	//�ļ��о�λ��
	int iPathLen = _tcslen(lpszFromPath);
	TCHAR tczFromFolder[MAX_PATH+1];
	ZeroMemory(tczFromFolder, (MAX_PATH+1)*sizeof(TCHAR));
	_tcscpy(tczFromFolder, lpszFromPath);
	tczFromFolder[iPathLen] = _T('\0');
	tczFromFolder[iPathLen+1] = _T('\0');

	//�ļ�����λ��
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
	//�����ļ���λ��
	int iPathLen = _tcslen(lpszFromPath);
	TCHAR tczFromFolder[MAX_PATH+1];
	ZeroMemory(tczFromFolder, (MAX_PATH+1)*sizeof(TCHAR));
	_tcscpy(tczFromFolder, lpszFromPath);
	tczFromFolder[iPathLen] = _T('\0');
	tczFromFolder[iPathLen+1] = _T('\0');

	//�ļ�����λ��
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

