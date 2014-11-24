// PredictRecord.h : CPredictRecord 类的实现



// CPredictRecord 实现

// 代码生成在 2014年4月9日, 20:10

#include "stdafx.h"
#include "PredictRecord.h"
IMPLEMENT_DYNAMIC(CPredictRecord, CRecordset)

CPredictRecord::CPredictRecord(CDatabase* pdb)
	: CRecordset(pdb)
{
	m_ProjectName = L"";
	m_SpecNum = L"";
	m_RecvDate = L"";
	m_PredictResult = L"";
	m_Reliablility = L"";
	m_PredictDate = L"";
	m_SendBackDate = L"";
	m_nFields = 7;
	m_nDefaultType = dynaset;
}
//#error 安全问题：连接字符串可能包含密码。
// 此连接字符串中可能包含明文密码和/或其他重要
// 信息。请在查看完此连接字符串并找到所有与安全
// 有关的问题后移除 #error。可能需要将此密码存
// 储为其他格式或使用其他的用户身份验证。
CString CPredictRecord::GetDefaultConnect()
{
	return _T("DSN=\x767d\x9152\x9274\x5b9a\x4e0e\x6eaf\x6e90\x6570\x636e\x5e93;Trusted_Connection=Yes;APP=Microsoft\x00ae Visual Studio\x00ae 2010;WSID=WTH515812076-PC;DATABASE=\x767d\x9152\x9274\x5b9a\x4e0e\x6eaf\x6e90\x6570\x636e\x5e93");
}

CString CPredictRecord::GetDefaultSQL()
{
	return _T("[dbo].[PredictRecord]");
}

void CPredictRecord::DoFieldExchange(CFieldExchange* pFX)
{
	pFX->SetFieldType(CFieldExchange::outputColumn);
// RFX_Text() 和 RFX_Int() 这类宏依赖的是
// 成员变量的类型，而不是数据库字段的类型。
// ODBC 尝试自动将列值转换为所请求的类型
	RFX_Text(pFX, _T("[ProjectName]"), m_ProjectName);
	RFX_Text(pFX, _T("[SpecNum]"), m_SpecNum);
	RFX_Text(pFX, _T("[RecvDate]"), m_RecvDate);
	RFX_Text(pFX, _T("[PredictResult]"), m_PredictResult);
	RFX_Text(pFX, _T("[Reliablility]"), m_Reliablility);
	RFX_Text(pFX, _T("[PredictDate]"), m_PredictDate);
	RFX_Text(pFX, _T("[SendBackDate]"), m_SendBackDate);

}
/////////////////////////////////////////////////////////////////////////////
// CPredictRecord 诊断

#ifdef _DEBUG
void CPredictRecord::AssertValid() const
{
	CRecordset::AssertValid();
}

void CPredictRecord::Dump(CDumpContext& dc) const
{
	CRecordset::Dump(dc);
}
#endif //_DEBUG


