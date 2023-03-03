#ifndef __PTCM2G_MSDESTORYSCENE_H__
#define __PTCM2G_MSDESTORYSCENE_H__

// generate by ProtoGen at date: 2016/11/30 23:36:58

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCM2G_MSDESTORYSCENE_TYPE 19289

class PtcM2G_MSDestoryScene : public CProtocol
{
public:
    explicit PtcM2G_MSDestoryScene() : CProtocol(PTCM2G_MSDESTORYSCENE_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcM2G_MSDestoryScene()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::MSDestoryScene m_Data;
};

#endif