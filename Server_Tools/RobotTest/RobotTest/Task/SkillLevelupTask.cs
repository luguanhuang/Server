using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace XMainClient
{
    class SkillLevelupTask : Task
    {
        uint skillhash;

        public SkillLevelupTask(string skill)
        {
            this.skillhash = XHash(skill);
        }

        public override void OnStart()
        {
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
    }
}
