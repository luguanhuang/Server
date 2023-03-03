#ifndef __RPCC2M_GETMOBABATTLEINFO_H__
#define __RPCC2M_GETMOBABATTLEINFO_H__

// generate by ProtoGen at date: 2017/6/26 14:31:51

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2M_GETMOBABATTLEINFO_TYPE 64051

class RpcC2M_GetMobaBattleInfo : public CRpc
{
private:

	RpcC2M_GetMobaBattleInfo():CRpc(RPCC2M_GETMOBABATTLEINFO_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2M_GetMobaBattleInfo *CreateRpc()
	{
		return new RpcC2M_GetMobaBattleInfo;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const GetMobaBattleInfoArg &roArg, const GetMobaBattleInfoRes &roRes, const CUserData &roUserData);
	void OnTimeout(const GetMobaBattleInfoArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const GetMobaBattleInfoArg &roArg, GetMobaBattleInfoRes &roRes);
	void OnDelayReplyRpc(const GetMobaBattleInfoArg &roArg, GetMobaBattleInfoRes &roRes, const CUserData &roUserData);

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
	GetMobaBattleInfoArg m_oArg;
	GetMobaBattleInfoRes m_oRes;
};

#endif
