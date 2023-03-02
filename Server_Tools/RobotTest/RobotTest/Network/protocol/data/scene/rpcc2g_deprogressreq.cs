using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/9/3 17:29:16

namespace XMainClient
{
	using ArgDataType = KKSG.DEProgressArg;
    using ResDataType = KKSG.DEProgressRes;

    class RpcC2G_DEProgressReq : Rpc
    {
        public ArgDataType oArg = new ArgDataType();
        public ResDataType oRes = new ResDataType();

        public RpcC2G_DEProgressReq()
        {
        }

        public override uint GetRpcType()
        {
            return 5238;
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
            Process_RpcC2G_DEProgressReq.OnReply(oArg, oRes);
        }

        public override void OnTimeout(object args)
        {
            Process_RpcC2G_DEProgressReq.OnTimeout(oArg);
        }
    }
}
