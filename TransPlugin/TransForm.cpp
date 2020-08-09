// TransForm.cpp : 实现文件
//

#include "stdafx.h"
#include "TransForm.h"
#include "afxdialogex.h"
#include "netudp.h"

// TransForm 对话框

IMPLEMENT_DYNAMIC(TransForm, CDialogEx)

TransForm::TransForm(CWnd* pParent /*=NULL*/)
	: CDialogEx(TransForm::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

TransForm::~TransForm()
{
}

BOOL TransForm::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标
	SetWindowText(L"数据传输系统");
	SetBackgroundColor(RGB(185, 209, 234));

	long dwStyle = m_list.GetExtendedStyle();
	dwStyle |= LVS_EX_FULLROWSELECT;
	dwStyle |= LVS_EX_GRIDLINES; //网格线（只适用与报表风格的listctrl）
	dwStyle |= LVS_EX_CHECKBOXES;//选择CheckBox
	m_list.SetExtendedStyle(dwStyle);

	//字体
	CFont font;
	font.CreatePointFont(24, L"微软雅黑");
	GetDlgItem(IDC_STATIC)->SetFont(&font);

	int col = 0;
	m_list.InsertColumn(col++,L"通道名称",0,120);
	m_list.InsertColumn(col++,L"采样频率",0,100);
	m_list.InsertColumn(col++,L"采样间隔",0,100);
	m_list.InsertColumn(col++,L"通道描述",0,150);
	m_list.InsertColumn(col++,L"单位",0,60);
	m_list.InsertColumn(col++,L"最大值",0,85);
	m_list.InsertColumn(col++,L"最小值",0,85);
	m_list.SetBkColor(RGB(255, 251, 240));

	m_edit.SetReadOnly(TRUE);
	m_edit.SetWindowTextW(TEXT("show running infomation here"));
	
	m_combox.ShowWindow(false);
	GetDlgItem(IDC_STATIC2)->ShowWindow(false);

	//初始化网络模块
	char hostname[254]={0};
	gethostname(hostname,sizeof(hostname));
	gethostbyname(hostname)->h_addr_list;
	
	hostent *phostent = gethostbyname(hostname);
	in_addr addr;
	memcpy(&addr.S_un.S_addr,phostent->h_addr_list[0],phostent->h_length);
	addr.S_un.S_un_b.s_b4 = 255;
	char *strIP = inet_ntoa(addr);

	if(!NetUdpClientInit(strIP, 8010))
	{
		MessageBox(L"NetUdpClientInit Failed!",L"网络模块初始化失败",MB_OK);
		assert(0);
		return 0;
	}

	AddListData(L"通道1", L"5000", L"500", L"-", L"V", L"INF", L"-INF");

	return TRUE;
}

void TransForm::AddListData(CString strChannelName,
						CString strSampleRate,
						CString strMeasDual,
						CString strDescript,
						CString strUnit,
						CString strMinRage,
						CString strMaxRage)
{
	int icount = m_list.GetItemCount();
	int icol = 1;
	m_list.InsertItem(icount,strChannelName);
	m_list.SetItemText(icount,icol++,strSampleRate);
	m_list.SetItemText(icount,icol++,strMeasDual);
	m_list.SetItemText(icount,icol++,strDescript);
	m_list.SetItemText(icount,icol++,strUnit);
	m_list.SetItemText(icount,icol++,strMinRage);
	m_list.SetItemText(icount,icol++,strMaxRage);
}

HBRUSH TransForm::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	if (pWnd->GetDlgCtrlID() == IDC_EDIT1)
	{
		pDC->SetTextColor(RGB(211, 211, 211));
		pDC->SetBkColor(RGB(255, 255, 255));
	}
	return hbr;
}

void TransForm::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_list);
	DDX_Control(pDX, IDC_COMBO1, m_combox);
	DDX_Control(pDX, IDC_EDIT1, m_edit);
}


BEGIN_MESSAGE_MAP(TransForm, CDialogEx)
	ON_EN_CHANGE(IDC_EDIT1, &TransForm::OnEnChangeEdit1)
	ON_BN_CLICKED(IDC_CHECK1, &TransForm::OnBnClickedCheck1)
END_MESSAGE_MAP()


// TransForm 消息处理程序


void TransForm::OnEnChangeEdit1()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
}


void TransForm::OnBnClickedCheck1()
{
	// TODO: 在此添加控件通知处理程序代码
}
