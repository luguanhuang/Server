#ifndef __PTCM2D_UPDATEDRAGONGUILDDATA_H__
#define __PTCM2D_UPDATEDRAGONGUILDDATA_H__

// generate by ProtoGen at date: 2017/9/7 19:28:10

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCM2D_UPDATEDRAGONGUILDDATA_TYPE 13352

class PtcM2D_UpdateDragonGuildData : public CProtocol
{
public:
    explicit PtcM2D_UpdateDragonGuildData() : CProtocol(PTCM2D_UPDATEDRAGONGUILDDATA_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcM2D_UpdateDragonGuildData()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::UpdateDragonGuildData m_Data;
};

#endif