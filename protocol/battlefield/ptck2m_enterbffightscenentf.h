#ifndef __PTCK2M_ENTERBFFIGHTSCENENTF_H__
#define __PTCK2M_ENTERBFFIGHTSCENENTF_H__

// generate by ProtoGen at date: 2017/9/1 11:54:43

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCK2M_ENTERBFFIGHTSCENENTF_TYPE 33660

class PtcK2M_EnterBFFightSceneNtf : public CProtocol
{
public:
    explicit PtcK2M_EnterBFFightSceneNtf() : CProtocol(PTCK2M_ENTERBFFIGHTSCENENTF_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcK2M_EnterBFFightSceneNtf()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::EnterBFFightBatchScene m_Data;
};

#endif