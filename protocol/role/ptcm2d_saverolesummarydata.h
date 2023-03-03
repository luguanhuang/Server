#ifndef __PTCM2D_SAVEROLESUMMARYDATA_H__
#define __PTCM2D_SAVEROLESUMMARYDATA_H__

// generate by ProtoGen at date: 2016/8/24 22:12:48

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCM2D_SAVEROLESUMMARYDATA_TYPE 22696

class PtcM2D_SaveRoleSummaryData : public CProtocol
{
public:
    explicit PtcM2D_SaveRoleSummaryData() : CProtocol(PTCM2D_SAVEROLESUMMARYDATA_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcM2D_SaveRoleSummaryData()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::SaveRoleSummaryData m_Data;
};

#endif