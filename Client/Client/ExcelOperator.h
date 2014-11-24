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

class CExcelOperator
{
public:
	CExcelOperator():
	  covOptional((long)DISP_E_PARAMNOTFOUND,VT_ERROR)
	  {
		  m_Released=1;
	  }
	~CExcelOperator(void)
	{
		if(!m_Released)
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
	bool ReleaseExcel();

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
	//��ȡ��ʹ���������Ϣ
	m_range.AttachDispatch(m_sheet.get_UsedRange());
	//ȡ���Ѿ�ʹ�õ�����
	m_range.AttachDispatch(m_range.get_Rows());
	m_rowCount = m_range.get_Count();
	//ȡ���Ѿ�ʹ�õ�����
	m_range.AttachDispatch(m_range.get_Columns());
	m_colCount = m_range.get_Count();
	//ȡ���Ѿ�ʹ���������ʼ����
	m_startRow = m_range.get_Row();
	m_startCol = m_range.get_Column();
	m_Released=0;
	return 1;
}

inline bool CExcelOperator::ReleaseExcel()
{	
	m_app.Quit();
	//�ͷŶ���
	m_range.ReleaseDispatch(); 
	m_sheet.ReleaseDispatch(); 
	m_sheets.ReleaseDispatch(); 
	m_book.ReleaseDispatch(); 
	m_books.ReleaseDispatch(); 
	m_app.ReleaseDispatch();
	m_Released=1;
	return 1;
}

#endif

