#ifndef __RPCM2D_READROLEDATA_H__
#define __RPCM2D_READROLEDATA_H__

// generate by ProtoGen at date: 2016/7/10 14:06:11

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCM2D_READROLEDATA_TYPE 65264

class RpcM2D_ReadRoleData : public CRpc
{
private:

	RpcM2D_ReadRoleData():CRpc(RPCM2D_READROLEDATA_TYPE)
	{
        m_dwTimeout = 30000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcM2D_ReadRoleData *CreateRpc()
	{
		return new RpcM2D_ReadRoleData;
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
