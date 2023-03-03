#ifndef __PTCM2C_GROUPCHATDISMISS_H__
#define __PTCM2C_GROUPCHATDISMISS_H__

// generate by ProtoGen at date: 2017/7/15 11:55:43

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCM2C_GROUPCHATDISMISS_TYPE 18973

class PtcM2C_GroupChatDismiss : public CProtocol
{
public:
    explicit PtcM2C_GroupChatDismiss() : CProtocol(PTCM2C_GROUPCHATDISMISS_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcM2C_GroupChatDismiss()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::GroupChatDismissPtc m_Data;
};

#endif