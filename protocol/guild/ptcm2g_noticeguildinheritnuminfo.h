#ifndef __PTCM2G_NOTICEGUILDINHERITNUMINFO_H__
#define __PTCM2G_NOTICEGUILDINHERITNUMINFO_H__

// generate by ProtoGen at date: 2016/11/19 14:48:19

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCM2G_NOTICEGUILDINHERITNUMINFO_TYPE 22629

class PtcM2G_NoticeGuildInheritNumInfo : public CProtocol
{
public:
    explicit PtcM2G_NoticeGuildInheritNumInfo() : CProtocol(PTCM2G_NOTICEGUILDINHERITNUMINFO_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcM2G_NoticeGuildInheritNumInfo()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::NoticeGuildInheritNumInfo m_Data;
};

#endif