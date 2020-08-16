#pragma once
#include "afxcmn.h"
#include "afxwin.h"
#include <vector>

// TransForm 对话框
using namespace std;

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
	void OnGetData(IChannel *channel, double *value);
	void AppendText(CString strAdd);
	void SendChannelData(unsigned int nParamNum, const char* data, size_t len);

protected:
	HICON m_hIcon;
	virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnEnChangeEdit1();
	afx_msg void OnBnClickedCheck1();
	CListCtrl m_list;
	CComboBox m_combox;
	CEdit m_edit;
	IApp *m_app;
	vector<IChannel*> m_channelVec;
	long m_sampleRate;
	CButton m_checkBox;
};
