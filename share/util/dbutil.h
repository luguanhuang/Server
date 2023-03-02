#ifndef _Db_Utility_H__
#define _Db_Utility_H__

class IMysqlConn;


bool RunSqlScript(const char* pszFilePath, IMysqlConn* poMysqlConn);


inline INT32 MapProfessionToSlot(INT32 nProf)
{
	return nProf%10 - 1;
}

inline INT32 MapSlotToProfession(INT32 nSlot)
{
	return nSlot + 1;
}

#endif