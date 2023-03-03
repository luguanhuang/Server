#ifndef __PTCG2M_LEVELCHANGENTFMS_H__
#define __PTCG2M_LEVELCHANGENTFMS_H__

// generate by ProtoGen at date: 2017/1/18 20:23:51

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCG2M_LEVELCHANGENTFMS_TYPE 38683

class PtcG2M_LevelChangeNtfMs : public CProtocol
{
public:
    explicit PtcG2M_LevelChangeNtfMs() : CProtocol(PTCG2M_LEVELCHANGENTFMS_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcG2M_LevelChangeNtfMs()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::LevelChangeNtfMs m_Data;
};

#endif