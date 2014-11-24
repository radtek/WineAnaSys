// ConcentrationTraceBackModel.h : CConcentrationTraceBackModel ���ʵ��



// CConcentrationTraceBackModel ʵ��

// ���������� 2014��11��12��, 22:03

#include "stdafx.h"
#include "ConcentrationTraceBackModel.h"
IMPLEMENT_DYNAMIC(CConcentrationTraceBackModel, CRecordset)

CConcentrationTraceBackModel::CConcentrationTraceBackModel(CDatabase* pdb)
	: CRecordset(pdb)
{
	m_TypeIndex = 0;
	m_StandardComponentConcentration = L"";
	m_Threshold = 0.0;
	m_nFields = 3;
	m_nDefaultType = dynaset;
}
//#error ��ȫ���⣺�����ַ������ܰ������롣
// �������ַ����п��ܰ������������/��������Ҫ
// ��Ϣ�����ڲ鿴��������ַ������ҵ������밲ȫ
// �йص�������Ƴ� #error��������Ҫ���������
// ��Ϊ������ʽ��ʹ���������û������֤��
CString CConcentrationTraceBackModel::GetDefaultConnect()
{
	return _T("DSN=\x767d\x9152\x9274\x5b9a\x4e0e\x6eaf\x6e90\x6570\x636e\x5e93;Trusted_Connection=Yes;APP=Microsoft\x00ae Visual Studio\x00ae 2010;WSID=WTH515812076-PC;DATABASE=\x767d\x9152\x9274\x5b9a\x4e0e\x6eaf\x6e90\x6570\x636e\x5e93");
}

CString CConcentrationTraceBackModel::GetDefaultSQL()
{
	return _T("[dbo].[ConcentrationTraceBackModel]");
}

void CConcentrationTraceBackModel::DoFieldExchange(CFieldExchange* pFX)
{
	pFX->SetFieldType(CFieldExchange::outputColumn);
// RFX_Text() �� RFX_Int() �������������
// ��Ա���������ͣ����������ݿ��ֶε����͡�
// ODBC �����Զ�����ֵת��Ϊ�����������
	RFX_Long(pFX, _T("[TypeIndex]"), m_TypeIndex);
	RFX_Text(pFX, _T("[StandardComponentConcentration]"), m_StandardComponentConcentration);
	RFX_Single(pFX, _T("[Threshold]"), m_Threshold);

}
/////////////////////////////////////////////////////////////////////////////
// CConcentrationTraceBackModel ���

#ifdef _DEBUG
void CConcentrationTraceBackModel::AssertValid() const
{
	CRecordset::AssertValid();
}

void CConcentrationTraceBackModel::Dump(CDumpContext& dc) const
{
	CRecordset::Dump(dc);
}
#endif //_DEBUG


