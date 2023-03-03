#ifndef __PTCG2M_INVFIGHTAGAINREQG2M_H__
#define __PTCG2M_INVFIGHTAGAINREQG2M_H__

// generate by ProtoGen at date: 2017/8/30 15:40:13

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCG2M_INVFIGHTAGAINREQG2M_TYPE 25754

class PtcG2M_InvfightAgainReqG2M : public CProtocol
{
public:
    explicit PtcG2M_InvfightAgainReqG2M() : CProtocol(PTCG2M_INVFIGHTAGAINREQG2M_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcG2M_InvfightAgainReqG2M()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::InvFightAgainPara m_Data;
};

#endif