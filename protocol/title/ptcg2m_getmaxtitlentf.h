#ifndef __PTCG2M_GETMAXTITLENTF_H__
#define __PTCG2M_GETMAXTITLENTF_H__

// generate by ProtoGen at date: 2016/9/11 18:21:17

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCG2M_GETMAXTITLENTF_TYPE 37218

class PtcG2M_GetMaxTitleNtf : public CProtocol
{
public:
    explicit PtcG2M_GetMaxTitleNtf() : CProtocol(PTCG2M_GETMAXTITLENTF_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcG2M_GetMaxTitleNtf()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::GetMaxTitle m_Data;
};

#endif