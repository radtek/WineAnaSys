// AnalasisHistory.h : CAnalasisHistory ���ʵ��



// CAnalasisHistory ʵ��

// ���������� 2013��12��30��, 16:22

#include "stdafx.h"
#include "AnalasisHistory.h"
IMPLEMENT_DYNAMIC(CAnalasisHistory, CRecordset)

CAnalasisHistory::CAnalasisHistory(CDatabase* pdb)
	: CRecordset(pdb)
{
	m_AnalasisType = L"";
	m_ComFrom = L"";
	m_ProjectName = L"";
	m_AnalasisResult = L"";
	m_AnalasisDate = L"";
	m_nFields = 5;
	m_nDefaultType = dynaset;
}
//#error ��ȫ���⣺�����ַ������ܰ������롣
// �������ַ����п��ܰ������������/��������Ҫ
// ��Ϣ�����ڲ鿴��������ַ������ҵ������밲ȫ
// �йص�������Ƴ� #error��������Ҫ���������
// ��Ϊ������ʽ��ʹ���������û������֤��
CString CAnalasisHistory::GetDefaultConnect()
{
	return _T("DSN=\x767d\x9152\x9274\x5b9a\x4e0e\x6eaf\x6e90\x6570\x636e\x5e93;Trusted_Connection=Yes;APP=Microsoft\x00ae Visual Studio\x00ae 2010;WSID=WTH515812076-PC;DATABASE=\x767d\x9152\x9274\x5b9a\x4e0e\x6eaf\x6e90\x6570\x636e\x5e93");
}

CString CAnalasisHistory::GetDefaultSQL()
{
	return _T("[dbo].[AnalasisHistory]");
}

void CAnalasisHistory::DoFieldExchange(CFieldExchange* pFX)
{
	pFX->SetFieldType(CFieldExchange::outputColumn);
// RFX_Text() �� RFX_Int() �������������
// ��Ա���������ͣ����������ݿ��ֶε����͡�
// ODBC �����Զ�����ֵת��Ϊ�����������
	RFX_Text(pFX, _T("[AnalasisType]"), m_AnalasisType);
	RFX_Text(pFX, _T("[ComFrom]"), m_ComFrom);
	RFX_Text(pFX, _T("[ProjectName]"), m_ProjectName);
	RFX_Text(pFX, _T("[AnalasisResult]"), m_AnalasisResult);
	RFX_Text(pFX, _T("[AnalasisDate]"), m_AnalasisDate);

}
/////////////////////////////////////////////////////////////////////////////
// CAnalasisHistory ���

#ifdef _DEBUG
void CAnalasisHistory::AssertValid() const
{
	CRecordset::AssertValid();
}

void CAnalasisHistory::Dump(CDumpContext& dc) const
{
	CRecordset::Dump(dc);
}
#endif //_DEBUG


