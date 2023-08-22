#pragma once

//#include <afxwin.h>         // MFC 核心组件和标准组件
//#include <afxext.h>         // MFC 扩展
//#include <afxdisp.h>        // MFC 自动化类

extern CString  YmdTime;   //日期缓存空间
extern CString  HmsTime;   //时间缓存空间

class SysTime
{
public:
	SysTime();
	~SysTime();
	void timeYmd(CString YMD);
	void timeHms(CString HMS);
};
