#ifndef __PTCM2C_WEDDINGINVITENTF_H__
#define __PTCM2C_WEDDINGINVITENTF_H__

// generate by ProtoGen at date: 2017/7/25 9:58:12

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCM2C_WEDDINGINVITENTF_TYPE 35104

class PtcM2C_WeddingInviteNtf : public CProtocol
{
public:
    explicit PtcM2C_WeddingInviteNtf() : CProtocol(PTCM2C_WEDDINGINVITENTF_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcM2C_WeddingInviteNtf()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::WeddingInviteNtf m_Data;
};

#endif