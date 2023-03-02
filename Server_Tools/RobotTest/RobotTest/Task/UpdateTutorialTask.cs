using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace XMainClient
{
    class UpdateTutorialTask : Task
    {
        uint tutorialID = 0;

        public UpdateTutorialTask(uint ttid)
        {
            tutorialID = ttid;
        }

        public override void OnStart()
        {
            PtcC2G_UpdateTutorial oPtc = new PtcC2G_UpdateTutorial();
            oPtc.Data.tutorialID = tutorialID;
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
