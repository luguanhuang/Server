#ifndef __PTCM2D_UPDATEDESIGNATION_H__
#define __PTCM2D_UPDATEDESIGNATION_H__

// generate by ProtoGen at date: 2016/9/20 23:17:21

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCM2D_UPDATEDESIGNATION_TYPE 20483

class PtcM2D_UpdateDesignation : public CProtocol
{
public:
    explicit PtcM2D_UpdateDesignation() : CProtocol(PTCM2D_UPDATEDESIGNATION_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcM2D_UpdateDesignation()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::AllDesignationEvent m_Data;
};

#endif