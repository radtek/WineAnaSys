// �����Ϳ������á�����ࡱ�����ļ�������ɵ� IDispatch ��װ��

#import "C:\\Program Files\\TeeChart Pro v5 ActiveX Control\\TeeChart5.ocx" no_namespace
// CADXFunction ��װ��

class CADXFunction : public COleDispatchDriver
{
public:
	CADXFunction(){} // ���� COleDispatchDriver Ĭ�Ϲ��캯��
	CADXFunction(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	CADXFunction(const CADXFunction& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

	// ����
public:

	// ����
public:


	// IADXFunction ����
public:
	LPDISPATCH get_DMDown()
	{
		LPDISPATCH result;
		InvokeHelper(0x1, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	LPDISPATCH get_DMUp()
	{
		LPDISPATCH result;
		InvokeHelper(0x2, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}

	// IADXFunction ����
public:

};
