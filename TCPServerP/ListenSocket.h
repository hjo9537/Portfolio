#pragma once

class CTCPServerPDlg;

class CListenSocket : public CSocket
{
public:
	CListenSocket();
	CListenSocket(CTCPServerPDlg* pDlg);
	virtual ~CListenSocket();
	CTCPServerPDlg* m_pDlg;
	virtual void OnAccept(int nErrorCode);
};


