#pragma once

//#include <afxwin.h>         // MFC ��������ͱ�׼���
//#include <afxext.h>         // MFC ��չ
//#include <afxdisp.h>        // MFC �Զ�����

extern CString  YmdTime;   //���ڻ���ռ�
extern CString  HmsTime;   //ʱ�仺��ռ�

class SysTime
{
public:
	SysTime();
	~SysTime();
	void timeYmd(CString YMD);
	void timeHms(CString HMS);
};
