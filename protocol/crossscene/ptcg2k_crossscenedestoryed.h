#ifndef __PTCG2K_CROSSSCENEDESTORYED_H__
#define __PTCG2K_CROSSSCENEDESTORYED_H__

// generate by ProtoGen at date: 2016/11/1 15:44:59

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCG2K_CROSSSCENEDESTORYED_TYPE 47334

class PtcG2K_CrossSceneDestoryed : public CProtocol
{
public:
    explicit PtcG2K_CrossSceneDestoryed() : CProtocol(PTCG2K_CROSSSCENEDESTORYED_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcG2K_CrossSceneDestoryed()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::CrossSceneDestoryed m_Data;
};

#endif