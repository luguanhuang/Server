using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2017/9/8 9:57:25

namespace XMainClient
{
	using ArgDataType = KKSG.TurnOverWeeklyTaskItemArg;
    using ResDataType = KKSG.TurnOverWeeklyTaskItemRes;

    class RpcC2G_TurnOverWeeklyTaskItem : Rpc
    {
        public ArgDataType oArg = new ArgDataType();
        public ResDataType oRes = null;

        public RpcC2G_TurnOverWeeklyTaskItem()
        {
        }

        public override uint GetRpcType()
        {
            return 19937;
        }

        public override void Serialize(MemoryStream stream)
        {
            Serializer.Serialize(stream, oArg);
        }

        public override void DeSerialize(MemoryStream stream)
        {
            oRes = Serializer.Deserialize<ResDataType>(stream);
        }

        public override void Process()
        {
            base.Process();
            Process_RpcC2G_TurnOverWeeklyTaskItem.OnReply(oArg, oRes);
        }

        public override void OnTimeout(object args)
        {
            Process_RpcC2G_TurnOverWeeklyTaskItem.OnTimeout(oArg);
        }
    }
}
