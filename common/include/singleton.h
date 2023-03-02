#ifndef _Singleton_H__
#define _Singleton_H__

#include <stdio.h>

#define DECLARE_SINGLETON(cls)\
private:								\
	static cls*	sm_poSingleton;			\
										\
public:									\
	static void CreateInstance()		\
	{									\
		if(sm_poSingleton == NULL)		\
		{								\
			sm_poSingleton = new cls(); \
		}								\
	}									\
	static void DestroyInstance()		\
	{									\
		if(sm_poSingleton != NULL)		\
		{								\
			delete sm_poSingleton;		\
			sm_poSingleton = NULL;		\
		}								\
	}									\
	static cls* Instance()				\
	{									\
		return sm_poSingleton;			\
	}


#define INSTANCE_SINGLETON(type)	type* type::sm_poSingleton = NULL;
	
#define SINGLETON_CREATE_INIT(Klass)    Klass::CreateInstance(); LogDebug("Start Init <"#Klass">"); if (Klass::Instance()->Init() == false) { LogError("Init <"#Klass"> failed!"); return false; } LogDebug("End Init <"#Klass">");
#define SINGLETON_CREATE_INIT_ARG1(Klass, arg1)    Klass::CreateInstance(); LogDebug("Start Init <"#Klass">"); if (Klass::Instance()->Init(arg1) == false) { LogError("Init <"#Klass"> failed!"); return false; };
#define SINGLETON_CREATE_INIT_ARG2(Klass, arg1, arg2)    Klass::CreateInstance(); LogDebug("Start Init <"#Klass">"); if (Klass::Instance()->Init(arg1, arg2) == false) { LogError("Init <"#Klass"> failed!"); return false; };

#define SINGLETON_DESTORY_UNINIT(Klass) Klass::Instance()->Uninit(); Klass::DestroyInstance(); LogDebug("Destory Class <"#Klass">");


#endif