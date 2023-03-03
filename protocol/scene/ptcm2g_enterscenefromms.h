#ifndef __PTCM2G_ENTERSCENEFROMMS_H__
#define __PTCM2G_ENTERSCENEFROMMS_H__

// generate by ProtoGen at date: 2017/4/16 16:59:59

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCM2G_ENTERSCENEFROMMS_TYPE 30827

class PtcM2G_EnterSceneFromMs : public CProtocol
{
public:
    explicit PtcM2G_EnterSceneFromMs() : CProtocol(PTCM2G_ENTERSCENEFROMMS_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcM2G_EnterSceneFromMs()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::EnterSceneFromMsData m_Data;
};

#endif