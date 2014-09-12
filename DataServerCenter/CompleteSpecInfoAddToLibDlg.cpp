// CompleteSpecInfoAddToLibDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "DataServerCenter.h"
#include "CompleteSpecInfoAddToLibDlg.h"
#include "afxdialogex.h"


// CCompleteSpecInfoAddToLibDlg �Ի���

IMPLEMENT_DYNAMIC(CCompleteSpecInfoAddToLibDlg, CDialogEx)

CCompleteSpecInfoAddToLibDlg::CCompleteSpecInfoAddToLibDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CCompleteSpecInfoAddToLibDlg::IDD, pParent)
	, m_WineName(_T(""))
	, m_AlcoholContent(_T(""))
	, m_Flavour(_T(""))
	, m_Brand(_T(""))
	, m_ProductionDate(_T(""))
	, m_BatchNo(_T(""))
	, m_SpecIDTitle(_T(""))
	, m_Manager(_T(""))
	, m_ComFrom(_T(""))
    , m_WineNameInstruction(_T(""))
	, m_AlcoholContentInstruction(_T(""))
	, m_FlavourInstruction(_T(""))
	, m_BrandInstruction(_T(""))
	, m_ProductionDateInstruction(_T(""))
	, m_BatchNoInstruction(_T(""))
	, m_SpecIDTitleInstruction(_T(""))
	, m_ComFromInstruction(_T(""))
	, m_ManagerInstruction(_T(""))
{

}

CCompleteSpecInfoAddToLibDlg::~CCompleteSpecInfoAddToLibDlg()
{
}

void CCompleteSpecInfoAddToLibDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_WINENAME, m_EditWineName);
	DDX_Text(pDX, IDC_EDIT_WINENAME, m_WineName);
	DDX_Control(pDX, IDC_EDIT_WINENAMEINSTRUCTION, m_EditWineNameInstruction);
	DDX_Text(pDX, IDC_EDIT_WINENAMEINSTRUCTION, m_WineNameInstruction);
	DDX_Text(pDX, IDC_EDIT_ALCOHOLCONTENT, m_AlcoholContent);
	DDX_Text(pDX, IDC_EDIT_FLAVOUR, m_Flavour);
	DDX_Text(pDX, IDC_EDIT_BRAND, m_Brand);
	DDX_Text(pDX, IDC_EDIT_PRODUCTIONDATE, m_ProductionDate);
	DDX_Text(pDX, IDC_EDIT_BATCHNO, m_BatchNo);
	DDX_Text(pDX, IDC_EDIT_SPECIDTITLE, m_SpecIDTitle);
	DDX_Text(pDX, IDC_EDIT_COMFROM, m_ComFrom);
	DDX_Text(pDX, IDC_EDIT_MANAGER, m_Manager);
	DDX_Text(pDX, IDC_EDIT_ALCOHOLCONTENTINSTRUCTION, m_AlcoholContentInstruction);
	DDX_Text(pDX, IDC_EDIT_FLAVOURINSTRUCTION, m_FlavourInstruction);
	DDX_Text(pDX, IDC_EDIT_BRANDINSTRUCTION, m_BrandInstruction);
	DDX_Text(pDX, IDC_EDIT_PRODUCTIONDATEINSTRUCTION, m_ProductionDateInstruction);
	DDX_Text(pDX, IDC_EDIT_BATCHNOINSTRUCTION, m_BatchNoInstruction);
	DDX_Text(pDX, IDC_EDIT_SPECIDTITLEINSTRUCTION, m_SpecIDTitleInstruction);
	DDX_Text(pDX, IDC_EDIT_COMFROMINSTRUCTION, m_ComFromInstruction);
	DDX_Text(pDX, IDC_EDIT_MANAGERINSTRUCTION, m_ManagerInstruction);
}


