
// CollectorDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Collector.h"
#include "CollectorDlg.h"
#include "afxdialogex.h"
#include "FilePath.h"
#include "SysTime.h"
#include <math.h>
#include "Global.h"
#include <Dbt.h>


#ifdef _DEBUG
#define new DEBUG_NEW
#endif
CMFCButton *objTempBtn;
CEdit *objRecBtn;


// CCollectorDlg 对话框
CCollectorDlg::CCollectorDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_COLLECTOR_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CCollectorDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CUSTOM1, m_ChartCtrl);
	DDX_Control(pDX, IDC_COMBO1, objComBox1);
	DDX_Control(pDX, IDC_COMBO2, objComBox2);
	DDX_Control(pDX, IDC_BUTTON1, objOpenBtn);
	DDX_Control(pDX, IDC_BUTTON2, objStartBtn);
	DDX_Control(pDX, IDC_MFCBUTTON1, m_MFCButton);
}

BEGIN_MESSAGE_MAP(CCollectorDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_CBN_DROPDOWN(IDC_COMBO1, &CCollectorDlg::OnCbnDropdownCombo1)
	ON_CBN_DROPDOWN(IDC_COMBO2, &CCollectorDlg::OnCbnDropdownCombo2)
	ON_BN_CLICKED(IDC_BUTTON1, &CCollectorDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CCollectorDlg::OnBnClickedButton2)
	ON_WM_DEVICECHANGE()
	ON_WM_TIMER()
	ON_WM_DESTROY()

	
END_MESSAGE_MAP()


// CCollectorDlg 消息处理程序


BOOL CCollectorDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	
	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	objTempBtn = &m_MFCButton;
	objTempBtn->m_bTransparent = FALSE;
	objTempBtn->m_bDontUseWinXPTheme = TRUE;
	objTempBtn->SetFaceColor(RGB(255, 255, 255));

	//修改字体大小
	cfont1.CreatePointFont(200, _T("宋体。"),NULL);
	GetDlgItem(IDC_BUTTON2)->SetFont(&cfont1);

//初始化目标显示坐标轴
	pAxis = m_ChartCtrl.CreateStandardAxis(CChartCtrl::BottomAxis);	//底部坐标轴
	//pAxis->SetAutomatic(true);	
	//根据具体数值自动调节坐标轴坐标，如果要固定坐标轴需要将其参数修改为false
	pAxis->SetMinMax(-1, 1);
	//pAxis->SetAutomatic(true);
		
	pAxis = m_ChartCtrl.CreateStandardAxis(CChartCtrl::LeftAxis);	//左边坐标轴
	pAxis->SetMinMax(0, 800);	//固定y轴坐标为0-100
	//pAxis->SetAutomatic(true);
	

	//添加标题
	TChartString str1;
	str1 = _T("雷达目标显示");
	m_ChartCtrl.GetTitle()->AddString(str1);
	//设置颜色
	m_ChartCtrl.GetTitle()->SetColor(RGB(255, 255, 255));   //标题字体白色
	m_ChartCtrl.GetLeftAxis()->SetTextColor(RGB(255, 255, 255));  //左坐标轴白色
	m_ChartCtrl.GetBottomAxis()->SetTextColor(RGB(255, 255, 255));  //底部坐标轴白色
	m_ChartCtrl.SetBorderColor(RGB(255, 255, 255));  //边框颜色白色
	m_ChartCtrl.SetBackColor(RGB(85, 85, 85));  //背景颜色深灰色

	pPointSerie1 = m_ChartCtrl.CreatePointsSerie();	//绑定绘图变量
	objLogEdit.Init((CEdit*)GetDlgItem(IDC_EDIT1));
	objLogEdit.LOGP(_T("初始化成功"));
	Createfolder();

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}



// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CCollectorDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CCollectorDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

