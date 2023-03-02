using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;

namespace XMainClient
{
    public abstract class Rpc
    {
        public static Dictionary<uint, Rpc> sm_RpcWaitingReply = new Dictionary<uint, Rpc>();
        private static uint sTagID = 0;
        public virtual uint GetRpcType() { return 0; }
        public Connection conn;

        public void SerializeWithHead(MemoryStream stream)
        {
            long begin = stream.Position;

            ProtocolHead head = new ProtocolHead();
            head.type = GetRpcType();
            head.flag = (uint)PtcHeadFlag.RPC_TYPE_REQUEST;
            head.tagID = tagID;
            head.Serialize(stream);

            Serialize(stream);

            long position = stream.Position;
            uint length = (uint)(position - begin - sizeof(int));
            stream.Position = begin;
            stream.Write(BitConverter.GetBytes(length), 0, sizeof(UInt32));
            stream.Position = position;
        }

        public abstract void Serialize(MemoryStream stream);
        public abstract void DeSerialize(MemoryStream stream);

        private uint tagID = 0;
        private uint timeout = 5000;

        public uint Timeout
        {
            get { return timeout; }
            set { timeout = value; }
        }

        public void OnSend()
        {
            ++sTagID;
            tagID = sTagID;
            sm_RpcWaitingReply[tagID] = this;
            //timerToken = XTimerMgr.singleton.SetTimer(timeout, this.OnTimeout, null);
        }

        public abstract void OnTimeout(object args);

        public static Rpc GetRpcByTag(uint dwTag)
        {
            if (sm_RpcWaitingReply.ContainsKey(dwTag))
            {
                Rpc rpc;
                sm_RpcWaitingReply.TryGetValue(dwTag, out rpc);
                return rpc;
            }

            return null;
        }

        public virtual void Process()
        {
            //XTimerMgr.singleton.KillTimer(timerToken);
        }
    }
}
