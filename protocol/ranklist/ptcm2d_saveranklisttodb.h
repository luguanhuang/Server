#ifndef __PTCM2D_SAVERANKLISTTODB_H__
#define __PTCM2D_SAVERANKLISTTODB_H__

// generate by ProtoGen at date: 2016/8/22 14:17:38

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCM2D_SAVERANKLISTTODB_TYPE 38499

class PtcM2D_SaveRankListToDB : public CProtocol
{
public:
    explicit PtcM2D_SaveRankListToDB() : CProtocol(PTCM2D_SAVERANKLISTTODB_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcM2D_SaveRankListToDB()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::RankListRawData m_Data;
};

#endif