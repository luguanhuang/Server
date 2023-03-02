#ifndef _audio_Gate_Link_H__
#define _audio_Gate_Link_H__

#include "pb/enum.pb.h"
#include "linelink.h"

struct GateServerInfo
{
	UINT32		dwLine;

	UINT32		listenPort;
	std::string listenIP;

	GateServerInfo()
		:dwLine(0)
		,listenPort(0)
	{
	}
};

class GateLink : public LineLink
{
	GateLink();
	virtual ~GateLink();
	DECLARE_SINGLETON(GateLink)

public:
	virtual const std::string & GetName();

	virtual void OnLineConnected(UINT32 dwLine);
	virtual void OnLineClosed(UINT32 dwLine);

	bool SendToSession(const CProtocol &roPtc);
	KKSG::ErrorCode AddGateServer(UINT32 dwConnID, UINT32 dwServerID, UINT32 dwLine, const std::string& strIP, UINT32 dwPort);

	const std::map<UINT32, GateServerInfo>& GetGateInfoMap() const { return m_oGtInfoMap; }
private:
	typedef std::map<UINT32, GateServerInfo>	CGateServerInfoMap;
	CGateServerInfoMap	m_oGtInfoMap;
};

#endif
