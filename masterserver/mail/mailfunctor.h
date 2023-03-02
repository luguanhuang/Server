#ifndef _H_Mail_Functor_H__
#define _H_Mail_Functor_H__


class CMailDataAddFunctor
{
public:
	CMailDataAddFunctor();
	~CMailDataAddFunctor();
	
	void operator ()();

public:
	UINT64	m_qwRoleID;
	const KKSG::ReadMailFromDBRes* m_poRes;
};



class CMailDataDelFunctor
{
public:
	CMailDataDelFunctor();
	~CMailDataDelFunctor();

	void operator ()(UINT64 key);
};


#endif