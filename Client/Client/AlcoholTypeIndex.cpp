// AlcoholTypeIndex.h : CAlcoholTypeIndex ���ʵ��



// CAlcoholTypeIndex ʵ��

// ���������� 2014��2��21��, 16:41

#include "stdafx.h"
#include "AlcoholTypeIndex.h"
IMPLEMENT_DYNAMIC(CAlcoholTypeIndex, CRecordset)

CAlcoholTypeIndex::CAlcoholTypeIndex(CDatabase* pdb)
	: CRecordset(pdb)
{
	m_AlcoholContent = L"";
	m_AlcoholIndex = 0;
	m_nFields = 2;
	m_nDefaultType = dynaset;
}
//#error ��ȫ���⣺�����ַ������ܰ������롣
// �������ַ����п��ܰ������������/��������Ҫ
// ��Ϣ�����ڲ鿴��������ַ������ҵ������밲ȫ
// �йص�������Ƴ� #error��������Ҫ���������
// ��Ϊ������ʽ��ʹ���������û������֤��
CString CAlcoholTypeIndex::GetDefaultConnect()
{
	return _T("DSN=\x767d\x9152\x9274\x5b9a\x4e0e\x6eaf\x6e90\x6570\x636e\x5e93;Trusted_Connection=Yes;APP=Microsoft\x00ae Visual Studio\x00ae 2010;Source = 122.205.14.95;WSID=WTH515812076-PC;DATABASE=\x767d\x9152\x9274\x5b9a\x4e0e\x6eaf\x6e90\x6570\x636e\x5e93");
}

CString CAlcoholTypeIndex::GetDefaultSQL()
{
	return _T("[dbo].[AlcoholTypeIndex]");
}

void CAlcoholTypeIndex::DoFieldExchange(CFieldExchange* pFX)
{
	pFX->SetFieldType(CFieldExchange::outputColumn);
// RFX_Text() �� RFX_Int() �������������
// ��Ա���������ͣ����������ݿ��ֶε����͡�
// ODBC �����Զ�����ֵת��Ϊ�����������
	RFX_Text(pFX, _T("[AlcoholContent]"), m_AlcoholContent);
	RFX_Long(pFX, _T("[AlcoholIndex]"), m_AlcoholIndex);

}
/////////////////////////////////////////////////////////////////////////////
// CAlcoholTypeIndex ���

#ifdef _DEBUG
void CAlcoholTypeIndex::AssertValid() const
{
	CRecordset::AssertValid();
}

void CAlcoholTypeIndex::Dump(CDumpContext& dc) const
{
	CRecordset::Dump(dc);
}
#endif //_DEBUG


