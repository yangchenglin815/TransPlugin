// TransForm.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "TransForm.h"
#include "afxdialogex.h"
#include "netudp.h"

// TransForm �Ի���
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

	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��
	SetWindowText(L"���ݴ���ϵͳ");
	SetBackgroundColor(RGB(185, 209, 234));

	long dwStyle = m_list.GetExtendedStyle();
	dwStyle |= LVS_EX_FULLROWSELECT;
	dwStyle |= LVS_EX_GRIDLINES; //�����ߣ�ֻ�����뱨�����listctrl��
	dwStyle |= LVS_EX_CHECKBOXES;//ѡ��CheckBox
	m_list.SetExtendedStyle(dwStyle);

	//����
	CFont font;
	font.CreatePointFont(24, L"΢���ź�");

	int col = 0;
	m_list.InsertColumn(col++,L"ͨ������",0,120);
	m_list.InsertColumn(col++,L"����Ƶ��",0,100);
	m_list.InsertColumn(col++,L"ʵʱֵ",0,100);
	m_list.InsertColumn(col++,L"ͨ������",0,150);
	m_list.InsertColumn(col++,L"��λ",0,60);
	m_list.InsertColumn(col++,L"���ֵ",0,85);
	m_list.InsertColumn(col++,L"��Сֵ",0,85);
	m_list.SetBkColor(RGB(255, 251, 240));

	m_edit.SetReadOnly(TRUE);
	m_edit.SetWindowTextW(TEXT("show running infomation here"));
	
	m_combox.InsertString(0, L"�ɿز���ϵͳ");
	m_combox.InsertString(1, L"Һѹ����ϵͳ");
	m_combox.InsertString(2, L"����ܲ���ϵͳ");
	m_combox.SetCurSel(0);

	//��ʼ������ģ��
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
		MessageBox(L"NetUdpClientInit Failed!", L"����ģ���ʼ��ʧ��", MB_OK);
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
		double value = 0.0;
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
		sValue.Format(_T("%lf"), value);
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
		double value = 0.0;
		(*iter)->get_AbsMax(&max);
		(*iter)->get_AbsMin(&min);
		OnGetData(*iter, &value);

		CString sampleRate;
		CString sValue;
		CString absMax;
		CString absMin;
		sampleRate.Format(_T("%ld"), m_sampleRate);
		sValue.Format(_T("%lf"), value);
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
	int icount = m_list.GetItemCount();
	for (int i = 0; i < icount; i++)
	{
		if (m_list.GetCheck(i))
		{
			selCount++;
		}
	}
	return selCount;
}


void TransForm::OnGetData(IChannel *channel, double *value)
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
		channel->get_DBValuesDouble(index, value);
	}
}


void TransForm::AppendText(CString strAdd)
{
	m_edit.Clear();
	m_edit.SetWindowTextW(strAdd);
}


void TransForm::SendChannelData(unsigned int nParamNum, const char* data, size_t len)
{
	int devID = getDevID(m_combox.GetCurSel());
	char *pDataBuff = new char[len + 2];
	ZeroMemory(pDataBuff, len + 2);
	pDataBuff[0] = (char)(0xAA);
	sprintf_s(pDataBuff + 1, len + 2, "%x", nParamNum);
	memcpy(pDataBuff + 2, data, len);

	NetUdpClientSend(pDataBuff, len + 2);
	delete[] pDataBuff;
}

void TransForm::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	switch(nIDEvent)
	{
	case 1000:
		{
		    UpdateValue();
			int selChannelCount = GetSelectedChannels();
			DWORD sec = 0;
			ULONGLONG nowTime = GetTickCount64();
			ULONGLONG timeElapsed = nowTime - st_beginTime;
			sec = static_cast<DWORD>(timeElapsed / 1000);//��ʾÿ��ÿ�ε�����
			WCHAR wstrTimeElapsed[100] = { 0 };
			if (selChannelCount == 0)
			{
				sec = 0;
			}
			swprintf_s(wstrTimeElapsed, sizeof(wstrTimeElapsed) / 2, L"ѡ��ͨ��:%d\n��������:%d", selChannelCount, sec);
			AppendText(wstrTimeElapsed);
			int index = m_combox.GetCurSel();
			CString devID;
			devID.Format(_T("%d"), index);
			CString msg = L"ѡ���豸��" + devID;
			AppendText(msg);

			int num = GetSelectedChannels();
			CString cNum;
			cNum.Format(_T("%d"), num);
			msg = L"ѡ��ͨ����" + cNum;
			AppendText(msg);

			unsigned int paramNum = 1;
			size_t length = paramNum * 9;
			char *buffer = new char[length];
			ZeroMemory(buffer, length);
		    sprintf_s(buffer, length, "%x", 6);
			double value = 2.234;
			sprintf_s(buffer + 1, length, "%x", value);

			SendChannelData(paramNum, buffer, length);
			delete[] buffer;
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


// TransForm ��Ϣ�������


void TransForm::OnEnChangeEdit1()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CDialogEx::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
}


void TransForm::OnBnClickedCheck1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	int checkState = m_checkBox.GetCheck();
	int icount = m_list.GetItemCount();
	for (int i = 0; i < icount; i++)
	{
		m_list.SetCheck(i, checkState);
	}
}
