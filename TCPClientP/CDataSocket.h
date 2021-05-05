#pragma once

// CDataSocket 명령 대상

class CTCPClientPDlg;

class CDataSocket : public CSocket
{
public:
	CDataSocket(CTCPClientPDlg *pDlg);
	virtual ~CDataSocket();
	CTCPClientPDlg *m_pDlg;
	virtual void OnReceive(int nErrorCode);
};


