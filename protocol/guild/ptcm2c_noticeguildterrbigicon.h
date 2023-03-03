#ifndef __PTCM2C_NOTICEGUILDTERRBIGICON_H__
#define __PTCM2C_NOTICEGUILDTERRBIGICON_H__

// generate by ProtoGen at date: 2017/2/9 14:56:16

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCM2C_NOTICEGUILDTERRBIGICON_TYPE 13723

class PtcM2C_NoticeGuildTerrBigIcon : public CProtocol
{
public:
    explicit PtcM2C_NoticeGuildTerrBigIcon() : CProtocol(PTCM2C_NOTICEGUILDTERRBIGICON_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcM2C_NoticeGuildTerrBigIcon()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::NoticeGuildTerrBigIcon m_Data;
};

#endif