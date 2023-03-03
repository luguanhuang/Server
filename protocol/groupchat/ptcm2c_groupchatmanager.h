#ifndef __PTCM2C_GROUPCHATMANAGER_H__
#define __PTCM2C_GROUPCHATMANAGER_H__

// generate by ProtoGen at date: 2017/7/15 11:52:56

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCM2C_GROUPCHATMANAGER_TYPE 17710

class PtcM2C_GroupChatManager : public CProtocol
{
public:
    explicit PtcM2C_GroupChatManager() : CProtocol(PTCM2C_GROUPCHATMANAGER_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcM2C_GroupChatManager()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::GroupChatManagerPtc m_Data;
};

#endif