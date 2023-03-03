#ifndef __RPCC2G_GETWATCHINFOBYID_H__
#define __RPCC2G_GETWATCHINFOBYID_H__

// generate by ProtoGen at date: 2016/7/11 11:04:35

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2G_GETWATCHINFOBYID_TYPE 45635

class RpcC2G_GetWatchInfoByID : public CRpc
{
private:

	RpcC2G_GetWatchInfoByID():CRpc(RPCC2G_GETWATCHINFOBYID_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2G_GetWatchInfoByID *CreateRpc()
	{
		return new RpcC2G_GetWatchInfoByID;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const GetWatchInfoByIDArg &roArg, const GetWatchInfoByIDRes &roRes, const CUserData &roUserData);
	void OnTimeout(const GetWatchInfoByIDArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const GetWatchInfoByIDArg &roArg, GetWatchInfoByIDRes &roRes);
	void OnDelayReplyRpc(const GetWatchInfoByIDArg &roArg, GetWatchInfoByIDRes &roRes, const CUserData &roUserData);

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
	GetWatchInfoByIDArg m_oArg;
	GetWatchInfoByIDRes m_oRes;
};

#endif
