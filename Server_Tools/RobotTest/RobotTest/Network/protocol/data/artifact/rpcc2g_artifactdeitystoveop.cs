using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2017/7/31 21:24:02

namespace XMainClient
{
	using ArgDataType = KKSG.ArtifactDeityStoveOpArg;
    using ResDataType = KKSG.ArtifactDeityStoveOpRes;

    class RpcC2G_ArtifactDeityStoveOp : Rpc
    {
        public ArgDataType oArg = new ArgDataType();
        public ResDataType oRes = null;

        public RpcC2G_ArtifactDeityStoveOp()
        {
        }

        public override uint GetRpcType()
        {
            return 35155;
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
            Process_RpcC2G_ArtifactDeityStoveOp.OnReply(oArg, oRes);
        }

        public override void OnTimeout(object args)
        {
            Process_RpcC2G_ArtifactDeityStoveOp.OnTimeout(oArg);
        }
    }
}
