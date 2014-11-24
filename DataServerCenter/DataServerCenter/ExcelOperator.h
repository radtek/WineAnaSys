#ifndef EXCELOPERATOR_H
#define EXCELOPERATOR_H
#pragma once
#include "CApplication.h"
#include "CFont0.h"
#include "CRange.h"
#include "CWorkbook.h"
#include "CWorkbooks.h"
#include "CWorksheet.h"
#include "CWorksheets.h"
#include <stdexcept>

class CExcelOperator
{
public:
	CExcelOperator():
	  covOptional((long)DISP_E_PARAMNOTFOUND,VT_ERROR),
		  m_Released(true)
	  {

	  }
	~CExcelOperator(void)
	{
		ReleaseExcel();
	}
	bool LoadExcel(const CString &path);
	bool SaveExcel(const CString &path)
	{
		m_book.put_Saved(TRUE);
		m_book.SaveAs((COleVariant)path,covOptional,covOptional,covOptional,covOptional,covOptional,
			(long)0,covOptional,covOptional,covOptional,covOptional,covOptional);
		return true;
	}
	void ReleaseExcel();

	int GetColumns()
	{
		return m_colCount;
	}

	int GetRows()
	{
		return m_rowCount;
	}

	bool GetItem(int row,int column,CString outType,CString &stritem);
	bool SetItem(int row,int column,const CString &str);
	bool Save()
	{
		m_book.Save();
		return true;
	}
private:
	bool m_Released;
	CString m_Excelpath;
	CApplication m_app;
	CWorkbook m_book;
	CWorkbooks m_books;
	CWorksheet m_sheet;
	CWorksheets m_sheets;
	CRange m_range;
	LPDISPATCH m_lpDisp;
	COleVariant covOptional;
	int m_rowCount;
	int m_colCount;
	int m_startRow;
	int m_startCol;
};

inline bool CExcelOperator::LoadExcel(const CString &path)
{
	ReleaseExcel();
	m_Released = false;
	m_Excelpath=path;
	if (!m_app.CreateDispatch(_T("Excel.Application")))
	{
		int lastErr = GetLastError();
		return 0;	
	}
	m_books =m_app.get_Workbooks();
	m_lpDisp = m_books.Open(m_Excelpath,covOptional
		,covOptional,covOptional,covOptional
		,covOptional,covOptional,covOptional
		,covOptional,covOptional,covOptional
		,covOptional,covOptional,covOptional
		,covOptional);
	m_book.AttachDispatch(m_lpDisp);
	m_sheets = m_book.get_Worksheets();
	m_sheet = m_sheets.get_Item(COleVariant((short)1));
	//读取已使用区域的信息
	m_range.AttachDispatch(m_sheet.get_UsedRange());
	//取得已经使用的行数
	m_range.AttachDispatch(m_range.get_Rows());
	m_rowCount = m_range.get_Count();
	//取得已经使用的列数
	m_range.AttachDispatch(m_range.get_Columns());
	m_colCount = m_range.get_Count();
	//取得已经使用区域的起始行列
	m_startRow = m_range.get_Row();
	m_startCol = m_range.get_Column();
	return 1;
}

inline void CExcelOperator::ReleaseExcel()
{	
	if(!m_Released)
	{
		//释放对象
		m_app.Quit();
		m_range.ReleaseDispatch(); 
		m_sheet.ReleaseDispatch(); 
		m_sheets.ReleaseDispatch(); 
		m_book.ReleaseDispatch(); 
		m_books.ReleaseDispatch(); 
		m_app.ReleaseDispatch();
		m_Released = !m_Released;
	}
}

#endif

