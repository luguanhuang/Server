#ifndef __PTCK2M_ENTERBMFIGHTSCENENTF_H__
#define __PTCK2M_ENTERBMFIGHTSCENENTF_H__

// generate by ProtoGen at date: 2017/6/22 8:18:55

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCK2M_ENTERBMFIGHTSCENENTF_TYPE 17198

class PtcK2M_EnterBMFightSceneNtf : public CProtocol
{
public:
    explicit PtcK2M_EnterBMFightSceneNtf() : CProtocol(PTCK2M_ENTERBMFIGHTSCENENTF_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcK2M_EnterBMFightSceneNtf()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::EnterBMFightScene m_Data;
};

#endif