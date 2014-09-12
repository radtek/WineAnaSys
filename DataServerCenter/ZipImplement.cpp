#include "stdafx.h" 
#include "zipimplement.h" 
#include <direct.h> 
#include <vector> 
#include <xstring> 

CZipImplement::CZipImplement(void) 
{ 
} 

CZipImplement::~CZipImplement(void) 
{ 
} 

///////////////////////////////////////////////////////////////////////////// 
// 函数说明: 实现压缩文件夹操作 
// 参数说明: [in]： pFilePath         要被压缩的文件夹 
//                         mZipFileFullPath  压缩后的文件名和路径 
// 返回值: 参数有误的情况下返回FALSE,压缩成功后返回TRUE 
// 函数作者: <hwfly> 
// 创建日期: 2009-09-27 下午 04:58:52 
///////////////////////////////////////////////////////////////////////////// 
BOOL CZipImplement::Zip_PackFiles(CString& pFilePath, CString& mZipFileFullPath) 
{ 
	//参数错误 
	if ((pFilePath == L"") || (mZipFileFullPath == L"")) 
	{ 
		//路径异常返回 
		return FALSE ; 
	} 

	if(!CZipImplement::FolderExist(pFilePath)) 
	{ 
		//要被压缩的文件夹不存在 
		return FALSE ; 
	} 

	CString tZipFilePath = mZipFileFullPath.Left(mZipFileFullPath.ReverseFind('\\') + 1); 
	if(!CZipImplement::FolderExist(tZipFilePath)) 
	{ 
		//ZIP文件存放的文件夹不存在创建它 
		wchar_t* temp=tZipFilePath.GetBuffer(tZipFilePath.GetLength()); 
		if (FALSE == CreatedMultipleDirectory(temp)) 
		{ 
			//创建目录失败 
			return FALSE; 
		} 
	} 

	//获得文件夹的名字 
	if(pFilePath.Right(1) == L"\\") 
	{ 
		this->m_FolderPath = pFilePath.Left(pFilePath.GetLength() - 1); 
		m_FolderName = m_FolderPath.Right(m_FolderPath.GetLength() - m_FolderPath.ReverseFind('\\') - 1); 
	} 
	else 
	{ 
		this->m_FolderPath = pFilePath; 
		m_FolderName = pFilePath.Right(pFilePath.GetLength() - pFilePath.ReverseFind('\\') - 1); 
	} 

	/************************************************************************/ 

	//创建ZIP文件 
	hz=CreateZip(mZipFileFullPath,0); 
	if(hz == 0) 
	{ 
		//创建Zip文件失败 
		return FALSE; 
	} 

	//递归文件夹,将获取的问价加入ZIP文件 
	BrowseFile(pFilePath); 
	//关闭ZIP文件 
	CloseZip(hz); 

	/************************************************************************/ 

	CFileFind tFFind; 
	if (!tFFind.FindFile(mZipFileFullPath)) 
	{ 
		//压缩失败(未发现压缩后的文件) 
		return FALSE; 
	} 

	return TRUE; 
} 

///////////////////////////////////////////////////////////////////////////// 
// 函数说明: 解压缩文件夹 
// 参数说明: [in]： mUnPackPath     解压后文件存放的路径 
//                         mZipFileFullPath  ZIP文件的路径 
// 返回值: 
// 函数作者: <hwfly> 
// 创建日期: 2009-09-27 上午 11:04:28 
///////////////////////////////////////////////////////////////////////////// 
BOOL CZipImplement::Zip_UnPackFiles(CString &mZipFileFullPath, CString& mUnPackPath) 
{ 
	//参数错误 
	if ((mUnPackPath == L"") || (mZipFileFullPath == L"")) 
	{ 
		//路径异常返回 
		return FALSE ; 
	} 

	CFileFind tFFind; 
	if (!tFFind.FindFile(mZipFileFullPath)) 
	{ 
		//压缩失败(未发现压缩文件) 
		return FALSE; 
	} 

	//如果解压缩的路径不存在 试图创建它 
	CString tZipFilePath = mUnPackPath; 
	if(!CZipImplement::FolderExist(tZipFilePath)) 
	{ 
		//解压后存放的文件夹不存在 创建它 
		wchar_t* temp=tZipFilePath.GetBuffer(tZipFilePath.GetLength()); 
		if (FALSE == CreatedMultipleDirectory(temp)) 
		{ 
			//创建目录失败 
			return FALSE; 
		} 
	} 
	/************************************************************************/ 
	//打开ZIP文件 E:\演示平台\DataServerCenter\DataServerCenter\FileRecv
	hz=OpenZip(mZipFileFullPath,0); 
	if(hz == 0) 
	{ 
		//打开Zip文件失败 
		return FALSE; 
	} 

	zr=SetUnzipBaseDir(hz,mUnPackPath); 
	if(zr != ZR_OK) 
	{ 
		//打开Zip文件失败 
		CloseZip(hz); 
		return FALSE;       
	} 

	zr=GetZipItem(hz,-1,&ze); 
	if(zr != ZR_OK) 
	{ 
		//获取Zip文件内容失败 
		CloseZip(hz); 
		return FALSE;       
	} 

	int numitems=ze.index; 
	for (int i=0; i<numitems; i++) 
	{ 
		zr=GetZipItem(hz,i,&ze); 
		zr=UnzipItem(hz,i,ze.name); 

		if(zr != ZR_OK) 
		{ 
			//获取Zip文件内容失败 
			CloseZip(hz); 
			return FALSE;       
		} 
	} 

	CloseZip(hz); 
	return TRUE; 
} 

