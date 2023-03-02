using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/2/22 16:49:39

namespace XMainClient
{
	using ArgDataType = KKSG.UseSupplementReq;
    using ResDataType = KKSG.UseSupplementRes;

    class RpcC2G_UseSupplement : Rpc
    {
        public ArgDataType oArg = new ArgDataType();
        public ResDataType oRes = new ResDataType();

        public RpcC2G_UseSupplement()
        {
        }

        public override uint GetRpcType()
        {
            return 20068;
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
            Process_RpcC2G_UseSupplement.OnReply(oArg, oRes);
        }

        public override void OnTimeout(object args)
        {
            Process_RpcC2G_UseSupplement.OnTimeout(oArg);
        }
    }
}
