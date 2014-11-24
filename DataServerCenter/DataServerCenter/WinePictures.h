// WinePictures.h : CWinePictures ������

#pragma once

// ���������� 2014��5��19��, 16:31

class CWinePictures : public CRecordset
{
public:
	CWinePictures(CDatabase* pDatabase = NULL);
	DECLARE_DYNAMIC(CWinePictures)

// �ֶ�/��������

// �����ַ�������(�������)��ӳ���ݿ��ֶ�(ANSI �������͵� CStringA �� Unicode
// �������͵� CStringW)��ʵ���������͡�
//  ����Ϊ��ֹ ODBC ��������ִ�п���
// ����Ҫ��ת�������ϣ�������Խ���Щ��Ա����Ϊ
// CString ���ͣ�ODBC ��������ִ�����б�Ҫ��ת����
// (ע��: ����ʹ�� 3.5 �����߰汾�� ODBC ��������
// ��ͬʱ֧�� Unicode ����Щת��)��

	CStringW	m_WineName;
	CStringW	m_AlcoholContent;
	CStringW	m_Flavour;
	CStringW	m_Brand;
	CStringW	m_ProductionBatchNo;
	CStringW	m_PicturesDir;
	CStringW	m_ImportDate;
	CStringW	m_Manager;
	CStringW	m_SpecIDTitle;

// ��д
	// �����ɵ��麯����д
	public:
	virtual CString GetDefaultConnect();	// Ĭ�������ַ���

	virtual CString GetDefaultSQL(); 	// ��¼����Ĭ�� SQL
	virtual void DoFieldExchange(CFieldExchange* pFX);	// RFX ֧��

// ʵ��
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

};