///////////////////////////////////////////////////////////////////////////// 
// 函数说明: 检查指定的文件夹是否存在 
// 参数说明: [in]：strPath 检查的文件夹 (此方法会主动向路径末尾添加*.*) 
// 返回值:BOOL类型,存在返回TRUE,否则为FALSE 
// 函数作者: <hwfly> 
// 创建日期: 2009-09-27 下午 02:16:36 
///////////////////////////////////////////////////////////////////////////// 
BOOL CZipImplement::FolderExist(CString& strPath) 
{ 
	CString sCheckPath = strPath; 

	if(sCheckPath.Right(1) != L"\\") 
		sCheckPath += L"\\"; 

	sCheckPath += L"*.*"; 

	WIN32_FIND_DATA wfd; 
	BOOL rValue = FALSE; 

	HANDLE hFind = FindFirstFile(sCheckPath, &wfd); 

	if ((hFind!=INVALID_HANDLE_VALUE) && 
		(wfd.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY) || (wfd.dwFileAttributes&FILE_ATTRIBUTE_ARCHIVE)) 
	{ 
		//如果存在并类型是文件夹 
		rValue = TRUE; 
	} 

	FindClose(hFind); 
	return rValue; 
} 

///////////////////////////////////////////////////////////////////////////// 
// 函数说明: 遍历文件夹 
// 参数说明: [in]：strFile 遍历的文件夹(此方法会主动向路径末尾添加*.*) 
// 返回值:BOOL类型,存在返回TRUE,否则为FALSE 
// 函数作者: <hwfly> 
// 创建日期: 2009-09-27 下午 02:16:36 
///////////////////////////////////////////////////////////////////////////// 
void CZipImplement::BrowseFile(CString &strFile) 
{ 
	CFileFind ff; 
	CString szDir = strFile; 

	if(szDir.Right(1) != L"\\") 
		szDir += L"\\"; 

	szDir += L"*.*"; 

	BOOL res = ff.FindFile(szDir); 
	while(res) 
	{ 
		res = ff.FindNextFile(); 
		if(ff.IsDirectory() && !ff.IsDots()) 
		{ 
			//如果是一个子目录，用递归继续往深一层找 
			CString strPath = ff.GetFilePath(); 

			CString subPath; 
			GetRelativePath(strPath,subPath); 
			//将文件添加到ZIP文件 
			ZipAddFolder(hz,subPath); 
			BrowseFile(strPath); 
		} 
		else if(!ff.IsDirectory() && !ff.IsDots()) 
		{ 
			//显示当前访问的文件(完整路径) 
			CString strPath = ff.GetFilePath(); 
			CString subPath; 

			GetRelativePath(strPath,subPath); 
			//将文件添加到ZIP文件 
			ZipAdd(hz,subPath,strPath); 
		} 
	} 

	//关闭 
	ff.Close(); 
} 

///////////////////////////////////////////////////////////////////////////// 
// 函数说明: 获取相对路径 
// 参数说明: [in]：pFullPath 当前文件的完整路径 [out] : 解析后的相对路径 
// 函数作者: <hwfly> 
// 创建日期: 2009-9-28 上午 11:17:21 
///////////////////////////////////////////////////////////////////////////// 
void CZipImplement::GetRelativePath(CString& pFullPath,CString& pSubString) 
{ 
	pSubString = pFullPath.Right(pFullPath.GetLength() - this->m_FolderPath.GetLength() + this->m_FolderName.GetLength()); 
} 

///////////////////////////////////////////////////////////////////////////// 
// 函数说明: 创建多级目录 
// 参数说明: [in]： 路径字符串 
// 返回值: BOOL 成功True 失败False 
// 函数作者: <hwfly> 
// 创建日期: 2009-9-28 下午 04:53:20 
///////////////////////////////////////////////////////////////////////////// 
BOOL CZipImplement::CreatedMultipleDirectory(wchar_t* direct) 
{ 
	std::wstring Directoryname = direct; 

	if (Directoryname[Directoryname.length() - 1] !=  '\\') 
	{ 
		Directoryname.append(1, '\\'); 
	} 
	std::vector< std::wstring> vpath; 
	std::wstring strtemp; 
	BOOL  bSuccess = FALSE; 
	for (int i = 0; i < Directoryname.length(); i++) 
	{ 
		if ( Directoryname[i] != '\\') 
		{ 
			strtemp.append(1,Directoryname[i]);    
		} 
		else 
		{ 
			vpath.push_back(strtemp); 
			strtemp.append(1, '\\'); 
		} 
	} 
	std::vector<std::wstring>:: const_iterator vIter; 
	for (vIter = vpath.begin();vIter != vpath.end(); vIter++) 
	{ 
		bSuccess = CreateDirectory(vIter->c_str(), NULL) ? TRUE :FALSE; 
	} 

	return bSuccess; 
}
