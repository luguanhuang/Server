using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/8/8 21:01:12

namespace XMainClient
{
	using ArgDataType = KKSG.QueryPowerPointArg;
    using ResDataType = KKSG.QueryPowerPointRes;

    class RpcC2G_QueryPowerPoint : Rpc
    {
        public ArgDataType oArg = new ArgDataType();
        public ResDataType oRes = new ResDataType();

        public RpcC2G_QueryPowerPoint()
        {
        }

        public override uint GetRpcType()
        {
            return 44381;
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
            Process_RpcC2G_QueryPowerPoint.OnReply(oArg, oRes);
        }

        public override void OnTimeout(object args)
        {
            Process_RpcC2G_QueryPowerPoint.OnTimeout(oArg);
        }
    }
}
