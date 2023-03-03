#ifndef __PTCG2C_NOTICEDOINGGUILDINHERIT_H__
#define __PTCG2C_NOTICEDOINGGUILDINHERIT_H__

// generate by ProtoGen at date: 2016/11/18 21:08:43

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCG2C_NOTICEDOINGGUILDINHERIT_TYPE 61639

class PtcG2C_NoticeDoingGuildInherit : public CProtocol
{
public:
    explicit PtcG2C_NoticeDoingGuildInherit() : CProtocol(PTCG2C_NOTICEDOINGGUILDINHERIT_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcG2C_NoticeDoingGuildInherit()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::NoticeDoingGuildInherit m_Data;
};

#endif