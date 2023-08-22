#include "stdafx.h"
#include "time.h"
#include "SysTime.h"


SysTime::SysTime()
{
}


SysTime::~SysTime()
{
}



CString  YmdTime;     //日期缓存空间
CString  HmsTime;   //时间缓存空间


void SysTime::timeYmd(CString YMD)  //获取年月日
{
	CTime m_time;
	m_time = CTime::GetCurrentTime();             //获取当前时间日期  
	YmdTime = m_time.Format(_T("%Y-%m-%d"));
}
void SysTime::timeHms(CString HMS)  //获取当前时分秒
{
	CTime m_time;
	m_time = CTime::GetCurrentTime();
	HmsTime = m_time.Format(_T("%H-%M-%S"));          //格式化时间  时分秒
}