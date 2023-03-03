#ifndef __PTCM2C_NOTICEGUILDTERRWAR_H__
#define __PTCM2C_NOTICEGUILDTERRWAR_H__

// generate by ProtoGen at date: 2016/12/21 15:35:30

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCM2C_NOTICEGUILDTERRWAR_TYPE 17274

class PtcM2C_NoticeGuildTerrWar : public CProtocol
{
public:
    explicit PtcM2C_NoticeGuildTerrWar() : CProtocol(PTCM2C_NOTICEGUILDTERRWAR_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcM2C_NoticeGuildTerrWar()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::NoticeGuildTerrWar m_Data;
};

#endif