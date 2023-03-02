#include "pch.h"
#include "teamcommon.h"
#include "pb/enum.pb.h"
#include "define/teamdef.h"

namespace HTeam
{
	bool IsHelper(int type)
	{
		return (KKSG::TMT_HELPER == type);
	}

	bool IsUseTicket(int type)
	{
		return (KKSG::TMT_USETICKET == type);
	}

	bool IsTsTeam(int teamID)
	{
		return (teamID < 0);
	}

	bool IsMsTeam(int teamID)
	{
		return (teamID > 0);
	}

	bool IsTsFastMatch(int teamType)
	{
		if(DRAGON_TEAM_SCENE == teamType)
		{
			return false;
		}
		return true;
	}

}