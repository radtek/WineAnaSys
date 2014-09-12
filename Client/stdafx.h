// 这段 MFC 示例源代码演示如何使用 MFC Microsoft Office Fluent 用户界面 
// (“Fluent UI”)。该示例仅供参考，
// 用以补充《Microsoft 基础类参考》和 
// MFC C++ 库软件随附的相关电子文档。
// 复制、使用或分发 Fluent UI 的许可条款是单独提供的。
// 若要了解有关 Fluent UI 许可计划的详细信息，请访问  
// http://msdn.microsoft.com/officeui。
//
// 版权所有(C) Microsoft Corporation
// 保留所有权利。

// stdafx.h : 标准系统包含文件的包含文件，
// 或是经常使用但不常更改的
// 特定于项目的包含文件

#pragma once

#ifndef _SECURE_ATL
#define _SECURE_ATL 1
#endif

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN            // 从 Windows 头中排除极少使用的资料
#endif

#include "targetver.h"

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // 某些 CString 构造函数将是显式的

// 关闭 MFC 对某些常见但经常可放心忽略的警告消息的隐藏
#define _AFX_ALL_WARNINGS

#include <afxwin.h>         // MFC 核心组件和标准组件
#include <afxext.h>         // MFC 扩展


#include <afxdisp.h>        // MFC 自动化类



#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>           // MFC 对 Internet Explorer 4 公共控件的支持
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>             // MFC 对 Windows 公共控件的支持
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <afxcontrolbars.h>     // 功能区和控件条的 MFC 支持
#include <afxdisp.h>
#include <afxdb.h>


extern char skin_path[MAX_PATH];
#include "BCMenu.h"
#include "BtnST.h"
#include "CeXDib.h"
#include "ShadeButtonST.h"

#define SPEC_LENTH 869
#define WM_CHANGESELECTEDCURVE WM_USER+109
#define WM_CHANGECHARTMODE WM_USER+111
#define WM_PCACHANGECHARTMODE WM_USER+110
#define WM_PLAYLOGININGANIMATION WM_USER+501
//客户端端口号
#define EXIT_THREADRECV 2014
#define MAX_THREAD_NUM 32
#define DATA_BUFSIZE 12288
#define CLIENT_RECV_PORT 2000
#define SERVER_PROT 8000
#define MAX_USERNAMELEN 32
#define MAX_PASSWORDLEN 32
#define WM_BEGINSHOWSENDPROGRESS WM_USER+102
#define WM_ENDSHOWSENDPROGRESS WM_USER+103
#define WM_BEGINCHECKCONNECT WM_USER+104
#define WM_ADDFINISHEDPRO WM_USER+107
#define WM_SHOWUSERNAME WM_USER+500
#define WM_SHOWSENDPROGRESS WM_USER+101
#define WM_UIDISPLAY WM_USER+1
#define WM_UISELECT WM_USER+2

#define	WM_MY_PRINT			(WM_USER+1003)
#define	WM_BEGIN_PRINTING		(WM_USER+1004)
#define	WM_END_PRINTING			(WM_USER+1005)
#define PRINTMARGIN 2




#ifdef _UNICODE
#if defined _M_IX86
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_X64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#else
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif
#endif


