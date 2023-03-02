#ifndef __SOCIALOP_H__
#define __SOCIALOP_H__

struct SocialInfo;
class SocialOp
{
public:
	SocialOp(UINT64 roleid);
	virtual ~SocialOp();

protected:
	UINT64 m_roleid;
	SocialInfo* m_info;
};

#endif