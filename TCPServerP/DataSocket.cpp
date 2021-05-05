// DataSocket.cpp: 구현 파일
//

#include "pch.h"
#include "TCPServerP.h"
#include "DataSocket.h"
#include "TCPServerPDlg.h"


// CDataSocket

CDataSocket::CDataSocket()
{
}

CDataSocket::CDataSocket(CTCPServerPDlg* pDlg) {
	m_pDlg = pDlg;
}

CDataSocket::~CDataSocket()
{
}


// CDataSocket 멤버 함수


void CDataSocket::OnClose(int nErrorCode)
{
	CSocket::OnClose(nErrorCode);
	m_pDlg->ProcessClose(this, nErrorCode);
}


void CDataSocket::OnReceive(int nErrorCode)
{
	CSocket::OnReceive(nErrorCode);
	m_pDlg->ProcessReceive(this, nErrorCode);
}
