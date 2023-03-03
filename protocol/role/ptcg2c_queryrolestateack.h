#ifndef __PTCG2C_QUERYROLESTATEACK_H__
#define __PTCG2C_QUERYROLESTATEACK_H__

// generate by ProtoGen at date: 2016/12/28 11:10:45

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCG2C_QUERYROLESTATEACK_TYPE 53402

class PtcG2C_QueryRoleStateAck : public CProtocol
{
public:
    explicit PtcG2C_QueryRoleStateAck() : CProtocol(PTCG2C_QUERYROLESTATEACK_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcG2C_QueryRoleStateAck()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::QueryRoleStateAck m_Data;
};

#endif