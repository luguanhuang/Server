#ifndef __PTCM2C_TEAMCHANGEM2CNTF_H__
#define __PTCM2C_TEAMCHANGEM2CNTF_H__

// generate by ProtoGen at date: 2016/8/16 10:10:47

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCM2C_TEAMCHANGEM2CNTF_TYPE 53586

class PtcM2C_TeamChangeM2CNtf : public CProtocol
{
public:
    explicit PtcM2C_TeamChangeM2CNtf() : CProtocol(PTCM2C_TEAMCHANGEM2CNTF_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcM2C_TeamChangeM2CNtf()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::TeamChanged m_Data;
};

#endif