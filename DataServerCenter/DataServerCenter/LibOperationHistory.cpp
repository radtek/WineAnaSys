// LibOperationHistory.h : CLibOperationHistory ���ʵ��



// CLibOperationHistory ʵ��

// ���������� 2013��12��30��, 16:19

#include "stdafx.h"
#include "LibOperationHistory.h"
IMPLEMENT_DYNAMIC(CLibOperationHistory, CRecordset)

CLibOperationHistory::CLibOperationHistory(CDatabase* pdb)
	: CRecordset(pdb)
{
	m_OperationType = L"";
	m_Lib = L"";
	m_IsModelUpdate = L"";
	m_OperationDate = L"";
	m_WineName = L"";
	m_ProductionBatchNo = L"";
	m_ComFrom = L"";
	m_nFields = 7;
	m_nDefaultType = dynaset;
}
//#error ��ȫ���⣺�����ַ������ܰ������롣
// �������ַ����п��ܰ������������/��������Ҫ
// ��Ϣ�����ڲ鿴��������ַ������ҵ������밲ȫ
// �йص�������Ƴ� #error��������Ҫ���������
// ��Ϊ������ʽ��ʹ���������û������֤��
CString CLibOperationHistory::GetDefaultConnect()
{
	return _T("DSN=\x767d\x9152\x9274\x5b9a\x4e0e\x6eaf\x6e90\x6570\x636e\x5e93;Trusted_Connection=Yes;APP=Microsoft\x00ae Visual Studio\x00ae 2010;WSID=WTH515812076-PC;DATABASE=\x767d\x9152\x9274\x5b9a\x4e0e\x6eaf\x6e90\x6570\x636e\x5e93");
}

CString CLibOperationHistory::GetDefaultSQL()
{
	return _T("[dbo].[LibOperationHistory]");
}

void CLibOperationHistory::DoFieldExchange(CFieldExchange* pFX)
{
	pFX->SetFieldType(CFieldExchange::outputColumn);
// RFX_Text() �� RFX_Int() �������������
// ��Ա���������ͣ����������ݿ��ֶε����͡�
// ODBC �����Զ�����ֵת��Ϊ�����������
	RFX_Text(pFX, _T("[OperationType]"), m_OperationType);
	RFX_Text(pFX, _T("[Lib]"), m_Lib);
	RFX_Text(pFX, _T("[IsModelUpdate]"), m_IsModelUpdate);
	RFX_Text(pFX, _T("[OperationDate]"), m_OperationDate);
	RFX_Text(pFX, _T("[WineName]"), m_WineName);
	RFX_Text(pFX, _T("[ProductionBatchNo]"), m_ProductionBatchNo);
	RFX_Text(pFX, _T("[ComFrom]"), m_ComFrom);

}
/////////////////////////////////////////////////////////////////////////////
// CLibOperationHistory ���

#ifdef _DEBUG
void CLibOperationHistory::AssertValid() const
{
	CRecordset::AssertValid();
}

void CLibOperationHistory::Dump(CDumpContext& dc) const
{
	CRecordset::Dump(dc);
}
#endif //_DEBUG


