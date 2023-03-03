#ifndef __PTCG2M_NOTICEGUILDBOSSDIE_H__
#define __PTCG2M_NOTICEGUILDBOSSDIE_H__

// generate by ProtoGen at date: 2016/8/6 18:05:41

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCG2M_NOTICEGUILDBOSSDIE_TYPE 21896

class PtcG2M_NoticeGuildBossDie : public CProtocol
{
public:
    explicit PtcG2M_NoticeGuildBossDie() : CProtocol(PTCG2M_NOTICEGUILDBOSSDIE_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcG2M_NoticeGuildBossDie()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::guildid m_Data;
};

#endif