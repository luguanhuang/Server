#ifndef __RPCC2M_GARDENEXPELSPRITE_H__
#define __RPCC2M_GARDENEXPELSPRITE_H__

// generate by ProtoGen at date: 2016/10/24 12:30:20

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2M_GARDENEXPELSPRITE_TYPE 3250

class RpcC2M_GardenExpelSprite : public CRpc
{
private:

	RpcC2M_GardenExpelSprite():CRpc(RPCC2M_GARDENEXPELSPRITE_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2M_GardenExpelSprite *CreateRpc()
	{
		return new RpcC2M_GardenExpelSprite;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const GardenExpelSpriteArg &roArg, const GardenExpelSpriteRes &roRes, const CUserData &roUserData);
	void OnTimeout(const GardenExpelSpriteArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const GardenExpelSpriteArg &roArg, GardenExpelSpriteRes &roRes);
	void OnDelayReplyRpc(const GardenExpelSpriteArg &roArg, GardenExpelSpriteRes &roRes, const CUserData &roUserData);

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
	GardenExpelSpriteArg m_oArg;
	GardenExpelSpriteRes m_oRes;
};

#endif
