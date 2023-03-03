#ifndef __PTCC2M_GMSETGUILDBOSSTIME_H__
#define __PTCC2M_GMSETGUILDBOSSTIME_H__

// generate by ProtoGen at date: 2016/8/5 17:27:08

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCC2M_GMSETGUILDBOSSTIME_TYPE 62392

class PtcC2M_GMSetGuildBossTime : public CProtocol
{
public:
    explicit PtcC2M_GMSetGuildBossTime() : CProtocol(PTCC2M_GMSETGUILDBOSSTIME_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcC2M_GMSetGuildBossTime()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::gmguildbosstime m_Data;
};

#endif