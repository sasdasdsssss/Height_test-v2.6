#include "stdafx.h"


#include "FilePath.h"
#include"SysTime.h"

using namespace std;

int numberone = 1;

//�ļ���·��
CString folderOne;
CString folderThree;


CString stringhour;
CString stringminute;
CTime tm;
//ʱ�亯��
SysTime* timePort;     //����ʱ������� 
CString timeYmd;
CString timeHms;

//(1)������Ƶ
void Createfolder() //01�����ļ���
{
	tm = CTime::GetCurrentTime();         //��ȡ��ǰϵͳʱ��
	timePort->timeYmd(YmdTime);//ʱ�亯��
	timeYmd = YmdTime;//��ȡ��ǰ����
	timePort->timeHms(HmsTime);//ʱ�亯��
	timeHms = HmsTime;//��ȡ��ǰʱ��
	stringhour = tm.Format(_T("%Hʱ"));
	stringminute = tm.Format(_T("%Hʱ%M��"));

	folderOne = (TEXT("D:\\Ԫ�пƼ�\\�߶ȱ�����\\"));
	if (!PathIsDirectory(folderOne))
	{
		CreateDirectory(folderOne, 0);
	}



}

//002-�洢ԭʼ����
void ReadFile(unsigned char* RecvData, int dataLength, int number)
{

	CFile mFile;
	CString FileName = folderThree + "\\" + timeHms + _T(".txt");//�ļ����ļ���
	CFileFind FileFind;

	timePort->timeHms(HmsTime);//ʱ�亯��
	timeHms = HmsTime;//��ȡ��ǰʱ��


	if (FileFind.FindFile(FileName))//�Ҳ�������ļ�
	{
		mFile.Open(FileName, CFile::modeNoTruncate | CFile::modeReadWrite);//�Ա���ģʽ��
	}
	else
	{
		mFile.Open(FileName, CFile::modeCreate | CFile::modeReadWrite);//���½�ģʽ��
	}


	mFile.SeekToEnd();//��λĩβλ��
	mFile.Write(RecvData, 136);//д������
	numberone = numberone + 1;
	mFile.Close();

}

//(3)�����ļ��н���ԭʼ����
void ReadFolder()
{
	CFile mFile;
	CFileFind ff;
	CString strFile;//��ȡ�ļ���·��
					//��ǰ������(ʵʱ)
	timePort->timeYmd(YmdTime);//ʱ�亯��
	timeYmd = YmdTime;//��ȡ��ǰ����

					  //��ȡ16�������ݻ�����
	BYTE FileNum[5000];

	CString strUDDir = _T("D:\\Ԫ�пƼ�\\�ɼ�������\\") + timeYmd + _T("\\");//���Ҹ��ļ��µ������ļ�

	ShellExecute(NULL, _T("open"), strUDDir, NULL, NULL, SW_SHOWNORMAL);//��������ļ���

	BOOL res = ff.FindFile(strUDDir + _T("*.csv"));//���Ҹ��ļ����µ�����.csv�ļ�
	while (res)
	{
		FileNum[5000] = { 0 }; //��ʼ����ȡ����  ��С==136*֡��
		res = ff.FindNextFile();//������һ���ļ�
		strFile = ff.GetFilePath();//��ȡ��ǰ�ļ�·��
		mFile.Open(strFile, CFile::modeReadWrite);//�Կɶ���дģʽ��
		mFile.Read(FileNum, sizeof(FileNum));//��ȡ��ǰ�ļ���������ȫ��������Byte������

											 //����
		int f = sizeof(FileNum);
		int a = FileNum[4078];//֡β
		int b = FileNum[4079];
		int c = FileNum[0];//֡ͷ
		int d = FileNum[1];

		//CA2W a2w();
		//CString CCCC = a2w;
		mFile.Close();
	}
	ff.Close(); // �ر��ļ�����
}