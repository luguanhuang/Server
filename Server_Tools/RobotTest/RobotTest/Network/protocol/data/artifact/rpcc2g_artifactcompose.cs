using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2017/5/6 11:06:29

namespace XMainClient
{
	using ArgDataType = KKSG.ArtifactComposeArg;
    using ResDataType = KKSG.ArtifactComposeRes;

    class RpcC2G_ArtifactCompose : Rpc
    {
        public ArgDataType oArg = new ArgDataType();
        public ResDataType oRes = null;

        public RpcC2G_ArtifactCompose()
        {
        }

        public override uint GetRpcType()
        {
            return 599;
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
            Process_RpcC2G_ArtifactCompose.OnReply(oArg, oRes);
        }

        public override void OnTimeout(object args)
        {
            Process_RpcC2G_ArtifactCompose.OnTimeout(oArg);
        }
    }
}
