#ifndef __PTCG2C_UPDATEVOIPROOMMEMBERNTF_H__
#define __PTCG2C_UPDATEVOIPROOMMEMBERNTF_H__

// generate by ProtoGen at date: 2016/8/9 22:46:47

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCG2C_UPDATEVOIPROOMMEMBERNTF_TYPE 25546

class PtcG2C_UpdateVoipRoomMemberNtf : public CProtocol
{
public:
    explicit PtcG2C_UpdateVoipRoomMemberNtf() : CProtocol(PTCG2C_UPDATEVOIPROOMMEMBERNTF_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcG2C_UpdateVoipRoomMemberNtf()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::UpdateVoipRoomMemberNtf m_Data;
};

#endif