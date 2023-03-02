#ifndef _ClientLink_H__
#define _ClientLink_H__

#include "linelink.h"
#include "pb/enum.pb.h"


class GatetLink : public LineLink
{
	GatetLink();
	virtual ~GatetLink();
	DECLARE_SINGLETON(GatetLink)

public:
	virtual void OnConnectSucceed(UINT32 dwConnID);
	virtual void OnConnectFailed();
	virtual const std::string & GetName();

	virtual void OnLineConnected(UINT32 dwLine);
	virtual void OnLineClosed(UINT32 dwLine);

	bool SendToSession(const CProtocol &roPtc);

	void BroadCast(const CProtocol &roPtc);

	KKSG::ErrorCode AddGateServer(UINT32 dwConnID, UINT32 dwServerID, UINT32 dwLine);

	void AddToSet(UINT32 dwLine)		{ m_oAllSet.insert(dwLine); }
	void DelFromSet(UINT32 dwLine)		{ m_oAllSet.erase(dwLine); }
	bool IsInSet(UINT32 dwLine) const	{ return m_oAllSet.find(dwLine) != m_oAllSet.end(); }

	//UINT32 GetConnID(UINT32 dwLine){return LineLink::GetConnID(dwLine);}
	void CloseAllLink();
private:
	std::set<UINT32>	m_oAllSet;
};

#endif
