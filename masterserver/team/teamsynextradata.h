#ifndef __TEAMSYNEXTRADATA_H__
#define __TEAMSYNEXTRADATA_H__

class CRole;

class HTeamSynExtraData
{
public:
	HTeamSynExtraData();
	~HTeamSynExtraData();

	void FillExtraData(CRole* pRole, KKSG::TeamSynExtraData& data);

private:		

};

#endif