// WineTypeIndex.h : CWineTypeIndex ���ʵ��



// CWineTypeIndex ʵ��

// ���������� 2014��2��21��, 16:43

#include "stdafx.h"
#include "WineTypeIndex.h"
IMPLEMENT_DYNAMIC(CWineTypeIndex, CRecordset)

CWineTypeIndex::CWineTypeIndex(CDatabase* pdb)
	: CRecordset(pdb)
{
	m_WineName = L"";
	m_AlcoholContent = L"";
	m_Brand = L"";
	m_Flavour = L"";
	m_ProductionBatchNo = L"";
	m_TypeIndex = 0;
	m_nFields = 6;
	m_nDefaultType = dynaset;
}
//#error ��ȫ���⣺�����ַ������ܰ������롣
// �������ַ����п��ܰ������������/��������Ҫ
// ��Ϣ�����ڲ鿴��������ַ������ҵ������밲ȫ
// �йص�������Ƴ� #error��������Ҫ���������
// ��Ϊ������ʽ��ʹ���������û������֤��
CString CWineTypeIndex::GetDefaultConnect()
{
	return _T("DSN=\x767d\x9152\x9274\x5b9a\x4e0e\x6eaf\x6e90\x6570\x636e\x5e93;Trusted_Connection=Yes;APP=Microsoft\x00ae Visual Studio\x00ae 2010;Source = 122.205.14.117;WSID=WTH515812076-PC;DATABASE=\x767d\x9152\x9274\x5b9a\x4e0e\x6eaf\x6e90\x6570\x636e\x5e93");
}

CString CWineTypeIndex::GetDefaultSQL()
{
	return _T("[dbo].[WineTypeIndex]");
}

void CWineTypeIndex::DoFieldExchange(CFieldExchange* pFX)
{
	pFX->SetFieldType(CFieldExchange::outputColumn);
// RFX_Text() �� RFX_Int() �������������
// ��Ա���������ͣ����������ݿ��ֶε����͡�
// ODBC �����Զ�����ֵת��Ϊ�����������
	RFX_Text(pFX, _T("[WineName]"), m_WineName);
	RFX_Text(pFX, _T("[AlcoholContent]"), m_AlcoholContent);
	RFX_Text(pFX, _T("[Brand]"), m_Brand);
	RFX_Text(pFX, _T("[Flavour]"), m_Flavour);
	RFX_Text(pFX, _T("[ProductionBatchNo]"), m_ProductionBatchNo);
	RFX_Long(pFX, _T("[TypeIndex]"), m_TypeIndex);

}
/////////////////////////////////////////////////////////////////////////////
// CWineTypeIndex ���

#ifdef _DEBUG
void CWineTypeIndex::AssertValid() const
{
	CRecordset::AssertValid();
}

void CWineTypeIndex::Dump(CDumpContext& dc) const
{
	CRecordset::Dump(dc);
}
#endif //_DEBUG


