#ifndef __PTCG2C_DPSNOTIFY_H__
#define __PTCG2C_DPSNOTIFY_H__

// generate by ProtoGen at date: 2016/10/13 22:41:07

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCG2C_DPSNOTIFY_TYPE 36800

class PtcG2C_DPSNotify : public CProtocol
{
public:
    explicit PtcG2C_DPSNotify() : CProtocol(PTCG2C_DPSNOTIFY_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcG2C_DPSNotify()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::DPSNotify m_Data;
};

#endif