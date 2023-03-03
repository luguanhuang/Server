#ifndef __PTCM2G_NOTICEGUILDBOSSATTR_H__
#define __PTCM2G_NOTICEGUILDBOSSATTR_H__

// generate by ProtoGen at date: 2017/1/15 15:42:35

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCM2G_NOTICEGUILDBOSSATTR_TYPE 830

class PtcM2G_NoticeGuildBossAttr : public CProtocol
{
public:
    explicit PtcM2G_NoticeGuildBossAttr() : CProtocol(PTCM2G_NOTICEGUILDBOSSATTR_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcM2G_NoticeGuildBossAttr()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::NoticeGuildBossAttr m_Data;
};

#endif