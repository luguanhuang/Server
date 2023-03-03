#ifndef __PTCM2C_NOTICEGUILDARENABEGINNEW_H__
#define __PTCM2C_NOTICEGUILDARENABEGINNEW_H__

// generate by ProtoGen at date: 2016/9/8 15:16:56

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCM2C_NOTICEGUILDARENABEGINNEW_TYPE 12290

class PtcM2C_NoticeGuildArenaBeginNew : public CProtocol
{
public:
    explicit PtcM2C_NoticeGuildArenaBeginNew() : CProtocol(PTCM2C_NOTICEGUILDARENABEGINNEW_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcM2C_NoticeGuildArenaBeginNew()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::NoticeGuildArenaBegin m_Data;
};

#endif