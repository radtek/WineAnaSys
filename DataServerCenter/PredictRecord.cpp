// PredictRecord.h : CPredictRecord ���ʵ��



// CPredictRecord ʵ��

// ���������� 2014��4��9��, 20:10

#include "stdafx.h"
#include "PredictRecord.h"
IMPLEMENT_DYNAMIC(CPredictRecord, CRecordset)

CPredictRecord::CPredictRecord(CDatabase* pdb)
	: CRecordset(pdb)
{
	m_ProjectName = L"";
	m_SpecNum = L"";
	m_RecvDate = L"";
	m_PredictResult = L"";
	m_Reliablility = L"";
	m_PredictDate = L"";
	m_SendBackDate = L"";
	m_nFields = 7;
	m_nDefaultType = dynaset;
}
//#error ��ȫ���⣺�����ַ������ܰ������롣
// �������ַ����п��ܰ������������/��������Ҫ
// ��Ϣ�����ڲ鿴��������ַ������ҵ������밲ȫ
// �йص�������Ƴ� #error��������Ҫ���������
// ��Ϊ������ʽ��ʹ���������û������֤��
CString CPredictRecord::GetDefaultConnect()
{
	return _T("DSN=\x767d\x9152\x9274\x5b9a\x4e0e\x6eaf\x6e90\x6570\x636e\x5e93;Trusted_Connection=Yes;APP=Microsoft\x00ae Visual Studio\x00ae 2010;WSID=WTH515812076-PC;DATABASE=\x767d\x9152\x9274\x5b9a\x4e0e\x6eaf\x6e90\x6570\x636e\x5e93");
}

CString CPredictRecord::GetDefaultSQL()
{
	return _T("[dbo].[PredictRecord]");
}

void CPredictRecord::DoFieldExchange(CFieldExchange* pFX)
{
	pFX->SetFieldType(CFieldExchange::outputColumn);
// RFX_Text() �� RFX_Int() �������������
// ��Ա���������ͣ����������ݿ��ֶε����͡�
// ODBC �����Զ�����ֵת��Ϊ�����������
	RFX_Text(pFX, _T("[ProjectName]"), m_ProjectName);
	RFX_Text(pFX, _T("[SpecNum]"), m_SpecNum);
	RFX_Text(pFX, _T("[RecvDate]"), m_RecvDate);
	RFX_Text(pFX, _T("[PredictResult]"), m_PredictResult);
	RFX_Text(pFX, _T("[Reliablility]"), m_Reliablility);
	RFX_Text(pFX, _T("[PredictDate]"), m_PredictDate);
	RFX_Text(pFX, _T("[SendBackDate]"), m_SendBackDate);

}
/////////////////////////////////////////////////////////////////////////////
// CPredictRecord ���

#ifdef _DEBUG
void CPredictRecord::AssertValid() const
{
	CRecordset::AssertValid();
}

void CPredictRecord::Dump(CDumpContext& dc) const
{
	CRecordset::Dump(dc);
}
#endif //_DEBUG