/***********************************************
**函数名：OnCbnDropdownCombo1
**功能：选择串口号
**输入参数1：无
**输出参数1：无
**作者：tdw
**时间：2023.2.28
************************************************/

unsigned char u8RecvBuffer[INCOMMAX] = { 0 };			// 串口接收的缓冲区
int sfDatacnt = 0;//多余帧数量
CString strTime1;
CString strHour1;
CString strMinute1;
CString strSecond1;
CString strData;
CTime tm1;

void CCollectorDlg::OnCbnDropdownCombo1()
{
	LPCTSTR regDir = _T("HARDWARE\\DEVICEMAP\\SERIALCOMM");					// 串口设备的路径
	HKEY hKEY;																// 注册表句柄
	DWORD dwIndex = 0;														// 注册表值的索引
	DWORD uRegState = ERROR_SUCCESS;										// 读取注册表的状态,默认为成功

	if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, regDir, 0, KEY_READ, &hKEY))		// 打开注册表
	{
		MessageBox(_T("错误: 无法打开有关的hKEY!"));
		return;
	}

	objComBox1.ResetContent();											// 先清空下拉框再做添加

	while (uRegState == ERROR_SUCCESS)
	{

		TCHAR ucBuffer[_MAX_PATH + 1] = {};									// 一个缓冲区的指针
		DWORD ulufferSize = sizeof(ucBuffer);								// 缓冲区大小
		TCHAR uComName[255] = {};											// 串口名称缓冲区
		DWORD uComNameSize = sizeof(uComName);								// 串口名称长

		uRegState = RegEnumValue(hKEY, dwIndex, ucBuffer, &ulufferSize, NULL, NULL, reinterpret_cast<LPBYTE>(uComName), &uComNameSize);
		if (uRegState == ERROR_SUCCESS)
		{
			objComBox1.AddString(uComName);
			dwIndex++;
		}
	}

	RegCloseKey(hKEY);														//关闭注册表
}

/***********************************************
**函数名：OnCbnDropdownCombo2
**功能：选择命令码
**输入参数1：无
**输出参数1：无
**作者：ZJC
**时间：2023.8.18
************************************************/

void CCollectorDlg::OnCbnDropdownCombo2()
{
	objComBox2.ResetContent();											// 先清空下拉框再做添加

	CString strCmdName1 = _T("01");  // cmd
	CString strCmdName2 = _T("02");  
	CString strCmdName3 = _T("03");  

	objComBox2.AddString(strCmdName1);  
	objComBox2.AddString(strCmdName2); 
	objComBox2.AddString(strCmdName3);
}


/***********************************************
**函数名：OnBnClickedButton1
**功能：打开串口
**输入参数1：无
**输出参数1：无
**作者：tdw
**时间：2023.2.28
************************************************/
void CCollectorDlg::OnBnClickedButton1()
{
	CString  sComName;
	CString sBaudRate;
	CString stData;
	int errCode;
	objComBox1.GetWindowTextW(sComName);
	objOpenBtn.GetWindowTextW(stData);

	if (stData == _T("打开串口"))
	{
		CSerial = new SerialTxRx(sComName, 115200, 8, 0, 0);
		CSerial->Purge(PURGE_RXCLEAR);								//清除接收缓冲区的数据，进行初始化		
		errCode = CSerial->GetComStatus();

		if (errCode == 1)   //配置成功
		{
			objLogEdit.LOGP(_T("打开串口成功"));
			objOpenBtn.SetWindowTextW(_T("关闭串口"));
		}
		else if (errCode == 0)  //未配置串口号
		{
			MessageBox(_T("请配置串口"));
		}
	}
	if (stData == _T("关闭串口"))
	{
		if (CSerial != NULL)
		{
			CSerial->~SerialTxRx();
		}
		objOpenBtn.SetWindowTextW(_T("打开串口"));
		objLogEdit.LOGP(_T("关闭串口成功"));
	}


}

