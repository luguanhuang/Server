#ifndef __PTCG2M_REQGUILDSIMPLEDATA_H__
#define __PTCG2M_REQGUILDSIMPLEDATA_H__

// generate by ProtoGen at date: 2016/9/12 20:34:28

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCG2M_REQGUILDSIMPLEDATA_TYPE 21016

class PtcG2M_ReqGuildSimpleData : public CProtocol
{
public:
    explicit PtcG2M_ReqGuildSimpleData() : CProtocol(PTCG2M_REQGUILDSIMPLEDATA_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcG2M_ReqGuildSimpleData()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::ReqGuildSimpleData m_Data;
};

#endif