#ifndef __RPCC2M_GARDENBANQUET_H__
#define __RPCC2M_GARDENBANQUET_H__

// generate by ProtoGen at date: 2016/10/13 19:17:33

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2M_GARDENBANQUET_TYPE 22527

class RpcC2M_GardenBanquet : public CRpc
{
private:

	RpcC2M_GardenBanquet():CRpc(RPCC2M_GARDENBANQUET_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2M_GardenBanquet *CreateRpc()
	{
		return new RpcC2M_GardenBanquet;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const GardenBanquetArg &roArg, const GardenBanquetRes &roRes, const CUserData &roUserData);
	void OnTimeout(const GardenBanquetArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const GardenBanquetArg &roArg, GardenBanquetRes &roRes);
	void OnDelayReplyRpc(const GardenBanquetArg &roArg, GardenBanquetRes &roRes, const CUserData &roUserData);

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
	GardenBanquetArg m_oArg;
	GardenBanquetRes m_oRes;
};

#endif
