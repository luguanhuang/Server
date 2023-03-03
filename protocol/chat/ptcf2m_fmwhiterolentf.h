#ifndef __PTCF2M_FMWHITEROLENTF_H__
#define __PTCF2M_FMWHITEROLENTF_H__

// generate by ProtoGen at date: 2016/11/24 23:18:57

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCF2M_FMWHITEROLENTF_TYPE 27611

class PtcF2M_FMWhiteRoleNtf : public CProtocol
{
public:
    explicit PtcF2M_FMWhiteRoleNtf() : CProtocol(PTCF2M_FMWHITEROLENTF_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcF2M_FMWhiteRoleNtf()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::FMWhiteRoleList m_Data;
};

#endif