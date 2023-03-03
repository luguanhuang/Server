#ifndef __PTCG2C_AIDEBUGINFO_H__
#define __PTCG2C_AIDEBUGINFO_H__

// generate by ProtoGen at date: 2016/8/19 16:29:32

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCG2C_AIDEBUGINFO_TYPE 60081

class PtcG2C_AIDebugInfo : public CProtocol
{
public:
    explicit PtcG2C_AIDebugInfo() : CProtocol(PTCG2C_AIDEBUGINFO_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcG2C_AIDebugInfo()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::AIDebugMsg m_Data;
};

#endif