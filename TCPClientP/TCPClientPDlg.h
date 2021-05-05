
// TCPClientPDlg.h: 헤더 파일
//
#pragma once
#include "afxwin.h"
#include "afxcoll.h"
#include "CDataSocket.h"
#include "afxcmn.h"

struct ThreadArg
{
	CStringList* pList;
	CDialogEx* pDlg;
	int Thread_run;		// thread의 실행 또는 종료를 제어
};

// CTCPClientPDlg 대화 상자
class CTCPClientPDlg : public CDialogEx
{
// 생성입니다.
public:
	CTCPClientPDlg(CWnd* pParent = nullptr);	// 표준 생성자입니다.

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_TCPCLIENTP_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.


// 구현입니다.
protected:
	HICON m_hIcon;

	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedConnect();
	afx_msg void OnBnClickedDisconnect();
	afx_msg void OnBnClickedSend();
	void ProcessReceive(CDataSocket* pSocket, int nErrorCode);

	CWinThread* pThread1;		// thread1의 객체 주소
	CWinThread* pThread2;		// thread의 객체 주소
	ThreadArg arg1, arg2;		// thread 전달 인자
	CDataSocket* m_pDataSocket = 0;
	CIPAddressCtrl m_ipaddr;
	CEdit m_tx_edit_short;
	CEdit m_rx_edit;
	CEdit m_tx_edit;


};
