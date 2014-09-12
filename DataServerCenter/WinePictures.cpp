// WinePictures.h : CWinePictures ���ʵ��



// CWinePictures ʵ��

// ���������� 2014��5��19��, 16:31

#include "stdafx.h"
#include "WinePictures.h"
IMPLEMENT_DYNAMIC(CWinePictures, CRecordset)

CWinePictures::CWinePictures(CDatabase* pdb)
	: CRecordset(pdb)
{
	m_WineName = L"";
	m_AlcoholContent = L"";
	m_Flavour = L"";
	m_Brand = L"";
	m_ProductionBatchNo = L"";
	m_PicturesDir = L"";
	m_ImportDate = L"";
	m_Manager = L"";
	m_SpecIDTitle = L"";
	m_nFields = 9;
	m_nDefaultType = dynaset;
}
//#error ��ȫ���⣺�����ַ������ܰ������롣
// �������ַ����п��ܰ������������/��������Ҫ
// ��Ϣ�����ڲ鿴��������ַ������ҵ������밲ȫ
// �йص�������Ƴ� #error��������Ҫ���������
// ��Ϊ������ʽ��ʹ���������û������֤��
CString CWinePictures::GetDefaultConnect()
{
	return _T("DSN=\x767d\x9152\x9274\x5b9a\x4e0e\x6eaf\x6e90\x6570\x636e\x5e93;Trusted_Connection=Yes;APP=Microsoft\x00ae Visual Studio\x00ae 2010;WSID=WTH515812076-PC;DATABASE=\x767d\x9152\x9274\x5b9a\x4e0e\x6eaf\x6e90\x6570\x636e\x5e93");
}

CString CWinePictures::GetDefaultSQL()
{
	return _T("[dbo].[WinePictures]");
}

void CWinePictures::DoFieldExchange(CFieldExchange* pFX)
{
	pFX->SetFieldType(CFieldExchange::outputColumn);
// RFX_Text() �� RFX_Int() �������������
// ��Ա���������ͣ����������ݿ��ֶε����͡�
// ODBC �����Զ�����ֵת��Ϊ�����������
	RFX_Text(pFX, _T("[WineName]"), m_WineName);
	RFX_Text(pFX, _T("[AlcoholContent]"), m_AlcoholContent);
	RFX_Text(pFX, _T("[Flavour]"), m_Flavour);
	RFX_Text(pFX, _T("[Brand]"), m_Brand);
	RFX_Text(pFX, _T("[ProductionBatchNo]"), m_ProductionBatchNo);
	RFX_Text(pFX, _T("[PicturesDir]"), m_PicturesDir);
	RFX_Text(pFX, _T("[ImportDate]"), m_ImportDate);
	RFX_Text(pFX, _T("[Manager]"), m_Manager);
	RFX_Text(pFX, _T("[SpecIDTitle]"), m_SpecIDTitle);

}
/////////////////////////////////////////////////////////////////////////////
// CWinePictures ���

#ifdef _DEBUG
void CWinePictures::AssertValid() const
{
	CRecordset::AssertValid();
}

void CWinePictures::Dump(CDumpContext& dc) const
{
	CRecordset::Dump(dc);
}
#endif //_DEBUG


