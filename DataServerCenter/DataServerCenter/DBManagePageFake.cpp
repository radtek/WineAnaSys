// DBManagePageFake.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "DataServerCenter.h"
#include "DBManagePageFake.h"
#include "afxdialogex.h"

extern CString g_CurrentDir;
// CDBManagePageFake �Ի���

IMPLEMENT_DYNAMIC(CDBManagePageFake, CPropertyPage)

CDBManagePageFake::CDBManagePageFake()
	: CPropertyPage(CDBManagePageFake::IDD)
	, m_KeyWords(_T(""))
{
	m_pAddToLibDlg = NULL;
}

CDBManagePageFake::~CDBManagePageFake()
{
	if(m_FakeWineLib.IsOpen())
	{
		m_FakeWineLib.Close();
	}
}

void CDBManagePageFake::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_TOADD_FAKE, m_ListSpectrumToAdd);
	DDX_Control(pDX, IDC_LIST_SPECLIB_FAKE, m_ListSpectrunQueryed);
	DDX_Text(pDX, IDC_EDIT_KEY_FAKE, m_KeyWords);
	DDX_Control(pDX, IDC_BUTTON_LAST_FAKE, m_btnLastPic);
	DDX_Control(pDX, IDC_BUTTON_NEXT_FAKE, m_btnNextPic);
	DDX_Control(pDX, IDC_BUTTON_LASTLIB_FAKE, m_btnLastPicLib);
	DDX_Control(pDX, IDC_BUTTON_NEXTLIB_FAKE, m_btnNextPicLib);
}


BEGIN_MESSAGE_MAP(CDBManagePageFake, CPropertyPage)
	ON_BN_CLICKED(IDC_BUTTON_OPENDATAPACKAGE_FAKE, &CDBManagePageFake::OnBnClickedButtonOpendatapackageFake)
	ON_WM_SHOWWINDOW()
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_BUTTON_SEARCHWITHKEYWORDS_FAKE, &CDBManagePageFake::OnBnClickedButtonSearchwithkeywordsFake)
	ON_BN_CLICKED(IDC_BUTTON_SHOWALL_FAKE, &CDBManagePageFake::OnBnClickedButtonShowallFake)
	ON_BN_CLICKED(IDC_BUTTON_CLEARSPECTOADD_FAKE, &CDBManagePageFake::OnBnClickedButtonClearspectoaddFake)
	ON_BN_CLICKED(IDC_BUTTON_ADDTOLIB_FAKE, &CDBManagePageFake::OnBnClickedButtonAddtolibFake)
	ON_BN_CLICKED(IDC_BUTTON_SELECTALL_FAKE, &CDBManagePageFake::OnBnClickedButtonSelectallFake)
	ON_BN_CLICKED(IDC_BUTTON_INVERTSELECT_FAKE, &CDBManagePageFake::OnBnClickedButtonInvertselectFake)
	ON_BN_CLICKED(IDC_BUTTON_CLEARSEARCH_FAKE, &CDBManagePageFake::OnBnClickedButtonClearsearchFake)
	ON_BN_CLICKED(IDC_BUTTON_DELETE_FAKE, &CDBManagePageFake::OnBnClickedButtonDeleteFake)
	ON_BN_CLICKED(IDC_BUTTON_LAST_FAKE, &CDBManagePageFake::OnBnClickedButtonLastFake)
	ON_BN_CLICKED(IDC_BUTTON_NEXT_FAKE, &CDBManagePageFake::OnBnClickedButtonNextFake)
	ON_BN_CLICKED(IDC_BUTTON_LASTLIB_FAKE, &CDBManagePageFake::OnBnClickedButtonLastlibFake)
	ON_BN_CLICKED(IDC_BUTTON_NEXTLIB_FAKE, &CDBManagePageFake::OnBnClickedButtonNextlibFake)
	ON_WM_PAINT()
	ON_NOTIFY(NM_CLICK, IDC_LIST_SPECLIB_FAKE, &CDBManagePageFake::OnNMClickListSpeclibFake)
END_MESSAGE_MAP()


// CDBManagePageFake ��Ϣ�������


