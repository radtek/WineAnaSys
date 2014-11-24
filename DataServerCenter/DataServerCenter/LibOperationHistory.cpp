// LibOperationHistory.h : CLibOperationHistory 类的实现



// CLibOperationHistory 实现

// 代码生成在 2013年12月30日, 16:19

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
//#error 安全问题：连接字符串可能包含密码。
// 此连接字符串中可能包含明文密码和/或其他重要
// 信息。请在查看完此连接字符串并找到所有与安全
// 有关的问题后移除 #error。可能需要将此密码存
// 储为其他格式或使用其他的用户身份验证。
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
// RFX_Text() 和 RFX_Int() 这类宏依赖的是
// 成员变量的类型，而不是数据库字段的类型。
// ODBC 尝试自动将列值转换为所请求的类型
	RFX_Text(pFX, _T("[OperationType]"), m_OperationType);
	RFX_Text(pFX, _T("[Lib]"), m_Lib);
	RFX_Text(pFX, _T("[IsModelUpdate]"), m_IsModelUpdate);
	RFX_Text(pFX, _T("[OperationDate]"), m_OperationDate);
	RFX_Text(pFX, _T("[WineName]"), m_WineName);
	RFX_Text(pFX, _T("[ProductionBatchNo]"), m_ProductionBatchNo);
	RFX_Text(pFX, _T("[ComFrom]"), m_ComFrom);

}
/////////////////////////////////////////////////////////////////////////////
// CLibOperationHistory 诊断

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


