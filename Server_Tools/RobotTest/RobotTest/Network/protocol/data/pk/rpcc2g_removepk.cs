using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2015/12/16 12:20:59

namespace XMainClient
{
	using ArgDataType = KKSG.RemovePkReq;
    using ResDataType = KKSG.RemovePkRes;

    class RpcC2G_RemovePk : Rpc
    {
        public ArgDataType oArg = new ArgDataType();
        public ResDataType oRes = new ResDataType();

        public RpcC2G_RemovePk()
        {
        }

        public override uint GetRpcType()
        {
            return 38224;
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
            Process_RpcC2G_RemovePk.OnReply(oArg, oRes);
        }

        public override void OnTimeout(object args)
        {
            Process_RpcC2G_RemovePk.OnTimeout(oArg);
        }
    }
}
