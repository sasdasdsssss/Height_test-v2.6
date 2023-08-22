#pragma once
#include "stdafx.h"


#define MAX_LOG_BUFF	200

class CLogEdit
{
public:
	int LOG(LPCWSTR fmt, ...);
	int LOGI(LPCWSTR fmt, ...);
	int LOGT(LPCWSTR TAG, LPCWSTR fmt, ...);
	int LOGP(LPCWSTR fmt, ...);
	int LOGPT(LPCWSTR TAG, LPCWSTR fmt, ...);
	void Init(CEdit *pEdit);
	int append(LPCWSTR fmt, ...);
	CLogEdit(const CLogEdit& other);
	CLogEdit& operator=(const CLogEdit& other);
	CLogEdit();
	~CLogEdit();
protected:
	CEdit *objEdit;
private:
	wchar_t buff[MAX_LOG_BUFF];
	const int MaxBufSize = MAX_LOG_BUFF;
};

class CLogFile
{
public:

protected:

private:

};