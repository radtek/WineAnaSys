// ComponentPredictModel.h : CComponentPredictModel ���ʵ��



// CComponentPredictModel ʵ��

// ���������� 2014��11��20��, 21:23

#include "stdafx.h"
#include "ComponentPredictModel.h"
IMPLEMENT_DYNAMIC(CComponentPredictModel, CRecordset)

CComponentPredictModel::CComponentPredictModel(CDatabase* pdb)
	: CRecordset(pdb)
{
	m_Component = L"";
	m_Weight = L"";
	m_Offset = 0.0;
	m_nFields = 3;
	m_nDefaultType = dynaset;
}
//#error ��ȫ���⣺�����ַ������ܰ������롣
// �������ַ����п��ܰ������������/��������Ҫ
// ��Ϣ�����ڲ鿴��������ַ������ҵ������밲ȫ
// �йص�������Ƴ� #error��������Ҫ���������
// ��Ϊ������ʽ��ʹ���������û������֤��
CString CComponentPredictModel::GetDefaultConnect()
{
	return _T("DSN=\x767d\x9152\x9274\x5b9a\x4e0e\x6eaf\x6e90\x6570\x636e\x5e93;Trusted_Connection=Yes;APP=Microsoft\x00ae Visual Studio\x00ae 2010;WSID=WTH515812076-PC;DATABASE=\x767d\x9152\x9274\x5b9a\x4e0e\x6eaf\x6e90\x6570\x636e\x5e93");
}

CString CComponentPredictModel::GetDefaultSQL()
{
	return _T("[dbo].[ComponentPredictModel]");
}

void CComponentPredictModel::DoFieldExchange(CFieldExchange* pFX)
{
	pFX->SetFieldType(CFieldExchange::outputColumn);
// RFX_Text() �� RFX_Int() �������������
// ��Ա���������ͣ����������ݿ��ֶε����͡�
// ODBC �����Զ�����ֵת��Ϊ�����������
	RFX_Text(pFX, _T("[Component]"), m_Component);
	RFX_Text(pFX, _T("[Weight]"), m_Weight,1000000);
	RFX_Single(pFX, _T("[Offset]"), m_Offset);

}
/////////////////////////////////////////////////////////////////////////////
// CComponentPredictModel ���

#ifdef _DEBUG
void CComponentPredictModel::AssertValid() const
{
	CRecordset::AssertValid();
}

void CComponentPredictModel::Dump(CDumpContext& dc) const
{
	CRecordset::Dump(dc);
}
#endif //_DEBUG


