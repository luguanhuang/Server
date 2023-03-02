using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2017/6/26 14:36:04

namespace XMainClient
{
	using ArgDataType = KKSG.GetMobaBattleWeekRewardArg;
    using ResDataType = KKSG.GetMobaBattleWeekRewardRes;

    class RpcC2M_GetMobaBattleWeekReward : Rpc
    {
        public ArgDataType oArg = new ArgDataType();
        public ResDataType oRes = null;

        public RpcC2M_GetMobaBattleWeekReward()
        {
        }

        public override uint GetRpcType()
        {
            return 55678;
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
            Process_RpcC2M_GetMobaBattleWeekReward.OnReply(oArg, oRes);
        }

        public override void OnTimeout(object args)
        {
            Process_RpcC2M_GetMobaBattleWeekReward.OnTimeout(oArg);
        }
    }
}
