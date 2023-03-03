#ifndef __PTCG2C_LEVELSCRIPTSTATENTF_H__
#define __PTCG2C_LEVELSCRIPTSTATENTF_H__

// generate by ProtoGen at date: 2016/12/28 10:09:38

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCG2C_LEVELSCRIPTSTATENTF_TYPE 12789

class PtcG2C_LevelScriptStateNtf : public CProtocol
{
public:
    explicit PtcG2C_LevelScriptStateNtf() : CProtocol(PTCG2C_LEVELSCRIPTSTATENTF_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcG2C_LevelScriptStateNtf()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::LevelScriptStateData m_Data;
};

#endif