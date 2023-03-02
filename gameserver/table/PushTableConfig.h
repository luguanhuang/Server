#ifndef _PUSH_CONFIG_H_
#define _PUSH_CONFIG_H_

#include "PushSetting.h"

class PushTableConfig
{
	PushTableConfig();
	~PushTableConfig();
	DECLARE_SINGLETON(PushTableConfig)

public:
	bool Init();
	void Uninit();

	bool CheckFile();
	bool LoadFile();
	void ClearFile();

	bool IsConfigKey(const std::string& key);
	UINT32 GetType(const std::string& key);
	const std::map<UINT32, std::string>& GetTypeKey();

private:
	PushSetting m_table;
	std::map<UINT32, std::string> m_type2Key;	// 类型对应配置key
	std::map<std::string, UINT32> m_key2Type;	// key对应类型
};

#endif