void CDBManagePageFake::OnBnClickedButtonOpendatapackageFake()
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
	//���֮ǰ��������ݼ�
	OnBnClickedButtonClearspectoaddFake();

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
	if(!m_ExcelInfoPath.IsEmpty())
	{
		CString strDirExcel=m_ExcelInfoPath;
		CExcelOperator ExcelOperator;
		ExcelOperator.LoadExcel(strDirExcel);
		ExcelOperator.GetItem(1,1,L"String",m_WineName);
		ExcelOperator.ReleaseExcel();
	}
	m_PicIndex=0;
	CString Winename;
	CString Wineinfo=L"";
	CString Comfrom;
	CString ReceiveDate;
	CString Wineinfotemp;

	SortCArrayByFirst(m_ary_SpecFileName,m_ary_SpecFilePath);

	for(int i=0;i<m_ary_SpecFileName.GetCount();i++)
	{
		CString NuMber;
		NuMber.Format(L"%d",i+1);
		m_ListSpectrumToAdd.InsertItem(i,NuMber);
		m_ListSpectrumToAdd.SetItemText(i,1,m_ary_SpecFileName[i]);
		m_ListSpectrumToAdd.SetItemText(i,2,m_WineName);
	}
	OnPaint();


}


void CDBManagePageFake::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CPropertyPage::OnShowWindow(bShow, nStatus);

	// TODO: �ڴ˴������Ϣ����������
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


void CDBManagePageFake::OnSize(UINT nType, int cx, int cy)
{
	CPropertyPage::OnSize(nType, cx, cy);

	// TODO: �ڴ˴������Ϣ����������
	CWnd *pWnd[22]; 
	pWnd[0] = GetDlgItem(IDC_BUTTON_OPENDATAPACKAGE_FAKE);
	pWnd[1] = GetDlgItem(IDC_BUTTON_CLEARSPECTOADD_FAKE);
	pWnd[2] = GetDlgItem(IDC_BUTTON_ADDTOLIB_FAKE);
	pWnd[3] = GetDlgItem(IDC_LIST_TOADD_FAKE);
	pWnd[4] = GetDlgItem(IDC_STATIC_PICTUREOFSPECTOADD_FAKE);
	pWnd[5] = GetDlgItem(IDC_BUTTON_LAST_FAKE);
	pWnd[6] = GetDlgItem(IDC_BUTTON_NEXT_FAKE);
	pWnd[7] = GetDlgItem(IDC_STATIC1FAKE);
	pWnd[8] = GetDlgItem(IDC_EDIT_KEY_FAKE);
	pWnd[9] = GetDlgItem(IDC_STATIC_KEY);
	pWnd[10] = GetDlgItem(IDC_BUTTON_SEARCHWITHKEYWORDS_FAKE);
	pWnd[11] = GetDlgItem(IDC_BUTTON_SHOWALL_FAKE);
	pWnd[12] = GetDlgItem(IDC_BUTTON_DELETE_FAKE);
	pWnd[13] = GetDlgItem(IDC_BUTTON_UPDATEMODEL_FAKE);
	pWnd[14] = GetDlgItem(IDC_LIST_SPECLIB_FAKE);
	pWnd[15] = GetDlgItem(IDC_STATIC_PICTUREINLIB_FAKE);
	pWnd[16] = GetDlgItem(IDC_BUTTON_LASTLIB_FAKE);
	pWnd[17] = GetDlgItem(IDC_BUTTON_NEXTLIB_FAKE);
	pWnd[18] = GetDlgItem(IDC_STATIC2FAKE);
	pWnd[19] = GetDlgItem(IDC_BUTTON_SELECTALL_FAKE);
	pWnd[20] = GetDlgItem(IDC_BUTTON_INVERTSELECT_FAKE);
	pWnd[21] = GetDlgItem(IDC_BUTTON_CLEARSEARCH_FAKE);
	for(int i=0;i<22;i++)
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

	GetClientRect(&m_rect);//���仯��ĶԻ����С��Ϊ�ɴ�С 
}


