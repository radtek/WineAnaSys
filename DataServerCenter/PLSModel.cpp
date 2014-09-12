// PLSModel.h : CPLSModel 类的实现



// CPLSModel 实现

// 代码生成在 2014年1月15日, 16:14

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
//#error 安全问题：连接字符串可能包含密码。
// 此连接字符串中可能包含明文密码和/或其他重要
// 信息。请在查看完此连接字符串并找到所有与安全
// 有关的问题后移除 #error。可能需要将此密码存
// 储为其他格式或使用其他的用户身份验证。
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
// RFX_Text() 和 RFX_Int() 这类宏依赖的是
// 成员变量的类型，而不是数据库字段的类型。
// ODBC 尝试自动将列值转换为所请求的类型
	RFX_Text(pFX, _T("[AlcoholContent]"), m_AlcoholContent);
	RFX_Text(pFX, _T("[Flavour]"), m_Flavour);
	RFX_Text(pFX, _T("[Brand]"), m_Brand);
	RFX_Long(pFX, _T("[FactorNum]"), m_FactorNum);
	RFX_Long(pFX, _T("[ClassCount]"), m_ClassCount);
	RFX_Text(pFX, _T("[Weight]"), m_Weight,1000000);
	RFX_Text(pFX, _T("[Labels]"), m_Labels);
	RFX_Text(pFX, _T("[Num_EachLabel]"), m_Num_EachLabel);
	RFX_Text(pFX, _T("[StandardPC]"), m_StandardPC,600000);
	RFX_Text(pFX, _T("[TrainsetPC]"), m_TrainsetPC,1000000);
	RFX_Text(pFX, _T("[TrainsetMean]"), m_TrainsetMean,600000);
	RFX_Text(pFX, _T("[Threshold]"), m_Threshold);

}
/////////////////////////////////////////////////////////////////////////////
// CPLSModel 诊断

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


