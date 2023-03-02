using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/7/9 14:32:04

namespace XMainClient
{
	using ArgDataType = KKSG.MulActivityArg;
    using ResDataType = KKSG.MulActivityRes;

    class RpcC2G_MulActivityReq : Rpc
    {
        public ArgDataType oArg = new ArgDataType();
        public ResDataType oRes = new ResDataType();

        public RpcC2G_MulActivityReq()
        {
        }

        public override uint GetRpcType()
        {
            return 22806;
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
            Process_RpcC2G_MulActivityReq.OnReply(oArg, oRes);
        }

        public override void OnTimeout(object args)
        {
            Process_RpcC2G_MulActivityReq.OnTimeout(oArg);
        }
    }
}
