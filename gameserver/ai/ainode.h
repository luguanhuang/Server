#ifndef __AINODE_H__
#define __AINODE_H__

#include <list>
#include <tinyxml2.h>
#include "math/XVector3.h"

//获取变量名
#define VAR_NAME(m) (#m)

//#define  AIRETURN(name, m) {if (m) {LogDebug(name); LogDebug("true");}else{LogDebug(name); LogDebug("false");}  return m;}
//#define  AINODESTART(name) {LogDebug(name); LogDebug("start");}
//#define  AINODEEND(name) {LogDebug(name); LogDebug("end");}
//#define  AITREESTART(name) {LogDebug("BehaviorTree[%s] start +++++++++++++++++++++++++++++++", name); }
//#define  AITREEEND(name) {LogDebug("BehaviorTree[%s] end ---------------------------------", name);}

#define  AIRETURN(name, m) return (m);
#define  AINODESTART(name) {}
#define  AINODEEND(name) {}
#define  AITREESTART(name) {}
#define  AITREEEND(name) {}

#ifndef DNA_LIVE_ENV

#define AIDEBUG
#define DEFINE_AI_DEBUG_HELPER(pNode, pUnit) AIDebugHelper debugHelper(pNode, pUnit);
#define AI_DEBUG_STATEMENT(m) m;
#define  AI_DEBUG_RETURN(name, m) \
	{	\
		AIDebugHelper helper(this, unit);	\
		if (m)	\
		{	\
			m_DebugInfo << "Success";	\
			return true;	\
		}	\
		else	\
		{	\
			m_DebugInfo << "Faild";	\
			return false;	\
		}	\
	}
#define  AI_DEBUG_RETURN_WITHOUT_DEFINE(name, m) \
	{	\
		if (m)	\
		{	\
			m_DebugInfo << "Success";	\
			return true;	\
		}	\
		else	\
		{	\
			m_DebugInfo << "Faild";	\
			return false;	\
		}	\
	}

#else

#undef AIDEBUG
#define DEFINE_AI_DEBUG_HELPER(pNode, pUnit)
#define AI_DEBUG_STATEMENT(m)
#define  AI_DEBUG_RETURN(name, m) return (m);
#define  AI_DEBUG_RETURN_WITHOUT_DEFINE(name, m) return (m);

#endif



class Unit;

typedef Unit* Transform;
#define MAX_COUNT_NUM 80

enum NodeType //目前只有通用基类 类型
{
	NODE_TYPE_NONE = 0,
	NODE_TYPE_BASE = 1,
	NODE_TYPE_LOGIC = 2,
	NODE_TYPE_ROOT = 3,
	NODE_TYPE_ACTION = 4,
	NODE_TYPE_CONDITION = 5,
	NODE_TYPE_DECORATION = 6
};

class SharedData
{
public:
	SharedData();
	//bool HaveTransformByName(const std::string& name);
	//Transform GetTransformByName(const std::string& name);
	//void SetTransformByName(const std::string& name, Transform para);
	Transform GetTransformByIndex(UINT32 index);
	void SetTransformByIndex(UINT32 index, Transform para);

	//bool HaveFloatByName(const std::string& name);
	//float GetFloatByName(const std::string& name , float dvalue = 0.0);
	//void SetFloatByName(const std::string& name, const float& para);
	float GetFloatByIndex(UINT32 index, float dvalue = 0.0f);
	void SetFloatByIndex(UINT32 index, float dvalue = 0.0f);

	//bool HaveIntByName(const std::string& name);
	//int GetIntByName(const std::string& name , int dvalue = 0);
	//void SetIntByName(const std::string& name,const int& para);
	int GetIntByIndex(UINT32 index, int dvalue = 0);
	void SetIntByIndex(UINT32 index, int para);

	//bool HaveStringByName(const std::string& name);
	//std::string GetStringByName(const std::string& name , const std::string& dvalue="");
	//void SetStringByName(const std::string& name,const std::string& para);
	const std::string& GetStringByIndex(UINT32 index, const std::string& dvalue = _DefaultString);
	void SetStringByIndex(UINT32 index, const std::string& para);

	//bool HaveBoolByName(const std::string& name);
	//bool GetBoolByName(const std::string& name , bool dvalue = false);
	//void SetBoolByName(const std::string& name,const bool& para);
	bool GetBoolByIndex(UINT32 index, bool dvalue=false);
	void SetBoolByIndex(UINT32 index, bool para);

	//bool HaveVector3ByName(const std::string& name);
	//Vector3 GetVector3ByName(const std::string& name);
	//void SetVector3ByName(const std::string& name , const Vector3& para);
	Vector3 GetVector3ByIndex(UINT32 index, const Vector3& dvalue = Vector3::zero);
	void SetVector3ByIndex(UINT32 index, const Vector3& para);

	void Print();

	static void ParseVector3FromString(const std::string& strvalue, Vector3& inputVec);

private:

	//std::unordered_map<UINT32, Transform> m_transMap;
	//std::unordered_map<UINT32, float> m_floatsMap;
	//std::unordered_map<UINT32, int> m_intsMap;
	//std::unordered_map<UINT32, std::string> m_stringsMap;
	//std::unordered_map<UINT32, bool> m_boolsMap;
	//std::unordered_map<UINT32, Vector3> m_vector3sMap;

	Transform m_transMap[MAX_COUNT_NUM];
	float m_floatsMap[MAX_COUNT_NUM];
	int m_intsMap[MAX_COUNT_NUM];
	std::string m_stringsMap[MAX_COUNT_NUM];
	bool m_boolsMap[MAX_COUNT_NUM];
	Vector3 m_vector3sMap[MAX_COUNT_NUM];

	static const std::string _DefaultString;
};


class AINodeBase
{
public:
	AINodeBase()
	{
		m_type = NODE_TYPE_BASE;
#ifdef AIDEBUG
		m_bBreakPoint = false;
#endif
	}
	virtual ~AINodeBase(){}
	virtual bool Update(Unit* unit){return true;}	
	virtual void AddChild(AINodeBase* child){};

	NodeType GetType(){ return m_type; }

	virtual void Print();

	void PostCreate(const tinyxml2::XMLElement* elem);

#ifdef AIDEBUG
	inline bool IsBreakPoint() { return m_bBreakPoint; }
	inline const std::string& GetFriendlyName() { return m_strFriendlyName; }
	inline std::stringstream& GetDebugInfo() { return m_DebugInfo; }
#endif
protected:
	NodeType m_type;
#ifdef AIDEBUG
	bool m_bBreakPoint;
	std::string m_strFriendlyName;
	std::stringstream m_DebugInfo;
#endif
};

SharedData* GetSharedData(Unit* unit);


class RootNode:public AINodeBase 
{
public:
	RootNode():m_child(NULL),m_sharedData(NULL)
	{
		m_type = NODE_TYPE_ROOT;
		m_sharedData = new SharedData;
	}
	virtual ~RootNode();
	virtual void AddChild(AINodeBase* child);
	virtual bool Update(Unit* unit);

	virtual void Print();
	void PrintTree();

	SharedData* GetSharedData(){ return m_sharedData; }	

protected:
	AINodeBase* m_child;
	SharedData* m_sharedData;//针对不同unit的数据(本体) 
};


//通用根节点
class EntryTaskNode:public RootNode
{
public:
	EntryTaskNode(const tinyxml2::XMLElement* elem);
};



#endif