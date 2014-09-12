// FlavourTypeIndex.h : CFlavourTypeIndex ���ʵ��



// CFlavourTypeIndex ʵ��

// ���������� 2013��12��30��, 16:23

#include "stdafx.h"
#include "FlavourTypeIndex.h"
IMPLEMENT_DYNAMIC(CFlavourTypeIndex, CRecordset)

CFlavourTypeIndex::CFlavourTypeIndex(CDatabase* pdb)
	: CRecordset(pdb)
{
	m_Flavour = L"";
	m_FlavourIndex = 0;
	m_nFields = 2;
	m_nDefaultType = dynaset;
}
//#error ��ȫ���⣺�����ַ������ܰ������롣
// �������ַ����п��ܰ������������/��������Ҫ
// ��Ϣ�����ڲ鿴��������ַ������ҵ������밲ȫ
// �йص�������Ƴ� #error��������Ҫ���������
// ��Ϊ������ʽ��ʹ���������û������֤��
CString CFlavourTypeIndex::GetDefaultConnect()
{
	return _T("DSN=\x767d\x9152\x9274\x5b9a\x4e0e\x6eaf\x6e90\x6570\x636e\x5e93;Trusted_Connection=Yes;APP=Microsoft\x00ae Visual Studio\x00ae 2010;WSID=WTH515812076-PC;DATABASE=\x767d\x9152\x9274\x5b9a\x4e0e\x6eaf\x6e90\x6570\x636e\x5e93");
}

CString CFlavourTypeIndex::GetDefaultSQL()
{
	return _T("[dbo].[FlavourTypeIndex]");
}

void CFlavourTypeIndex::DoFieldExchange(CFieldExchange* pFX)
{
	pFX->SetFieldType(CFieldExchange::outputColumn);
// RFX_Text() �� RFX_Int() �������������
// ��Ա���������ͣ����������ݿ��ֶε����͡�
// ODBC �����Զ�����ֵת��Ϊ�����������
	RFX_Text(pFX, _T("[Flavour]"), m_Flavour);
	RFX_Long(pFX, _T("[FlavourIndex]"), m_FlavourIndex);

}
/////////////////////////////////////////////////////////////////////////////
// CFlavourTypeIndex ���

#ifdef _DEBUG
void CFlavourTypeIndex::AssertValid() const
{
	CRecordset::AssertValid();
}

void CFlavourTypeIndex::Dump(CDumpContext& dc) const
{
	CRecordset::Dump(dc);
}
#endif //_DEBUG


