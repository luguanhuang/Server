using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace XMainClient
{
    public enum TaskType
    {
        TYPE_TEAMTASK = 1, 
        TYPE_ENTERSCENE = 2,
        TYPE_SINGLEBATTLETASK = 3,
    }
    public abstract class Task
    {
        protected TaskType taskType = 0;
        protected int taskExecuteNum = 0;
        /*public Task(TaskType type) 
        {
            taskType = type;
        }*/
        protected ClientState state;

        public void SetState(ClientState state_)
        {
            state = state_;
        }

        public void Update()
        {
            OnUpdate();
        }

        public abstract void OnStart();
        public abstract void OnUpdate();
        public abstract bool IsFinish();
        public abstract void ResetTask();
        public TaskType GetTaskType() { return taskType; }
    }
}
