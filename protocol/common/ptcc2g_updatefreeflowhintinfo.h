#ifndef __PTCC2G_UPDATEFREEFLOWHINTINFO_H__
#define __PTCC2G_UPDATEFREEFLOWHINTINFO_H__

// generate by ProtoGen at date: 2017/9/21 19:43:57

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCC2G_UPDATEFREEFLOWHINTINFO_TYPE 27628

class PtcC2G_UpdateFreeflowHintInfo : public CProtocol
{
public:
    explicit PtcC2G_UpdateFreeflowHintInfo() : CProtocol(PTCC2G_UPDATEFREEFLOWHINTINFO_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcC2G_UpdateFreeflowHintInfo()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::UpdateFreeflowHintInfo m_Data;
};

#endif