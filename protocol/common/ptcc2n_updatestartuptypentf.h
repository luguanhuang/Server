#ifndef __PTCC2N_UPDATESTARTUPTYPENTF_H__
#define __PTCC2N_UPDATESTARTUPTYPENTF_H__

// generate by ProtoGen at date: 2016/12/21 21:29:01

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCC2N_UPDATESTARTUPTYPENTF_TYPE 60574

class PtcC2N_UpdateStartUpTypeNtf : public CProtocol
{
public:
    explicit PtcC2N_UpdateStartUpTypeNtf() : CProtocol(PTCC2N_UPDATESTARTUPTYPENTF_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcC2N_UpdateStartUpTypeNtf()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::UpdateStartUpType m_Data;
};

#endif