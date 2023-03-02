using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/11/16 10:01:12

namespace XMainClient
{
	using ArgDataType = KKSG.DailyTaskAskHelpArg;
    using ResDataType = KKSG.DailyTaskAskHelpRes;

    class RpcC2G_DailyTaskAskHelp : Rpc
    {
        public ArgDataType oArg = new ArgDataType();
        public ResDataType oRes = new ResDataType();

        public RpcC2G_DailyTaskAskHelp()
        {
        }

        public override uint GetRpcType()
        {
            return 9236;
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
            Process_RpcC2G_DailyTaskAskHelp.OnReply(oArg, oRes);
        }

        public override void OnTimeout(object args)
        {
            Process_RpcC2G_DailyTaskAskHelp.OnTimeout(oArg);
        }
    }
}
