#ifndef __PTCM2K_GETROLESUMMARYFROMWORLD_H__
#define __PTCM2K_GETROLESUMMARYFROMWORLD_H__

// generate by ProtoGen at date: 2017/2/13 16:15:55

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCM2K_GETROLESUMMARYFROMWORLD_TYPE 16628

class PtcM2K_GetRoleSummaryFromWorld : public CProtocol
{
public:
    explicit PtcM2K_GetRoleSummaryFromWorld() : CProtocol(PTCM2K_GETROLESUMMARYFROMWORLD_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcM2K_GetRoleSummaryFromWorld()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::GetRoleSummaryFromWorld m_Data;
};

#endif