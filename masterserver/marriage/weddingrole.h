#ifndef _WEDDING_ROLE_H_
#define _WEDDING_ROLE_H_

class Wedding;
class WeddingRole
{
public:
	WeddingRole(UINT64 roleId);

	inline void SetSelfWeddingId(UINT64 weddingId) { m_selfWeddingId = weddingId; }
	inline UINT64 GetSelfWeddingId() { return m_selfWeddingId; }
	Wedding* GetSelfWedding();

	UINT32 GetInviteNum() { return m_inviteList.size(); }
	void AddToInviteList(UINT64 roleId);
	const std::vector<UINT64>& GetInviteList() { return m_inviteList; }

	void OnSelfWeddingEnd();

	bool CanDestory();
private:
	UINT64 m_roleId;
	UINT64 m_selfWeddingId;
	std::vector<UINT64> m_inviteList;			// ÑûÇëÍæ¼ÒÁÐ±í
};

#endif