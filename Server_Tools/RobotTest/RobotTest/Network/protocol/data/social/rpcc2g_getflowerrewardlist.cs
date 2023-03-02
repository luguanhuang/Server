using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/6/25 18:04:23

namespace XMainClient
{
	using ArgDataType = KKSG.GetFlowerRewardListArg;
    using ResDataType = KKSG.GetFlowerRewardListRes;

    class RpcC2G_GetFlowerRewardList : Rpc
    {
        public ArgDataType oArg = new ArgDataType();
        public ResDataType oRes = new ResDataType();

        public RpcC2G_GetFlowerRewardList()
        {
        }

        public override uint GetRpcType()
        {
            return 26656;
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
            Process_RpcC2G_GetFlowerRewardList.OnReply(oArg, oRes);
        }

        public override void OnTimeout(object args)
        {
            Process_RpcC2G_GetFlowerRewardList.OnTimeout(oArg);
        }
    }
}
