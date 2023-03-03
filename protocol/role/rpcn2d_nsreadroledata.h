#ifndef __RPCN2D_NSREADROLEDATA_H__
#define __RPCN2D_NSREADROLEDATA_H__

// generate by ProtoGen at date: 2016/11/3 1:34:54

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCN2D_NSREADROLEDATA_TYPE 38912

class RpcN2D_NsReadRoleData : public CRpc
{
private:

	RpcN2D_NsReadRoleData():CRpc(RPCN2D_NSREADROLEDATA_TYPE)
	{
        m_dwTimeout = 50000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcN2D_NsReadRoleData *CreateRpc()
	{
		return new RpcN2D_NsReadRoleData;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const ReadRoleDataArg &roArg, const ReadRoleDataRes &roRes, const CUserData &roUserData);
	void OnTimeout(const ReadRoleDataArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const ReadRoleDataArg &roArg, ReadRoleDataRes &roRes);
	void OnDelayReplyRpc(const ReadRoleDataArg &roArg, ReadRoleDataRes &roRes, const CUserData &roUserData);

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
	ReadRoleDataArg m_oArg;
	ReadRoleDataRes m_oRes;
};

#endif
