#pragma once
#include "afxcmn.h"
#include "afxwin.h"
#include <vector>
#include <map>

// TransForm 对话框
using namespace std;

#define API_DATA_LEN     (6)

typedef struct api_udp_data
{
	unsigned char szChannelNo[API_DATA_LEN];
	float fValue;

	api_udp_data() {
		memset(&szChannelNo, 0, API_DATA_LEN);
		fValue = 0.000;
	}

	int get_size() {
		return sizeof(api_udp_data);
	}
}api_udp_data_t;

class TransForm : public CDialogEx
{
	DECLARE_DYNAMIC(TransForm)

public:
	TransForm(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~TransForm();

	void InitApp(IApp * DeweApp);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	void AddListData(CString strChannelName,
						CString strSampleRate,
						CString strMeasDual,
						CString strDescript,
						CString strUnit,
						CString strMinRage,
						CString strMaxRage);

// 对话框数据
	enum { IDD = IDD_TRANSFORM };

private:
	vector<IChannel*> getAllChannels();
	void UpdateList();
	void UpdateValue();
	int GetSelectedChannels();
	void OnGetData(IChannel *channel, float *value);
	void AppendText(CString strAdd);
	void SendChannelData(const char* data, size_t len);
	void UdpSendData(int paramNum);
	void UdpSendLongData(int paramNum);

protected:
	HICON m_hIcon;
	virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnEnChangeEdit1();
	afx_msg void OnBnClickedCheck1();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	CListCtrl m_list;
	CComboBox m_combox;
	CEdit m_edit;
	IApp *m_app;
	vector<IChannel*> m_channelVec;
	map<int, api_udp_data_t> m_apiDataMap;
	long m_sampleRate;
	CButton m_checkBox;
	afx_msg void OnEnChangeEdit2();
	CButton m_setBtn;
	CEdit m_timeEdit;
	afx_msg void OnBnClickedButton1();
	int m_nInterval;
	afx_msg void OnBnClickedRadio1();
	CEdit m_rateEdit;
	int m_nRateInterval;
};
