using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace XMainClient
{
    class TakeQuestTask : Task
    {
        int TaskID;

        public TakeQuestTask(int tid)
        {
            TaskID = tid;
        }

        public override void OnStart()
        {
            RpcC2G_TaskOperate oPtc = new RpcC2G_TaskOperate();
            oPtc.oArg.taskOP = (int)KKSG.TaskOPEnum.TAKE_TASK;
            oPtc.oArg.taskID = TaskID;
            state.Send(oPtc);
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
