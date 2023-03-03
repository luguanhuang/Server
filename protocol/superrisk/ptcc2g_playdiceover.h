#ifndef __PTCC2G_PLAYDICEOVER_H__
#define __PTCC2G_PLAYDICEOVER_H__

// generate by ProtoGen at date: 2016/9/5 16:14:04

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCC2G_PLAYDICEOVER_TYPE 2064

class PtcC2G_PlayDiceOver : public CProtocol
{
public:
    explicit PtcC2G_PlayDiceOver() : CProtocol(PTCC2G_PLAYDICEOVER_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcC2G_PlayDiceOver()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::PlayDiceOverData m_Data;
};

#endif