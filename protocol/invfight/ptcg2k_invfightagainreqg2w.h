#ifndef __PTCG2K_INVFIGHTAGAINREQG2W_H__
#define __PTCG2K_INVFIGHTAGAINREQG2W_H__

// generate by ProtoGen at date: 2017/8/30 15:40:38

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCG2K_INVFIGHTAGAINREQG2W_TYPE 23179

class PtcG2K_InvfightAgainReqG2W : public CProtocol
{
public:
    explicit PtcG2K_InvfightAgainReqG2W() : CProtocol(PTCG2K_INVFIGHTAGAINREQG2W_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcG2K_InvfightAgainReqG2W()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::InvFightAgainPara m_Data;
};

#endif