BOOL CDBManagePageFake::OnInitDialog()
{
	CPropertyPage::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	///////////////////
	m_ListSpectrumToAdd.InsertColumn( 0, L"", LVCFMT_LEFT,30 );//������ 
	m_ListSpectrumToAdd.InsertColumn( 1, L"�����ļ���", LVCFMT_LEFT,100 );//������ 
	m_ListSpectrumToAdd.InsertColumn( 2, L"��ð�׾�����", LVCFMT_LEFT, 100 );//������ 

	DWORD dwStyle = m_ListSpectrumToAdd.GetExtendedStyle(); //��ȡ��ǰ��չ��ʽ
	dwStyle |= LVS_EX_FULLROWSELECT; //ѡ��ĳ��ʹ���и�����report���ʱ��
	dwStyle |= LVS_EX_GRIDLINES; //�����ߣ�report���ʱ��
	//  dwStyle |= LVS_EX_CHECKBOXES; //itemǰ����checkbox�ؼ�
	m_ListSpectrumToAdd.SetExtendedStyle(dwStyle); //������չ���	

	m_ListSpectrunQueryed.InsertColumn( 0, L"", LVCFMT_LEFT,50);//������ 
	m_ListSpectrunQueryed.InsertColumn( 1, L"���ױ��", LVCFMT_LEFT, 100 );//������ 
	m_ListSpectrunQueryed.InsertColumn( 2, L"��ð�׾�����", LVCFMT_LEFT, 100 );//������ 
	m_ListSpectrunQueryed.InsertColumn( 3, L"��Դ˵��", LVCFMT_LEFT, 100 );//������
	m_ListSpectrunQueryed.InsertColumn( 4, L"�������", LVCFMT_LEFT, 100 );//������
	m_ListSpectrunQueryed.InsertColumn( 5, L"������Ա", LVCFMT_LEFT, 100 );//������


	dwStyle = m_ListSpectrunQueryed.GetExtendedStyle(); //��ȡ��ǰ��չ��ʽ
	dwStyle |= LVS_EX_FULLROWSELECT; //ѡ��ĳ��ʹ���и�����report���ʱ��
	dwStyle |= LVS_EX_GRIDLINES; //�����ߣ�report���ʱ��
    dwStyle |= LVS_EX_CHECKBOXES; //itemǰ����checkbox�ؼ�

	
	//m_ListSpectrumToAdd.SetExtendedStyle(dwStyle); 
	m_ListSpectrumToAdd.SetHeaderBackColor(168,193,221,2);
	m_ListSpectrumToAdd.SetRowHeigt(28);

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


void CDBManagePageFake::OnBnClickedButtonSearchwithkeywordsFake()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);
	if(m_KeyWords.IsEmpty())
	{
		MessageBox(L"�ؼ��ֲ���Ϊ�գ�",L"���ײ�ѯ",MB_ICONWARNING);
		return;
	}

	m_FakeWineLib.m_strSort=L"WineNameFaked";
	if(m_FakeWineLib.IsOpen())
	{
		m_FakeWineLib.Close();
	}
	if(!m_FakeWineLib.Open())
	{
		MessageBox(L"����ƿ�ʧ��!",L"��ʧ��",MB_ICONERROR);
		return;
	}
	m_FakeWineLib.m_strFilter.Format(L"WineNameFaked LIKE '%%%s%%' OR Comfrom LIKE '%%%s%%'",m_KeyWords,m_KeyWords);
	m_FakeWineLib.Requery();
	ShowQuerySpecRecord();
}


void CDBManagePageFake::OnBnClickedButtonShowallFake()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_FakeWineLib.m_strFilter=L"";
	m_FakeWineLib.m_strSort=L"WineNameFaked";

	if(m_FakeWineLib.IsOpen())
	{
		m_FakeWineLib.Close();
	}
	if(!m_FakeWineLib.Open())
	{
		MessageBox(L"����ƿ�ʧ��!",L"��ʧ��",MB_ICONERROR);
		return;
	}
	m_FakeWineLib.m_strSort=L"WineNameFaked";
	ShowQuerySpecRecord();

}
void CDBManagePageFake::ShowQuerySpecRecord()
{
	m_ListSpectrunQueryed.DeleteAllItems();
	if(m_FakeWineLib.IsEOF()||!m_FakeWineLib.IsOpen())
	{
		MessageBox(L"û����ؼ�¼!",L"�޼�¼",MB_ICONWARNING);
		return;
	}
	m_FakeWineLib.MoveFirst();

	int count=0;
	while(!m_FakeWineLib.IsEOF())
	{
		CString Number;
		Number.Format(L"%d",count+1);
		m_ListSpectrunQueryed.InsertItem(count,Number);
		m_ListSpectrunQueryed.SetItemText(count,1,m_FakeWineLib.m_SpectrumID);
		m_ListSpectrunQueryed.SetItemText(count,2,m_FakeWineLib.m_WineNameFaked);
		m_ListSpectrunQueryed.SetItemText(count,3,m_FakeWineLib.m_Comfrom);

		m_ListSpectrunQueryed.SetItemText(count,4,m_FakeWineLib.m_ImportDate);
		m_ListSpectrunQueryed.SetItemText(count,5,m_FakeWineLib.m_Manager);

		m_FakeWineLib.MoveNext();
		count++;
	}
	m_ListSpectrunQueryed.UpdateWindow();
	m_FakeWineLib.MoveFirst();
}

void CDBManagePageFake::OnBnClickedButtonClearspectoaddFake()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_ListSpectrumToAdd.DeleteAllItems();
	m_ary_PicturesPath.RemoveAll();
	m_ary_PicturesName.RemoveAll();

	m_ary_SpecFileName.RemoveAll();
	m_ary_SpecFilePath.RemoveAll();
	m_ExcelInfoPath=L"";

	m_WineName=L"";
	m_Comfrom=L"";
	m_SpecIDTitle=L"";
	m_Manager=L"";
}


