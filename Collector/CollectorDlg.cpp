
// CollectorDlg.cpp : ʵ���ļ�
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


// CCollectorDlg �Ի���
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


// CCollectorDlg ��Ϣ�������


BOOL CCollectorDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	
	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	objTempBtn = &m_MFCButton;
	objTempBtn->m_bTransparent = FALSE;
	objTempBtn->m_bDontUseWinXPTheme = TRUE;
	objTempBtn->SetFaceColor(RGB(255, 255, 255));

	//�޸������С
	cfont1.CreatePointFont(200, _T("���塣"),NULL);
	GetDlgItem(IDC_BUTTON2)->SetFont(&cfont1);

//��ʼ��Ŀ����ʾ������
	pAxis = m_ChartCtrl.CreateStandardAxis(CChartCtrl::BottomAxis);	//�ײ�������
	//pAxis->SetAutomatic(true);	
	//���ݾ�����ֵ�Զ��������������꣬���Ҫ�̶���������Ҫ��������޸�Ϊfalse
	pAxis->SetMinMax(-1, 1);
	//pAxis->SetAutomatic(true);
		
	pAxis = m_ChartCtrl.CreateStandardAxis(CChartCtrl::LeftAxis);	//���������
	pAxis->SetMinMax(0, 800);	//�̶�y������Ϊ0-100
	//pAxis->SetAutomatic(true);
	

	//��ӱ���
	TChartString str1;
	str1 = _T("�״�Ŀ����ʾ");
	m_ChartCtrl.GetTitle()->AddString(str1);
	//������ɫ
	m_ChartCtrl.GetTitle()->SetColor(RGB(255, 255, 255));   //���������ɫ
	m_ChartCtrl.GetLeftAxis()->SetTextColor(RGB(255, 255, 255));  //���������ɫ
	m_ChartCtrl.GetBottomAxis()->SetTextColor(RGB(255, 255, 255));  //�ײ��������ɫ
	m_ChartCtrl.SetBorderColor(RGB(255, 255, 255));  //�߿���ɫ��ɫ
	m_ChartCtrl.SetBackColor(RGB(85, 85, 85));  //������ɫ���ɫ

	pPointSerie1 = m_ChartCtrl.CreatePointsSerie();	//�󶨻�ͼ����
	objLogEdit.Init((CEdit*)GetDlgItem(IDC_EDIT1));
	objLogEdit.LOGP(_T("��ʼ���ɹ�"));
	Createfolder();

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}



// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CCollectorDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CCollectorDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

/***********************************************
**��������OnCbnDropdownCombo1
**���ܣ�ѡ�񴮿ں�
**�������1����
**�������1����
**���ߣ�tdw
**ʱ�䣺2023.2.28
************************************************/

unsigned char u8RecvBuffer[INCOMMAX] = { 0 };			// ���ڽ��յĻ�����
int sfDatacnt = 0;//����֡����
CString strTime1;
CString strHour1;
CString strMinute1;
CString strSecond1;
CString strData;
CTime tm1;

void CCollectorDlg::OnCbnDropdownCombo1()
{
	LPCTSTR regDir = _T("HARDWARE\\DEVICEMAP\\SERIALCOMM");					// �����豸��·��
	HKEY hKEY;																// ע�����
	DWORD dwIndex = 0;														// ע���ֵ������
	DWORD uRegState = ERROR_SUCCESS;										// ��ȡע����״̬,Ĭ��Ϊ�ɹ�

	if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, regDir, 0, KEY_READ, &hKEY))		// ��ע���
	{
		MessageBox(_T("����: �޷����йص�hKEY!"));
		return;
	}

	objComBox1.ResetContent();											// ������������������

	while (uRegState == ERROR_SUCCESS)
	{

		TCHAR ucBuffer[_MAX_PATH + 1] = {};									// һ����������ָ��
		DWORD ulufferSize = sizeof(ucBuffer);								// ��������С
		TCHAR uComName[255] = {};											// �������ƻ�����
		DWORD uComNameSize = sizeof(uComName);								// �������Ƴ�

		uRegState = RegEnumValue(hKEY, dwIndex, ucBuffer, &ulufferSize, NULL, NULL, reinterpret_cast<LPBYTE>(uComName), &uComNameSize);
		if (uRegState == ERROR_SUCCESS)
		{
			objComBox1.AddString(uComName);
			dwIndex++;
		}
	}

	RegCloseKey(hKEY);														//�ر�ע���
}

