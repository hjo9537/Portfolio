// CDataSocket.cpp: 구현 파일
//

#include "pch.h"
#include "TCPClientP.h"
#include "CDataSocket.h"
#include "TCPClientPDlg.h"

// CDataSocket


CDataSocket::CDataSocket(CTCPClientPDlg *pDlg)
{
	m_pDlg = pDlg;
}

CDataSocket::~CDataSocket()
{
}


// CDataSocket 멤버 함수


void CDataSocket::OnReceive(int nErrorCode)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	CSocket::OnReceive(nErrorCode);
	m_pDlg->ProcessReceive(this, nErrorCode);
}
