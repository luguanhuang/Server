using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace XMainClient
{
    class SkillBindTask : Task
    {
        uint skillhash;
        int skillslot;

        public SkillBindTask(string param)
        {
            string[] args = param.Split();
            if (args.Length != 2)
            {
                Console.WriteLine("bind skill arguments error, not 2");
                throw new Exception("bind skill arguments error, not 2");
            }

            skillhash = uint.Parse(args[0]);
            skillslot = int.Parse(args[1]);
        }

        public override void OnStart()
        {
            RpcC2G_BindSkill rpc = new RpcC2G_BindSkill();
            rpc.oArg.skillhash = skillhash;
            rpc.oArg.slot = skillslot;
            state.Send(rpc);
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
    }
}