/***********************************************
**��������OnCbnDropdownCombo2
**���ܣ�ѡ��������
**�������1����
**�������1����
**���ߣ�ZJC
**ʱ�䣺2023.8.18
************************************************/

void CCollectorDlg::OnCbnDropdownCombo2()
{
	objComBox2.ResetContent();											// ������������������

	CString strCmdName1 = _T("01");  // cmd
	CString strCmdName2 = _T("02");  
	CString strCmdName3 = _T("03");  

	objComBox2.AddString(strCmdName1);  
	objComBox2.AddString(strCmdName2); 
	objComBox2.AddString(strCmdName3);
}


/***********************************************
**��������OnBnClickedButton1
**���ܣ��򿪴���
**�������1����
**�������1����
**���ߣ�tdw
**ʱ�䣺2023.2.28
************************************************/
void CCollectorDlg::OnBnClickedButton1()
{
	CString  sComName;
	CString sBaudRate;
	CString stData;
	int errCode;
	objComBox1.GetWindowTextW(sComName);
	objOpenBtn.GetWindowTextW(stData);

	if (stData == _T("�򿪴���"))
	{
		CSerial = new SerialTxRx(sComName, 115200, 8, 0, 0);
		CSerial->Purge(PURGE_RXCLEAR);								//������ջ����������ݣ����г�ʼ��		
		errCode = CSerial->GetComStatus();

		if (errCode == 1)   //���óɹ�
		{
			objLogEdit.LOGP(_T("�򿪴��ڳɹ�"));
			objOpenBtn.SetWindowTextW(_T("�رմ���"));
		}
		else if (errCode == 0)  //δ���ô��ں�
		{
			MessageBox(_T("�����ô���"));
		}
	}
	if (stData == _T("�رմ���"))
	{
		if (CSerial != NULL)
		{
			CSerial->~SerialTxRx();
		}
		objOpenBtn.SetWindowTextW(_T("�򿪴���"));
		objLogEdit.LOGP(_T("�رմ��ڳɹ�"));
	}


}

/***********************************************
**��������OnBnClickedButton2
**���ܣ���ʼ�ɼ�
**�������1����
**�������1����
**���ߣ�tdw
**ʱ�䣺2023.2.28
************************************************/
CString sContent;
void CCollectorDlg::OnBnClickedButton2()
{

	objStartBtn.GetWindowTextW(sContent);
	if (sContent == _T("��ʼ��������"))
	{
		if (CSerial == NULL)
		{
			MessageBox(_T("����δ�򿪣�"));   //û������ �����������
		}
		else
		{
			CSerial->Purge(PURGE_RXCLEAR); //��ʼʱ����մ��ڻ���������
			SetTimer(RXTIMER, CONSTTIME_MS, NULL);
			objStartBtn.SetWindowTextW(_T("ֹͣ��������"));
			objTempBtn->m_bTransparent = FALSE;
			objTempBtn->m_bDontUseWinXPTheme = TRUE;
			objTempBtn->SetFaceColor(RGB(255, 255, 255));
			//���ݿ�ʼ���յ�ʱ�� �����ļ��� �洢���� 
			tm1 = CTime::GetCurrentTime();         //��ȡ��ǰϵͳʱ��
			strTime1 = tm1.Format(_T("%Y-%m-%d"));
			strHour1 = tm1.Format(_T("%H"));
			strMinute1 = tm1.Format(_T("%Hʱ%M��"));
			strSecond1 = tm1.Format(_T("%H-%M-%S"));
			strData = tm1.Format("%y��%m��%d�� %X");
			objComBox2.EnableWindow(FALSE);     //����������
		}
	}
	else   //ֹͣ��������
	{
		KillTimer(RXTIMER);
		memset(u8RecvBuffer,0,sizeof(u8RecvBuffer));
		sfDatacnt = 0;
		objStartBtn.SetWindowTextW(_T("��ʼ��������"));
		objComBox2.EnableWindow(TRUE);          //���������� 	
	}
}


