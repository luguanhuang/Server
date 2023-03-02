using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/9/3 17:33:58

namespace XMainClient
{
	using ArgDataType = KKSG.DERankArg;
    using ResDataType = KKSG.DERankRes;

    class RpcC2M_DERankReq : Rpc
    {
        public ArgDataType oArg = new ArgDataType();
        public ResDataType oRes = new ResDataType();

        public RpcC2M_DERankReq()
        {
        }

        public override uint GetRpcType()
        {
            return 16406;
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
            Process_RpcC2M_DERankReq.OnReply(oArg, oRes);
        }

        public override void OnTimeout(object args)
        {
            Process_RpcC2M_DERankReq.OnTimeout(oArg);
        }
    }
}