BEGIN_MESSAGE_MAP(CCompleteSpecInfoAddToLibDlg, CDialogEx)
	ON_EN_SETFOCUS(IDC_EDIT_WINENAME, &CCompleteSpecInfoAddToLibDlg::OnEnSetfocusEditWinename)
	ON_EN_SETFOCUS(IDC_EDIT_ALCOHOLCONTENT, &CCompleteSpecInfoAddToLibDlg::OnEnSetfocusEditAlcoholcontent)
	ON_EN_SETFOCUS(IDC_EDIT_FLAVOUR, &CCompleteSpecInfoAddToLibDlg::OnEnSetfocusEditFlavour)
	ON_EN_SETFOCUS(IDC_EDIT_BRAND, &CCompleteSpecInfoAddToLibDlg::OnEnSetfocusEditBrand)
	ON_EN_SETFOCUS(IDC_EDIT_PRODUCTIONDATE, &CCompleteSpecInfoAddToLibDlg::OnEnSetfocusEditProductiondate)
	ON_EN_SETFOCUS(IDC_EDIT_BATCHNO, &CCompleteSpecInfoAddToLibDlg::OnEnSetfocusEditBatchno)
	ON_EN_SETFOCUS(IDC_EDIT_SPECIDTITLE, &CCompleteSpecInfoAddToLibDlg::OnEnSetfocusEditSpecidtitle)
	ON_EN_SETFOCUS(IDC_EDIT_COMFROM, &CCompleteSpecInfoAddToLibDlg::OnEnSetfocusEditComfrom)
	ON_EN_SETFOCUS(IDC_EDIT_MANAGER, &CCompleteSpecInfoAddToLibDlg::OnEnSetfocusEditManager)
	ON_BN_CLICKED(IDOK, &CCompleteSpecInfoAddToLibDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CCompleteSpecInfoAddToLibDlg ��Ϣ�������


void CCompleteSpecInfoAddToLibDlg::OnEnSetfocusEditWinename()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);
	m_WineNameInstruction=L"����׾����ƣ�";
	m_AlcoholContentInstruction=L"";
	m_FlavourInstruction=L"";
	m_BrandInstruction=L"";
	m_ProductionDateInstruction=L"";
	m_BatchNoInstruction=L"";
	m_SpecIDTitleInstruction=L"";
	m_ComFromInstruction=L"";
	m_ManagerInstruction=L"";
	UpdateData(FALSE);

}


void CCompleteSpecInfoAddToLibDlg::OnEnSetfocusEditAlcoholcontent()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);
	m_WineNameInstruction=L"";
	m_AlcoholContentInstruction=L"����׾ƾƾ��ȣ�";
	m_FlavourInstruction=L"";
	m_BrandInstruction=L"";
	m_ProductionDateInstruction=L"";
	m_BatchNoInstruction=L"";
	m_SpecIDTitleInstruction=L"";
	m_ComFromInstruction=L"";
	m_ManagerInstruction=L"";
	UpdateData(FALSE);
}


void CCompleteSpecInfoAddToLibDlg::OnEnSetfocusEditFlavour()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);
	m_WineNameInstruction=L"";
	m_AlcoholContentInstruction=L"";
	m_FlavourInstruction=L"����׾����ͣ�";
	m_BrandInstruction=L"";
	m_ProductionDateInstruction=L"";
	m_BatchNoInstruction=L"";
	m_SpecIDTitleInstruction=L"";
	m_ComFromInstruction=L"";
	m_ManagerInstruction=L"";
	UpdateData(FALSE);
}


void CCompleteSpecInfoAddToLibDlg::OnEnSetfocusEditBrand()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);
	m_WineNameInstruction=L"";
	m_AlcoholContentInstruction=L"";
	m_FlavourInstruction=L"";
	m_BrandInstruction=L"����׾�Ʒ�ƣ�";
	m_ProductionDateInstruction=L"";
	m_BatchNoInstruction=L"";
	m_SpecIDTitleInstruction=L"";
	m_ComFromInstruction=L"";
	m_ManagerInstruction=L"";
	UpdateData(FALSE);
}


void CCompleteSpecInfoAddToLibDlg::OnEnSetfocusEditProductiondate()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);
	m_WineNameInstruction=L"";
	m_AlcoholContentInstruction=L"";
	m_FlavourInstruction=L"";
	m_BrandInstruction=L"";
	m_ProductionDateInstruction=L"����׾��������ڣ�";
	m_BatchNoInstruction=L"";
	m_SpecIDTitleInstruction=L"";
	m_ComFromInstruction=L"";
	m_ManagerInstruction=L"";
	UpdateData(FALSE);
}


