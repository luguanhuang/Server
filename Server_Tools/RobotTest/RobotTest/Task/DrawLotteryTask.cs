using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace XMainClient
{
    class DrawLotteryTask : Task
    {
        uint LotteryCount;

        public DrawLotteryTask(uint Lottery)
        {
            LotteryCount = Lottery;
        }

        public override void OnStart()
        {
            RpcC2G_DrawLottery oRpc = new RpcC2G_DrawLottery();
            oRpc.oArg.type = LotteryCount;
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
