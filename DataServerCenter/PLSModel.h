// PLSModel.h : CPLSModel ������

#pragma once

// ���������� 2014��1��15��, 16:14

class CPLSModel : public CRecordset
{
public:
	CPLSModel(CDatabase* pDatabase = NULL);
	DECLARE_DYNAMIC(CPLSModel)

// �ֶ�/��������

// �����ַ�������(�������)��ӳ���ݿ��ֶ�(ANSI �������͵� CStringA �� Unicode
// �������͵� CStringW)��ʵ���������͡�
//  ����Ϊ��ֹ ODBC ��������ִ�п���
// ����Ҫ��ת�������ϣ�������Խ���Щ��Ա����Ϊ
// CString ���ͣ�ODBC ��������ִ�����б�Ҫ��ת����
// (ע��: ����ʹ�� 3.5 �����߰汾�� ODBC ��������
// ��ͬʱ֧�� Unicode ����Щת��)��

	CStringW	m_AlcoholContent;
	CStringW	m_Flavour;
	CStringW	m_Brand;
	long	m_FactorNum;
	long	m_ClassCount;
	CStringW	m_Weight;
	CStringW	m_Labels;
	CStringW	m_Num_EachLabel;
	CStringW	m_StandardPC;
	CStringW	m_TrainsetPC;
	CStringW	m_TrainsetMean;
	CStringW	m_Threshold;

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


