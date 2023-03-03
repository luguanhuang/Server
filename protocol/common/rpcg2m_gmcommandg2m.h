#ifndef __RPCG2M_GMCOMMANDG2M_H__
#define __RPCG2M_GMCOMMANDG2M_H__

// generate by ProtoGen at date: 2016/8/22 17:04:04

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCG2M_GMCOMMANDG2M_TYPE 25293

class RpcG2M_GMCommandG2M : public CRpc
{
private:

	RpcG2M_GMCommandG2M():CRpc(RPCG2M_GMCOMMANDG2M_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcG2M_GMCommandG2M *CreateRpc()
	{
		return new RpcG2M_GMCommandG2M;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const GMCmdg2mArg &roArg, const GMCmdg2mRes &roRes, const CUserData &roUserData);
	void OnTimeout(const GMCmdg2mArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const GMCmdg2mArg &roArg, GMCmdg2mRes &roRes);
	void OnDelayReplyRpc(const GMCmdg2mArg &roArg, GMCmdg2mRes &roRes, const CUserData &roUserData);

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
	GMCmdg2mArg m_oArg;
	GMCmdg2mRes m_oRes;
};

#endif
