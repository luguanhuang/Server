#ifndef __PTCG2C_GMFKICKNTY_H__
#define __PTCG2C_GMFKICKNTY_H__

// generate by ProtoGen at date: 2016/10/2 16:33:43

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCG2C_GMFKICKNTY_TYPE 21295

class PtcG2C_GmfKickNty : public CProtocol
{
public:
    explicit PtcG2C_GmfKickNty() : CProtocol(PTCG2C_GMFKICKNTY_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcG2C_GmfKickNty()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::GmfKickRes m_Data;
};

#endif