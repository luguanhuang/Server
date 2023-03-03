#ifndef __PTCM2C_TEAMFULLDATAM2CNTF_H__
#define __PTCM2C_TEAMFULLDATAM2CNTF_H__

// generate by ProtoGen at date: 2016/8/16 17:07:14

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCM2C_TEAMFULLDATAM2CNTF_TYPE 39119

class PtcM2C_TeamFullDataM2CNtf : public CProtocol
{
public:
    explicit PtcM2C_TeamFullDataM2CNtf() : CProtocol(PTCM2C_TEAMFULLDATAM2CNTF_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcM2C_TeamFullDataM2CNtf()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::TeamFullDataNtf m_Data;
};

#endif