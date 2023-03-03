#ifndef __PTCM2G_CHANGEGUILDBOSSTIME_H__
#define __PTCM2G_CHANGEGUILDBOSSTIME_H__

// generate by ProtoGen at date: 2016/8/8 15:31:55

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCM2G_CHANGEGUILDBOSSTIME_TYPE 21461

class PtcM2G_ChangeGuildBossTime : public CProtocol
{
public:
    explicit PtcM2G_ChangeGuildBossTime() : CProtocol(PTCM2G_CHANGEGUILDBOSSTIME_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcM2G_ChangeGuildBossTime()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::gmguildbosstime m_Data;
};

#endif