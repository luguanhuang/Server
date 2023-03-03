#ifndef __PTCM2C_RESWARENEMYTIMENTF_H__
#define __PTCM2C_RESWARENEMYTIMENTF_H__

// generate by ProtoGen at date: 2016/12/26 17:41:19

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCM2C_RESWARENEMYTIMENTF_TYPE 48125

class PtcM2C_ResWarEnemyTimeNtf : public CProtocol
{
public:
    explicit PtcM2C_ResWarEnemyTimeNtf() : CProtocol(PTCM2C_RESWARENEMYTIMENTF_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcM2C_ResWarEnemyTimeNtf()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::ResWarEnemyTime m_Data;
};

#endif