using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace XMainClient
{
    class ReinforceEquip: Task
    {

        uint type;
        uint tolevel;
        uint slot;
        public ReinforceEquip(string param)
        {
            string[] args = param.Split();
            if (args.Length < 2)
            {
                Console.WriteLine("reinforceequip arguments error, not 2");
                throw new Exception("reinforceequip arguments error, not 2");
            }
            if (args[0] == "single")
            {
                type = 1;
                if (args.Length != 3)
                {
                    Console.WriteLine("reinforceequip arguments error, not 2");
                    throw new Exception("reinforceequip arguments error, not 2");
                }

                slot = uint.Parse(args[1]);
                tolevel = uint.Parse(args[2]);
            }
            else
            {
                type = 2;
                tolevel = uint.Parse(args[1]);
            }
        }

        public override void OnStart()
        {
            RpcC2G_LevelUpSlotAttr rpc = new RpcC2G_LevelUpSlotAttr();
            if (type == 1)
            {
                rpc.oArg.count = tolevel;
                rpc.oArg.slot = slot;
                state.Send(rpc);
            }
            else if (type == 2)
            {
                rpc.oArg.count = tolevel;
                for (uint i = 0; i < 10; ++i)
                {
                    Console.WriteLine("send this reinforce ok :" + i + ",level:"+tolevel);
                    rpc.oArg.slot = i;
                    state.Send(rpc);
                }
            }
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
