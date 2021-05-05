#pragma once

// CDataSocket 명령 대상
class CTCPServerPDlg;

class CDataSocket : public CSocket
{
public:
	CDataSocket();
	CDataSocket(CTCPServerPDlg* pDlg);
	virtual ~CDataSocket();
	CTCPServerPDlg* m_pDlg;
	virtual void OnClose(int nErrorCode);
	virtual void OnReceive(int nErrorCode);
};


