#ifndef __RPCC2G_TALKGUILDCAMPSPRITE_H__
#define __RPCC2G_TALKGUILDCAMPSPRITE_H__

// generate by ProtoGen at date: 2017/4/26 11:13:22

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2G_TALKGUILDCAMPSPRITE_TYPE 5683

class RpcC2G_TalkGuildCampSprite : public CRpc
{
private:

	RpcC2G_TalkGuildCampSprite():CRpc(RPCC2G_TALKGUILDCAMPSPRITE_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2G_TalkGuildCampSprite *CreateRpc()
	{
		return new RpcC2G_TalkGuildCampSprite;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const TalkGuildCampSpriteArg &roArg, const TalkGuildCampSpriteRes &roRes, const CUserData &roUserData);
	void OnTimeout(const TalkGuildCampSpriteArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const TalkGuildCampSpriteArg &roArg, TalkGuildCampSpriteRes &roRes);
	void OnDelayReplyRpc(const TalkGuildCampSpriteArg &roArg, TalkGuildCampSpriteRes &roRes, const CUserData &roUserData);

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
	TalkGuildCampSpriteArg m_oArg;
	TalkGuildCampSpriteRes m_oRes;
};

#endif
