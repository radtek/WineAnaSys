// PLSModel.h : CPLSModel ���ʵ��



// CPLSModel ʵ��

// ���������� 2014��1��15��, 16:14

#include "stdafx.h"
#include "PLSModel.h"
IMPLEMENT_DYNAMIC(CPLSModel, CRecordset)

CPLSModel::CPLSModel(CDatabase* pdb)
	: CRecordset(pdb)
{
	m_AlcoholContent = L"";
	m_Flavour = L"";
	m_Brand = L"";
	m_FactorNum = 0;
	m_ClassCount = 0;
	m_Weight = L"";
	m_Labels = L"";
	m_Num_EachLabel = L"";
	m_StandardPC = L"";
	m_TrainsetPC = L"";
	m_TrainsetMean = L"";
	m_Threshold = L"";
	m_nFields = 12;
	m_nDefaultType = dynaset;
}
//#error ��ȫ���⣺�����ַ������ܰ������롣
// �������ַ����п��ܰ������������/��������Ҫ
// ��Ϣ�����ڲ鿴��������ַ������ҵ������밲ȫ
// �йص�������Ƴ� #error��������Ҫ���������
// ��Ϊ������ʽ��ʹ���������û������֤��
CString CPLSModel::GetDefaultConnect()
{
	return _T("DSN=\x767d\x9152\x9274\x5b9a\x4e0e\x6eaf\x6e90\x6570\x636e\x5e93;Trusted_Connection=Yes;APP=Microsoft\x00ae Visual Studio\x00ae 2010;WSID=WTH515812076-PC;DATABASE=\x767d\x9152\x9274\x5b9a\x4e0e\x6eaf\x6e90\x6570\x636e\x5e93");
}

CString CPLSModel::GetDefaultSQL()
{
	return _T("[dbo].[PLSModel]");
}

void CPLSModel::DoFieldExchange(CFieldExchange* pFX)
{
	pFX->SetFieldType(CFieldExchange::outputColumn);
// RFX_Text() �� RFX_Int() �������������
// ��Ա���������ͣ����������ݿ��ֶε����͡�
// ODBC �����Զ�����ֵת��Ϊ�����������
	RFX_Text(pFX, _T("[AlcoholContent]"), m_AlcoholContent);
	RFX_Text(pFX, _T("[Flavour]"), m_Flavour);
	RFX_Text(pFX, _T("[Brand]"), m_Brand);
	RFX_Long(pFX, _T("[FactorNum]"), m_FactorNum);
	RFX_Long(pFX, _T("[ClassCount]"), m_ClassCount);
	RFX_Text(pFX, _T("[Weight]"), m_Weight,1000000);
	RFX_Text(pFX, _T("[Labels]"), m_Labels,1000000);
	RFX_Text(pFX, _T("[Num_EachLabel]"), m_Num_EachLabel,1000000);
	RFX_Text(pFX, _T("[StandardPC]"), m_StandardPC,1000000);
	RFX_Text(pFX, _T("[TrainsetPC]"), m_TrainsetPC,100000000);
	RFX_Text(pFX, _T("[TrainsetMean]"), m_TrainsetMean,1000000);
	RFX_Text(pFX, _T("[Threshold]"), m_Threshold,1000000);

}
/////////////////////////////////////////////////////////////////////////////
// CPLSModel ���

#ifdef _DEBUG
void CPLSModel::AssertValid() const
{
	CRecordset::AssertValid();
}

void CPLSModel::Dump(CDumpContext& dc) const
{
	CRecordset::Dump(dc);
}
#endif //_DEBUG


