#include "stdafx.h"
#include "Log.h"

/* 
	@url https://learn.microsoft.com/zh-cn/cpp/c-runtime-library/reference/vsnprintf-vsnprintf-vsnprintf-l-vsnwprintf-vsnwprintf-l?view=msvc-140
*/
int CLogEdit::LOG(LPCWSTR fmt, ...)
{
	int iEditLen, iSize = 0;

	if (objEdit)
	{
		memset(buff, 0, sizeof(buff));
		/**����**/
		va_list args;
		va_start(args, fmt);
		iSize = _vsnwprintf_s(buff, MaxBufSize, _TRUNCATE, fmt, args); // C4996
		va_end(args);
		iEditLen = objEdit->GetWindowTextLength();
		objEdit->SetSel(iEditLen, iEditLen);
		objEdit->ReplaceSel(buff);
		return iSize;
	}

	return -1;
}

int CLogEdit::LOGI(LPCWSTR fmt, ...)
{
	int iEditLen, iSize = 0;

	if (objEdit > 0)
	{
		memset(buff, 0, sizeof(buff));
		va_list args;
		va_start(args, fmt);
		iSize = _vsnwprintf_s(buff, MaxBufSize, MaxBufSize-3, fmt, args);
		va_end(args);
		buff[iSize] = _T('\r');
		buff[iSize + 1] = _T('\n');
		buff[iSize + 2] = _T('\0');
		iEditLen = objEdit->GetWindowTextLength();
		objEdit->SetSel(iEditLen, iEditLen);
		objEdit->ReplaceSel(buff);
		return iSize;
	}
	return -1;
}

int CLogEdit::LOGT(LPCWSTR TAG, LPCWSTR fmt, ...)
{
	CString tmpStr;
	int iEditLen, iSize = 0;

	memset(buff, 0, sizeof(buff));

	va_list args;
	va_start(args, fmt);
	iSize = _vsnwprintf_s(buff, MaxBufSize, _TRUNCATE, fmt, args);
	va_end(args);

	tmpStr.Format(_T("[%s]%s\r\n"), TAG, buff);
	iEditLen = objEdit->GetWindowTextLength();
	objEdit->SetSel(iEditLen, iEditLen);
	objEdit->ReplaceSel(tmpStr);

	return 0;
}

/*
int CLogEdit::LOGP(LPCWSTR fmt, ...)
{
	CString msg;
	CTime tTimeNow = CTime::GetCurrentTime();
	int iEditLen, iSize = 0;
	objEdit->SetLimitText(5000000); // ��������Ϊ 5MB

	memset(buff, 0, sizeof(buff));
	va_list args;
	va_start(args, fmt);
	iSize = _vsnwprintf_s(buff, MaxBufSize, _TRUNCATE, fmt, args);
	va_end(args);

	msg.Format(_T("[%s]:%s\r\n"), tTimeNow.Format(_T("%H:%M:%S")), buff);
	iEditLen = objEdit->GetWindowTextLength();

	// ��������Ƿ񳬹�����
	int iLineCount = objEdit->GetLineCount();
	int iMaxLineCount = 100; // ��������Ϊ 100 ��

	if (iLineCount >= iMaxLineCount)
	{
		// ��������������
		int iFirstLineLength = objEdit->LineLength(0);
		objEdit->SetSel(0, iFirstLineLength);
		objEdit->ReplaceSel(_T(""));
	}

	objEdit->SetSel(iEditLen, iEditLen);
	objEdit->ReplaceSel(msg);

	// ���������·�
	//objEdit->LineScroll(objEdit->GetLineCount());

	return 0;
}
*/

int CLogEdit::LOGP(LPCWSTR fmt, ...)
{
	CString msg;
	CTime tTimeNow = CTime::GetCurrentTime();
	int iEditLen, iSize = 0;
	objEdit->SetLimitText(5000000); // ��������Ϊ 5MB

	memset(buff, 0, sizeof(buff));
	va_list args;
	va_start(args, fmt);
	iSize = _vsnwprintf_s(buff, MaxBufSize, _TRUNCATE, fmt, args);
	va_end(args);

	msg.Format(_T("[%s]:%s\r\n"), tTimeNow.Format(_T("%H:%M:%S")), buff);
	iEditLen = objEdit->GetWindowTextLength();

	// ��������Ƿ񳬹�����
	int iLineCount = objEdit->GetLineCount();
	int iMaxLineCount = 30; // ����Ϊ 30 ��
	int iMaxLineLength = objEdit->LineLength(0); // ��ȡ��һ�еĳ���

	if (iLineCount >= iMaxLineCount)
	{
		// ɾ���������5��
		int iFirstLineLength = objEdit->LineLength(0);
		int iLinesToRemove = 5; // Ҫɾ��������
		int iCharsToRemove = 0;

		for (int i = 0; i < iLinesToRemove; ++i)
		{
			iCharsToRemove += objEdit->LineLength(i);
		}

		objEdit->SetSel(0, iCharsToRemove);
		objEdit->ReplaceSel(_T(""));
	}

	objEdit->SetSel(iEditLen, iEditLen);
	objEdit->ReplaceSel(msg);

	// ���������·�
	objEdit->LineScroll(objEdit->GetLineCount());

	return 0;
}


int CLogEdit::LOGPT(LPCWSTR TAG, LPCWSTR fmt, ...)
{
	CString msg;
	CTime tTimeNow = CTime::GetCurrentTime();
	int iEditLen, iSize = 0;

	memset(buff, 0, sizeof(buff));
	va_list args;
	va_start(args, fmt);
	iSize = _vsnwprintf_s(buff, MaxBufSize, _TRUNCATE, fmt, args);
	va_end(args);

	msg.Format(_T("[%s][%s]:%s\r\n"), tTimeNow.Format(_T("%H:%M:%S")), TAG, buff);
	iEditLen = objEdit->GetWindowTextLength();
	objEdit->SetSel(iEditLen, iEditLen);
	objEdit->ReplaceSel(msg);

	return 0;
}

void CLogEdit::Init(CEdit * pEdit)
{
	objEdit = pEdit;
	objEdit->SetWindowTextW(_T(""));
}

int CLogEdit::append(LPCWSTR fmt, ...)
{
	return 0;
}

CLogEdit::CLogEdit(const CLogEdit & other)
{
	objEdit = other.objEdit;
}

CLogEdit & CLogEdit::operator=(const CLogEdit &other)
{
	// TODO: �ڴ˴����� return ���
	this->objEdit = other.objEdit;
	return *this;
}

CLogEdit::CLogEdit()
{
	objEdit = NULL;
}
CLogEdit::~CLogEdit()
{
	objEdit = NULL;
}


