using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/12/14 23:27:10

namespace XMainClient
{
	using ArgDataType = KKSG.GCFReadyInfoArg;
    using ResDataType = KKSG.GCFReadyInfoRes;

    class RpcC2M_GCFReadyInfoReq : Rpc
    {
        public ArgDataType oArg = new ArgDataType();
        public ResDataType oRes = null;

        public RpcC2M_GCFReadyInfoReq()
        {
        }

        public override uint GetRpcType()
        {
            return 44917;
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
            Process_RpcC2M_GCFReadyInfoReq.OnReply(oArg, oRes);
        }

        public override void OnTimeout(object args)
        {
            Process_RpcC2M_GCFReadyInfoReq.OnTimeout(oArg);
        }
    }
}
