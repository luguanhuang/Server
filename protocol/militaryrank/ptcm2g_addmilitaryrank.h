#ifndef __PTCM2G_ADDMILITARYRANK_H__
#define __PTCM2G_ADDMILITARYRANK_H__

// generate by ProtoGen at date: 2017/6/24 19:08:26

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCM2G_ADDMILITARYRANK_TYPE 61218

class PtcM2G_AddMilitaryRank : public CProtocol
{
public:
    explicit PtcM2G_AddMilitaryRank() : CProtocol(PTCM2G_ADDMILITARYRANK_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcM2G_AddMilitaryRank()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::AddMilitaryRankData m_Data;
};

#endif