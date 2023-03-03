#ifndef __PTCC2G_SETVOIPMEMBERSTATE_H__
#define __PTCC2G_SETVOIPMEMBERSTATE_H__

// generate by ProtoGen at date: 2016/9/9 15:44:18

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCC2G_SETVOIPMEMBERSTATE_TYPE 3881

class PtcC2G_SetVoipMemberState : public CProtocol
{
public:
    explicit PtcC2G_SetVoipMemberState() : CProtocol(PTCC2G_SETVOIPMEMBERSTATE_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcC2G_SetVoipMemberState()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::SetVoipMemberState m_Data;
};

#endif