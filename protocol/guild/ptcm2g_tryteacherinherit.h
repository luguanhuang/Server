#ifndef __PTCM2G_TRYTEACHERINHERIT_H__
#define __PTCM2G_TRYTEACHERINHERIT_H__

// generate by ProtoGen at date: 2016/12/22 11:32:57

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCM2G_TRYTEACHERINHERIT_TYPE 23393

class PtcM2G_TryTeacherInherit : public CProtocol
{
public:
    explicit PtcM2G_TryTeacherInherit() : CProtocol(PTCM2G_TRYTEACHERINHERIT_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcM2G_TryTeacherInherit()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::TryTeacherInherit m_Data;
};

#endif