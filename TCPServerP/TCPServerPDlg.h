
// TCPServerPDlg.h: 헤더 파일
//

#pragma once
#include "afxwin.h"
#include "afxcoll.h"
#include "DataSocket.h"
#include "ListenSocket.h"

struct ThreadArg
{
	CStringList* pList;
	CDialogEx* pDlg;
	int Thread_run;		// thread의 실행 또는 종료를 제어하기 위해 정의한 변수
};

// CTCPServerPDlg 대화 상자
class CTCPServerPDlg : public CDialogEx
{
// 생성입니다.
public:
	CTCPServerPDlg(CWnd* pParent = nullptr);	// 표준 생성자입니다.

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_TCPSERVERP_DIALOG };
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
	

	CWinThread* pThread1, * pThread2;	// thread 객체 주소
	ThreadArg arg1, arg2;	// thread 전달 인자
	CListenSocket* m_pListenSocket = NULL;
	CDataSocket* m_pDataSocket = NULL;

	CEdit m_tx_edit_short;
	CEdit m_rx_edit;
	CEdit m_tx_edit;
	void ProcessAccept(int nErrorCode);
	afx_msg void OnBnClickedSend();
	afx_msg void OnBnClickedDisconnect();
	void ProcessReceive(CDataSocket* pSocket, int nErrorCode);
	void ProcessClose(CDataSocket* pSocket, int nErrorCode);
};
