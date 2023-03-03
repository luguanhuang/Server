#ifndef __RPCC2M_RESWAREXPLORE_H__
#define __RPCC2M_RESWAREXPLORE_H__

// generate by ProtoGen at date: 2016/10/28 19:10:45

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2M_RESWAREXPLORE_TYPE 33965

class RpcC2M_ResWarExplore : public CRpc
{
private:

	RpcC2M_ResWarExplore():CRpc(RPCC2M_RESWAREXPLORE_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2M_ResWarExplore *CreateRpc()
	{
		return new RpcC2M_ResWarExplore;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const ResWarExploreArg &roArg, const ResWarExploreRes &roRes, const CUserData &roUserData);
	void OnTimeout(const ResWarExploreArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const ResWarExploreArg &roArg, ResWarExploreRes &roRes);
	void OnDelayReplyRpc(const ResWarExploreArg &roArg, ResWarExploreRes &roRes, const CUserData &roUserData);

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
	ResWarExploreArg m_oArg;
	ResWarExploreRes m_oRes;
};

#endif
