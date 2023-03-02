using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/10/1 16:23:39

namespace XMainClient
{
	using ArgDataType = KKSG.GmfReadyArg;
    using ResDataType = KKSG.GmfReadyRes;

    class RpcC2G_GmfReadyReq : Rpc
    {
        public ArgDataType oArg = new ArgDataType();
        public ResDataType oRes = new ResDataType();

        public RpcC2G_GmfReadyReq()
        {
        }

        public override uint GetRpcType()
        {
            return 12219;
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
            Process_RpcC2G_GmfReadyReq.OnReply(oArg, oRes);
        }

        public override void OnTimeout(object args)
        {
            Process_RpcC2G_GmfReadyReq.OnTimeout(oArg);
        }
    }
}
