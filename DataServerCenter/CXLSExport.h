// �����Ϳ������á�����ࡱ�����ļ�������ɵ� IDispatch ��װ��

#import "C:\\Program Files\\TeeChart Pro v5 ActiveX Control\\TeeChart5.ocx" no_namespace
// CXLSExport ��װ��

class CXLSExport : public COleDispatchDriver
{
public:
	CXLSExport(){} // ���� COleDispatchDriver Ĭ�Ϲ��캯��
	CXLSExport(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	CXLSExport(const CXLSExport& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

	// ����
public:

	// ����
public:


	// IXLSExport ����
public:
	BOOL get_IncludeHeader()
	{
		BOOL result;
		InvokeHelper(0x1, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	void put_IncludeHeader(BOOL newValue)
	{
		static BYTE parms[] = VTS_BOOL ;
		InvokeHelper(0x1, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	BOOL get_IncludeLabels()
	{
		BOOL result;
		InvokeHelper(0x2, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	void put_IncludeLabels(BOOL newValue)
	{
		static BYTE parms[] = VTS_BOOL ;
		InvokeHelper(0x2, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	BOOL get_IncludeIndex()
	{
		BOOL result;
		InvokeHelper(0x3, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	void put_IncludeIndex(BOOL newValue)
	{
		static BYTE parms[] = VTS_BOOL ;
		InvokeHelper(0x3, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	VARIANT get_Series()
	{
		VARIANT result;
		InvokeHelper(0x4, DISPATCH_PROPERTYGET, VT_VARIANT, (void*)&result, NULL);
		return result;
	}
	void put_Series(VARIANT& newValue)
	{
		static BYTE parms[] = VTS_VARIANT ;
		InvokeHelper(0x4, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, &newValue);
	}
	void SaveToFile(LPCTSTR FileName)
	{
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0x5, DISPATCH_METHOD, VT_EMPTY, NULL, parms, FileName);
	}
	VARIANT SaveToStream()
	{
		VARIANT result;
		InvokeHelper(0x6, DISPATCH_METHOD, VT_VARIANT, (void*)&result, NULL);
		return result;
	}
	CString AsString()
	{
		CString result;
		InvokeHelper(0x7, DISPATCH_METHOD, VT_BSTR, (void*)&result, NULL);
		return result;
	}

	// IXLSExport ����
public:

};
