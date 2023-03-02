using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace XMainClient
{
    class SkillLearnAllTask : Task
    {

        public SkillLearnAllTask()
        {
        }

        public override void OnStart()
        {
            uint skillhash = 0;
            if (state.basicProfession == 1)
            {
                

            }
            else if (state.basicProfession == 2)
            {

            }
            else if (state.basicProfession == 3)
            {

            }
            else if (state.basicProfession == 4)
            {

            }
            RpcC2G_SkillLevelup ptc = new RpcC2G_SkillLevelup();
            ptc.oArg.skillHash = skillhash;
            state.Send(ptc);
        }

        public override void OnUpdate()
        {
        }

        public override bool IsFinish()
        {
            return true;
        }
        public override void ResetTask()
        {
        }
        //Daniel J. Bernstein, Times 33 with Addition Hash Function
        public uint XHash(string str)
        {
            if (str == null) return 0;

            uint hash = 0;
            for (int i = 0; i < str.Length; i++)
            {
                hash = (hash << 5) + hash + str[i];
            }

            return hash;
        }
        /*public List<string> LearnWarriorSkill()
        {
            uint skillhash = XHash("");
            List<string> list;
            list.Add();

        }*/

    }
}