/***********************************************
**函数名：OnBnClickedButton2
**功能：开始采集
**输入参数1：无
**输出参数1：无
**作者：tdw
**时间：2023.2.28
************************************************/
CString sContent;
void CCollectorDlg::OnBnClickedButton2()
{

	objStartBtn.GetWindowTextW(sContent);
	if (sContent == _T("开始接收数据"))
	{
		if (CSerial == NULL)
		{
			MessageBox(_T("串口未打开！"));   //没开串口 点这个会闪退
		}
		else
		{
			CSerial->Purge(PURGE_RXCLEAR); //开始时先清空串口缓冲区数据
			SetTimer(RXTIMER, CONSTTIME_MS, NULL);
			objStartBtn.SetWindowTextW(_T("停止接收数据"));
			objTempBtn->m_bTransparent = FALSE;
			objTempBtn->m_bDontUseWinXPTheme = TRUE;
			objTempBtn->SetFaceColor(RGB(255, 255, 255));
			//根据开始接收的时间 创建文件夹 存储数据 
			tm1 = CTime::GetCurrentTime();         //获取当前系统时间
			strTime1 = tm1.Format(_T("%Y-%m-%d"));
			strHour1 = tm1.Format(_T("%H"));
			strMinute1 = tm1.Format(_T("%H时%M分"));
			strSecond1 = tm1.Format(_T("%H-%M-%S"));
			strData = tm1.Format("%y年%m月%d日 %X");
			objComBox2.EnableWindow(FALSE);     //下拉框锁定
		}
	}
	else   //停止接收数据
	{
		KillTimer(RXTIMER);
		memset(u8RecvBuffer,0,sizeof(u8RecvBuffer));
		sfDatacnt = 0;
		objStartBtn.SetWindowTextW(_T("开始接收数据"));
		objComBox2.EnableWindow(TRUE);          //解锁下拉框 	
	}
}


