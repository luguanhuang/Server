#ifndef __PTCC2G_INVFIGHTAGAINREQC2G_H__
#define __PTCC2G_INVFIGHTAGAINREQC2G_H__

// generate by ProtoGen at date: 2017/8/30 15:39:46

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCC2G_INVFIGHTAGAINREQC2G_TYPE 2055

class PtcC2G_InvfightAgainReqC2G : public CProtocol
{
public:
    explicit PtcC2G_InvfightAgainReqC2G() : CProtocol(PTCC2G_INVFIGHTAGAINREQC2G_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcC2G_InvfightAgainReqC2G()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::InvFightAgainPara m_Data;
};

#endif