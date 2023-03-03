#ifndef __PTCG2M_GMSETGUILDBOSSTIME_H__
#define __PTCG2M_GMSETGUILDBOSSTIME_H__

// generate by ProtoGen at date: 2016/8/5 17:38:58

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCG2M_GMSETGUILDBOSSTIME_TYPE 51628

class PtcG2M_GmsetGuildBossTime : public CProtocol
{
public:
    explicit PtcG2M_GmsetGuildBossTime() : CProtocol(PTCG2M_GMSETGUILDBOSSTIME_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcG2M_GmsetGuildBossTime()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::gmguildbosstime m_Data;
};

#endif