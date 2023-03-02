#ifndef __TEAMSYNEXTRADATA_H__
#define __TEAMSYNEXTRADATA_H__

class Role;

class HTeamSynExtraData
{
public:
	HTeamSynExtraData();
	~HTeamSynExtraData();

	void FillExtraDataExpID(Role* pRole, int expID, KKSG::TeamSynExtraData& data);
	void FillExtraDataTType(Role* pRole, int ttype, KKSG::TeamSynExtraData& data, bool lcflag = true);
	void TrySynExtraDataToTs(Role* pRole, int ttype);
	void TrySynExtraDataToTs(std::vector<Role*> roles, int ttype);

private:		
	bool NeedSynToTs(Role* pRole, int ttype);	
};


#endif