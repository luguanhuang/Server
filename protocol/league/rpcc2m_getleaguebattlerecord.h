#ifndef __RPCC2M_GETLEAGUEBATTLERECORD_H__
#define __RPCC2M_GETLEAGUEBATTLERECORD_H__

// generate by ProtoGen at date: 2017/1/13 9:34:06

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2M_GETLEAGUEBATTLERECORD_TYPE 51407

class RpcC2M_GetLeagueBattleRecord : public CRpc
{
private:

	RpcC2M_GetLeagueBattleRecord():CRpc(RPCC2M_GETLEAGUEBATTLERECORD_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2M_GetLeagueBattleRecord *CreateRpc()
	{
		return new RpcC2M_GetLeagueBattleRecord;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const GetLeagueBattleRecordArg &roArg, const GetLeagueBattleRecordRes &roRes, const CUserData &roUserData);
	void OnTimeout(const GetLeagueBattleRecordArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const GetLeagueBattleRecordArg &roArg, GetLeagueBattleRecordRes &roRes);
	void OnDelayReplyRpc(const GetLeagueBattleRecordArg &roArg, GetLeagueBattleRecordRes &roRes, const CUserData &roUserData);

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
	GetLeagueBattleRecordArg m_oArg;
	GetLeagueBattleRecordRes m_oRes;
};

#endif