void CDBManagePageFake::OnBnClickedButtonAddtolibFake()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if(m_ary_SpecFileName.IsEmpty())
	{
		MessageBox(L"û��Ҫ���Ĺ�������,����ӹ�������",L"�������",MB_ICONERROR);
		return ;
	}
	if(m_ary_PicturesPath.IsEmpty())
	{
		MessageBox(L"ȱ�ٱ�Ҫ��ͼƬ�����ʧ��",L"�������",MB_ICONERROR);
		return ;
	}

	CCompleteSpecAddToFakeLibInfoDlg CompleteSpecAddToFakeLibInfoDlg;
	CompleteSpecAddToFakeLibInfoDlg.m_WineName=m_WineName;
	CompleteSpecAddToFakeLibInfoDlg.m_Comfrom=m_Comfrom;
	CompleteSpecAddToFakeLibInfoDlg.m_SpecIDTitle=m_SpecIDTitle;
	CompleteSpecAddToFakeLibInfoDlg.m_Manager=m_Manager;
	

	if(IDOK == CompleteSpecAddToFakeLibInfoDlg.DoModal())
	{
		if(NULL!=m_pAddToLibDlg)
		{
			m_pAddToLibDlg->DestroyWindow();
			m_pAddToLibDlg=NULL;
		}
		m_Comfrom=CompleteSpecAddToFakeLibInfoDlg.m_Comfrom;
		m_SpecIDTitle=CompleteSpecAddToFakeLibInfoDlg.m_SpecIDTitle;
		m_Manager=CompleteSpecAddToFakeLibInfoDlg.m_Manager;
		m_WineName = CompleteSpecAddToFakeLibInfoDlg.m_WineName;

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

		CreateDirectory(m_strFolderPath, NULL );
		for(int i=0;i<m_ary_PicturesPath.GetCount();i++)
		{
			//���ɱ���ͼƬ���ļ���
			CString newpicpath;
			newpicpath.Format(L"\\Lib\\Pictures\\%s\\%s",m_SpecIDTitle,m_ary_PicturesName[i]);
			CopyFile(m_ary_PicturesPath[i],g_CurrentDir+newpicpath,0);
			WinePictures.AddNew();
			WinePictures.m_WineName=m_WineName;
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
		pThreadAddSpecToLib=AfxBeginThread(ThreadAddSpecToFakeLib,this);
	}
}
UINT ThreadAddSpecToFakeLib(LPVOID lpParam)
{
	CDBManagePageFake* pDBManageFake=(CDBManagePageFake*)lpParam;
	CFakeWineLib FakeWineLib;
	CCSVOperator CSVOperator;
	CTime tm=CTime::GetCurrentTime();
	CString Currenttime;
	Currenttime=tm.Format("%Y-%m-%d");
	if(!FakeWineLib.Open())
	{
		pDBManageFake->MessageBox(L"�򿪹��׿�ʧ�ܣ�",L"�������",MB_ICONERROR);
		return 0;
	}

	for(int i=0;i<pDBManageFake->m_ary_SpecFilePath.GetCount();i++)
	{
		pDBManageFake->m_pAddToLibDlg->m_CurrentProgress=i+1;
		::PostMessage((HWND)(pDBManageFake->m_pAddToLibDlg->m_hWnd),WM_DISPLAY,0,0);
		CString Current=pDBManageFake->m_ary_SpecFilePath[i];

		CSVOperator.LoadCSV(pDBManageFake->m_ary_SpecFilePath[i]);
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
		SpecID.Format(L"%s-%d",pDBManageFake->m_SpecIDTitle,i+1);

		FakeWineLib.AddNew();
		FakeWineLib.m_WineNameFaked=pDBManageFake->m_WineName;
		FakeWineLib.m_SpectrumData=Specdata;
		FakeWineLib.m_Comfrom=pDBManageFake->m_Comfrom;
		FakeWineLib.m_SpectrumID=SpecID;
		FakeWineLib.m_ImportDate=Currenttime;
		FakeWineLib.m_Manager=pDBManageFake->m_Manager;
		FakeWineLib.m_WaveNumber=WaveNumber;
		FakeWineLib.Update();

	}
	pDBManageFake->m_pAddToLibDlg->ShowWindow(SW_HIDE);
	pDBManageFake->MessageBox(L"�������ݵ���ɹ�",L"�����������",MB_ICONINFORMATION);

	return 0;
}

