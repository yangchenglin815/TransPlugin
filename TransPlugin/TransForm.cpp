// TransForm.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "TransForm.h"
#include "afxdialogex.h"
#include "netudp.h"

// TransForm �Ի���
static ULONGLONG st_beginTime = 0;
IMPLEMENT_DYNAMIC(TransForm, CDialogEx)

TransForm::TransForm(CWnd* pParent /*=NULL*/)
	: CDialogEx(TransForm::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

TransForm::~TransForm()
{

}

void TransForm::InitApp(IApp * DeweApp)
{
	m_app = DeweApp;
	this->DoModal();
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
	data->get_MaxCalcDelay(&m_calcDelay);

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
	GetDlgItem(IDC_STATIC)->SetFont(&font);

	int col = 0;
	m_list.InsertColumn(col++,L"ͨ������",0,120);
	m_list.InsertColumn(col++,L"����Ƶ��",0,100);
	m_list.InsertColumn(col++,L"�������",0,100);
	m_list.InsertColumn(col++,L"ͨ������",0,150);
	m_list.InsertColumn(col++,L"��λ",0,60);
	m_list.InsertColumn(col++,L"���ֵ",0,85);
	m_list.InsertColumn(col++,L"��Сֵ",0,85);
	m_list.SetBkColor(RGB(255, 251, 240));

	m_edit.SetReadOnly(TRUE);
	m_edit.SetWindowTextW(TEXT("show running infomation here"));
	
	m_combox.ShowWindow(false);
	GetDlgItem(IDC_STATIC2)->ShowWindow(false);

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
		double max;
		double min;
		(*iter)->get_ChNo(&chNo);
		(*iter)->get_Unit_(&unit);
		(*iter)->get_Description(&des);
		(*iter)->get_AbsMax(&max);
		(*iter)->get_AbsMin(&min);

		CString sampleRate;
		CString calcDelay;
		CString absMax;
		CString absMin;
		sampleRate.Format(_T("%ld"), m_sampleRate);
		calcDelay.Format(_T("%ld"), m_calcDelay);
		absMax.Format(_T("%lf"), max);
		absMin.Format(_T("%lf"), min);
		AddListData(chNo, sampleRate, calcDelay, des, unit, absMin, absMax);
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

void TransForm::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	switch(nIDEvent)
	{
	case 1000:
		{
			UpdateList();
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
			m_edit.SetWindowTextW(wstrTimeElapsed);
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
}