void CCompleteSpecInfoAddToLibDlg::OnEnSetfocusEditBatchno()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);
	m_WineNameInstruction=L"";
	m_AlcoholContentInstruction=L"";
	m_FlavourInstruction=L"";
	m_BrandInstruction=L"";
	m_ProductionDateInstruction=L"";
	m_BatchNoInstruction=L"����׾��������ţ�";
	m_SpecIDTitleInstruction=L"";
	m_ComFromInstruction=L"";
	m_ManagerInstruction=L"";
	UpdateData(FALSE);
}


void CCompleteSpecInfoAddToLibDlg::OnEnSetfocusEditSpecidtitle()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);
	m_WineNameInstruction=L"";
	m_AlcoholContentInstruction=L"";
	m_FlavourInstruction=L"";
	m_BrandInstruction=L"";
	m_ProductionDateInstruction=L"";
	m_BatchNoInstruction=L"";
	m_SpecIDTitleInstruction=L"��������׾ƹ������ݵ������ţ��������ɹ��ױ�ţ���ʽ�ɹ�����Դ��Ϣ����ĸ+���ײɼ�������ɣ��磺2012��5��1�����人�ɼ��İ��Ʊ�3�����������ݣ�BYB3NWH20120501";
	m_ComFromInstruction=L"";
	m_ManagerInstruction=L"";
	UpdateData(FALSE);
}


void CCompleteSpecInfoAddToLibDlg::OnEnSetfocusEditComfrom()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);
	m_WineNameInstruction=L"";
	m_AlcoholContentInstruction=L"";
	m_FlavourInstruction=L"";
	m_BrandInstruction=L"";
	m_ProductionDateInstruction=L"";
	m_BatchNoInstruction=L"";
	m_SpecIDTitleInstruction=L"";
	m_ComFromInstruction=L"�ü�̵�����˵���׾���Դ��Ϣ����Ҫ�����²�����ɣ��ɼ���Ա��λ���ɼ�ʱ�䣬�ɼ���ʽ";
	m_ManagerInstruction=L"";
	UpdateData(FALSE);
}


void CCompleteSpecInfoAddToLibDlg::OnEnSetfocusEditManager()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);
	m_WineNameInstruction=L"";
	m_AlcoholContentInstruction=L"";
	m_FlavourInstruction=L"";
	m_BrandInstruction=L"";
	m_ProductionDateInstruction=L"";
	m_BatchNoInstruction=L"";
	m_SpecIDTitleInstruction=L"";
	m_ComFromInstruction=L"";
	m_ManagerInstruction=L"�뵱ǰ����Ա��ʵ��д�Լ�������";
	UpdateData(FALSE);
}


BOOL CCompleteSpecInfoAddToLibDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}


void CCompleteSpecInfoAddToLibDlg::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);
	if(m_WineName.IsEmpty())
	{
		MessageBox(L"�׾����Ʋ���Ϊ�գ�",L"�������",MB_ICONWARNING);
		return ;
	}
	if(m_AlcoholContent.IsEmpty())
	{
		MessageBox(L"�׾����Ʋ���Ϊ�գ�",L"�������",MB_ICONWARNING);
		return ;
	}
	if(m_Flavour.IsEmpty())
	{
		MessageBox(L"�׾����Ʋ���Ϊ�գ�",L"�������",MB_ICONWARNING);
		return ;
	}
	if(m_Brand.IsEmpty())
	{
		MessageBox(L"�׾����Ʋ���Ϊ�գ�",L"�������",MB_ICONWARNING);
		return ;
	}
	if(m_ProductionDate.IsEmpty())
	{
		MessageBox(L"�׾��������ڲ���Ϊ�գ�",L"�������",MB_ICONWARNING);
		return ;
	}
	if(m_BatchNo.IsEmpty())
	{
		MessageBox(L"�׾����Ų���Ϊ�գ�",L"�������",MB_ICONWARNING);
		return ;
	}
	if(m_SpecIDTitle.IsEmpty())
	{
		MessageBox(L"�׾�����Ų���Ϊ�գ�",L"�������",MB_ICONWARNING);
		return ;
	}
	if(m_Manager.IsEmpty())
	{
		MessageBox(L"������Ա����Ϊ�գ�",L"�������",MB_ICONWARNING);
		return ;
	}
	if(m_ComFrom.IsEmpty())
	{
		MessageBox(L"�׾���Դ��Ϣ����Ϊ�գ�",L"�������",MB_ICONWARNING);
		return ;
	}
	CDialogEx::OnOK();
}
