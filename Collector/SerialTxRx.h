#pragma once
#include <afxwin.h>         // MFC ��������ͱ�׼���
#include <afxext.h>         // MFC ��չ
#include <afxdisp.h>        // MFC �Զ�����

#define  INCOMMAX   4096     //�����������ݳ���
#define  OUTCOMMAX  4096	 //����������ݳ���

extern BYTE  i8UartRcvData[INCOMMAX];     //�������ݻ���ռ�
extern BYTE  i8UartSendData[OUTCOMMAX];   //�������ݻ���ռ�

class SerialTxRx
{
public:
	SerialTxRx(); //����
	SerialTxRx(
		CString strCom,
		DWORD dwBaudRate = 9600,
		BYTE byteSize = 8,
		BYTE byteCheck = NOPARITY,
		BYTE byteStop = ONESTOPBIT,
		BOOL bSync = FALSE);
	~SerialTxRx();//������ɾ����
	inline SerialTxRx(const SerialTxRx &com)
	{
		m_hCom = com.m_hCom;
		m_hEventSend = com.m_hEventSend;
		m_hEventRecv = com.m_hEventRecv;
		m_bSync = com.m_bSync;
		m_bIsFirst = com.m_bIsFirst;
		m_strCom = com.m_strCom;
		m_dwBaudRate = com.m_dwBaudRate;
		m_byteSize = com.m_byteSize;
		m_byteCheck = com.m_byteCheck;
		m_byteStop = com.m_byteStop;
		m_dwInQueue = com.m_dwInQueue;
		m_dwOutQueue = com.m_dwOutQueue;
		m_Timeouts = com.m_Timeouts;
		m_nInitResult = com.m_nInitResult;
		m_pnReusecount = com.m_pnReusecount;
		if (m_pnReusecount)
		{
			(*m_pnReusecount)++;
		}
	}
	inline SerialTxRx & operator = (const SerialTxRx &com)
	{
		m_hCom = com.m_hCom;
		m_hEventSend = com.m_hEventSend;
		m_hEventRecv = com.m_hEventRecv;
		m_bSync = com.m_bSync;
		m_bIsFirst = com.m_bIsFirst;
		m_strCom = com.m_strCom;
		m_dwBaudRate = com.m_dwBaudRate;
		m_byteSize = com.m_byteSize;
		m_byteCheck = com.m_byteCheck;
		m_byteStop = com.m_byteStop;
		m_dwInQueue = com.m_dwInQueue;
		m_dwOutQueue = com.m_dwOutQueue;
		m_Timeouts = com.m_Timeouts;
		m_nInitResult = com.m_nInitResult;
		m_pnReusecount = com.m_pnReusecount;
		if (m_pnReusecount)
		{
			(*m_pnReusecount)++;
		}
		return *this;
	}
private:
	int *m_pnReusecount;
	BOOL m_bIsFirst;         //�Ƿ��ǵ�һ�γɹ��򿪴���
	HANDLE m_hCom;           //���ھ��
	HANDLE m_hEventSend;     //���������¼�
	HANDLE m_hEventRecv;     //���������¼�

	BOOL m_bSync;            //ͬ�����仹���첽����,TRUE��Ϊͬ����FALSEΪ�첽,Ĭ��Ϊͬ��

	CString m_strCom;        //���ڶ˿�
	DWORD m_dwBaudRate;      //������
	BYTE m_byteSize;         //����λ
	BYTE m_byteCheck;        //У�鷽ʽ
	BYTE m_byteStop;         //ֹͣλ

	DWORD m_dwInQueue;       //�������뻺������С     
	DWORD m_dwOutQueue;     //���������������С
							//��ʱ��ر���
	COMMTIMEOUTS m_Timeouts;

	int m_nInitResult;		//����״̬ 0��ʾû�д������ƣ�1��ʾ�򿪳ɹ���MAXINI32��ʾ���ڴ򿪴���
public:
	DWORD WriteData(unsigned char *pBuffer, DWORD dwLength, DWORD dwTimeout = 1000);  //dwTimeoutΪռλ������ʱδ�ã����أ�MAXDWORD��ʾд�����
	DWORD ReadData(unsigned char *pBuffer, DWORD dwLength, DWORD dwTimeout = 1000);   //dwTimeoutΪռλ������ʱδ�ã����أ�MAXDWORD��ʾ��ȡ����
	void CloseCom();
	int InitCom();//����1����ʾû�д��󣬷���������ʾ����
	void ClearError();
	void Purge(DWORD dwFlags);

	int SetComString(CString strCom);
	int SetBaudRate(DWORD dwBaudRate);
	int SetByteSize(BYTE byteSize);
	int SetCheck(BYTE byteCheck);
	int SetStopBit(BYTE byteStopBit);
	int SetSync(BOOL bSync);
	int SetInQue(DWORD dwInQue);
	int SetOutQue(DWORD dwOutQue);
	int SetTimeouts(COMMTIMEOUTS timeouts);
	BOOL gClearCommError(DWORD* dwErrorFlags, COMSTAT* ComStat);
	DWORD sWaitCommEvent(void);


	CString GetComString();
	DWORD GetBaudRate();
	BYTE GetByteSize();
	BYTE GetCheck();
	BYTE GetStopBit();
	BOOL GetSync();
	DWORD GetInQue();
	DWORD GetOutQue();
	int GetComStatus();//InitCom�ķ���ֵ,�۲촮���Ƿ�򿪳ɹ�,0��ʾû�д������ƣ�1��ʾ�򿪳ɹ���MAXINI32��ʾ���ڴ򿪴���
};