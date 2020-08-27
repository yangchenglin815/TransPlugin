// TransForm.cpp : 实现文件
//

#include "stdafx.h"
#include "TransForm.h"
#include "afxdialogex.h"
#include "netudp.h"

// TransForm 对话框
static ULONGLONG st_beginTime = 0;
IMPLEMENT_DYNAMIC(TransForm, CDialogEx)

static int getDevID(int index)
{
	int devID = 0;
	switch (index)
	{
	case 0:
		devID = 6;
		break;
	case 1:
		devID = 11;
		break;
	case 2:
		devID = 15;
		break;
	default:
		break;
	}

	return devID;
}

TransForm::TransForm(CWnd* pParent /*=NULL*/)
	: CDialogEx(TransForm::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

TransForm::~TransForm()
{
	int a = -1;
}

void TransForm::InitApp(IApp * DeweApp)
{
	m_app = DeweApp;
	this->Create(IDD_TRANSFORM, this);
	this->ShowWindow(SW_SHOW);
}

vector<IChannel*> TransForm::getAllChannels()
{
	vector<IChannel*> allChannels;
	IData* data; 
	m_app->get_Data(&data);
	data->BuildChannelList();

	IChannelList *channels;
	data->get_UsedChannels(&channels);
	data->get_SampleRate(&m_sampleRate);

	long count;
	channels->get_Count(&count);
	for (int i = 0; i <count; ++i)
	{
		IChannel *channel;
		channels->get_Item(i, &channel);

		allChannels.push_back(channel);
	}

	return allChannels;
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

	int col = 0;
	m_list.InsertColumn(col++,L"通道名称",0,120);
	m_list.InsertColumn(col++,L"采样频率",0,100);
	m_list.InsertColumn(col++,L"实时值",0,100);
	m_list.InsertColumn(col++,L"通道描述",0,150);
	m_list.InsertColumn(col++,L"单位",0,60);
	m_list.InsertColumn(col++,L"最大值",0,85);
	m_list.InsertColumn(col++,L"最小值",0,85);
	m_list.SetBkColor(RGB(255, 251, 240));

	m_edit.SetReadOnly(TRUE);
	m_edit.SetWindowTextW(TEXT("show running infomation here"));
	
	m_combox.InsertString(0, L"飞控测试系统");
	m_combox.InsertString(1, L"液压测试系统");
	m_combox.InsertString(2, L"起落架测试系统");

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
		MessageBox(L"NetUdpClientInit Failed!", L"网络模块初始化失败", MB_OK);
		assert(0);
		return 0;
	}

	m_channelVec = getAllChannels();
	UpdateList();

	SetTimer(1000, 1000, NULL);
	st_beginTime = GetTickCount64();

	return TRUE;
}

void TransForm::UpdateList()
{
	m_list.DeleteAllItems();

	vector<IChannel*>::const_iterator iter = m_channelVec.cbegin();
	for (; iter != m_channelVec.cend(); iter++)
    {
		BSTR chNo;
		BSTR unit;
		BSTR des;
		double max = 0.0;
		double min = 0.0;
		float value = 0.0;
		(*iter)->get_ChNo(&chNo);
		(*iter)->get_Unit_(&unit);
		(*iter)->get_Description(&des);
		(*iter)->get_AbsMax(&max);
		(*iter)->get_AbsMin(&min);
		OnGetData(*iter, &value);

		CString sampleRate;
		CString sValue;
		CString absMax;
		CString absMin;
		sampleRate.Format(_T("%ld"), m_sampleRate);
		sValue.Format(_T("%f"), value);
		absMax.Format(_T("%lf"), max);
		absMin.Format(_T("%lf"), min);
		AddListData(chNo, sampleRate, sValue, des, unit, absMin, absMax);
    }
}


void TransForm::UpdateValue()
{
	int index = 0;
	vector<IChannel*>::const_iterator iter = m_channelVec.cbegin();
	for (; iter != m_channelVec.cend(); iter++, index++)
	{
		double max = 0.0;
		double min = 0.0;
		float value = 0.0;
		(*iter)->get_AbsMax(&max);
		(*iter)->get_AbsMin(&min);
		OnGetData(*iter, &value);

		CString sampleRate;
		CString sValue;
		CString absMax;
		CString absMin;
		sampleRate.Format(_T("%ld"), m_sampleRate);
		sValue.Format(_T("%f"), value);
		absMax.Format(_T("%lf"), max);
		absMin.Format(_T("%lf"), min);

		m_list.SetItemText(index, 1, sampleRate);
		m_list.SetItemText(index, 2, sValue);
		m_list.SetItemText(index, 5, absMax);
		m_list.SetItemText(index, 6, absMin);
	}
}

