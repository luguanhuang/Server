using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/9/19 11:53:29

namespace XMainClient
{
	using ArgDataType = KKSG.GetTowerFirstPassRewardArg;
    using ResDataType = KKSG.GetTowerFirstPassRewardRes;

    class RpcC2G_GetTowerFirstPassReward : Rpc
    {
        public ArgDataType oArg = new ArgDataType();
        public ResDataType oRes = new ResDataType();

        public RpcC2G_GetTowerFirstPassReward()
        {
        }

        public override uint GetRpcType()
        {
            return 55009;
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
            Process_RpcC2G_GetTowerFirstPassReward.OnReply(oArg, oRes);
        }

        public override void OnTimeout(object args)
        {
            Process_RpcC2G_GetTowerFirstPassReward.OnTimeout(oArg);
        }
    }
}
