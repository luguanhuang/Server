#ifndef __PTCG2C_GARDENPLANTEVENTNOTICE_H__
#define __PTCG2C_GARDENPLANTEVENTNOTICE_H__

// generate by ProtoGen at date: 2016/10/22 9:51:11

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCG2C_GARDENPLANTEVENTNOTICE_TYPE 60686

class PtcG2C_GardenPlantEventNotice : public CProtocol
{
public:
    explicit PtcG2C_GardenPlantEventNotice() : CProtocol(PTCG2C_GARDENPLANTEVENTNOTICE_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcG2C_GardenPlantEventNotice()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::GardenPlantEventNoticeArg m_Data;
};

#endif