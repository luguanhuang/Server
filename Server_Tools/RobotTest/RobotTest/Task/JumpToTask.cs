using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace XMainClient
{
    class JumpToTask : Task
    {
        private int jumpTo;

        public JumpToTask(int step)
        {
            jumpTo = step;
        }

        public void SetJumpTo(int step)
        {
            jumpTo = step;
        }

        public override void OnUpdate()
        {
            state.SetTaskStep(jumpTo);
            state.ResetTaskChain();
        }

        public override bool IsFinish()
        {
            if (state.GetCurrentTask() == this)
            {
                Console.WriteLine("Jump to self, finished");
                return true;
            }
            return state.GetCurrentTask().IsFinish();
            //return true;
        }

        public override void OnStart()
        {
        }
        public override void ResetTask()
        {
        }
    }
}
