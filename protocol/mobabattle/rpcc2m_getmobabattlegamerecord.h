#ifndef __RPCC2M_GETMOBABATTLEGAMERECORD_H__
#define __RPCC2M_GETMOBABATTLEGAMERECORD_H__

// generate by ProtoGen at date: 2017/6/16 0:20:27

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2M_GETMOBABATTLEGAMERECORD_TYPE 9583

class RpcC2M_GetMobaBattleGameRecord : public CRpc
{
private:

	RpcC2M_GetMobaBattleGameRecord():CRpc(RPCC2M_GETMOBABATTLEGAMERECORD_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2M_GetMobaBattleGameRecord *CreateRpc()
	{
		return new RpcC2M_GetMobaBattleGameRecord;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const GetMobaBattleGameRecordArg &roArg, const GetMobaBattleGameRecordRes &roRes, const CUserData &roUserData);
	void OnTimeout(const GetMobaBattleGameRecordArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const GetMobaBattleGameRecordArg &roArg, GetMobaBattleGameRecordRes &roRes);
	void OnDelayReplyRpc(const GetMobaBattleGameRecordArg &roArg, GetMobaBattleGameRecordRes &roRes, const CUserData &roUserData);

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
	GetMobaBattleGameRecordArg m_oArg;
	GetMobaBattleGameRecordRes m_oRes;
};

#endif
