#ifndef _DRAGON_GUILD_BIND_INFO_H_
#define _DRAGON_GUILD_BIND_INFO_H_

#include "dragonguildbase.h"

class DragonGuildBindInfo: public DragonGuildBase
{
public:
	DragonGuildBindInfo(DragonGuild* dragonguild);

	virtual bool LoadFromDB(const KKSG::DragonGuildSaveData& data);
	virtual void SaveToDB(KKSG::DragonGuildSaveData& data);
	virtual void OnDayPass() {}

	bool IsBindGroup();

	const std::string& GetQQGroupCode() { return m_qqGroupCode; }
	const std::string& GetQQGroupName() { return m_qqGroupName; }
	UINT32 GetLastUpdateTime() { return m_lastUpdateBindTime; }
	void SetQQGroupCode(const std::string& gc);
	void SetQQGroupName(const std::string& name);
	void SetQQGroupOpenId(const std::string& openId);
	void SetLastUpdateTime(UINT32 t);

	void DoUnlinkWhenLeaderChange(UINT64 oldLeaderId, UINT64 newLeaderId);

private:

	std::string m_qqGroupCode;		// QQȺ����
	std::string m_qqGroupOpenId;	// QQȺopenId
	std::string m_qqGroupName;		// QQȺ����
	UINT32 m_lastUpdateBindTime;	// �ϴθ��°���Ϣʱ���
};

#endif