#ifndef __RPCC2M_GARDENBANQUETAWARD_H__
#define __RPCC2M_GARDENBANQUETAWARD_H__

// generate by ProtoGen at date: 2016/10/20 16:54:21

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2M_GARDENBANQUETAWARD_TYPE 1091

class RpcC2M_GardenBanquetAward : public CRpc
{
private:

	RpcC2M_GardenBanquetAward():CRpc(RPCC2M_GARDENBANQUETAWARD_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2M_GardenBanquetAward *CreateRpc()
	{
		return new RpcC2M_GardenBanquetAward;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const BanquetAwardArg &roArg, const BanquetAwardRes &roRes, const CUserData &roUserData);
	void OnTimeout(const BanquetAwardArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const BanquetAwardArg &roArg, BanquetAwardRes &roRes);
	void OnDelayReplyRpc(const BanquetAwardArg &roArg, BanquetAwardRes &roRes, const CUserData &roUserData);

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
	BanquetAwardArg m_oArg;
	BanquetAwardRes m_oRes;
};

#endif
