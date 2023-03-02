#ifndef __ROLESERILIZEINTERFACE_H__
#define __ROLESERILIZEINTERFACE_H__

#include <typeinfo>

using std::type_info;

namespace KKSG 
{
	class RoleAllInfo;
}

typedef std::set<google::protobuf::MessageLite*>	CDataFieldSet;


class RoleSerializeInterface
{
public:
	virtual ~RoleSerializeInterface(){}

	virtual bool AutoDestory() { return true; }
	virtual bool Load(const KKSG::RoleAllInfo *poRoleAllInfo) = 0;
	virtual void Save(KKSG::RoleAllInfo *poRoleAllInfo, CDataFieldSet& roChanged) = 0;
	virtual void FirstInit(UINT32 roleNum) {}
};

class Switch
{
public:
	Switch()
	{
		m_flag = false;
	}

	~Switch()
	{

	}

	void Set()
	{
		m_flag = true;
	}

	bool TestAndReset()
	{
		bool ret = m_flag;
		m_flag = false;
		return ret;
	}

private:
	bool m_flag;
};

#endif // __ROLESERILIZEINTERFACE_H__
