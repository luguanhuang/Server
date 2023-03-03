#ifndef __RPCC2M_FETCHPLANTINFO_H__
#define __RPCC2M_FETCHPLANTINFO_H__

// generate by ProtoGen at date: 2016/10/5 10:27:58

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2M_FETCHPLANTINFO_TYPE 19949

class RpcC2M_FetchPlantInfo : public CRpc
{
private:

	RpcC2M_FetchPlantInfo():CRpc(RPCC2M_FETCHPLANTINFO_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2M_FetchPlantInfo *CreateRpc()
	{
		return new RpcC2M_FetchPlantInfo;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const FetchPlantInfoArg &roArg, const FetchPlantInfoRes &roRes, const CUserData &roUserData);
	void OnTimeout(const FetchPlantInfoArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const FetchPlantInfoArg &roArg, FetchPlantInfoRes &roRes);
	void OnDelayReplyRpc(const FetchPlantInfoArg &roArg, FetchPlantInfoRes &roRes, const CUserData &roUserData);

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
	FetchPlantInfoArg m_oArg;
	FetchPlantInfoRes m_oRes;
};

#endif
