#ifndef __PTCR2G_MSCRASHRECOVERR2G_H__
#define __PTCR2G_MSCRASHRECOVERR2G_H__

// generate by ProtoGen at date: 2016/11/23 20:40:18

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCR2G_MSCRASHRECOVERR2G_TYPE 9568

class PtcR2G_mscrashrecoverr2g : public CProtocol
{
public:
    explicit PtcR2G_mscrashrecoverr2g() : CProtocol(PTCR2G_MSCRASHRECOVERR2G_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcR2G_mscrashrecoverr2g()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::MsCrashRecoverR2G m_Data;
};

#endif