#ifndef __PTCG2C_PETINVITENTF_H__
#define __PTCG2C_PETINVITENTF_H__

// generate by ProtoGen at date: 2017/6/16 14:24:53

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCG2C_PETINVITENTF_TYPE 19818

class PtcG2C_PetInviteNtf : public CProtocol
{
public:
    explicit PtcG2C_PetInviteNtf() : CProtocol(PTCG2C_PETINVITENTF_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcG2C_PetInviteNtf()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::PetInviteNtf m_Data;
};

#endif