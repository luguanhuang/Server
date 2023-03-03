#ifndef __RPCC2M_GETMOBABATTLEBRIEFRECORD_H__
#define __RPCC2M_GETMOBABATTLEBRIEFRECORD_H__

// generate by ProtoGen at date: 2017/6/16 11:00:33

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2M_GETMOBABATTLEBRIEFRECORD_TYPE 35507

class RpcC2M_GetMobaBattleBriefRecord : public CRpc
{
private:

	RpcC2M_GetMobaBattleBriefRecord():CRpc(RPCC2M_GETMOBABATTLEBRIEFRECORD_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2M_GetMobaBattleBriefRecord *CreateRpc()
	{
		return new RpcC2M_GetMobaBattleBriefRecord;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const GetMobaBattleBriefRecordArg &roArg, const GetMobaBattleBriefRecordRes &roRes, const CUserData &roUserData);
	void OnTimeout(const GetMobaBattleBriefRecordArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const GetMobaBattleBriefRecordArg &roArg, GetMobaBattleBriefRecordRes &roRes);
	void OnDelayReplyRpc(const GetMobaBattleBriefRecordArg &roArg, GetMobaBattleBriefRecordRes &roRes, const CUserData &roUserData);

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
	GetMobaBattleBriefRecordArg m_oArg;
	GetMobaBattleBriefRecordRes m_oRes;
};

#endif