/***********************************************
**��������OnTimer
**���ܣ���ʱ������
**�������1����ʱ����
**�������1����
**���ߣ�tdw
**ʱ�䣺2023.2.28
************************************************/
void CCollectorDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	COMSTAT  ComStat;										// ����״̬������
	unsigned int u16DataLen;								// ���洮�ڻ������ĳ���
	DWORD dwErrorFlags;										// ���ڵĴ�������,�ݲ�����
	unsigned char comDataBuffer[25] = { 0 };			// ����֡����

	CString sTAG;
	int rightLength = 0;
	int comDatacnt = 0;
	int PraseData2;
	double realHight = 0;
	double realSpeed = 0;
	int framenum = 0;

	CString sCmdName;
	objComBox2.GetWindowTextW(sCmdName);

	if (nIDEvent == RXTIMER)								// ���ڽ��յ�����
	{
		KillTimer(RXTIMER);									// ����ֻ����һ��, �����ظ�����
		CSerial->gClearCommError(&dwErrorFlags, &ComStat);	// ��ȡ����״̬
	

		/**1����ȡ�ɼ�����**/
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
		
		//��������
		if (sCmdName == _T("01")) { framenum = 8; }
		if (sCmdName == _T("02")) { framenum = 10; }
		if (sCmdName == _T("03")) { framenum = 22; }
		

		
		

		SetTimer(RXTIMER, CONSTTIME_MS, NULL);

		 if (uDatacnt >0)
		{
			rightLength = RxDataCrc(&u8RecvBuffer[0], uDatacnt);//CRCУ�鲢������ʱ��ȷ���ݳ���
			comDatacnt = (rightLength / framenum) * framenum;  //����֡����
			sfDatacnt = rightLength - comDatacnt;//����֡����

			//������֡�����ݱ��浽��һ�����ݽ���ƴ��
			memcpy(&comDataBuffer[0], &u8RecvBuffer[0], framenum);//����֡���ݣ����Ǵ�ͷ��ʼ
			//memcpy(&u8RecvBuffer[0], &u8RecvBuffer[comDatacnt], sfDatacnt);//����֡���ݱ�������һ�����ݸ��½���


			if (comDataBuffer[2] == 0x80)
			{
				//������Ч������B3/B4������ϣ���Ҫ�пؼ���ʾ������Ч���ҽ��߶Ȼ��ڿ���
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
				objLogEdit.LOGP(_T("�߶�Ϊ: %.2f m\n"), realHight);
				objLogEdit.LOGP(_T("�ٶ�Ϊ: %.2f m/s\n"), realSpeed);
				
				if (sCmdName == _T("03"))
				{ 
					objLogEdit.LOGP(_T("x���ٶ�Ϊ: %.2f m/s\u00B2\n"), acc_x);  // \u00B2��ʾƽ��
					objLogEdit.LOGP(_T("y���ٶ�Ϊ: %.2f m/s\u00B2\n"), acc_y);
					objLogEdit.LOGP(_T("z���ٶ�Ϊ: %.2f m/s\u00B2\n"), acc_z);
					objLogEdit.LOGP(_T("x��ת��Ϊ: %.2f ��/s\u00B2\n"), rotation_x);
					objLogEdit.LOGP(_T("y��ת��Ϊ: %.2f ��/s\u00B2\n"), rotation_y);
					objLogEdit.LOGP(_T("z��ת��Ϊ: %.2f ��/s\u00B2\n"), rotation_z);
				}
				
				objLogEdit.LOGP(_T("\n"));          // 8.7  LOGP�����
				//��ͼ
				DrawTarget(realHight, RGB(255,0,0));
				CSerial->Purge(PURGE_RXCLEAR);

			}
			else if (comDataBuffer[2] == 0x00)
			{
				//������Ч���������B3/B4����������Ч��ʾ�ڿؼ���
				comDataBuffer[3] = 0;
				comDataBuffer[4] = 0;

				WriteRawData(&comDataBuffer[0], comDatacnt, strSecond1);
				objTempBtn->m_bTransparent = FALSE;
				objTempBtn->m_bDontUseWinXPTheme = TRUE;
				objTempBtn->SetFaceColor(RGB(165, 42, 42));
				objLogEdit.LOGP(_T("������Ч"));
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
**��������RxDataCrc
**���ܣ���ԭʼ���ݽ���CRCУ�飬��ɾ������֡,������ȷ֡����
**�������1��pBuffer   ���ݻ���
**�������2��u16DataLen  ���ݳ���
**�������1������������
**���ߣ�GQ
**ʱ�䣺2023.3.24
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

	//����ͷ֡
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

	//ƽ������
	for (int i = 0; i < u16DataLen; i++)
	{
		pBuffer[i] = pBuffer[i + HeadnNum];  //�˴����ݳ����Լ�СHeadnNum
	}

	//����ʣ�����鳤��
	int NewNum = (u16DataLen - HeadnNum);
	return NewNum;
}

/***********************************************
**��������DealDataAnalyze
**���ܣ�����Ҫ�����ԭʼ���ݽ������
**�������1��pBuffer   ���ݻ���
**�������2��u16DataLen  ���ݳ���
**�������1����
**���ߣ�GQ
**ʱ�䣺2023.3.9
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
	
	//�ٶȸ��������
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
**��������DrawTarget
**���ܣ����ݴ���ĸ߶����ݻ�ͼ
**�������1��hight    �߶�����
**�������2��crColor  ��ɫ����
**�������1����
**���ߣ�ZJC
**ʱ�䣺2023.8.3
************************************************/
void CCollectorDlg::DrawTarget(double hight, COLORREF crColor)
{
	double Rec_Hight = hight;
	m_ChartCtrl.SetZoomEnabled(true);
	m_ChartCtrl.RemoveAllSeries();//�����
	pPointSerie1 = m_ChartCtrl.CreatePointsSerie();	//�󶨻�ͼ����
	pPointSerie1->SetSeriesOrdering(poNoOrdering);//����Ϊ����
	m_ChartCtrl.EnableRefresh(false);
	pPointSerie1->SetPointSize(25, 25);
	pPointSerie1->SetColor(crColor); //��ɫ	
	pPointSerie1->AddPoint(0, Rec_Hight);
	//pPointSerie1->AddPoints(XValues, YValues, LastTargetNum);
	m_ChartCtrl.EnableRefresh(true);
}





//��������   һ��һ���ļ���
void CCollectorDlg::WriteRawData(unsigned char* pBuffer, unsigned int u16DataLen, CString time)
{
	CFile mFile;
	CFileFind FindFile;
	CString Line_break = _T("\r\n"); // ����
	CString Space = _T("    ");      // �ո�
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
	strSecondnow = tm1.Format(_T("ʱ�䣺%H:%M:%S"));

	USES_CONVERSION;
	char* time1 = T2A(strTime1);
	// ��ԭʼ����д��Bin�ļ���
	CString filename1 = _T("D:\\Ԫ�пƼ�");
		if (!PathIsDirectory(filename1))
		{
			CreateDirectory(filename1, NULL); // �������򴴽��ļ�
		}
	CString filename2 = _T("D:\\Ԫ�пƼ�\\�߶ȱ�����\\");
		if (!PathIsDirectory(filename2))
		{
			CreateDirectory(filename2, NULL); // �������򴴽��ļ�
		}


	CString filename0000 = _T("D:\\Ԫ�пƼ�\\�߶ȱ�����\\") + strTime1 + _T("\\"); // �ļ����ļ���,����
	if (!PathIsDirectory(filename0000))
	{
		CreateDirectory(filename0000, NULL); // �������򴴽��ļ�
	}

	CString filename00 = filename0000 + time;
	CreateDirectory(filename00, NULL);

	CString filename000 = filename00 + _T("\\") + _T("Hight_Data.txt");
	if (FindFile.FindFile(filename000)) // �ҵ�����ļ�
	{
		mFile.Open(filename000, CFile::modeNoTruncate | CFile::modeReadWrite); // �Ա���ģʽ��
	}
	else // �Ҳ���
	{
		mFile.Open(filename000, CFile::modeCreate | CFile::modeReadWrite); // ���½�ģʽ��
	}
	mFile.SeekToEnd();
	wchar_t unicode = 0xFEFF; // �������ļ�ͷ�����±��򿪲���ʶ������ĺ��֣��������롣
	mFile.Write(&unicode, 2);

	// д��ʱ��
	mFile.Write(strSecondnow, wcslen(strSecondnow) * sizeof(wchar_t));
	mFile.Write(Space, wcslen(Space) * sizeof(wchar_t));

	// д������
	if (pBuffer[2] == 0x80) // ������Ч
	{
		// ���B3/B4����
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


		// ���±�����ֵ
		bData = data[0] * 0.15;
		cData = data[1] * 156.8 / 32768;
		dData = data[2] * 156.8 / 32768;
		eData = data[3] * 156.8 / 32768;
		fData = data[4] * 2000.0 / 32768;
		gData = data[5] * 2000.0 / 32768;
		hData = data[6] * 2000.0 / 32768;



		// ��DealDataд���ļ�
		CString strDealData1;
		CString strDealData2;
		strDealData1.Format(_T("�߶ȣ�%.2f m"), aData);
		strDealData2.Format(_T("�ٶȣ�%.2f m/s"), bData);
		mFile.Write(strDealData1, wcslen(strDealData1) * sizeof(wchar_t));
		mFile.Write(Space, wcslen(Space) * sizeof(wchar_t));
		mFile.Write(strDealData2, wcslen(strDealData2) * sizeof(wchar_t));
		mFile.Write(Space, wcslen(Space) * sizeof(wchar_t));
		if (pBuffer[1] == 0x03)
		{
			CString strData[6];
			strData[0].Format(_T("x���ٶȣ�%.2f m/s\u00B2"), cData);  // \u00B2��ʾƽ��
			strData[1].Format(_T("y���ٶȣ�%.2f m/s\u00B2"), dData);
			strData[2].Format(_T("z���ٶȣ�%.2f m/s\u00B2"), eData);
			strData[3].Format(_T("x��ת�ȣ�%.2f ��/s\u00B2"), fData);
			strData[4].Format(_T("y��ת�ȣ�%.2f ��/s\u00B2"), gData);
			strData[5].Format(_T("z��ת�ȣ�%.2f ��/s\u00B2"), hData);

			for (int i = 0; i < 6; i++)
			{
				mFile.Write(strData[i], wcslen(strData[i]) * sizeof(wchar_t));
				mFile.Write(Space, wcslen(Space) * sizeof(wchar_t));
			}

		}
	}
	else if (pBuffer[2] == 0x00) // ������Ч
	{
		// д�� "������Ч"
		CString strInvalidData = _T("������Ч");
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
		//���ڱ��Ƴ�  
	case DBT_DEVICEREMOVECOMPLETE:
		KillTimer(RXTIMER);
		MessageBox(_T("�������ӶϿ�"));
		break;

	case DBT_DEVICEARRIVAL:
		break;

	default:
		break;
	}

	return TRUE;
}
