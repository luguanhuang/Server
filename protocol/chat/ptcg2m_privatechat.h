#ifndef __PTCG2M_PRIVATECHAT_H__
#define __PTCG2M_PRIVATECHAT_H__

// generate by ProtoGen at date: 2017/9/15 15:00:28

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCG2M_PRIVATECHAT_TYPE 30597

class PtcG2M_PrivateChat : public CProtocol
{
public:
    explicit PtcG2M_PrivateChat() : CProtocol(PTCG2M_PRIVATECHAT_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcG2M_PrivateChat()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::PrivateChatGS m_Data;
};

#endif