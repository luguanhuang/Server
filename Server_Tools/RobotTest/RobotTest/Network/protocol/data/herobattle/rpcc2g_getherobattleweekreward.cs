using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/12/28 22:48:38

namespace XMainClient
{
	using ArgDataType = KKSG.GetHeroBattleWeekRewardArg;
    using ResDataType = KKSG.GetHeroBattleWeekRewardRes;

    class RpcC2G_GetHeroBattleWeekReward : Rpc
    {
        public ArgDataType oArg = new ArgDataType();
        public ResDataType oRes = null;

        public RpcC2G_GetHeroBattleWeekReward()
        {
        }

        public override uint GetRpcType()
        {
            return 63058;
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
            Process_RpcC2G_GetHeroBattleWeekReward.OnReply(oArg, oRes);
        }

        public override void OnTimeout(object args)
        {
            Process_RpcC2G_GetHeroBattleWeekReward.OnTimeout(oArg);
        }
    }
}
