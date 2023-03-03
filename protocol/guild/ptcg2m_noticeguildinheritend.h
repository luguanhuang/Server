#ifndef __PTCG2M_NOTICEGUILDINHERITEND_H__
#define __PTCG2M_NOTICEGUILDINHERITEND_H__

// generate by ProtoGen at date: 2016/11/23 17:03:20

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCG2M_NOTICEGUILDINHERITEND_TYPE 4062

class PtcG2M_NoticeGuildInheritEnd : public CProtocol
{
public:
    explicit PtcG2M_NoticeGuildInheritEnd() : CProtocol(PTCG2M_NOTICEGUILDINHERITEND_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcG2M_NoticeGuildInheritEnd()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::NoticeGuildInheritEnd m_Data;
};

#endif