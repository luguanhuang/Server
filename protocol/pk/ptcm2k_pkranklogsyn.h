#ifndef __PTCM2K_PKRANKLOGSYN_H__
#define __PTCM2K_PKRANKLOGSYN_H__

// generate by ProtoGen at date: 2017/11/10 11:57:34

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCM2K_PKRANKLOGSYN_TYPE 64474

class PtcM2K_PkRankLogSyn : public CProtocol
{
public:
    explicit PtcM2K_PkRankLogSyn() : CProtocol(PTCM2K_PKRANKLOGSYN_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcM2K_PkRankLogSyn()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::PkRoleRankLogPara m_Data;
};

#endif