/***********************************************
**函数名：OnTimer
**功能：定时器任务
**输入参数1：定时器号
**输出参数1：无
**作者：tdw
**时间：2023.2.28
************************************************/
void CCollectorDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	COMSTAT  ComStat;										// 串口状态缓冲区
	unsigned int u16DataLen;								// 保存串口缓冲区的长度
	DWORD dwErrorFlags;										// 串口的错误类型,暂不处理
	unsigned char comDataBuffer[25] = { 0 };			// 正常帧数组

	CString sTAG;
	int rightLength = 0;
	int comDatacnt = 0;
	int PraseData2;
	double realHight = 0;
	double realSpeed = 0;
	int framenum = 0;

	CString sCmdName;
	objComBox2.GetWindowTextW(sCmdName);

	if (nIDEvent == RXTIMER)								// 串口接收的任务
	{
		KillTimer(RXTIMER);									// 串口只接收一次, 无需重复发送
		CSerial->gClearCommError(&dwErrorFlags, &ComStat);	// 获取串口状态
	

		/**1、读取采集数据**/
		u16DataLen = ComStat.cbInQue > INCOMMAX ? INCOMMAX : ComStat.cbInQue;

		if (u16DataLen >= 8)
		{
			CSerial->ReadData(&u8RecvBuffer[0], u16DataLen, NULL);
			CSerial->Purge(PURGE_RXCLEAR);
			uDatacnt = u16DataLen;
		}
		else {
			uDatacnt = 0;
		}
		
		//按命令码
		if (sCmdName == _T("01")) { framenum = 8; }
		if (sCmdName == _T("02")) { framenum = 10; }
		if (sCmdName == _T("03")) { framenum = 22; }
		

		
		

		SetTimer(RXTIMER, CONSTTIME_MS, NULL);

		 if (uDatacnt >0)
		{
			rightLength = RxDataCrc(&u8RecvBuffer[0], uDatacnt);//CRC校验并返回暂时正确数据长度
			comDatacnt = (rightLength / framenum) * framenum;  //完整帧数量
			sfDatacnt = rightLength - comDatacnt;//多余帧数量

			//将多余帧的数据保存到下一次数据进行拼接
			memcpy(&comDataBuffer[0], &u8RecvBuffer[0], framenum);//正常帧数据，均是从头开始
			//memcpy(&u8RecvBuffer[0], &u8RecvBuffer[comDatacnt], sfDatacnt);//多余帧数据保留到下一次数据更新接上


			if (comDataBuffer[2] == 0x80)
			{
				//数据有效，进行B3/B4数据组合，需要有控件显示数据有效并且将高度画在框里
				objTempBtn->m_bTransparent = FALSE;
				objTempBtn->m_bDontUseWinXPTheme = TRUE;
				objTempBtn->SetFaceColor(RGB(34, 139, 34));
				
				PraseData2 = DealDataAnalyze(&comDataBuffer[0], comDatacnt);
				WriteRawData(&comDataBuffer[0], comDatacnt, strSecond1);
				realHight = DealData1 * 0.01;
				realSpeed = DealData2 * 0.15;
				acc_x *= 156.8 / 32768;
				acc_y *= 156.8 / 32768;
				acc_z *= 156.8 / 32768;
				rotation_x *= 2000.0 / 32768;
				rotation_y *= 2000.0 / 32768;
				rotation_z *= 2000.0 / 32768;
				objLogEdit.LOGP(_T("高度为: %.2f m\n"), realHight);
				objLogEdit.LOGP(_T("速度为: %.2f m/s\n"), realSpeed);
				
				if (sCmdName == _T("03"))
				{ 
					objLogEdit.LOGP(_T("x加速度为: %.2f m/s\u00B2\n"), acc_x);  // \u00B2表示平方
					objLogEdit.LOGP(_T("y加速度为: %.2f m/s\u00B2\n"), acc_y);
					objLogEdit.LOGP(_T("z加速度为: %.2f m/s\u00B2\n"), acc_z);
					objLogEdit.LOGP(_T("x旋转度为: %.2f °/s\u00B2\n"), rotation_x);
					objLogEdit.LOGP(_T("y旋转度为: %.2f °/s\u00B2\n"), rotation_y);
					objLogEdit.LOGP(_T("z旋转度为: %.2f °/s\u00B2\n"), rotation_z);
				}
				
				objLogEdit.LOGP(_T("\n"));          // 8.7  LOGP定义改
				//绘图
				DrawTarget(realHight, RGB(255,0,0));
				CSerial->Purge(PURGE_RXCLEAR);

			}
			else if (comDataBuffer[2] == 0x00)
			{
				//数据无效，无需组合B3/B4，将数据无效显示在控件中
				comDataBuffer[3] = 0;
				comDataBuffer[4] = 0;

				WriteRawData(&comDataBuffer[0], comDatacnt, strSecond1);
				objTempBtn->m_bTransparent = FALSE;
				objTempBtn->m_bDontUseWinXPTheme = TRUE;
				objTempBtn->SetFaceColor(RGB(165, 42, 42));
				objLogEdit.LOGP(_T("数据无效"));
				DrawTarget(0, RGB(85, 85, 85));
				CSerial->Purge(PURGE_RXCLEAR);
			}
		}	
		 
	}
}


void CCollectorDlg::OnDestroy()
{
	CDialogEx::OnDestroy();
	if (CSerial != NULL)
	{
		CSerial->~SerialTxRx();
	}
}

