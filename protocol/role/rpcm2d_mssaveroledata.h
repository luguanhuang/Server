#ifndef __RPCM2D_MSSAVEROLEDATA_H__
#define __RPCM2D_MSSAVEROLEDATA_H__

// generate by ProtoGen at date: 2016/11/1 22:38:56

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCM2D_MSSAVEROLEDATA_TYPE 57109

class RpcM2D_MsSaveRoleData : public CRpc
{
private:

	RpcM2D_MsSaveRoleData():CRpc(RPCM2D_MSSAVEROLEDATA_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcM2D_MsSaveRoleData *CreateRpc()
	{
		return new RpcM2D_MsSaveRoleData;
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
