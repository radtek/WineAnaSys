// PredictedRecordDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "DataServerCenter.h"
#include "PredictedRecordDlg.h"
#include "afxdialogex.h"
#include "PredictRecord.h"

// CPredictedRecordDlg 对话框

IMPLEMENT_DYNAMIC(CPredictedRecordDlg, CDialogEx)

CPredictedRecordDlg::CPredictedRecordDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CPredictedRecordDlg::IDD, pParent)
{

}

CPredictedRecordDlg::~CPredictedRecordDlg()
{
}

void CPredictedRecordDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_PREDICTEDRECORD, m_ListPredictedRecord);
}


BEGIN_MESSAGE_MAP(CPredictedRecordDlg, CDialogEx)
END_MESSAGE_MAP()


// CPredictedRecordDlg 消息处理程序


BOOL CPredictedRecordDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_ListPredictedRecord.InsertColumn(0,L"", LVCFMT_LEFT, 50 );
	m_ListPredictedRecord.InsertColumn(1,L"项目名称", LVCFMT_LEFT, 150 );
	m_ListPredictedRecord.InsertColumn(2,L"光谱数", LVCFMT_LEFT, 150 );
	m_ListPredictedRecord.InsertColumn(3,L"接收日期", LVCFMT_LEFT, 150 );
	m_ListPredictedRecord.InsertColumn(4,L"检测结果", LVCFMT_LEFT, 150 );
	m_ListPredictedRecord.InsertColumn(5,L"检测结果可靠度", LVCFMT_LEFT, 150 );
	m_ListPredictedRecord.InsertColumn(6,L"检测日期", LVCFMT_LEFT, 150 );
	m_ListPredictedRecord.InsertColumn(7,L"结果回送日期", LVCFMT_LEFT, 150 );

	DWORD dwStyle = m_ListPredictedRecord.GetExtendedStyle(); //获取当前扩展样式
	dwStyle |= LVS_EX_FULLROWSELECT; //选中某行使整行高亮（report风格时）
	dwStyle |= LVS_EX_GRIDLINES; //网格线（report风格时）
	//dwStyle |= LVS_EX_FULLROWSELECT; //
	m_ListPredictedRecord.SetExtendedStyle(dwStyle); //设置扩展风格

	CPredictRecord PredictRecord;
	if(!PredictRecord.Open())
	{
		MessageBox(L"打开数据库失败！",L"数据查询",MB_ICONWARNING);
		return 0;
	}
	if(PredictRecord.IsEOF())
	{
		MessageBox(L"记录为空！",L"数据查询",MB_ICONWARNING);
		return 0;
	}
	else
	{
		PredictRecord.MoveFirst();
		int i=0;
		while(!PredictRecord.IsEOF())
		{
			CString num;
			num.Format(L"%d",i+1);
			m_ListPredictedRecord.InsertItem(i,num);
			m_ListPredictedRecord.SetItemText(i,1,PredictRecord.m_ProjectName);
			m_ListPredictedRecord.SetItemText(i,2,PredictRecord.m_SpecNum);
			m_ListPredictedRecord.SetItemText(i,3,PredictRecord.m_RecvDate);
			m_ListPredictedRecord.SetItemText(i,4,PredictRecord.m_PredictResult);
			m_ListPredictedRecord.SetItemText(i,5,PredictRecord.m_Reliablility);
			m_ListPredictedRecord.SetItemText(i,6,PredictRecord.m_SendBackDate);
			PredictRecord.MoveNext();
			i++;
		}
	}


	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}
