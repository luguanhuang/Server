#ifndef __PTCG2M_REQDRAGONGUILDSIMPLEDATA_H__
#define __PTCG2M_REQDRAGONGUILDSIMPLEDATA_H__

// generate by ProtoGen at date: 2017/9/6 15:37:41

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCG2M_REQDRAGONGUILDSIMPLEDATA_TYPE 54058

class PtcG2M_reqdragonguildsimpledata : public CProtocol
{
public:
    explicit PtcG2M_reqdragonguildsimpledata() : CProtocol(PTCG2M_REQDRAGONGUILDSIMPLEDATA_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcG2M_reqdragonguildsimpledata()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::DragonGuildDataReq m_Data;
};

#endif