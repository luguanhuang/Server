#ifndef __PTCG2C_TEAMFULLDATANTF_H__
#define __PTCG2C_TEAMFULLDATANTF_H__

// generate by ProtoGen at date: 2016/3/30 15:13:26

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCG2C_TEAMFULLDATANTF_TYPE 48618

class PtcG2C_TeamFullDataNtf : public CProtocol
{
public:
    explicit PtcG2C_TeamFullDataNtf() : CProtocol(PTCG2C_TEAMFULLDATANTF_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcG2C_TeamFullDataNtf()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::TeamFullDataNtf m_Data;
};

#endif