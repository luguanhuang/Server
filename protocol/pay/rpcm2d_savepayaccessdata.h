#ifndef __RPCM2D_SAVEPAYACCESSDATA_H__
#define __RPCM2D_SAVEPAYACCESSDATA_H__

// generate by ProtoGen at date: 2016/12/21 17:24:44

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCM2D_SAVEPAYACCESSDATA_TYPE 46712

class RpcM2D_SavePayAccessData : public CRpc
{
private:

	RpcM2D_SavePayAccessData():CRpc(RPCM2D_SAVEPAYACCESSDATA_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcM2D_SavePayAccessData *CreateRpc()
	{
		return new RpcM2D_SavePayAccessData;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const SavePayAccessArg &roArg, const SavePayAccessDataRes &roRes, const CUserData &roUserData);
	void OnTimeout(const SavePayAccessArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const SavePayAccessArg &roArg, SavePayAccessDataRes &roRes);
	void OnDelayReplyRpc(const SavePayAccessArg &roArg, SavePayAccessDataRes &roRes, const CUserData &roUserData);

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
	SavePayAccessArg m_oArg;
	SavePayAccessDataRes m_oRes;
};

#endif
