#ifndef __PTCM2G_ROLEEVENTSYNCGS_H__
#define __PTCM2G_ROLEEVENTSYNCGS_H__

// generate by ProtoGen at date: 2016/10/14 16:11:45

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCM2G_ROLEEVENTSYNCGS_TYPE 43221

class PtcM2G_RoleEventSyncGS : public CProtocol
{
public:
    explicit PtcM2G_RoleEventSyncGS() : CProtocol(PTCM2G_ROLEEVENTSYNCGS_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcM2G_RoleEventSyncGS()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::RoleEventSync m_Data;
};

#endif