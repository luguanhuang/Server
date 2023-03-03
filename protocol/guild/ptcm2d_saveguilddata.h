#ifndef __PTCM2D_SAVEGUILDDATA_H__
#define __PTCM2D_SAVEGUILDDATA_H__

// generate by ProtoGen at date: 2016/8/26 10:56:00

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCM2D_SAVEGUILDDATA_TYPE 7855

class PtcM2D_SaveGuildData : public CProtocol
{
public:
    explicit PtcM2D_SaveGuildData() : CProtocol(PTCM2D_SAVEGUILDDATA_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcM2D_SaveGuildData()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::GuildAllDBData m_Data;
};

#endif