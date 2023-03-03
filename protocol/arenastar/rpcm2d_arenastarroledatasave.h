#ifndef __RPCM2D_ARENASTARROLEDATASAVE_H__
#define __RPCM2D_ARENASTARROLEDATASAVE_H__

// generate by ProtoGen at date: 2017/3/29 11:14:35

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCM2D_ARENASTARROLEDATASAVE_TYPE 24822

class RpcM2D_ArenaStarRoleDataSave : public CRpc
{
private:

	RpcM2D_ArenaStarRoleDataSave():CRpc(RPCM2D_ARENASTARROLEDATASAVE_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcM2D_ArenaStarRoleDataSave *CreateRpc()
	{
		return new RpcM2D_ArenaStarRoleDataSave;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const ArenaStarDataSaveArg &roArg, const ArenaStarDataSaveRes &roRes, const CUserData &roUserData);
	void OnTimeout(const ArenaStarDataSaveArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const ArenaStarDataSaveArg &roArg, ArenaStarDataSaveRes &roRes);
	void OnDelayReplyRpc(const ArenaStarDataSaveArg &roArg, ArenaStarDataSaveRes &roRes, const CUserData &roUserData);

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
	ArenaStarDataSaveArg m_oArg;
	ArenaStarDataSaveRes m_oRes;
};

#endif
