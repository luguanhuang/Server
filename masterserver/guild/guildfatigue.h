#ifndef __GUILDFATIGUE_H__
#define __GUILDFATIGUE_H__

#include "guild/guilddef.h"

class CRole;
class Guild;

class GuildFatigue : public IGuildSerializerInterface
{
public:

	GuildFatigue(Guild *pGuild);
	~GuildFatigue();

	static const UINT32 GuildFatigueId = 813;

	virtual bool IsChanged();
	virtual bool Load(const KKSG::GuildAllDBData & guilddata);
	virtual void Save(KKSG::GuildAllDBData & guilddata);
	virtual const std::string ToBinary();

	int  SendFatigue(CRole *pRole, UINT64 ToRole, int &totalSend);
	int  RecvFatigue(CRole *pRole, UINT64 FromRole, int &totalRecv, UINT32 recvNum);
	bool CanSendFatigueTo(UINT64 qwRole, UINT64 ToRole);
	bool CanRecvFatigueFrom(UINT64 qwRole, UINT64 FromRole, bool &hasRecved);

	void Hint(CRole *pRole);

	void Clear();

	struct SendFatigueInfo
	{
		UINT64 qwSender;
		bool   IsRecved;
	};

private:
	
	void SendAll(CRole *pRole, int &totalSend);
	void RecvAll(CRole *pRole, int &totalRecv, UINT32 recvNum);

	int  SendFatigueTo(UINT64 qwRole, UINT64 toRole, int &totalSend);

	Guild *m_pGuild;
	std::unordered_map<UINT64, std::list<SendFatigueInfo>> m_SendFatigueInfo;

};

#endif // __GUILDFATIGUE_H__