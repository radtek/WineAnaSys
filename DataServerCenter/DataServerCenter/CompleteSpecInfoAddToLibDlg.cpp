// CompleteSpecInfoAddToLibDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "DataServerCenter.h"
#include "CompleteSpecInfoAddToLibDlg.h"
#include "afxdialogex.h"


// CCompleteSpecInfoAddToLibDlg 对话框

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


// CCompleteSpecInfoAddToLibDlg 消息处理程序


void CCompleteSpecInfoAddToLibDlg::OnEnSetfocusEditWinename()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	m_WineNameInstruction=L"输入白酒名称！";
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
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	m_WineNameInstruction=L"";
	m_AlcoholContentInstruction=L"输入白酒酒精度！";
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
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	m_WineNameInstruction=L"";
	m_AlcoholContentInstruction=L"";
	m_FlavourInstruction=L"输入白酒香型！";
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
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	m_WineNameInstruction=L"";
	m_AlcoholContentInstruction=L"";
	m_FlavourInstruction=L"";
	m_BrandInstruction=L"输入白酒品牌！";
	m_ProductionDateInstruction=L"";
	m_BatchNoInstruction=L"";
	m_SpecIDTitleInstruction=L"";
	m_ComFromInstruction=L"";
	m_ManagerInstruction=L"";
	UpdateData(FALSE);
}


void CCompleteSpecInfoAddToLibDlg::OnEnSetfocusEditProductiondate()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	m_WineNameInstruction=L"";
	m_AlcoholContentInstruction=L"";
	m_FlavourInstruction=L"";
	m_BrandInstruction=L"";
	m_ProductionDateInstruction=L"输入白酒生产日期！";
	m_BatchNoInstruction=L"";
	m_SpecIDTitleInstruction=L"";
	m_ComFromInstruction=L"";
	m_ManagerInstruction=L"";
	UpdateData(FALSE);
}


void CCompleteSpecInfoAddToLibDlg::OnEnSetfocusEditBatchno()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	m_WineNameInstruction=L"";
	m_AlcoholContentInstruction=L"";
	m_FlavourInstruction=L"";
	m_BrandInstruction=L"";
	m_ProductionDateInstruction=L"";
	m_BatchNoInstruction=L"输入白酒生产批号！";
	m_SpecIDTitleInstruction=L"";
	m_ComFromInstruction=L"";
	m_ManagerInstruction=L"";
	UpdateData(FALSE);
}


void CCompleteSpecInfoAddToLibDlg::OnEnSetfocusEditSpecidtitle()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	m_WineNameInstruction=L"";
	m_AlcoholContentInstruction=L"";
	m_FlavourInstruction=L"";
	m_BrandInstruction=L"";
	m_ProductionDateInstruction=L"";
	m_BatchNoInstruction=L"";
	m_SpecIDTitleInstruction=L"输入此批白酒光谱数据的总体编号，用于生成光谱编号，形式由光谱来源信息首字母+光谱采集日期组成，如：2012年5月1日在武汉采集的白云边3年陈酿光谱数据：BYB3NWH20120501";
	m_ComFromInstruction=L"";
	m_ManagerInstruction=L"";
	UpdateData(FALSE);
}


void CCompleteSpecInfoAddToLibDlg::OnEnSetfocusEditComfrom()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	m_WineNameInstruction=L"";
	m_AlcoholContentInstruction=L"";
	m_FlavourInstruction=L"";
	m_BrandInstruction=L"";
	m_ProductionDateInstruction=L"";
	m_BatchNoInstruction=L"";
	m_SpecIDTitleInstruction=L"";
	m_ComFromInstruction=L"用简短的文字说明白酒来源信息，主要由以下部分组成：采集人员或单位，采集时间，采集方式";
	m_ManagerInstruction=L"";
	UpdateData(FALSE);
}


void CCompleteSpecInfoAddToLibDlg::OnEnSetfocusEditManager()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	m_WineNameInstruction=L"";
	m_AlcoholContentInstruction=L"";
	m_FlavourInstruction=L"";
	m_BrandInstruction=L"";
	m_ProductionDateInstruction=L"";
	m_BatchNoInstruction=L"";
	m_SpecIDTitleInstruction=L"";
	m_ComFromInstruction=L"";
	m_ManagerInstruction=L"请当前管理员如实填写自己的姓名";
	UpdateData(FALSE);
}


BOOL CCompleteSpecInfoAddToLibDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CCompleteSpecInfoAddToLibDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	if(m_WineName.IsEmpty())
	{
		MessageBox(L"白酒名称不能为空！",L"光谱入库",MB_ICONWARNING);
		return ;
	}
	if(m_AlcoholContent.IsEmpty())
	{
		MessageBox(L"白酒名称不能为空！",L"光谱入库",MB_ICONWARNING);
		return ;
	}
	if(m_Flavour.IsEmpty())
	{
		MessageBox(L"白酒名称不能为空！",L"光谱入库",MB_ICONWARNING);
		return ;
	}
	if(m_Brand.IsEmpty())
	{
		MessageBox(L"白酒名称不能为空！",L"光谱入库",MB_ICONWARNING);
		return ;
	}
	if(m_ProductionDate.IsEmpty())
	{
		MessageBox(L"白酒生产日期不能为空！",L"光谱入库",MB_ICONWARNING);
		return ;
	}
	if(m_BatchNo.IsEmpty())
	{
		MessageBox(L"白酒批号不能为空！",L"光谱入库",MB_ICONWARNING);
		return ;
	}
	if(m_SpecIDTitle.IsEmpty())
	{
		MessageBox(L"白酒入库编号不能为空！",L"光谱入库",MB_ICONWARNING);
		return ;
	}
	if(m_Manager.IsEmpty())
	{
		MessageBox(L"操作人员不能为空！",L"光谱入库",MB_ICONWARNING);
		return ;
	}
	if(m_ComFrom.IsEmpty())
	{
		MessageBox(L"白酒来源信息不能为空！",L"光谱入库",MB_ICONWARNING);
		return ;
	}
	CDialogEx::OnOK();
}
