#ifndef __RPCC2G_DRAGONGROUPRECORD_H__
#define __RPCC2G_DRAGONGROUPRECORD_H__

// generate by ProtoGen at date: 2017/7/28 13:15:08

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2G_DRAGONGROUPRECORD_TYPE 62181

class RpcC2G_DragonGroupRecord : public CRpc
{
private:

	RpcC2G_DragonGroupRecord():CRpc(RPCC2G_DRAGONGROUPRECORD_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2G_DragonGroupRecord *CreateRpc()
	{
		return new RpcC2G_DragonGroupRecord;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const DragonGroupRecordC2S &roArg, const DragonGroupRecordS2C &roRes, const CUserData &roUserData);
	void OnTimeout(const DragonGroupRecordC2S &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const DragonGroupRecordC2S &roArg, DragonGroupRecordS2C &roRes);
	void OnDelayReplyRpc(const DragonGroupRecordC2S &roArg, DragonGroupRecordS2C &roRes, const CUserData &roUserData);

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
	DragonGroupRecordC2S m_oArg;
	DragonGroupRecordS2C m_oRes;
};

#endif
