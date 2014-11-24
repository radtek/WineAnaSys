// BrandTypeIndex.h : CBrandTypeIndex ���ʵ��



// CBrandTypeIndex ʵ��

// ���������� 2014��2��21��, 16:42

#include "stdafx.h"
#include "BrandTypeIndex.h"
IMPLEMENT_DYNAMIC(CBrandTypeIndex, CRecordset)

CBrandTypeIndex::CBrandTypeIndex(CDatabase* pdb)
	: CRecordset(pdb)
{
	m_Brand = L"";
	m_BrandIndex = 0;
	m_nFields = 2;
	m_nDefaultType = dynaset;
}
//#error ��ȫ���⣺�����ַ������ܰ������롣
// �������ַ����п��ܰ������������/��������Ҫ
// ��Ϣ�����ڲ鿴��������ַ������ҵ������밲ȫ
// �йص�������Ƴ� #error��������Ҫ���������
// ��Ϊ������ʽ��ʹ���������û������֤��
CString CBrandTypeIndex::GetDefaultConnect()
{
	return _T("DSN=\x767d\x9152\x9274\x5b9a\x4e0e\x6eaf\x6e90\x6570\x636e\x5e93;Trusted_Connection=Yes;APP=Microsoft\x00ae Visual Studio\x00ae 2010;Source = 122.205.14.95;WSID=WTH515812076-PC;DATABASE=\x767d\x9152\x9274\x5b9a\x4e0e\x6eaf\x6e90\x6570\x636e\x5e93");
}

CString CBrandTypeIndex::GetDefaultSQL()
{
	return _T("[dbo].[BrandTypeIndex]");
}

void CBrandTypeIndex::DoFieldExchange(CFieldExchange* pFX)
{
	pFX->SetFieldType(CFieldExchange::outputColumn);
// RFX_Text() �� RFX_Int() �������������
// ��Ա���������ͣ����������ݿ��ֶε����͡�
// ODBC �����Զ�����ֵת��Ϊ�����������
	RFX_Text(pFX, _T("[Brand]"), m_Brand);
	RFX_Long(pFX, _T("[BrandIndex]"), m_BrandIndex);

}
/////////////////////////////////////////////////////////////////////////////
// CBrandTypeIndex ���

#ifdef _DEBUG
void CBrandTypeIndex::AssertValid() const
{
	CRecordset::AssertValid();
}

void CBrandTypeIndex::Dump(CDumpContext& dc) const
{
	CRecordset::Dump(dc);
}
#endif //_DEBUG


