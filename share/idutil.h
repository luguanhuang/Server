#ifndef _ID_UTIL_H_
#define _ID_UTIL_H_

inline UINT32 GetServerIDByRoleID(UINT64 roleID)
{
	return (UINT32)((roleID >> 32) & 0xFFFF);
}

inline UINT32 GetServerIDByGuildID(UINT64 guildID)
{
	return (UINT32)((guildID >> 48) & 0xFFFF);
}

#endif