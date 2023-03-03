#ifndef __PTCM2D_MSUPDATEGLOBALCONFIG_H__
#define __PTCM2D_MSUPDATEGLOBALCONFIG_H__

// generate by ProtoGen at date: 2016/7/29 18:35:07

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCM2D_MSUPDATEGLOBALCONFIG_TYPE 54919

class PtcM2D_MsUpdateGlobalConfig : public CProtocol
{
public:
    explicit PtcM2D_MsUpdateGlobalConfig() : CProtocol(PTCM2D_MSUPDATEGLOBALCONFIG_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcM2D_MsUpdateGlobalConfig()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::UpdateGlobalConfig m_Data;
};

#endif