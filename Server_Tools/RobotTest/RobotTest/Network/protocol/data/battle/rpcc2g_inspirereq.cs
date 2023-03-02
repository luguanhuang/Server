using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/10/3 16:25:34

namespace XMainClient
{
	using ArgDataType = KKSG.InspireArg;
    using ResDataType = KKSG.InspireRes;

    class RpcC2G_InspireReq : Rpc
    {
        public ArgDataType oArg = new ArgDataType();
        public ResDataType oRes = new ResDataType();

        public RpcC2G_InspireReq()
        {
        }

        public override uint GetRpcType()
        {
            return 54147;
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
            Process_RpcC2G_InspireReq.OnReply(oArg, oRes);
        }

        public override void OnTimeout(object args)
        {
            Process_RpcC2G_InspireReq.OnTimeout(oArg);
        }
    }
}
