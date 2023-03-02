#ifndef __TEAMCOMMON_H__
#define __TEAMCOMMON_H__

namespace HTeam
{
	bool IsHelper(int type);
	bool IsUseTicket(int type);

	bool IsTsTeam(int teamID);
	bool IsMsTeam(int teamID);

	bool IsTsFastMatch(int teamType);
}

#endif