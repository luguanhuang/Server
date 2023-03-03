#ifndef __RPCG2D_SAVEROLEDATA_H__
#define __RPCG2D_SAVEROLEDATA_H__

// generate by ProtoGen at date: 2017/3/31 17:14:27

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCG2D_SAVEROLEDATA_TYPE 54811

class RpcG2D_SaveRoleData : public CRpc
{
private:

	RpcG2D_SaveRoleData():CRpc(RPCG2D_SAVEROLEDATA_TYPE)
	{
        m_dwTimeout = 20000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcG2D_SaveRoleData *CreateRpc()
	{
		return new RpcG2D_SaveRoleData;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const SaveRoleDataArg &roArg, const SaveRoleDataRes &roRes, const CUserData &roUserData);
	void OnTimeout(const SaveRoleDataArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const SaveRoleDataArg &roArg, SaveRoleDataRes &roRes);
	void OnDelayReplyRpc(const SaveRoleDataArg &roArg, SaveRoleDataRes &roRes, const CUserData &roUserData);

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
	SaveRoleDataArg m_oArg;
	SaveRoleDataRes m_oRes;
};

#endif