/***********************************************
**函数名：RxDataCrc
**功能：对原始数据进行CRC校验，并删除错误帧,返回正确帧数量
**输入参数1：pBuffer   数据缓存
**输入参数2：u16DataLen  数据长度
**输出参数1：错误数据量
**作者：GQ
**时间：2023.3.24
************************************************/
int  CCollectorDlg::RxDataCrc(BYTE* pBuffer, unsigned int u16DataLen)
{
	int reDataLen = 0;
	int HeadnNum = 0;
	int crcSum = 0;
	int framenum = 0;
	CString sCmdName;
	objComBox2.GetWindowTextW(sCmdName);

	if (sCmdName == _T("01")) { framenum = 8; }
	if (sCmdName == _T("02")) { framenum = 10; }
	if (sCmdName == _T("03")) { framenum = 22; }

	//查找头帧
	for (int i = 0; i < framenum; i++)
	{
		if (pBuffer[i] == 0xaa)
		{
			if (pBuffer[i + 1] == 0xaa)
			{
				HeadnNum = i + 1;
				break;
			}
			else
			{
				HeadnNum = i;
				break;
			}
		}
	}

	//平移数组
	for (int i = 0; i < u16DataLen; i++)
	{
		pBuffer[i] = pBuffer[i + HeadnNum];  //此处数据长度以减小HeadnNum
	}

	//计算剩余数组长度
	int NewNum = (u16DataLen - HeadnNum);
	return NewNum;
}

/***********************************************
**函数名：DealDataAnalyze
**功能：对需要处理的原始数据进行组合
**输入参数1：pBuffer   数据缓存
**输入参数2：u16DataLen  数据长度
**输出参数1：无
**作者：GQ
**时间：2023.3.9
************************************************/
int  CCollectorDlg::DealDataAnalyze(BYTE* pBuffer, unsigned int u16DataLen)
{
	int reData = 0;
	double data[6] = { 0 };
	CString sCmdName;
	objComBox2.GetWindowTextW(sCmdName);
	
	if (sCmdName == _T("01"))
	{
		DealData1 = (pBuffer[3] << 8) + pBuffer[4];
		DealData2 = (pBuffer[5] << 8) + pBuffer[6];
	}

	else if (sCmdName == _T("02"))
	{
		DealData1 = (pBuffer[3] << 24) + (pBuffer[4] << 16) + (pBuffer[5] << 8) + (pBuffer[6]);
		DealData2 = (pBuffer[7] << 8) + pBuffer[8];
	}

	else if (sCmdName == _T("03"))
	{
		DealData1 = (pBuffer[3] << 24) + (pBuffer[4] << 16) + (pBuffer[5] << 8) + (pBuffer[6]);
		DealData2 = (pBuffer[7] << 8) + pBuffer[8];
		acc_x = (pBuffer[9] << 8) + pBuffer[10];
		acc_y = (pBuffer[11] << 8) + pBuffer[12];
		acc_z = (pBuffer[13] << 8) + pBuffer[14];
		rotation_x = (pBuffer[15] << 8) + pBuffer[16];
		rotation_y = (pBuffer[17] << 8) + pBuffer[18];
		rotation_z = (pBuffer[19] << 8) + pBuffer[20];

		data[0] = acc_x;		data[1] = acc_y;		data[2] = acc_z;
		data[3] = rotation_x;	data[4] = rotation_y;	data[5] = rotation_z;

		for (int i = 0; i < 6; i++)
		{
			if (data[i] > 32767)
			{
				data[i] -= 65536;
			}
		}
	}
	
	//速度负数的情况
	if (DealData2 > 32767)
	{
		DealData2 -= 65536;
	}

	acc_x = data[0];
	acc_y = data[1];
	acc_z = data[2];
	rotation_x = data[3];
	rotation_y = data[4];
	rotation_z = data[5];

	
	return reData;
}

