using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2015/10/8 20:17:14

namespace XMainClient
{
	using ArgDataType = KKSG.GetGuildBonusRewardArg;
    using ResDataType = KKSG.GetGuildBonusRewardResult;

    class RpcC2G_GetGuildBonusReward : Rpc
    {
        public ArgDataType oArg = new ArgDataType();
        public ResDataType oRes = new ResDataType();

        public RpcC2G_GetGuildBonusReward()
        {
        }

        public override uint GetRpcType()
        {
            return 55720;
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
            Process_RpcC2G_GetGuildBonusReward.OnReply(oArg, oRes);
        }

        public override void OnTimeout(object args)
        {
            Process_RpcC2G_GetGuildBonusReward.OnTimeout(oArg);
        }
    }
}
