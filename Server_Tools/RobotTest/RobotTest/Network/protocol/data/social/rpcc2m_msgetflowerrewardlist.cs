using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/8/19 9:55:51

namespace XMainClient
{
	using ArgDataType = KKSG.NewGetFlowerRewardListArg;
    using ResDataType = KKSG.NewGetFlowerRewardListRes;

    class RpcC2M_MSGetFlowerRewardList : Rpc
    {
        public ArgDataType oArg = new ArgDataType();
        public ResDataType oRes = new ResDataType();

        public RpcC2M_MSGetFlowerRewardList()
        {
        }

        public override uint GetRpcType()
        {
            return 16271;
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
            Process_RpcC2M_MSGetFlowerRewardList.OnReply(oArg, oRes);
        }

        public override void OnTimeout(object args)
        {
            Process_RpcC2M_MSGetFlowerRewardList.OnTimeout(oArg);
        }
    }
}
