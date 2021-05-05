// ListenSocket.cpp: 구현 파일
//

#include "pch.h"
#include "TCPServerP.h"
#include "ListenSocket.h"
#include "TCPServerPDlg.h"


// CListenSocket

CListenSocket::CListenSocket()
{
}

CListenSocket::CListenSocket(CTCPServerPDlg* pDlg) {
	m_pDlg = pDlg;
}

CListenSocket::~CListenSocket()
{
}


// CListenSocket 멤버 함수


void CListenSocket::OnAccept(int nErrorCode)
{
	CSocket::OnAccept(nErrorCode);
	m_pDlg->ProcessAccept(nErrorCode);

}
