using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/3/17 17:55:18

namespace XMainClient
{
	using ArgDataType = KKSG.QueryGateArg;
    using ResDataType = KKSG.QueryGateRes;

    class RpcC2I_QueryGateIP : Rpc
    {
        public ArgDataType oArg = new ArgDataType();
        public ResDataType oRes = new ResDataType();

        public RpcC2I_QueryGateIP()
        {
        }

        public override uint GetRpcType()
        {
            return 9179;
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
            Process_RpcC2I_QueryGateIP.OnReply(oArg, oRes);
        }

        public override void OnTimeout(object args)
        {
            Process_RpcC2I_QueryGateIP.OnTimeout(oArg);
        }
    }
}
