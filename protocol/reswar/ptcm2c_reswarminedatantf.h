#ifndef __PTCM2C_RESWARMINEDATANTF_H__
#define __PTCM2C_RESWARMINEDATANTF_H__

// generate by ProtoGen at date: 2016/11/19 16:34:35

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCM2C_RESWARMINEDATANTF_TYPE 57215

class PtcM2C_ResWarMineDataNtf : public CProtocol
{
public:
    explicit PtcM2C_ResWarMineDataNtf() : CProtocol(PTCM2C_RESWARMINEDATANTF_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcM2C_ResWarMineDataNtf()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::ResWarMineData m_Data;
};

#endif