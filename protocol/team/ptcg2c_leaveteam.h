#ifndef __PTCG2C_LEAVETEAM_H__
#define __PTCG2C_LEAVETEAM_H__

// generate by ProtoGen at date: 2015/10/16 16:27:33

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCG2C_LEAVETEAM_TYPE 47730

class PtcG2C_LeaveTeam : public CProtocol
{
public:
    explicit PtcG2C_LeaveTeam() : CProtocol(PTCG2C_LEAVETEAM_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcG2C_LeaveTeam()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::ErrorInfo m_Data;
};

#endif