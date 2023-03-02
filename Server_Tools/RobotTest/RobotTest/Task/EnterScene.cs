using System;
using System.Collections.Generic;
using System.IO;
using ProtoBuf;
using System.Linq;

namespace XMainClient
{
    class EnterSceneTask : Task
    {
        public int sceneID = 0;
        public bool isFinish = false;

        public EnterSceneTask(string sceneid)
        {
            sceneID = int.Parse(sceneid);
            taskType = TaskType.TYPE_ENTERSCENE;
        }

        public override void OnUpdate()
        {
        }

        public override bool IsFinish()
        {
            return isFinish;
        }
        public override void ResetTask()
        {
            isFinish = false;
        }


        public override void OnStart()
        {
            PtcC2G_EnterSceneReq req = new PtcC2G_EnterSceneReq();
            req.Data.sceneID = (uint)sceneID;
            state.Send(req);
        }

        public void Finish()
        {
            isFinish = true;
        }
    }
}
