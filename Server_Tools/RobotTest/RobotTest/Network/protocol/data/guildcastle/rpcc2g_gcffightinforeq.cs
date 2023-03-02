using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/12/15 0:33:16

namespace XMainClient
{
	using ArgDataType = KKSG.GCFFightInfoArg;
    using ResDataType = KKSG.GCFFightInfoRes;

    class RpcC2G_GCFFightInfoReq : Rpc
    {
        public ArgDataType oArg = new ArgDataType();
        public ResDataType oRes = null;

        public RpcC2G_GCFFightInfoReq()
        {
        }

        public override uint GetRpcType()
        {
            return 41638;
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
            Process_RpcC2G_GCFFightInfoReq.OnReply(oArg, oRes);
        }

        public override void OnTimeout(object args)
        {
            Process_RpcC2G_GCFFightInfoReq.OnTimeout(oArg);
        }
    }
}
