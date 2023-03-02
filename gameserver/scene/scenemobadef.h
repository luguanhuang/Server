#ifndef __SCENEMOBADEF_H__
#define __SCENEMOBADEF_H__

#define MOBA_GROUP_1 11
#define MOBA_GROUP_2 12

enum MobaHint
{
	MobaHint_Begin10 = 1001,			///> ��ʼ��Ϸ10�룺10���С����ʼ�����з�
	MobaHint_Begin20 = 1002,			///> ��ʼ��Ϸ20�룺ȫ���������ݻٵз��������ͻ���

	MobaHint_SelfLevel20 = 1003,		///> ����С���ȼ�20��ʱ���ҷ�����20���������ɳ��߼�����
	MobaHint_OtherLevel20 = 1004,		///> �з�С���ȼ�20��ʱ���ҷ�����20���������ɳ��߼�����
	MobaHint_SelfLevel50 = 1005,		///> ����С���ȼ�50��ʱ���ҷ�����50���������ɳ��ռ�����
	MobaHint_OtherLevel50 = 1006,		///> �з�С���ȼ�50��ʱ���ҷ�����50���������ɳ��ռ�����

	MobaHint_SelfBeAttack = 1007,		///> �ҷ����ر�����ʱ��CD1���ӣ����ҷ����������ܹ�������Ѹ�ٻط�

	MobaHint_SelfDestoryed = 1008,		///> �ҷ����������ݻ�ʱ���ҷ����������ݻ�
	MobaHint_OtherDestoryed = 1009,		///> �з����������ݻ�ʱ���ݻٵз�������

	MobaHint_SelfKill = 1010,			///> �ҷ���ɱʨЫ���ɹ���ɱʨЫ���ҷ����ɳ���������֧Ԯ
	MobaHint_OtherSkill = 1011,			///> �з���ɱʨЫ���з���ɱ��ʨЫ�����ɳ�������������ҷ�������
};

#endif