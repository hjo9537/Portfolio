
// TCPServerPDlg.cpp: 구현 파일
//

#include "pch.h"
#include "framework.h"
#include "TCPServerP.h"
#include "TCPServerPDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CCriticalSection tx_cs;
CCriticalSection rx_cs;

UINT TXThread(LPVOID arg);
UINT RXThread(LPVOID arg);

// CTCPServerPDlg 대화 상자



CTCPServerPDlg::CTCPServerPDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_TCPSERVERP_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CTCPServerPDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, m_tx_edit_short);
	DDX_Control(pDX, IDC_EDIT2, m_rx_edit);
	DDX_Control(pDX, IDC_EDIT3, m_tx_edit);
}

BEGIN_MESSAGE_MAP(CTCPServerPDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_SEND, &CTCPServerPDlg::OnBnClickedSend)
	ON_BN_CLICKED(IDC_DISCONNECT, &CTCPServerPDlg::OnBnClickedDisconnect)
END_MESSAGE_MAP()


// CTCPServerPDlg 메시지 처리기

BOOL CTCPServerPDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 이 대화 상자의 아이콘을 설정합니다.  응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	// TODO: 여기에 추가 초기화 작업을 추가합니다.
	CStringList* newlist = new CStringList;
	arg1.pList = newlist;
	arg1.Thread_run = 1;
	arg1.pDlg = this;

	CStringList* newlist2 = new CStringList;
	arg2.pList = newlist2;
	arg2.Thread_run = 1;
	arg2.pDlg = this;

	WSADATA wsa;
	int error_code;
	if ((error_code = WSAStartup(MAKEWORD(2, 2), &wsa)) != 0) {
		TCHAR buffer[256];
		FormatMessageW(FORMAT_MESSAGE_FROM_SYSTEM, NULL, error_code,
			MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), buffer, 256, NULL);
		AfxMessageBox(buffer, MB_ICONERROR);
	}

	ASSERT(m_pListenSocket == NULL);
	m_pListenSocket = new CListenSocket(this);
	if (m_pListenSocket->Create(8000)) {
		if (m_pListenSocket->Listen()) {
			AfxMessageBox(_T("서버를 시작합니다."), MB_ICONINFORMATION);
			pThread1 = AfxBeginThread(TXThread, (LPVOID)&arg1);
			pThread2 = AfxBeginThread(RXThread, (LPVOID)&arg2);
			return TRUE;
		}
	}
	else {
		int err = m_pListenSocket->GetLastError();
		TCHAR buffer[256];
		FormatMessageW(FORMAT_MESSAGE_FROM_SYSTEM, NULL, err,
			MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), buffer, 256, NULL);
		AfxMessageBox(buffer, MB_ICONERROR);
	}

	AfxMessageBox(_T("이미 실행 중인 서버가 있습니다.")_T("\n프로그램을 종료합니다."), MB_ICONERROR);

	return FALSE;
}

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다.  문서/뷰 모델을 사용하는 MFC 애플리케이션의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CTCPServerPDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CTCPServerPDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

UINT RXThread(LPVOID arg)
{
	ThreadArg* pArg = (ThreadArg*)arg;// 스레드 인자 포인터입니다.
	CStringList* plist = pArg->pList;// 스레드 인자의 CString리스트의 포인터입니다.
	CTCPServerPDlg* pDlg = (CTCPServerPDlg*)pArg->pDlg;// 서버 Dlg 객체 생성합니다.
	while (pArg->Thread_run) {// 스레드 실행합니다.
		POSITION pos = plist->GetHeadPosition();// 스레드 인자의 첫번째 주소 설정합니다.
		POSITION current_pos;// 현재 위치입니다.
		while (pos != NULL) {
			current_pos = pos;
			tx_cs.Lock();
			CString str = plist->GetNext(pos);// str에 스레드 인자가 가리키는 값 저장합니다.
			tx_cs.Unlock();

			CString message;

			pDlg->m_rx_edit.GetWindowText(message);// 현재 m_rx_edit의 메세지를 가져옵니다.
			message += str;// 메세지에서 스레드 인자가 가지고있는 문자열을 더합니다.

			pDlg->m_rx_edit.SetWindowText(message);// 더해진 메세지들을 출력합니다.
			pDlg->m_rx_edit.LineScroll(pDlg->m_rx_edit.GetLineCount());// 스크롤을 자동화합니다.
			pDlg->m_rx_edit.SetFocus();// 커서 위치를 rx_edit로 set합니다.


			plist->RemoveAt(current_pos);// 현재 위치에 있는 데이터 포인터를 삭제합니다.

		}
		Sleep(10);
	}
	return 0;
}

class CTCPClientPDlg;

