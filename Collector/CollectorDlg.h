
// CollectorDlg.h : 头文件
//

#pragma once

#include "./ChartCtrl/ChartTitle.h"	//曲线标题设置头文件
#include "./ChartCtrl/ChartBarSerie.h"		//柱状图头文件

#include "./ChartCtrl/ChartPointsSerie.h"
#include "./ChartCtrl/ChartPointsSerie.h"
#include "./ChartCtrl/ChartSurfaceSerie.h"
#include "./ChartCtrl/ChartXYSerie.h"
#include "./ChartCtrl/ChartCtrl.h"
#include "./ChartCtrl/ChartLineSerie.h"
#include "./ChartCtrl/ChartMouseListener.h"
#include "./ChartCtrl/ChartCrossHairCursor.h"
#include "./ChartCtrl/ChartCursor.h"
#include "afxwin.h"
#include "SerialTxRx.h"
#include <complex>
#include "Log.h"
#include "afxcmn.h"
#include "afxbutton.h"
#include "stdafx.h"



#define  DATA_SAMPLE   200   //定义数据采样频率
#define  CONSTTIME_MS 100 //定时50ms  

class CCustomCursorListener : public CChartCursorListener
{
public:
	void OnCursorMoved(CChartCursor *pCursor, double xValue, double yValue);
	CStatic *objStatic;
	HWND m_hwnd;
};


// CCollectorDlg 对话框
class CCollectorDlg : public CDialogEx
{
// 构造
public:
	CCollectorDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_COLLECTOR_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	//Chart绘制
	CChartCtrl m_ChartCtrl;
	CChartPointsSerie* pPointSerie1;
	CChartAxis* pAxis;
	

	CComboBox objComBox1;
	CComboBox objComBox2;
	afx_msg void OnCbnDropdownCombo1();
	afx_msg void OnCbnDropdownCombo2();
	afx_msg void OnBnClickedButton1();
	CButton objOpenBtn;
	CButton objStartBtn;
	afx_msg void OnBnClickedButton2();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnDestroy();
	void DrawTarget(double hight, COLORREF crColor);


// personal declare
public:
	SerialTxRx *CSerial;
	double DealData1;
	double DealData2;
	double acc_x;
	double acc_y;
	double acc_z;
	double rotation_x;
	double rotation_y;
	double rotation_z;
	unsigned int uDatacnt;     //读取数据
	int DealDataAnalyze(BYTE* pBuffer, unsigned int u16DataLen);
	int RxDataCrc(BYTE* pBuffer, unsigned int u16DataLen);
protected:
	BOOL bRxIdelFlag;
	double dAxis[4096];

	afx_msg BOOL OnDeviceChange(UINT nEventType, DWORD_PTR dwData);

	unsigned int uFFTcnt;

	int iStatus;
	unsigned int uDataPeriod=1;
	unsigned int uDataCollectNum=1024;
	unsigned int uDataPkgLen = 512;
public:

	CLogEdit objLogEdit;
//接收数据


	afx_msg void OnEnChangeEdit1();
	

	//修改字体大小
	CFont cfont1;
	

	CMFCButton m_MFCButton;

	//数据保存
	void WriteRawData(unsigned char *pBuffer, unsigned int u16DataLen,CString time);
};


