#ifndef __PTCG2C_NOTICEGUILDBOSSEND_H__
#define __PTCG2C_NOTICEGUILDBOSSEND_H__

// generate by ProtoGen at date: 2016/7/18 11:58:30

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCG2C_NOTICEGUILDBOSSEND_TYPE 34184

class PtcG2C_NoticeGuildBossEnd : public CProtocol
{
public:
    explicit PtcG2C_NoticeGuildBossEnd() : CProtocol(PTCG2C_NOTICEGUILDBOSSEND_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcG2C_NoticeGuildBossEnd()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::NoticeGuildBossEnd m_Data;
};

#endif