#include "StdAfx.h"
#include "ExcelOperator.h"

bool CExcelOperator::GetItem(int row,int column,CString outType,CString &stritem)
{
	m_range.AttachDispatch(m_sheet.get_Cells());
	m_range.AttachDispatch(m_range.get_Item(COleVariant((long)row),COleVariant((long)column)).pdispVal);
	COleVariant  vResult = m_range.get_Value2();
	CString stry,strm,strd;
	SYSTEMTIME st;
	if(outType ==L"String")     //若是字符串
	{
		stritem = vResult.bstrVal;
		return true;
	}
	else if(outType ==L"Date") //时间格式
	{
		VariantTimeToSystemTime(vResult.date, &st);
		stry.Format(L"%d",st.wYear);
		strm.Format(L"%d",st.wMonth);
		strd.Format(L"%d",st.wDay);
		stritem = stry+L"-"+strm+L"-"+strd;
		return true;
	}
	else if (outType ==L"Number") //8字节的数字
	{
		stritem.Format(L"%f",vResult.dblVal);
		return true;
	}
	else if (outType ==L"int")
	{
		stritem.Format(_T("%ld"), (int)vResult.dblVal);
		return true;
	}
	return false;
}
bool CExcelOperator::SetItem(int row,int column,const CString &str)
{
	m_range.AttachDispatch(m_sheet.get_Cells());
	m_range.AttachDispatch(m_range.get_Item(COleVariant((long)row),COleVariant((long)column)).pdispVal);
	VARIANT var;
	var.vt = VT_BSTR;
	var.bstrVal = _bstr_t((LPCTSTR)str);
	m_range.put_Value2(var);
	return true;
}