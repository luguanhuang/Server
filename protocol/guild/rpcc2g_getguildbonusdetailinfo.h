#ifndef __RPCC2G_GETGUILDBONUSDETAILINFO_H__
#define __RPCC2G_GETGUILDBONUSDETAILINFO_H__

// generate by ProtoGen at date: 2015/10/8 20:03:53

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2G_GETGUILDBONUSDETAILINFO_TYPE 20027

class RpcC2G_GetGuildBonusDetailInfo : public CRpc
{
private:

	RpcC2G_GetGuildBonusDetailInfo():CRpc(RPCC2G_GETGUILDBONUSDETAILINFO_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2G_GetGuildBonusDetailInfo *CreateRpc()
	{
		return new RpcC2G_GetGuildBonusDetailInfo;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const GetGuildBonusDetailInfoArg &roArg, const GetGuildBonusDetailInfoResult &roRes, const CUserData &roUserData);
	void OnTimeout(const GetGuildBonusDetailInfoArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const GetGuildBonusDetailInfoArg &roArg, GetGuildBonusDetailInfoResult &roRes);
	void OnDelayReplyRpc(const GetGuildBonusDetailInfoArg &roArg, GetGuildBonusDetailInfoResult &roRes, const CUserData &roUserData);

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
	GetGuildBonusDetailInfoArg m_oArg;
	GetGuildBonusDetailInfoResult m_oRes;
};

#endif
