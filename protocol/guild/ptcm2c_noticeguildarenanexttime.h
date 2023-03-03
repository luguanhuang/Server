#ifndef __PTCM2C_NOTICEGUILDARENANEXTTIME_H__
#define __PTCM2C_NOTICEGUILDARENANEXTTIME_H__

// generate by ProtoGen at date: 2016/10/17 14:11:48

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCM2C_NOTICEGUILDARENANEXTTIME_TYPE 21612

class PtcM2C_NoticeGuildArenaNextTime : public CProtocol
{
public:
    explicit PtcM2C_NoticeGuildArenaNextTime() : CProtocol(PTCM2C_NOTICEGUILDARENANEXTTIME_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcM2C_NoticeGuildArenaNextTime()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::NoticeGuildArenaNextTime m_Data;
};

#endif