#include "stdafx.h"
#include "time.h"
#include "SysTime.h"


SysTime::SysTime()
{
}


SysTime::~SysTime()
{
}



CString  YmdTime;     //���ڻ���ռ�
CString  HmsTime;   //ʱ�仺��ռ�


void SysTime::timeYmd(CString YMD)  //��ȡ������
{
	CTime m_time;
	m_time = CTime::GetCurrentTime();             //��ȡ��ǰʱ������  
	YmdTime = m_time.Format(_T("%Y-%m-%d"));
}
void SysTime::timeHms(CString HMS)  //��ȡ��ǰʱ����
{
	CTime m_time;
	m_time = CTime::GetCurrentTime();
	HmsTime = m_time.Format(_T("%H-%M-%S"));          //��ʽ��ʱ��  ʱ����
}