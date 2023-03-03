#ifndef __PTCM2G_UPDATEPARTNERTOGS_H__
#define __PTCM2G_UPDATEPARTNERTOGS_H__

// generate by ProtoGen at date: 2017/3/15 10:56:33

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCM2G_UPDATEPARTNERTOGS_TYPE 31031

class PtcM2G_UpdatePartnerToGS : public CProtocol
{
public:
    explicit PtcM2G_UpdatePartnerToGS() : CProtocol(PTCM2G_UPDATEPARTNERTOGS_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcM2G_UpdatePartnerToGS()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::UpdatePartnerToGS m_Data;
};

#endif