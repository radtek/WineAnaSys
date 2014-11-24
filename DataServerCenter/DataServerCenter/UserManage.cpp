// UserManage.h : CUserManage ���ʵ��



// CUserManage ʵ��

// ���������� 2014��7��12��, 19:51

#include "stdafx.h"
#include "UserManage.h"
IMPLEMENT_DYNAMIC(CUserManage, CRecordset)

CUserManage::CUserManage(CDatabase* pdb)
	: CRecordset(pdb)
{
	m_UserName = L"";
	m_PassWord = L"";
	m_Petitioner = L"";
	m_Institution = L"";
	m_Telephone = L"";
	m_Email = L"";
	m_Applicationdate = L"";
	m_Authority = 0;
	m_nFields = 8;
	m_nDefaultType = dynaset;
}
//#error ��ȫ���⣺�����ַ������ܰ������롣
// �������ַ����п��ܰ������������/��������Ҫ
// ��Ϣ�����ڲ鿴��������ַ������ҵ������밲ȫ
// �йص�������Ƴ� #error��������Ҫ���������
// ��Ϊ������ʽ��ʹ���������û������֤��
CString CUserManage::GetDefaultConnect()
{
	return _T("DSN=\x767d\x9152\x9274\x5b9a\x4e0e\x6eaf\x6e90\x6570\x636e\x5e93;Trusted_Connection=Yes;APP=Microsoft\x00ae Visual Studio\x00ae 2010;WSID=WTH515812076-PC;DATABASE=\x767d\x9152\x9274\x5b9a\x4e0e\x6eaf\x6e90\x6570\x636e\x5e93");
}

CString CUserManage::GetDefaultSQL()
{
	return _T("[dbo].[UserManage]");
}

void CUserManage::DoFieldExchange(CFieldExchange* pFX)
{
	pFX->SetFieldType(CFieldExchange::outputColumn);
// RFX_Text() �� RFX_Int() �������������
// ��Ա���������ͣ����������ݿ��ֶε����͡�
// ODBC �����Զ�����ֵת��Ϊ�����������
	RFX_Text(pFX, _T("[UserName]"), m_UserName);
	RFX_Text(pFX, _T("[PassWord]"), m_PassWord);
	RFX_Text(pFX, _T("[Petitioner]"), m_Petitioner);
	RFX_Text(pFX, _T("[Institution]"), m_Institution);
	RFX_Text(pFX, _T("[Telephone]"), m_Telephone);
	RFX_Text(pFX, _T("[Email]"), m_Email);
	RFX_Text(pFX, _T("[Applicationdate]"), m_Applicationdate);
	RFX_Long(pFX, _T("[Authority]"), m_Authority);

}
/////////////////////////////////////////////////////////////////////////////
// CUserManage ���

#ifdef _DEBUG
void CUserManage::AssertValid() const
{
	CRecordset::AssertValid();
}

void CUserManage::Dump(CDumpContext& dc) const
{
	CRecordset::Dump(dc);
}
#endif //_DEBUG


