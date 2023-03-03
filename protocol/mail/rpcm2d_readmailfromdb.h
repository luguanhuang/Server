#ifndef __RPCM2D_READMAILFROMDB_H__
#define __RPCM2D_READMAILFROMDB_H__

// generate by ProtoGen at date: 2017/11/6 17:39:44

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCM2D_READMAILFROMDB_TYPE 19098

class RpcM2D_ReadMailFromDB : public CRpc
{
private:

	RpcM2D_ReadMailFromDB():CRpc(RPCM2D_READMAILFROMDB_TYPE)
	{
        m_dwTimeout = 60000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcM2D_ReadMailFromDB *CreateRpc()
	{
		return new RpcM2D_ReadMailFromDB;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const ReadMailFromDBArg &roArg, const ReadMailFromDBRes &roRes, const CUserData &roUserData);
	void OnTimeout(const ReadMailFromDBArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const ReadMailFromDBArg &roArg, ReadMailFromDBRes &roRes);
	void OnDelayReplyRpc(const ReadMailFromDBArg &roArg, ReadMailFromDBRes &roRes, const CUserData &roUserData);

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
	ReadMailFromDBArg m_oArg;
	ReadMailFromDBRes m_oRes;
};

#endif
