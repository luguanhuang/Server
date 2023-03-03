#ifndef __PTCM2G_SYNFATIGUERESULT_H__
#define __PTCM2G_SYNFATIGUERESULT_H__

// generate by ProtoGen at date: 2016/9/19 10:58:23

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCM2G_SYNFATIGUERESULT_TYPE 55471

class PtcM2G_SynFatigueResult : public CProtocol
{
public:
    explicit PtcM2G_SynFatigueResult() : CProtocol(PTCM2G_SYNFATIGUERESULT_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcM2G_SynFatigueResult()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::SynFatigueResult m_Data;
};

#endif