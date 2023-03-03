#ifndef __PTCG2M_MSCRASHRECOVERG2M_H__
#define __PTCG2M_MSCRASHRECOVERG2M_H__

// generate by ProtoGen at date: 2016/11/23 20:49:12

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCG2M_MSCRASHRECOVERG2M_TYPE 37460

class PtcG2M_mscrashrecoverg2m : public CProtocol
{
public:
    explicit PtcG2M_mscrashrecoverg2m() : CProtocol(PTCG2M_MSCRASHRECOVERG2M_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcG2M_mscrashrecoverg2m()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::MSCrashRecoverG2M m_Data;
};

#endif