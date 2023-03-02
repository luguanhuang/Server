using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/11/4 14:28:20

namespace XMainClient
{
	using ArgDataType = KKSG.GetGuildIntegralInfoArg;
    using ResDataType = KKSG.GetGuildIntegralInfoRes;

    class RpcC2M_GetGuildIntegralInfo : Rpc
    {
        public ArgDataType oArg = new ArgDataType();
        public ResDataType oRes = new ResDataType();

        public RpcC2M_GetGuildIntegralInfo()
        {
        }

        public override uint GetRpcType()
        {
            return 56762;
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
            Process_RpcC2M_GetGuildIntegralInfo.OnReply(oArg, oRes);
        }

        public override void OnTimeout(object args)
        {
            Process_RpcC2M_GetGuildIntegralInfo.OnTimeout(oArg);
        }
    }
}
