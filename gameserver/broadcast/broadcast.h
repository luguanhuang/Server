#ifndef __H_BROADCAST_H__
#define __H_BROADCAST_H__


class ITimer;
class Scene;
class Role;
class Unit;
class CProtocol;
struct RoleViewSet;


#pragma pack (4)
struct ClientProtocolHead
{
	UINT32 dwLen;
	UINT32 dwType;
	UINT32 dwFlag;
};
#pragma pack ()


class BroadCastMgr
{
	BroadCastMgr();
	~BroadCastMgr();

public:
	bool Init();
	void Uninit();

	static void BroadCast(const CProtocol& protocol, KKSG::BroadCastType type = KKSG::BroadCastToAll);

private:

};


//broad protocol to session list
class CBroadCastSender
{
public:
	static void Broadcast(const std::vector<Role*>& roles, const CProtocol& roPtc);
	static void Broadcast(const std::unordered_map<UINT64, Role*>& roles, const CProtocol& roPtc);
	static void Broadcast(const std::unordered_map<UINT64, RoleViewSet*>& roles, const CProtocol& roPtc, Unit* poExlude = NULL);

private:
	static bool SerializePtc(const CProtocol& roPtc, std::string& strData);
};

#endif // 
