#ifndef __PTCM2C_INVUNFSTATEM2CNTF_H__
#define __PTCM2C_INVUNFSTATEM2CNTF_H__

// generate by ProtoGen at date: 2016/10/13 22:08:33

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCM2C_INVUNFSTATEM2CNTF_TYPE 2693

class PtcM2C_InvUnfStateM2CNtf : public CProtocol
{
public:
    explicit PtcM2C_InvUnfStateM2CNtf() : CProtocol(PTCM2C_INVUNFSTATEM2CNTF_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcM2C_InvUnfStateM2CNtf()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::InvUnfState m_Data;
};

#endif