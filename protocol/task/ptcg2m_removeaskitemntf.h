#ifndef __PTCG2M_REMOVEASKITEMNTF_H__
#define __PTCG2M_REMOVEASKITEMNTF_H__

// generate by ProtoGen at date: 2016/12/6 10:50:43

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCG2M_REMOVEASKITEMNTF_TYPE 28190

class PtcG2M_RemoveAskItemNtf : public CProtocol
{
public:
    explicit PtcG2M_RemoveAskItemNtf() : CProtocol(PTCG2M_REMOVEASKITEMNTF_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcG2M_RemoveAskItemNtf()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::RemoveAskItemNtf m_Data;
};

#endif