UINT TXThread(LPVOID arg)
{
	ThreadArg* pArg = (ThreadArg*)arg;// 스레드 인자 포인터입니다.
	CStringList* plist = pArg->pList;// 스레드 인자의 CString리스트의 포인터입니다.
	CTCPServerPDlg* pDlg = (CTCPServerPDlg*)pArg->pDlg;// 서버 Dlg 객체를 생성합니다.
	pArg = (ThreadArg*)arg;// 스레드 인자 포인터입니다.
	while (pArg->Thread_run) {// 스레드를 실행합니다.
		POSITION current_pos;// 현재 위치입니다.
		POSITION pos = plist->GetHeadPosition();// 스레드 인자의 첫번째 주소 설정합니다.

		while (pos != NULL) {
			current_pos = pos;
			tx_cs.Lock();
			CString str = plist->GetNext(pos);// 현재 m_rx_edit의 메세지를 가져옵니다.
			tx_cs.Unlock();

			CString message;
			int len = pDlg->m_tx_edit.GetWindowTextLengthW();// Dlg의 화면에 입력받을 수 있는 메세지의 길이를 가져옵니다.

			pDlg->m_tx_edit_short.GetWindowText(message);// 현재 m_tx_edit_short의 메세지를 가져옵니다.
			message = message + str;	// 메세지에서 스레드 인자가 가지고있는 문자열을 더합니다.

			pDlg->m_pDataSocket->Send((LPCTSTR)message, (message.GetLength() + 1) * sizeof(TCHAR));
			// 입력받은 메세지를 DataSocket의 Send를 이용하여 보냅니다.

			pDlg->m_tx_edit_short.SetFocus();

			pDlg->m_tx_edit.SetSel(len, len);// Dlg의 서버 메세지에서 입력받은 메세지를 입력 받을 수 있는 길이만큼 화면에 표시합니다.

			plist->RemoveAt(current_pos);// 현재 위치에 있는 데이터를 삭제합니다.

		}
		Sleep(10);
	}
	return 0;
}



void CTCPServerPDlg::ProcessAccept(int nErrorCode)
{
	CString PeerAddr;
	UINT PeerPort;
	CString str;

	ASSERT(nErrorCode == 0);
	if (m_pDataSocket == NULL) {
		m_pDataSocket = new CDataSocket(this);
		if (m_pListenSocket->Accept(*m_pDataSocket)) {
			m_pDataSocket->GetPeerName(PeerAddr, PeerPort);
			str.Format(_T("### IP주소: %s, 포트 번호: %d ###\r\n"),
				PeerAddr, PeerPort);
			m_rx_edit.SetWindowTextW(str);
		}
		else {
			delete m_pDataSocket;
			m_pDataSocket = NULL;
		}
	}
}

void CTCPServerPDlg::ProcessReceive(CDataSocket* pSocket, int nErrorCode)
{
	TCHAR pBuf[1024 + 1];
	CString strData;
	int nbytes;

	nbytes = pSocket->Receive(pBuf, 1024);
	pBuf[nbytes] = NULL;
	strData = (LPCTSTR)pBuf;
	//AfxMessageBox(strData);

	rx_cs.Lock();
	arg2.pList->AddTail((LPCTSTR)strData);
	rx_cs.Unlock();
}

void CTCPServerPDlg::ProcessClose(CDataSocket* pSocket, int nErrorCode)
{
	arg1.Thread_run = 0;
	arg2.Thread_run = 0;
	pSocket->Close();
	delete m_pDataSocket;
	m_pDataSocket = NULL;

	int len = m_rx_edit.GetWindowTextLengthW();
	CString message = _T("### 접속 종료 ###\r\n\r\n");
	m_rx_edit.SetSel(len, len);
	m_rx_edit.ReplaceSel(message);
}


void CTCPServerPDlg::OnBnClickedSend()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CString tx_message;
	m_tx_edit_short.GetWindowText(tx_message);
	tx_message += _T("\r\n");
	tx_cs.Lock();
	arg1.pList->AddTail(tx_message);
	//m_pDataSocket->Send((LPCTSTR)tx_message, (tx_message.GetLength() + 1) * sizeof(TCHAR));
	tx_cs.Unlock();
	m_tx_edit_short.SetWindowText(_T(""));
	m_tx_edit_short.SetFocus();

	int len = m_tx_edit.GetWindowTextLengthW();
	m_tx_edit.SetSel(len, len);
	m_tx_edit.ReplaceSel(tx_message);
}


void CTCPServerPDlg::OnBnClickedDisconnect()
{
	if (m_pDataSocket == NULL) {
		AfxMessageBox(_T("이미 접속 종료"));
	}
	else {
		arg1.Thread_run = 0;
		arg2.Thread_run = 0;
		m_pDataSocket->Close();
		delete m_pDataSocket;
		m_pDataSocket = NULL;
		m_pListenSocket->Close();
		delete m_pListenSocket;
		m_pListenSocket = NULL;
	}
}




