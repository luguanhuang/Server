#ifndef __PTCM2D_DELETECROSSROLESUMMARY_H__
#define __PTCM2D_DELETECROSSROLESUMMARY_H__

// generate by ProtoGen at date: 2017/2/7 21:48:42

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCM2D_DELETECROSSROLESUMMARY_TYPE 33732

class PtcM2D_DeleteCrossRoleSummary : public CProtocol
{
public:
    explicit PtcM2D_DeleteCrossRoleSummary() : CProtocol(PTCM2D_DELETECROSSROLESUMMARY_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcM2D_DeleteCrossRoleSummary()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::DeleteCrossRoleSummary m_Data;
};

#endif