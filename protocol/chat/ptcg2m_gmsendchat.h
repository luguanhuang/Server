#ifndef __PTCG2M_GMSENDCHAT_H__
#define __PTCG2M_GMSENDCHAT_H__

// generate by ProtoGen at date: 2017/6/6 18:14:42

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCG2M_GMSENDCHAT_TYPE 11601

class PtcG2M_GMSendChat : public CProtocol
{
public:
    explicit PtcG2M_GMSendChat() : CProtocol(PTCG2M_GMSENDCHAT_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcG2M_GMSendChat()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::ChatForward m_Data;
};

#endif