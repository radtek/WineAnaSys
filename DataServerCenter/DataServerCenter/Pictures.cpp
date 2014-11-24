// Pictures.h : CPictures ���ʵ��



// CPictures ʵ��

// ���������� 2014��1��3��, 14:44

#include "stdafx.h"
#include "Pictures.h"
IMPLEMENT_DYNAMIC(CPictures, CRecordset)

CPictures::CPictures(CDatabase* pdb)
	: CRecordset(pdb)
{
	m_WineName = L"";
	m_ProductionBatchNo = L"";
	m_PicturesDir = L"";
	m_nFields = 3;
	m_nDefaultType = dynaset;
}
//#error ��ȫ���⣺�����ַ������ܰ������롣
// �������ַ����п��ܰ������������/��������Ҫ
// ��Ϣ�����ڲ鿴��������ַ������ҵ������밲ȫ
// �йص�������Ƴ� #error��������Ҫ���������
// ��Ϊ������ʽ��ʹ���������û������֤��
CString CPictures::GetDefaultConnect()
{
	return _T("DSN=\x767d\x9152\x9274\x5b9a\x4e0e\x6eaf\x6e90\x6570\x636e\x5e93;Trusted_Connection=Yes;APP=Microsoft\x00ae Visual Studio\x00ae 2010;WSID=WTH515812076-PC;DATABASE=\x767d\x9152\x9274\x5b9a\x4e0e\x6eaf\x6e90\x6570\x636e\x5e93");
}

CString CPictures::GetDefaultSQL()
{
	return _T("[dbo].[Pictures]");
}

void CPictures::DoFieldExchange(CFieldExchange* pFX)
{
	pFX->SetFieldType(CFieldExchange::outputColumn);
// RFX_Text() �� RFX_Int() �������������
// ��Ա���������ͣ����������ݿ��ֶε����͡�
// ODBC �����Զ�����ֵת��Ϊ�����������
	RFX_Text(pFX, _T("[WineName]"), m_WineName);
	RFX_Text(pFX, _T("[ProductionBatchNo]"), m_ProductionBatchNo);
	RFX_Text(pFX, _T("[PicturesDir]"), m_PicturesDir);

}
/////////////////////////////////////////////////////////////////////////////
// CPictures ���

#ifdef _DEBUG
void CPictures::AssertValid() const
{
	CRecordset::AssertValid();
}

void CPictures::Dump(CDumpContext& dc) const
{
	CRecordset::Dump(dc);
}
#endif //_DEBUG


