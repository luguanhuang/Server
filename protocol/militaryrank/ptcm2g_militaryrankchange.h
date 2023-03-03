#ifndef __PTCM2G_MILITARYRANKCHANGE_H__
#define __PTCM2G_MILITARYRANKCHANGE_H__

// generate by ProtoGen at date: 2017/3/10 17:32:25

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCM2G_MILITARYRANKCHANGE_TYPE 47816

class PtcM2G_MilitaryRankChange : public CProtocol
{
public:
    explicit PtcM2G_MilitaryRankChange() : CProtocol(PTCM2G_MILITARYRANKCHANGE_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcM2G_MilitaryRankChange()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::MilitaryRankChangeArg m_Data;
};

#endif