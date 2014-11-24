// AnalasisHistory.h : CAnalasisHistory 类的实现



// CAnalasisHistory 实现

// 代码生成在 2013年12月30日, 16:22

#include "stdafx.h"
#include "AnalasisHistory.h"
IMPLEMENT_DYNAMIC(CAnalasisHistory, CRecordset)

CAnalasisHistory::CAnalasisHistory(CDatabase* pdb)
	: CRecordset(pdb)
{
	m_AnalasisType = L"";
	m_ComFrom = L"";
	m_ProjectName = L"";
	m_AnalasisResult = L"";
	m_AnalasisDate = L"";
	m_nFields = 5;
	m_nDefaultType = dynaset;
}
//#error 安全问题：连接字符串可能包含密码。
// 此连接字符串中可能包含明文密码和/或其他重要
// 信息。请在查看完此连接字符串并找到所有与安全
// 有关的问题后移除 #error。可能需要将此密码存
// 储为其他格式或使用其他的用户身份验证。
CString CAnalasisHistory::GetDefaultConnect()
{
	return _T("DSN=\x767d\x9152\x9274\x5b9a\x4e0e\x6eaf\x6e90\x6570\x636e\x5e93;Trusted_Connection=Yes;APP=Microsoft\x00ae Visual Studio\x00ae 2010;WSID=WTH515812076-PC;DATABASE=\x767d\x9152\x9274\x5b9a\x4e0e\x6eaf\x6e90\x6570\x636e\x5e93");
}

CString CAnalasisHistory::GetDefaultSQL()
{
	return _T("[dbo].[AnalasisHistory]");
}

void CAnalasisHistory::DoFieldExchange(CFieldExchange* pFX)
{
	pFX->SetFieldType(CFieldExchange::outputColumn);
// RFX_Text() 和 RFX_Int() 这类宏依赖的是
// 成员变量的类型，而不是数据库字段的类型。
// ODBC 尝试自动将列值转换为所请求的类型
	RFX_Text(pFX, _T("[AnalasisType]"), m_AnalasisType);
	RFX_Text(pFX, _T("[ComFrom]"), m_ComFrom);
	RFX_Text(pFX, _T("[ProjectName]"), m_ProjectName);
	RFX_Text(pFX, _T("[AnalasisResult]"), m_AnalasisResult);
	RFX_Text(pFX, _T("[AnalasisDate]"), m_AnalasisDate);

}
/////////////////////////////////////////////////////////////////////////////
// CAnalasisHistory 诊断

#ifdef _DEBUG
void CAnalasisHistory::AssertValid() const
{
	CRecordset::AssertValid();
}

void CAnalasisHistory::Dump(CDumpContext& dc) const
{
	CRecordset::Dump(dc);
}
#endif //_DEBUG


