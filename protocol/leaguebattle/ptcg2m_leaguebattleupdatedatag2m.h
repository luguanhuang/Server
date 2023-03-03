#ifndef __PTCG2M_LEAGUEBATTLEUPDATEDATAG2M_H__
#define __PTCG2M_LEAGUEBATTLEUPDATEDATAG2M_H__

// generate by ProtoGen at date: 2017/1/14 15:54:51

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCG2M_LEAGUEBATTLEUPDATEDATAG2M_TYPE 26510

class PtcG2M_LeagueBattleUpdateDataG2M : public CProtocol
{
public:
    explicit PtcG2M_LeagueBattleUpdateDataG2M() : CProtocol(PTCG2M_LEAGUEBATTLEUPDATEDATAG2M_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcG2M_LeagueBattleUpdateDataG2M()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::LeagueBattleUpdateDataG2M m_Data;
};

#endif