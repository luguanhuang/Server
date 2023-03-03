#ifndef __PTCM2D_SAVEPUNISHDATA_H__
#define __PTCM2D_SAVEPUNISHDATA_H__

// generate by ProtoGen at date: 2016/9/2 16:30:29

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCM2D_SAVEPUNISHDATA_TYPE 5616

class PtcM2D_SavePunishData : public CProtocol
{
public:
    explicit PtcM2D_SavePunishData() : CProtocol(PTCM2D_SAVEPUNISHDATA_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcM2D_SavePunishData()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::PunishData m_Data;
};

#endif