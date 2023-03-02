using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace XMainClient
{
    class SweepTask : Task
    {
        uint SceneID = 101;
        uint Count = 1;

        public SweepTask(string arg)
        {
            string[] col = arg.Split(' ');
            SceneID = uint.Parse(col[0]);
            if (col.Length == 2)
            {
                Count = uint.Parse(col[1]);
            }
        }

        public override void OnStart()
        {
            RpcC2G_Sweep oRpc = new RpcC2G_Sweep();
            oRpc.oArg.sceneID = SceneID;
            oRpc.oArg.count = Count;
            state.Send(oRpc);
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
