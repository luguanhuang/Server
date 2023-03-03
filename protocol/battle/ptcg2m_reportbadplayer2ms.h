#ifndef __PTCG2M_REPORTBADPLAYER2MS_H__
#define __PTCG2M_REPORTBADPLAYER2MS_H__

// generate by ProtoGen at date: 2017/5/11 2:11:35

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCG2M_REPORTBADPLAYER2MS_TYPE 55873

class PtcG2M_ReportBadPlayer2Ms : public CProtocol
{
public:
    explicit PtcG2M_ReportBadPlayer2Ms() : CProtocol(PTCG2M_REPORTBADPLAYER2MS_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcG2M_ReportBadPlayer2Ms()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::ReportBadPlayer2Ms m_Data;
};

#endif