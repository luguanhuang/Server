using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/9/12 11:53:57

namespace XMainClient
{
	using ArgDataType = KKSG.GetSweepTowerRewardArg;
    using ResDataType = KKSG.GetSweepTowerRewardRes;

    class RpcC2G_GetSweepTowerReward : Rpc
    {
        public ArgDataType oArg = new ArgDataType();
        public ResDataType oRes = new ResDataType();

        public RpcC2G_GetSweepTowerReward()
        {
        }

        public override uint GetRpcType()
        {
            return 23703;
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
            Process_RpcC2G_GetSweepTowerReward.OnReply(oArg, oRes);
        }

        public override void OnTimeout(object args)
        {
            Process_RpcC2G_GetSweepTowerReward.OnTimeout(oArg);
        }
    }
}
