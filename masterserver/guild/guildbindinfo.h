#ifndef _GUILD_BIND_INFO_H_
#define _GUILD_BIND_INFO_H_

struct GuildBrief;

namespace KKSG
{
	class GuildBindGroupInfo;
}

class GuildBindInfo
{
public:
	GuildBindInfo(GuildBrief* brief);

	void LoadFromKKSG(const KKSG::GuildBindGroupInfo& info);
	void ToKKSG(KKSG::GuildBindGroupInfo& info);

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
	GuildBrief* m_owner;

	std::string m_qqGroupCode;		// QQȺ����
	std::string m_qqGroupOpenId;	// QQȺopenId
	std::string m_qqGroupName;		// QQȺ����
	UINT32 m_lastUpdateBindTime;	// �ϴθ��°���Ϣʱ���
};

#endif