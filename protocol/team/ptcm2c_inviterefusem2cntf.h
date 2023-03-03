#ifndef __PTCM2C_INVITEREFUSEM2CNTF_H__
#define __PTCM2C_INVITEREFUSEM2CNTF_H__

// generate by ProtoGen at date: 2016/8/18 14:19:18

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCM2C_INVITEREFUSEM2CNTF_TYPE 33486

class PtcM2C_InviteRefuseM2CNtf : public CProtocol
{
public:
    explicit PtcM2C_InviteRefuseM2CNtf() : CProtocol(PTCM2C_INVITEREFUSEM2CNTF_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcM2C_InviteRefuseM2CNtf()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::InviteRufuse m_Data;
};

#endif