#ifndef __PTCG2C_ENTERSCENECOOLDOWNNOTIFY_H__
#define __PTCG2C_ENTERSCENECOOLDOWNNOTIFY_H__

// generate by ProtoGen at date: 2015/7/30 18:16:17

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCG2C_ENTERSCENECOOLDOWNNOTIFY_TYPE 38664

class PtcG2C_EnterSceneCoolDownNotify : public CProtocol
{
public:
    explicit PtcG2C_EnterSceneCoolDownNotify() : CProtocol(PTCG2C_ENTERSCENECOOLDOWNNOTIFY_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcG2C_EnterSceneCoolDownNotify()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::EnterSceneCoolDownNotify m_Data;
};

#endif