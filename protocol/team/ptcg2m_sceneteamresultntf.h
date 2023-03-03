#ifndef __PTCG2M_SCENETEAMRESULTNTF_H__
#define __PTCG2M_SCENETEAMRESULTNTF_H__

// generate by ProtoGen at date: 2017/5/17 21:01:00

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCG2M_SCENETEAMRESULTNTF_TYPE 14783

class PtcG2M_SceneTeamResultNtf : public CProtocol
{
public:
    explicit PtcG2M_SceneTeamResultNtf() : CProtocol(PTCG2M_SCENETEAMRESULTNTF_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcG2M_SceneTeamResultNtf()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::SceneTeamResultPara m_Data;
};

#endif