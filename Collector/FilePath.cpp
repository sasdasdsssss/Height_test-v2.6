#include "stdafx.h"


#include "FilePath.h"
#include"SysTime.h"

using namespace std;

int numberone = 1;

//文件夹路径
CString folderOne;
CString folderThree;


CString stringhour;
CString stringminute;
CTime tm;
//时间函数
SysTime* timePort;     //定义时间类变量 
CString timeYmd;
CString timeHms;

//(1)保存视频
void Createfolder() //01创建文件夹
{
	tm = CTime::GetCurrentTime();         //获取当前系统时间
	timePort->timeYmd(YmdTime);//时间函数
	timeYmd = YmdTime;//获取当前日期
	timePort->timeHms(HmsTime);//时间函数
	timeHms = HmsTime;//获取当前时间
	stringhour = tm.Format(_T("%H时"));
	stringminute = tm.Format(_T("%H时%M分"));

	folderOne = (TEXT("D:\\元感科技\\高度表数据\\"));
	if (!PathIsDirectory(folderOne))
	{
		CreateDirectory(folderOne, 0);
	}



}

//002-存储原始数据
void ReadFile(unsigned char* RecvData, int dataLength, int number)
{

	CFile mFile;
	CString FileName = folderThree + "\\" + timeHms + _T(".txt");//文件的文件名
	CFileFind FileFind;

	timePort->timeHms(HmsTime);//时间函数
	timeHms = HmsTime;//获取当前时间


	if (FileFind.FindFile(FileName))//找不到这个文件
	{
		mFile.Open(FileName, CFile::modeNoTruncate | CFile::modeReadWrite);//以保留模式打开
	}
	else
	{
		mFile.Open(FileName, CFile::modeCreate | CFile::modeReadWrite);//以新建模式打开
	}


	mFile.SeekToEnd();//定位末尾位置
	mFile.Write(RecvData, 136);//写入数据
	numberone = numberone + 1;
	mFile.Close();

}

//(3)查找文件夹解析原始数据
void ReadFolder()
{
	CFile mFile;
	CFileFind ff;
	CString strFile;//获取文件的路径
					//当前的日期(实时)
	timePort->timeYmd(YmdTime);//时间函数
	timeYmd = YmdTime;//获取当前日期

					  //读取16进制数据缓冲区
	BYTE FileNum[5000];

	CString strUDDir = _T("D:\\元感科技\\采集卡数据\\") + timeYmd + _T("\\");//查找该文件下的所有文件

	ShellExecute(NULL, _T("open"), strUDDir, NULL, NULL, SW_SHOWNORMAL);//弹出这个文件夹

	BOOL res = ff.FindFile(strUDDir + _T("*.csv"));//查找该文件夹下的所有.csv文件
	while (res)
	{
		FileNum[5000] = { 0 }; //初始化读取数组  大小==136*帧数
		res = ff.FindNextFile();//查找下一个文件
		strFile = ff.GetFilePath();//获取当前文件路径
		mFile.Open(strFile, CFile::modeReadWrite);//以可读可写模式打开
		mFile.Read(FileNum, sizeof(FileNum));//读取当前文件并把数据全部保存在Byte数组中

											 //测试
		int f = sizeof(FileNum);
		int a = FileNum[4078];//帧尾
		int b = FileNum[4079];
		int c = FileNum[0];//帧头
		int d = FileNum[1];

		//CA2W a2w();
		//CString CCCC = a2w;
		mFile.Close();
	}
	ff.Close(); // 关闭文件查找
}