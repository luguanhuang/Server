using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/12/28 19:16:51

namespace XMainClient
{
	using ArgDataType = KKSG.GCFCommonArg;
    using ResDataType = KKSG.GCFCommonRes;

    class RpcC2M_GCFCommonReq : Rpc
    {
        public ArgDataType oArg = new ArgDataType();
        public ResDataType oRes = null;

        public RpcC2M_GCFCommonReq()
        {
        }

        public override uint GetRpcType()
        {
            return 28945;
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
            Process_RpcC2M_GCFCommonReq.OnReply(oArg, oRes);
        }

        public override void OnTimeout(object args)
        {
            Process_RpcC2M_GCFCommonReq.OnTimeout(oArg);
        }
    }
}
