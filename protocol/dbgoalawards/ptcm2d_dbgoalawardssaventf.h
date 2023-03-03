#ifndef __PTCM2D_DBGOALAWARDSSAVENTF_H__
#define __PTCM2D_DBGOALAWARDSSAVENTF_H__

// generate by ProtoGen at date: 2017/9/6 20:17:26

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCM2D_DBGOALAWARDSSAVENTF_TYPE 10887

class PtcM2D_DBGoalAwardsSaveNtf : public CProtocol
{
public:
    explicit PtcM2D_DBGoalAwardsSaveNtf() : CProtocol(PTCM2D_DBGOALAWARDSSAVENTF_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcM2D_DBGoalAwardsSaveNtf()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::DBGoalAwardsSave m_Data;
};

#endif