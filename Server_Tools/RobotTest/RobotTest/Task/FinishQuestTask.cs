using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace XMainClient
{
    class FinishQuestTask : Task
    {
        int TaskID;

        public FinishQuestTask(int tid)
        {
            TaskID = tid;
        }

        public override void OnStart()
        {
            RpcC2G_TaskOperate oPtc = new RpcC2G_TaskOperate();
            oPtc.oArg.taskOP = (int)KKSG.TaskOPEnum.FINISH_TASK;
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
