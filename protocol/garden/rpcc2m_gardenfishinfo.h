#ifndef __RPCC2M_GARDENFISHINFO_H__
#define __RPCC2M_GARDENFISHINFO_H__

// generate by ProtoGen at date: 2016/10/22 17:31:29

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2M_GARDENFISHINFO_TYPE 10768

class RpcC2M_GardenFishInfo : public CRpc
{
private:

	RpcC2M_GardenFishInfo():CRpc(RPCC2M_GARDENFISHINFO_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2M_GardenFishInfo *CreateRpc()
	{
		return new RpcC2M_GardenFishInfo;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const GardenFishInfoArg &roArg, const GardenFishInfoRes &roRes, const CUserData &roUserData);
	void OnTimeout(const GardenFishInfoArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const GardenFishInfoArg &roArg, GardenFishInfoRes &roRes);
	void OnDelayReplyRpc(const GardenFishInfoArg &roArg, GardenFishInfoRes &roRes, const CUserData &roUserData);

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
	GardenFishInfoArg m_oArg;
	GardenFishInfoRes m_oRes;
};

#endif
