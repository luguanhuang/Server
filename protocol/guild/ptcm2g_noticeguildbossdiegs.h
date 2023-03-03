#ifndef __PTCM2G_NOTICEGUILDBOSSDIEGS_H__
#define __PTCM2G_NOTICEGUILDBOSSDIEGS_H__

// generate by ProtoGen at date: 2016/8/6 18:05:50

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCM2G_NOTICEGUILDBOSSDIEGS_TYPE 55124

class PtcM2G_NoticeGuildBossDieGS : public CProtocol
{
public:
    explicit PtcM2G_NoticeGuildBossDieGS() : CProtocol(PTCM2G_NOTICEGUILDBOSSDIEGS_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcM2G_NoticeGuildBossDieGS()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::guildid m_Data;
};

#endif