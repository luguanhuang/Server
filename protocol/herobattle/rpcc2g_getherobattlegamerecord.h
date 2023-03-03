#ifndef __RPCC2G_GETHEROBATTLEGAMERECORD_H__
#define __RPCC2G_GETHEROBATTLEGAMERECORD_H__

// generate by ProtoGen at date: 2016/12/27 15:37:44

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2G_GETHEROBATTLEGAMERECORD_TYPE 41057

class RpcC2G_GetHeroBattleGameRecord : public CRpc
{
private:

	RpcC2G_GetHeroBattleGameRecord():CRpc(RPCC2G_GETHEROBATTLEGAMERECORD_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2G_GetHeroBattleGameRecord *CreateRpc()
	{
		return new RpcC2G_GetHeroBattleGameRecord;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const GetHeroBattleGameRecordArg &roArg, const GetHeroBattleGameRecordRes &roRes, const CUserData &roUserData);
	void OnTimeout(const GetHeroBattleGameRecordArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const GetHeroBattleGameRecordArg &roArg, GetHeroBattleGameRecordRes &roRes);
	void OnDelayReplyRpc(const GetHeroBattleGameRecordArg &roArg, GetHeroBattleGameRecordRes &roRes, const CUserData &roUserData);

	virtual void OnReplyTimeout() 
	{ 
		OnTimeout(m_oArg, m_oUser); 
	}

	virtual void OnDelayRpc(const CUserData &userData)
	{
		OnDelayReplyRpc( m_oArg, m_oRes, userData);
	}

	virtual CRpc *NewRpc() 
	{
		return CreateRpc();
	}

public:
	GetHeroBattleGameRecordArg m_oArg;
	GetHeroBattleGameRecordRes m_oRes;
};

#endif