int TransForm::GetSelectedChannels()
{
	int selCount = 0;
	m_apiDataMap.clear();
	int icount = m_list.GetItemCount();
	for (int i = 0; i < icount; i++)
	{
		if (m_list.GetCheck(i))
		{
			CString chNo  = m_list.GetItemText(i, 0);
			CString value = m_list.GetItemText(i, 2);

			api_udp_data_t api_data;
			memcpy(api_data.szChannelNo, chNo, API_DATA_LEN);
			api_data.fValue = (float)_tstof(value);
			m_apiDataMap.insert(std::pair<int, api_udp_data_t>(selCount, api_data));

			selCount++;
		}
	}
	return selCount;
}


void TransForm::OnGetData(IChannel *channel, float *value)
{
	if (!channel) 
		return;

	long size = 0;
	channel->get_DBDataSize(&size);
	if (size > 0)
	{
		long pos = 0;
		channel->get_DBPos(&pos);
		long bufSize = 0;
		channel->get_DBBufSize(&bufSize);
		long index = (pos - 1 + bufSize) % bufSize;
		channel->get_DBValues(index, value);
	}
}


void TransForm::AppendText(CString strAdd)
{
	m_edit.Clear();
	m_edit.SetWindowTextW(strAdd);
}


void TransForm::SendChannelData(const char* data, size_t len)
{
	int devID = getDevID(m_combox.GetCurSel());
	char *pDataBuff = new char[len + 2];
	ZeroMemory(pDataBuff, len + 2);
	pDataBuff[0] = (char)(0xAA);
	pDataBuff[1] = devID & 0xFF;

	memcpy(pDataBuff + 2, data, len);

	NetUdpClientSend(pDataBuff, len + 2);
	delete[] pDataBuff;
}


void TransForm::UdpSendData(int paramNum)
{
	size_t length = paramNum * 9 + 1;
	char *buffer = new char[length];
	ZeroMemory(buffer, length);
	buffer[0] = paramNum & 0xFF;

	int index = 1;
	for (int num = 0; num < paramNum; num++, index++)
	{
		buffer[num * 9 + 1] = 5 & 0xFF;
		float value = m_apiDataMap[num].fValue;
		unsigned char *hex = (unsigned char *)&value;
		for (int i = 0; i < 4; i++) {
			buffer[9 * index - 4 - i] = hex[i];
		}

		for (int j = 9 * index - 3; j < 9 * index + 1; ++j) {
			buffer[j] = (char)0xFF;
		}
	}

	SendChannelData(buffer, length);
	delete[] buffer;
}

void TransForm::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	switch(nIDEvent)
	{
	case 1000:
		{
		    UpdateValue();

			int index = m_combox.GetCurSel();
			int num = GetSelectedChannels();
			CString cNum;
			CString msg;
			cNum.Format(_T("%d"), num);
			msg = L"选择通道：" + cNum;
			AppendText(msg);

			if (index != -1)
			   UdpSendData(num);
		}
		break;
	default:break;
	}

	CDialogEx::OnTimer(nIDEvent);
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
	m_list.SetItemText(icount,icol++,strMaxRage);
	m_list.SetItemText(icount,icol++,strMinRage);
}

HBRUSH TransForm::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	if (pWnd->GetDlgCtrlID() == IDC_EDIT1)
	{
		pDC->SetTextColor(RGB(211, 211, 211));
		pDC->SetBkColor(RGB(255, 255, 255));
		pDC->SetDCBrushColor(RGB(255, 255, 255));
	}
	return hbr;
}

void TransForm::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_list);
	DDX_Control(pDX, IDC_COMBO1, m_combox);
	DDX_Control(pDX, IDC_EDIT1, m_edit);
	DDX_Control(pDX, IDC_CHECK1, m_checkBox);
}


BEGIN_MESSAGE_MAP(TransForm, CDialogEx)
	ON_EN_CHANGE(IDC_EDIT1, &TransForm::OnEnChangeEdit1)
	ON_BN_CLICKED(IDC_CHECK1, &TransForm::OnBnClickedCheck1)
	ON_WM_TIMER()
	ON_WM_CTLCOLOR()
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
	int checkState = m_checkBox.GetCheck();
	int icount = m_list.GetItemCount();
	for (int i = 0; i < icount; i++)
	{
		m_list.SetCheck(i, checkState);
	}
}
