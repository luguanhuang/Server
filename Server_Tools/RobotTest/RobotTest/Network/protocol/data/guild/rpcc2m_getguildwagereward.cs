using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/10/17 5:31:02

namespace XMainClient
{
	using ArgDataType = KKSG.GetGuildWageRewardArg;
    using ResDataType = KKSG.GetGuildWageReward;

    class RpcC2M_GetGuildWageReward : Rpc
    {
        public ArgDataType oArg = new ArgDataType();
        public ResDataType oRes = new ResDataType();

        public RpcC2M_GetGuildWageReward()
        {
        }

        public override uint GetRpcType()
        {
            return 50133;
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
            Process_RpcC2M_GetGuildWageReward.OnReply(oArg, oRes);
        }

        public override void OnTimeout(object args)
        {
            Process_RpcC2M_GetGuildWageReward.OnTimeout(oArg);
        }
    }
}
