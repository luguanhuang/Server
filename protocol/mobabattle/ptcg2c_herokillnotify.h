#ifndef __PTCG2C_HEROKILLNOTIFY_H__
#define __PTCG2C_HEROKILLNOTIFY_H__

// generate by ProtoGen at date: 2017/6/9 17:10:16

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCG2C_HEROKILLNOTIFY_TYPE 58962

class PtcG2C_HeroKillNotify : public CProtocol
{
public:
    explicit PtcG2C_HeroKillNotify() : CProtocol(PTCG2C_HEROKILLNOTIFY_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcG2C_HeroKillNotify()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::HeroKillNotifyData m_Data;
};

#endif