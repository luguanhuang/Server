#ifndef __RPCC2M_QUERYRESWARROLERANK_H__
#define __RPCC2M_QUERYRESWARROLERANK_H__

// generate by ProtoGen at date: 2016/11/7 20:10:17

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2M_QUERYRESWARROLERANK_TYPE 27001

class RpcC2M_QueryResWarRoleRank : public CRpc
{
private:

	RpcC2M_QueryResWarRoleRank():CRpc(RPCC2M_QUERYRESWARROLERANK_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2M_QueryResWarRoleRank *CreateRpc()
	{
		return new RpcC2M_QueryResWarRoleRank;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const ResWarRoleRankArg &roArg, const ResWarRoleRankRes &roRes, const CUserData &roUserData);
	void OnTimeout(const ResWarRoleRankArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const ResWarRoleRankArg &roArg, ResWarRoleRankRes &roRes);
	void OnDelayReplyRpc(const ResWarRoleRankArg &roArg, ResWarRoleRankRes &roRes, const CUserData &roUserData);

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
	ResWarRoleRankArg m_oArg;
	ResWarRoleRankRes m_oRes;
};

#endif
