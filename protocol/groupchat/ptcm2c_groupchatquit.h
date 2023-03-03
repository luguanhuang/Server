#ifndef __PTCM2C_GROUPCHATQUIT_H__
#define __PTCM2C_GROUPCHATQUIT_H__

// generate by ProtoGen at date: 2017/7/15 11:54:32

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCM2C_GROUPCHATQUIT_TYPE 56654

class PtcM2C_GroupChatQuit : public CProtocol
{
public:
    explicit PtcM2C_GroupChatQuit() : CProtocol(PTCM2C_GROUPCHATQUIT_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcM2C_GroupChatQuit()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::GroupChatQuitPtc m_Data;
};

#endif