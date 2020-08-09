#pragma once
#include "afxcmn.h"
#include "afxwin.h"


// TransForm �Ի���

class TransForm : public CDialogEx
{
	DECLARE_DYNAMIC(TransForm)

public:
	TransForm(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~TransForm();

	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	void AddListData(CString strChannelName,
						CString strSampleRate,
						CString strMeasDual,
						CString strDescript,
						CString strUnit,
						CString strMinRage,
						CString strMaxRage);

// �Ի�������
	enum { IDD = IDD_TRANSFORM };

protected:
	HICON m_hIcon;
	virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnEnChangeEdit1();
	afx_msg void OnBnClickedCheck1();
	CListCtrl m_list;
	CComboBox m_combox;
	CEdit m_edit;
};
