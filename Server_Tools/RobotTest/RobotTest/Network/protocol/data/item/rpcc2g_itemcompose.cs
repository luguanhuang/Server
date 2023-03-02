using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2017/3/8 21:14:03

namespace XMainClient
{
	using ArgDataType = KKSG.ItemComposeArg;
    using ResDataType = KKSG.ItemComposeRes;

    class RpcC2G_ItemCompose : Rpc
    {
        public ArgDataType oArg = new ArgDataType();
        public ResDataType oRes = null;

        public RpcC2G_ItemCompose()
        {
        }

        public override uint GetRpcType()
        {
            return 16118;
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
            Process_RpcC2G_ItemCompose.OnReply(oArg, oRes);
        }

        public override void OnTimeout(object args)
        {
            Process_RpcC2G_ItemCompose.OnTimeout(oArg);
        }
    }
}
