#ifndef __PTCG2M_UPDATEGLOBALVALUE_H__
#define __PTCG2M_UPDATEGLOBALVALUE_H__

// generate by ProtoGen at date: 2017/4/23 22:29:10

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCG2M_UPDATEGLOBALVALUE_TYPE 55899

class PtcG2M_UpdateGlobalValue : public CProtocol
{
public:
    explicit PtcG2M_UpdateGlobalValue() : CProtocol(PTCG2M_UPDATEGLOBALVALUE_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcG2M_UpdateGlobalValue()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::UpdateGlobalValue m_Data;
};

#endif