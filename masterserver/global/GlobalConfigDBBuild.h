#ifndef __GLOBALCONFIG_DBBUILD_H_
#define __GLOBALCONFIG_DBBUILD_H_

#include "table/GlobalToDbTable.h"
#include "define/globalconfigtodbdef.h"

#define INVALID_CONFIGVALUE ""


class CGlobalConfigDBBuild
{
	CGlobalConfigDBBuild();
	~CGlobalConfigDBBuild();
	DECLARE_SINGLETON(CGlobalConfigDBBuild);
public:
	bool Init();
	void Uninit();

	bool ReadDataFromDB();
	bool UpdateToDb(EGlobalConfigToDb eConfig);

	bool SaveData(EGlobalConfigToDb eConfig,const std::string &refData);
	std::string GetConfigValue(UINT32 dwId);
	void SetConfigValue(UINT32 dwId, const std::string& strData);

private:
	GlobalToDbTable m_oTable;
	std::map<UINT32,std::string> m_mapGlobalConfig;
};

#endif