using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2017/5/6 22:16:09

namespace XMainClient
{
	using ArgDataType = KKSG.GetGuildCampPartyRewardArg;
    using ResDataType = KKSG.GetGuildCampPartyRewardRes;

    class RpcC2G_GetGuildCampPartyReward : Rpc
    {
        public ArgDataType oArg = new ArgDataType();
        public ResDataType oRes = null;

        public RpcC2G_GetGuildCampPartyReward()
        {
        }

        public override uint GetRpcType()
        {
            return 58935;
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
            Process_RpcC2G_GetGuildCampPartyReward.OnReply(oArg, oRes);
        }

        public override void OnTimeout(object args)
        {
            Process_RpcC2G_GetGuildCampPartyReward.OnTimeout(oArg);
        }
    }
}
