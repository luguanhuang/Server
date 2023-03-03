#ifndef __PTCH2M_TEAMTRANTS2MS_H__
#define __PTCH2M_TEAMTRANTS2MS_H__

// generate by ProtoGen at date: 2017/7/10 21:22:43

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCH2M_TEAMTRANTS2MS_TYPE 40019

class PtcH2M_TeamTranTs2Ms : public CProtocol
{
public:
    explicit PtcH2M_TeamTranTs2Ms() : CProtocol(PTCH2M_TEAMTRANTS2MS_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcH2M_TeamTranTs2Ms()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::TeamTransData m_Data;
};

#endif