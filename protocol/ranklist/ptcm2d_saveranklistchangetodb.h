#ifndef __PTCM2D_SAVERANKLISTCHANGETODB_H__
#define __PTCM2D_SAVERANKLISTCHANGETODB_H__

// generate by ProtoGen at date: 2016/8/22 14:16:30

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCM2D_SAVERANKLISTCHANGETODB_TYPE 48865

class PtcM2D_SaveRankListChangeToDB : public CProtocol
{
public:
    explicit PtcM2D_SaveRankListChangeToDB() : CProtocol(PTCM2D_SAVERANKLISTCHANGETODB_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcM2D_SaveRankListChangeToDB()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::SaveRankListChange m_Data;
};

#endif