using System;
using System.Collections.Generic;
using System.Text;

namespace XMainClient
{
    class SyncTimeTask : Task
    {
        public override void OnStart()
        {
            RpcC2G_SyncTime syncTime = new RpcC2G_SyncTime();
            syncTime.oArg.time = 0;
            state.Send(syncTime);
            state.syncTimeState = SyncTimeState.Start;
        }

        public override void OnUpdate()
        {

        }

        public override bool IsFinish()
        {
            return (state.syncTimeState == SyncTimeState.End);
        }
        public override void ResetTask()
        {
        }
    }
}
