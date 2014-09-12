#include "zip.h" 
#include "unzip.h" 

class CZipImplement 
{ 
public: 
	CZipImplement(void); 
	~CZipImplement(void); 

private: 
	HZIP hz;          //Zip文件句柄 
	ZRESULT zr;    //操作返回值 
	ZIPENTRY ze;  //Zip文件入口 

	CString m_FolderPath;     //folder路径 
	CString  m_FolderName;  //folder将要被压缩的文件夹名 

private: 
	//实现遍历文件夹 
	void BrowseFile(CString &strFile); 

	//获取相对路径 
	void GetRelativePath(CString& pFullPath, CString& pSubString); 

	//创建路径 
	BOOL CreatedMultipleDirectory(wchar_t* direct); 
	///* 
	//*********************************************************************** 
	//* 函数： TransCStringToTCHAR 
	//* 描述：将CString 转换为 TCHAR* 
	//*********************************************************************** 
	//*/ 
	//TCHAR* CString2TCHAR(CString &str) 
	//{ 
	//    int iLen = str.GetLength(); 
	//    TCHAR* szRs = new TCHAR[iLen]; 
	//    lstrcpy(szRs, str.GetBuffer(iLen)); 
	//    str.ReleaseBuffer(); 
	//    return szRs; 
	//} 

public: 
	//压缩文件夹接口 
	BOOL Zip_PackFiles(CString& pFilePath, CString& mZipFileFullPath); 

	//解压缩文件夹接口 
	BOOL Zip_UnPackFiles(CString &mZipFileFullPath, CString& mUnPackPath); 

public: 
	//静态方法提供文件夹路径检查 
	static BOOL FolderExist(CString& strPath); 
};

