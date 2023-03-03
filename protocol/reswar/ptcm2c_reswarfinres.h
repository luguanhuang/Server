#ifndef __PTCM2C_RESWARFINRES_H__
#define __PTCM2C_RESWARFINRES_H__

// generate by ProtoGen at date: 2016/11/1 20:25:08

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCM2C_RESWARFINRES_TYPE 16904

class PtcM2C_ResWarFinRes : public CProtocol
{
public:
    explicit PtcM2C_ResWarFinRes() : CProtocol(PTCM2C_RESWARFINRES_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcM2C_ResWarFinRes()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::ResWarFinalInfo m_Data;
};

#endif