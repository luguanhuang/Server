#ifndef __PTCG2C_BMROLESCENESYNCNTF_H__
#define __PTCG2C_BMROLESCENESYNCNTF_H__

// generate by ProtoGen at date: 2017/6/22 8:18:47

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCG2C_BMROLESCENESYNCNTF_TYPE 40091

class PtcG2C_BMRoleSceneSyncNtf : public CProtocol
{
public:
    explicit PtcG2C_BMRoleSceneSyncNtf() : CProtocol(PTCG2C_BMROLESCENESYNCNTF_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcG2C_BMRoleSceneSyncNtf()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::BMRoleSceneSync m_Data;
};

#endif