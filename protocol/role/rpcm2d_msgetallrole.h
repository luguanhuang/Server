#ifndef __RPCM2D_MSGETALLROLE_H__
#define __RPCM2D_MSGETALLROLE_H__

// generate by ProtoGen at date: 2016/7/21 0:18:30

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCM2D_MSGETALLROLE_TYPE 13928

class RpcM2D_MsGetAllRole : public CRpc
{
private:

	RpcM2D_MsGetAllRole():CRpc(RPCM2D_MSGETALLROLE_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcM2D_MsGetAllRole *CreateRpc()
	{
		return new RpcM2D_MsGetAllRole;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const GetAllRoleArg &roArg, const GetAllRoleRes &roRes, const CUserData &roUserData);
	void OnTimeout(const GetAllRoleArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const GetAllRoleArg &roArg, GetAllRoleRes &roRes);
	void OnDelayReplyRpc(const GetAllRoleArg &roArg, GetAllRoleRes &roRes, const CUserData &roUserData);

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
	GetAllRoleArg m_oArg;
	GetAllRoleRes m_oRes;
};

#endif
