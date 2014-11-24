// TrueWineLib.h : CTrueWineLib ���ʵ��



// CTrueWineLib ʵ��

// ���������� 2014��6��6��, 15:17

#include "stdafx.h"
#include "TrueWineLib.h"
IMPLEMENT_DYNAMIC(CTrueWineLib, CRecordset)

CTrueWineLib::CTrueWineLib(CDatabase* pdb)
	: CRecordset(pdb)
{
	m_WineName = L"";
	m_AlcoholContent = L"";
	m_Flavour = L"";
	m_Brand = L"";
	m_ProductionBatchNo = L"";
	m_ProductionDate = L"";
	m_SpectrumData = L"";
	m_ComFrom = L"";
	m_ImportDate = L"";
	m_SpecID = L"";
	m_Manager = L"";
	m_WaveNumber = L"";
	m_nFields = 12;
	m_nDefaultType = dynaset;
}
//#error ��ȫ���⣺�����ַ������ܰ������롣
// �������ַ����п��ܰ������������/��������Ҫ
// ��Ϣ�����ڲ鿴��������ַ������ҵ������밲ȫ
// �йص�������Ƴ� #error��������Ҫ���������
// ��Ϊ������ʽ��ʹ���������û������֤��
CString CTrueWineLib::GetDefaultConnect()
{
	return _T("DSN=\x767d\x9152\x9274\x5b9a\x4e0e\x6eaf\x6e90\x6570\x636e\x5e93;Trusted_Connection=Yes;APP=Microsoft\x00ae Visual Studio\x00ae 2010;WSID=WTH515812076-PC;DATABASE=\x767d\x9152\x9274\x5b9a\x4e0e\x6eaf\x6e90\x6570\x636e\x5e93");
}

CString CTrueWineLib::GetDefaultSQL()
{
	return _T("[dbo].[TrueWineLib]");
}

void CTrueWineLib::DoFieldExchange(CFieldExchange* pFX)
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
	RFX_Text(pFX, _T("[ProductionDate]"), m_ProductionDate);
	RFX_Text(pFX, _T("[SpectrumData]"), m_SpectrumData,20000);
	RFX_Text(pFX, _T("[ComFrom]"), m_ComFrom);
	RFX_Text(pFX, _T("[ImportDate]"), m_ImportDate);
	RFX_Text(pFX, _T("[SpecID]"), m_SpecID);
	RFX_Text(pFX, _T("[Manager]"), m_Manager);
	RFX_Text(pFX, _T("[WaveNumber]"), m_WaveNumber,20000);

}
/////////////////////////////////////////////////////////////////////////////
// CTrueWineLib ���

#ifdef _DEBUG
void CTrueWineLib::AssertValid() const
{
	CRecordset::AssertValid();
}

void CTrueWineLib::Dump(CDumpContext& dc) const
{
	CRecordset::Dump(dc);
}
#endif //_DEBUG


