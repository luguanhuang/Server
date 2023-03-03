#ifndef __PTCG2K_BMSCENEFIGHTINFONTF_H__
#define __PTCG2K_BMSCENEFIGHTINFONTF_H__

// generate by ProtoGen at date: 2017/6/22 8:18:52

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCG2K_BMSCENEFIGHTINFONTF_TYPE 20128

class PtcG2K_BMSceneFightInfoNtf : public CProtocol
{
public:
    explicit PtcG2K_BMSceneFightInfoNtf() : CProtocol(PTCG2K_BMSCENEFIGHTINFONTF_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcG2K_BMSceneFightInfoNtf()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::BMSceneFightInfo m_Data;
};

#endif