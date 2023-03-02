using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2017/5/5 16:57:11

namespace XMainClient
{
	using ArgDataType = KKSG.SelectChestArg;
    using ResDataType = KKSG.SelectChestRes;

    class RpcC2G_SelectChestReward : Rpc
    {
        public ArgDataType oArg = new ArgDataType();
        public ResDataType oRes = null;

        public RpcC2G_SelectChestReward()
        {
        }

        public override uint GetRpcType()
        {
            return 40987;
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
            Process_RpcC2G_SelectChestReward.OnReply(oArg, oRes);
        }

        public override void OnTimeout(object args)
        {
            Process_RpcC2G_SelectChestReward.OnTimeout(oArg);
        }
    }
}
