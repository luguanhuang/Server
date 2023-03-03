#ifndef __PTCM2S_M2SUSERSTATUSSYNC_H__
#define __PTCM2S_M2SUSERSTATUSSYNC_H__

// generate by ProtoGen at date: 2017/4/14 19:29:40

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCM2S_M2SUSERSTATUSSYNC_TYPE 53444

class PtcM2S_M2SUserStatusSync : public CProtocol
{
public:
    explicit PtcM2S_M2SUserStatusSync() : CProtocol(PTCM2S_M2SUSERSTATUSSYNC_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcM2S_M2SUserStatusSync()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::M2SUserStatusSyncData m_Data;
};

#endif