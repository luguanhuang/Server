#ifndef _H_Line_Link_H__
#define _H_Line_Link_H__

#include <map>
#include "baselink.h"
#include "netproc.h"


class LineLink : public BaseLink
{
public:
	LineLink();
	virtual ~LineLink();

	virtual void OnConnectSucceed(UINT32 dwConnID) {}
	virtual void OnConnectFailed() {}
	virtual void OnPassiveConnect(UINT32 dwConnID) {}


	virtual void OnClose(UINT32 dwConnID, int err);


	//Line
	bool SendToLine(UINT32 dwLine, const CProtocol& roPtc);
	bool SendToLine(UINT32 dwLine, const CRpc& roRpc);

	bool SendToAllLine(const CProtocol& roPtc);
	bool SendToAllLine(const CRpc& roRpc);

	bool RegisterLine(UINT32 dwLine, UINT32 dwConnID);
	bool IsLineConnected(UINT32 dwLine) const;

	virtual void OnLineConnected(UINT32 dwLine) {}
	virtual void OnLineClosed(UINT32 dwLine) {}

	void SendToAllLine(const void* pData, UINT32 dwLen)
	{
		for (auto it = m_oMapLine2Conn.begin(); it != m_oMapLine2Conn.end(); ++it)
		{
			CNetProcessor::Instance()->Send(it->second, pData, dwLen);
		}
	}

protected:
	UINT32 GetConnID(UINT32 dwLine);

protected:
	typedef std::map<UINT32, UINT32>	CMapLine2Conn;
	CMapLine2Conn	m_oMapLine2Conn;
};


#endif