#ifndef __SCENEINSPIRE_H__
#define __SCENEINSPIRE_H__

namespace KKSG
{
	class InspireArg;
	class InspireRes;
}

class Scene;
class Role;
class SceneInspire
{
public:
	SceneInspire(Scene* scene);
	~SceneInspire();

	//�ж��Ƿ���Ҫ���蹦��
	static bool NeedInspire(Scene* pScene);

	//�����������
	void HandlerReq(Role* pRole, const KKSG::InspireArg& oArg, KKSG::InspireRes& oRes);

	//������̨ս����
	void GwbInspire(Role* pRole, KKSG::InspireRes& oRes);
	void GwbOnFightBegin(UINT64 roleID, UINT64 guildID);//������󼴿ɻ���ۼƵĹ���Ч��

	INT32 GetInspireCount(UINT64 inspireID);

private:
	//roleid ��������� ������ȴʱ��
	bool InCoolDown(UINT64 roleid, UINT32 cooldown);
	UINT32 GetCoolDownTime(UINT64 roleid, UINT32 cooldown);

	//roleid ��������� inspireID��������(��һ�������)
	void AddOneInspire(UINT64 roleid, UINT64 inspireID);

	//������� addattrID����������Ե���ң� 
	void AddTempAttr(std::vector<UINT64>& addAttrIDs, UINT64 inspireID, bool isfirst);

	std::unordered_map<UINT64,time_t> m_inspireTime;//(���������)���һ�ι���ʱ��
	std::unordered_map<UINT64,INT32> m_inspireCount;//(�������ߣ���һ������ң�) ���������

	Scene* m_pScene;
};

#endif