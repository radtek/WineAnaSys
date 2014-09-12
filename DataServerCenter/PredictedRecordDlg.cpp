// PredictedRecordDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "DataServerCenter.h"
#include "PredictedRecordDlg.h"
#include "afxdialogex.h"
#include "PredictRecord.h"

// CPredictedRecordDlg �Ի���

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


// CPredictedRecordDlg ��Ϣ�������


BOOL CPredictedRecordDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	m_ListPredictedRecord.InsertColumn(0,L"", LVCFMT_LEFT, 50 );
	m_ListPredictedRecord.InsertColumn(1,L"��Ŀ����", LVCFMT_LEFT, 150 );
	m_ListPredictedRecord.InsertColumn(2,L"������", LVCFMT_LEFT, 150 );
	m_ListPredictedRecord.InsertColumn(3,L"��������", LVCFMT_LEFT, 150 );
	m_ListPredictedRecord.InsertColumn(4,L"�����", LVCFMT_LEFT, 150 );
	m_ListPredictedRecord.InsertColumn(5,L"������ɿ���", LVCFMT_LEFT, 150 );
	m_ListPredictedRecord.InsertColumn(6,L"�������", LVCFMT_LEFT, 150 );
	m_ListPredictedRecord.InsertColumn(7,L"�����������", LVCFMT_LEFT, 150 );

	DWORD dwStyle = m_ListPredictedRecord.GetExtendedStyle(); //��ȡ��ǰ��չ��ʽ
	dwStyle |= LVS_EX_FULLROWSELECT; //ѡ��ĳ��ʹ���и�����report���ʱ��
	dwStyle |= LVS_EX_GRIDLINES; //�����ߣ�report���ʱ��
	//dwStyle |= LVS_EX_FULLROWSELECT; //
	m_ListPredictedRecord.SetExtendedStyle(dwStyle); //������չ���

	CPredictRecord PredictRecord;
	if(!PredictRecord.Open())
	{
		MessageBox(L"�����ݿ�ʧ�ܣ�",L"���ݲ�ѯ",MB_ICONWARNING);
		return 0;
	}
	if(PredictRecord.IsEOF())
	{
		MessageBox(L"��¼Ϊ�գ�",L"���ݲ�ѯ",MB_ICONWARNING);
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
	// �쳣: OCX ����ҳӦ���� FALSE
}
