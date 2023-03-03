#ifndef __PTCC2M_GARDENFISHSTOP_H__
#define __PTCC2M_GARDENFISHSTOP_H__

// generate by ProtoGen at date: 2016/11/26 19:49:00

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCC2M_GARDENFISHSTOP_TYPE 56656

class PtcC2M_GardenFishStop : public CProtocol
{
public:
    explicit PtcC2M_GardenFishStop() : CProtocol(PTCC2M_GARDENFISHSTOP_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcC2M_GardenFishStop()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::GardenFishStopArg m_Data;
};

#endif