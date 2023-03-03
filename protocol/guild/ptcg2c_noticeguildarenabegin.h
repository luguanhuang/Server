#ifndef __PTCG2C_NOTICEGUILDARENABEGIN_H__
#define __PTCG2C_NOTICEGUILDARENABEGIN_H__

// generate by ProtoGen at date: 2016/7/23 19:32:57

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCG2C_NOTICEGUILDARENABEGIN_TYPE 11695

class PtcG2C_NoticeGuildArenaBegin : public CProtocol
{
public:
    explicit PtcG2C_NoticeGuildArenaBegin() : CProtocol(PTCG2C_NOTICEGUILDARENABEGIN_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcG2C_NoticeGuildArenaBegin()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::NoticeGuildArenaBegin m_Data;
};

#endif