/***********************************************
**函数名：DrawTarget
**功能：根据传入的高度数据画图
**输入参数1：hight    高度数据
**输入参数2：crColor  颜色参数
**输出参数1：无
**作者：ZJC
**时间：2023.8.3
************************************************/
void CCollectorDlg::DrawTarget(double hight, COLORREF crColor)
{
	double Rec_Hight = hight;
	m_ChartCtrl.SetZoomEnabled(true);
	m_ChartCtrl.RemoveAllSeries();//先清空
	pPointSerie1 = m_ChartCtrl.CreatePointsSerie();	//绑定绘图变量
	pPointSerie1->SetSeriesOrdering(poNoOrdering);//设置为无序
	m_ChartCtrl.EnableRefresh(false);
	pPointSerie1->SetPointSize(25, 25);
	pPointSerie1->SetColor(crColor); //红色	
	pPointSerie1->AddPoint(0, Rec_Hight);
	//pPointSerie1->AddPoints(XValues, YValues, LastTargetNum);
	m_ChartCtrl.EnableRefresh(true);
}





//保存数据   一天一个文件夹
void CCollectorDlg::WriteRawData(unsigned char* pBuffer, unsigned int u16DataLen, CString time)
{
	CFile mFile;
	CFileFind FindFile;
	CString Line_break = _T("\r\n"); // 换行
	CString Space = _T("    ");      // 空格
	CString strSecondnow;
	double aData = 0.0;
	double bData = 0.0;
	double cData = 0.0;
	double dData = 0.0;
	double eData = 0.0;
	double fData = 0.0;
	double gData = 0.0;
	double hData = 0.0;
	double data[7] = {0};

	tm1 = CTime::GetCurrentTime();
	strSecondnow = tm1.Format(_T("时间：%H:%M:%S"));

	USES_CONVERSION;
	char* time1 = T2A(strTime1);
	// 将原始数据写入Bin文件中
	CString filename1 = _T("D:\\元感科技");
		if (!PathIsDirectory(filename1))
		{
			CreateDirectory(filename1, NULL); // 不存在则创建文件
		}
	CString filename2 = _T("D:\\元感科技\\高度表数据\\");
		if (!PathIsDirectory(filename2))
		{
			CreateDirectory(filename2, NULL); // 不存在则创建文件
		}


	CString filename0000 = _T("D:\\元感科技\\高度表数据\\") + strTime1 + _T("\\"); // 文件的文件名,日期
	if (!PathIsDirectory(filename0000))
	{
		CreateDirectory(filename0000, NULL); // 不存在则创建文件
	}

	CString filename00 = filename0000 + time;
	CreateDirectory(filename00, NULL);

	CString filename000 = filename00 + _T("\\") + _T("Hight_Data.txt");
	if (FindFile.FindFile(filename000)) // 找到这个文件
	{
		mFile.Open(filename000, CFile::modeNoTruncate | CFile::modeReadWrite); // 以保留模式打开
	}
	else // 找不到
	{
		mFile.Open(filename000, CFile::modeCreate | CFile::modeReadWrite); // 以新建模式打开
	}
	mFile.SeekToEnd();
	wchar_t unicode = 0xFEFF; // 添加这个文件头，记事本打开才能识别里面的汉字，不会乱码。
	mFile.Write(&unicode, 2);

	// 写入时间
	mFile.Write(strSecondnow, wcslen(strSecondnow) * sizeof(wchar_t));
	mFile.Write(Space, wcslen(Space) * sizeof(wchar_t));

	// 写入数据
	if (pBuffer[2] == 0x80) // 数据有效
	{
		// 组合B3/B4数据
		if (pBuffer[1] == 0x01) 
		{
			aData = static_cast<double>((pBuffer[3] << 8) + pBuffer[4]) * 0.01;
			bData = static_cast<double>((pBuffer[5] << 8) + pBuffer[6]);
			if (bData > 32767)
			{
				bData -= 65536;
			}
		}

		else if (pBuffer[1] == 0x02)
		{
			aData = static_cast<double>((pBuffer[3] << 24) + (pBuffer[4] << 16) + (pBuffer[5] << 8) + (pBuffer[6])) * 0.01;
			bData = static_cast<double>((pBuffer[7] << 8) + pBuffer[8]);
			if (bData > 32767)
			{
				bData -= 65536;
			}
		}

		else if (pBuffer[1] == 0x03)
		{
			aData = static_cast<double>((pBuffer[3] << 24) + (pBuffer[4] << 16) + (pBuffer[5] << 8) + (pBuffer[6])) * 0.01;
			bData = static_cast<double>((pBuffer[7] << 8) + pBuffer[8]);
			cData = static_cast<double>((pBuffer[9] << 8) + pBuffer[10]);
			dData = static_cast<double>((pBuffer[11] << 8) + pBuffer[12]);
			eData = static_cast<double>((pBuffer[13] << 8) + pBuffer[14]);
			fData = static_cast<double>((pBuffer[15] << 8) + pBuffer[16]);
			gData = static_cast<double>((pBuffer[17] << 8) + pBuffer[18]);
			hData = static_cast<double>((pBuffer[19] << 8) + pBuffer[20]);

			data[0] = bData;
			data[1] = cData;
			data[2] = dData;
			data[3] = eData;
			data[4] = fData;
			data[5] = gData;
			data[6] = hData;

			for (int i = 0; i < 7; i++)
			{
				if (data[i] > 32767)
				{
					data[i] -= 65536;
				}
			}
		}	


		// 更新变量的值
		bData = data[0] * 0.15;
		cData = data[1] * 156.8 / 32768;
		dData = data[2] * 156.8 / 32768;
		eData = data[3] * 156.8 / 32768;
		fData = data[4] * 2000.0 / 32768;
		gData = data[5] * 2000.0 / 32768;
		hData = data[6] * 2000.0 / 32768;



		// 将DealData写入文件
		CString strDealData1;
		CString strDealData2;
		strDealData1.Format(_T("高度：%.2f m"), aData);
		strDealData2.Format(_T("速度：%.2f m/s"), bData);
		mFile.Write(strDealData1, wcslen(strDealData1) * sizeof(wchar_t));
		mFile.Write(Space, wcslen(Space) * sizeof(wchar_t));
		mFile.Write(strDealData2, wcslen(strDealData2) * sizeof(wchar_t));
		mFile.Write(Space, wcslen(Space) * sizeof(wchar_t));
		if (pBuffer[1] == 0x03)
		{
			CString strData[6];
			strData[0].Format(_T("x加速度：%.2f m/s\u00B2"), cData);  // \u00B2表示平方
			strData[1].Format(_T("y加速度：%.2f m/s\u00B2"), dData);
			strData[2].Format(_T("z加速度：%.2f m/s\u00B2"), eData);
			strData[3].Format(_T("x旋转度：%.2f °/s\u00B2"), fData);
			strData[4].Format(_T("y旋转度：%.2f °/s\u00B2"), gData);
			strData[5].Format(_T("z旋转度：%.2f °/s\u00B2"), hData);

			for (int i = 0; i < 6; i++)
			{
				mFile.Write(strData[i], wcslen(strData[i]) * sizeof(wchar_t));
				mFile.Write(Space, wcslen(Space) * sizeof(wchar_t));
			}

		}
	}
	else if (pBuffer[2] == 0x00) // 数据无效
	{
		// 写入 "数据无效"
		CString strInvalidData = _T("数据无效");
		mFile.Write(strInvalidData, wcslen(strInvalidData) * sizeof(wchar_t));
		mFile.Write(Space, wcslen(Space) * sizeof(wchar_t));
	}

	mFile.Write(Line_break, wcslen(Line_break) * sizeof(wchar_t));
	mFile.Close();
}

BOOL CCollectorDlg::OnDeviceChange(UINT nEventType, DWORD_PTR dwData)
{
	switch (nEventType)
	{
		//串口被移除  
	case DBT_DEVICEREMOVECOMPLETE:
		KillTimer(RXTIMER);
		MessageBox(_T("串口连接断开"));
		break;

	case DBT_DEVICEARRIVAL:
		break;

	default:
		break;
	}

	return TRUE;
}
