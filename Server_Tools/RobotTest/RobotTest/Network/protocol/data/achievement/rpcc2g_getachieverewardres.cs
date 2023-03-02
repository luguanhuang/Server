using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/5/31 10:23:38

namespace XMainClient
{
	using ArgDataType = KKSG.GetAchieveRewardReq;
    using ResDataType = KKSG.GetAchieveRewardRes;

    class RpcC2G_GetAchieveRewardRes : Rpc
    {
        public ArgDataType oArg = new ArgDataType();
        public ResDataType oRes = new ResDataType();

        public RpcC2G_GetAchieveRewardRes()
        {
        }

        public override uint GetRpcType()
        {
            return 2754;
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
            Process_RpcC2G_GetAchieveRewardRes.OnReply(oArg, oRes);
        }

        public override void OnTimeout(object args)
        {
            Process_RpcC2G_GetAchieveRewardRes.OnTimeout(oArg);
        }
    }
}
