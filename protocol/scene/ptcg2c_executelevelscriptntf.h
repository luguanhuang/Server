#ifndef __PTCG2C_EXECUTELEVELSCRIPTNTF_H__
#define __PTCG2C_EXECUTELEVELSCRIPTNTF_H__

// generate by ProtoGen at date: 2015/10/13 22:26:39

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCG2C_EXECUTELEVELSCRIPTNTF_TYPE 47978

class PtcG2C_ExecuteLevelScriptNtf : public CProtocol
{
public:
    explicit PtcG2C_ExecuteLevelScriptNtf() : CProtocol(PTCG2C_EXECUTELEVELSCRIPTNTF_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcG2C_ExecuteLevelScriptNtf()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::ExecuteLevelScriptNtf m_Data;
};

#endif