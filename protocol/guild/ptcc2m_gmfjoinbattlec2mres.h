#ifndef __PTCC2M_GMFJOINBATTLEC2MRES_H__
#define __PTCC2M_GMFJOINBATTLEC2MRES_H__

// generate by ProtoGen at date: 2016/9/20 17:04:52

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCC2M_GMFJOINBATTLEC2MRES_TYPE 25047

class PtcC2M_GmfJoinBattleC2MRes : public CProtocol
{
public:
    explicit PtcC2M_GmfJoinBattleC2MRes() : CProtocol(PTCC2M_GMFJOINBATTLEC2MRES_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcC2M_GmfJoinBattleC2MRes()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::GmfJoinBattleRes m_Data;
};

#endif