#ifndef __PTCM2D_SAVEPLATNOTICEPTF_H__
#define __PTCM2D_SAVEPLATNOTICEPTF_H__

// generate by ProtoGen at date: 2016/11/19 22:28:12

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCM2D_SAVEPLATNOTICEPTF_TYPE 20396

class PtcM2D_SavePlatNoticePtf : public CProtocol
{
public:
    explicit PtcM2D_SavePlatNoticePtf() : CProtocol(PTCM2D_SAVEPLATNOTICEPTF_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcM2D_SavePlatNoticePtf()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::PlatNotice m_Data;
};

#endif