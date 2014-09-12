// FakeWineLib.h : CFakeWineLib ���ʵ��



// CFakeWineLib ʵ��

// ���������� 2014��6��6��, 16:31

#include "stdafx.h"
#include "FakeWineLib.h"
IMPLEMENT_DYNAMIC(CFakeWineLib, CRecordset)

CFakeWineLib::CFakeWineLib(CDatabase* pdb)
	: CRecordset(pdb)
{
	m_SpectrumID = L"";
	m_SpectrumData = L"";
	m_WineNameFaked = L"";
	m_Comfrom = L"";
	m_ImportDate = L"";
	m_Manager = L"";
	m_WaveNumber = L"";
	m_nFields = 7;
	m_nDefaultType = dynaset;
}
//#error ��ȫ���⣺�����ַ������ܰ������롣
// �������ַ����п��ܰ������������/��������Ҫ
// ��Ϣ�����ڲ鿴��������ַ������ҵ������밲ȫ
// �йص�������Ƴ� #error��������Ҫ���������
// ��Ϊ������ʽ��ʹ���������û������֤��
CString CFakeWineLib::GetDefaultConnect()
{
	return _T("DSN=\x767d\x9152\x9274\x5b9a\x4e0e\x6eaf\x6e90\x6570\x636e\x5e93;Trusted_Connection=Yes;APP=Microsoft\x00ae Visual Studio\x00ae 2010;WSID=WTH515812076-PC;DATABASE=\x767d\x9152\x9274\x5b9a\x4e0e\x6eaf\x6e90\x6570\x636e\x5e93");
}

CString CFakeWineLib::GetDefaultSQL()
{
	return _T("[dbo].[FakeWineLib]");
}

void CFakeWineLib::DoFieldExchange(CFieldExchange* pFX)
{
	pFX->SetFieldType(CFieldExchange::outputColumn);
// RFX_Text() �� RFX_Int() �������������
// ��Ա���������ͣ����������ݿ��ֶε����͡�
// ODBC �����Զ�����ֵת��Ϊ�����������
	RFX_Text(pFX, _T("[SpectrumID]"), m_SpectrumID);
	RFX_Text(pFX, _T("[SpectrumData]"), m_SpectrumData,20000);
	RFX_Text(pFX, _T("[WineNameFaked]"), m_WineNameFaked);
	RFX_Text(pFX, _T("[Comfrom]"), m_Comfrom);
	RFX_Text(pFX, _T("[ImportDate]"), m_ImportDate);
	RFX_Text(pFX, _T("[Manager]"), m_Manager);
	RFX_Text(pFX, _T("[WaveNumber]"), m_WaveNumber,20000);

}
/////////////////////////////////////////////////////////////////////////////
// CFakeWineLib ���

#ifdef _DEBUG
void CFakeWineLib::AssertValid() const
{
	CRecordset::AssertValid();
}

void CFakeWineLib::Dump(CDumpContext& dc) const
{
	CRecordset::Dump(dc);
}
#endif //_DEBUG


