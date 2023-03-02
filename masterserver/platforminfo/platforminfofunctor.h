#ifndef _H_Plat_Functor_H__
#define _H_Plat_Functor_H__


class CPlatFormInfoAddFunctor
{
public:
	CPlatFormInfoAddFunctor();
	~CPlatFormInfoAddFunctor();

	void operator ()();

public:
	UINT64	m_qwRoleID;
};



class CPlatFormInfoDelFunctor
{
public:
	CPlatFormInfoDelFunctor();
	~CPlatFormInfoDelFunctor();

	void operator ()(std::string key);
};


#endif