void CDBManagePageFake::OnBnClickedButtonSelectallFake()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
    int nCount=m_ListSpectrunQueryed.GetItemCount();
    for(int i=0;i<nCount;i++)
    {
        m_ListSpectrunQueryed.SetCheck(i,TRUE);
    }
}


void CDBManagePageFake::OnBnClickedButtonInvertselectFake()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	int nCount=m_ListSpectrunQueryed.GetItemCount();
	for(int i=0;i<nCount;i++)
	{
		m_ListSpectrunQueryed.SetCheck(i,m_ListSpectrunQueryed.GetCheck(i)?FALSE:TRUE);
	}
}


void CDBManagePageFake::OnBnClickedButtonClearsearchFake()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if(m_FakeWineLib.IsOpen())
	{
		m_FakeWineLib.Close();
	}
	m_ListSpectrunQueryed.DeleteAllItems();
	m_PicQueryedIndex=0;
	m_ary_PicturesQueryedPath.RemoveAll();
}


void CDBManagePageFake::OnBnClickedButtonDeleteFake()
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
			m_FakeWineLib.MoveFirst();
			for(int j=0;j!=i;j++)m_FakeWineLib.MoveNext();
			m_FakeWineLib.Delete();
		}

	}
	m_FakeWineLib.Requery();
	ShowQuerySpecRecord();
}




void CDBManagePageFake::OnBnClickedButtonLastFake()
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


void CDBManagePageFake::OnBnClickedButtonNextFake()
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


void CDBManagePageFake::OnBnClickedButtonLastlibFake()
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


void CDBManagePageFake::OnBnClickedButtonNextlibFake()
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


void CDBManagePageFake::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: �ڴ˴������Ϣ����������
	// ��Ϊ��ͼ��Ϣ���� CPropertyPage::OnPaint()

	BOOL ret = 0;
	if(!m_ary_PicturesPath.IsEmpty())
	{
		CRect rect;
		GetDlgItem(IDC_STATIC_PICTUREOFSPECTOADD_FAKE)->GetWindowRect(rect);
		DisplayPic(m_ary_PicturesPath[m_PicIndex],this->GetDlgItem(IDC_STATIC_PICTUREOFSPECTOADD_FAKE)->GetSafeHwnd(), rect.Width(), rect.Height());
	}
	else
	{	
		CRect rect;
		GetDlgItem(IDC_STATIC_PICTUREOFSPECTOADD_FAKE)->GetWindowRect(rect);
		CString path = g_CurrentDir;
		path+= L"\\Resource\\NOPic.jpg";
		ret = DisplayPic(path,this->GetDlgItem(IDC_STATIC_PICTUREOFSPECTOADD_FAKE)->GetSafeHwnd(), rect.Width(), rect.Height());
	}
	if(!m_ary_PicturesQueryedPath.IsEmpty())
	{
		CRect rect;
		GetDlgItem(IDC_STATIC_PICTUREINLIB_FAKE)->GetWindowRect(rect);
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
		DisplayPic(path,this->GetDlgItem(IDC_STATIC_PICTUREINLIB_FAKE)->GetSafeHwnd(), rect.Width(), rect.Height());
	}
	else
	{
		CRect rect;
		GetDlgItem(IDC_STATIC_PICTUREINLIB_FAKE)->GetWindowRect(rect);
		CString path = g_CurrentDir;
		path+= L"\\Resource\\NOPic.jpg";
		ret = DisplayPic(path,this->GetDlgItem(IDC_STATIC_PICTUREINLIB_FAKE)->GetSafeHwnd(), rect.Width(), rect.Height());
	}
}


void CDBManagePageFake::OnNMClickListSpeclibFake(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	*pResult = 0;
	if(m_ListSpectrunQueryed.GetSelectedCount()==0)
	{
		return ;
	}
	int nItem=m_ListSpectrunQueryed.GetSelectionMark();
	if(m_FakeWineLib.IsOpen())
	{
		if(!m_FakeWineLib.IsEOF())
		{
			m_FakeWineLib.MoveFirst();
		}
		int count=0;
		for(;count<nItem;count++)
		{
			m_FakeWineLib.MoveNext();
		}
		CWinePictures WinePictures;
		WinePictures.Open();
		CString specIDTitle = m_FakeWineLib.m_SpectrumID;
		specIDTitle = specIDTitle.Left(specIDTitle.ReverseFind('-'));
		WinePictures.m_strFilter.Format(L" SpecIDTitle = '%s'",specIDTitle);
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
		m_FakeWineLib.MoveFirst();
	}	
	OnPaint();
}
