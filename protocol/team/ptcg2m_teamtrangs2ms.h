#ifndef __PTCG2M_TEAMTRANGS2MS_H__
#define __PTCG2M_TEAMTRANGS2MS_H__

// generate by ProtoGen at date: 2017/7/4 11:07:54

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCG2M_TEAMTRANGS2MS_TYPE 21157

class PtcG2M_TeamTranGs2Ms : public CProtocol
{
public:
    explicit PtcG2M_TeamTranGs2Ms() : CProtocol(PTCG2M_TEAMTRANGS2MS_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcG2M_TeamTranGs2Ms()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::TeamTransData m_Data;
};

#endif