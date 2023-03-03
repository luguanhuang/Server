#ifndef __PTCM2G_CLICKNOTICENTF_H__
#define __PTCM2G_CLICKNOTICENTF_H__

// generate by ProtoGen at date: 2017/2/4 11:17:11

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCM2G_CLICKNOTICENTF_TYPE 61015

class PtcM2G_ClickNoticeNtf : public CProtocol
{
public:
    explicit PtcM2G_ClickNoticeNtf() : CProtocol(PTCM2G_CLICKNOTICENTF_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcM2G_ClickNoticeNtf()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::ClickNoticeData m_Data;
};

#endif