#ifndef _Gate_Link_H__
#define _Gate_Link_H__

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
	KKSG::ErrorCode AddGateServer(UINT32 dwConnID, UINT32 dwServerID, UINT32 dwLine);
};

#endif
