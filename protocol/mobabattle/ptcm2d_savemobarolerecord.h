#ifndef __PTCM2D_SAVEMOBAROLERECORD_H__
#define __PTCM2D_SAVEMOBAROLERECORD_H__

// generate by ProtoGen at date: 2017/6/17 19:15:36

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCM2D_SAVEMOBAROLERECORD_TYPE 16269

class PtcM2D_SaveMobaRoleRecord : public CProtocol
{
public:
    explicit PtcM2D_SaveMobaRoleRecord() : CProtocol(PTCM2D_SAVEMOBAROLERECORD_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcM2D_SaveMobaRoleRecord()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::SaveMobaRoleRecordData m_Data;
};

#endif