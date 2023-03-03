#ifndef __PTCM2C_LEAVETEAMM2CNTF_H__
#define __PTCM2C_LEAVETEAMM2CNTF_H__

// generate by ProtoGen at date: 2016/8/16 11:51:45

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCM2C_LEAVETEAMM2CNTF_TYPE 23306

class PtcM2C_LeaveTeamM2CNtf : public CProtocol
{
public:
    explicit PtcM2C_LeaveTeamM2CNtf() : CProtocol(PTCM2C_LEAVETEAMM2CNTF_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcM2C_LeaveTeamM2CNtf()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::ErrorInfo m_Data;
};

#endif