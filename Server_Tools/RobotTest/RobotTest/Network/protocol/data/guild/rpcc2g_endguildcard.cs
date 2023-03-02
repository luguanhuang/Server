using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2015/10/2 10:45:21

namespace XMainClient
{
	using ArgDataType = KKSG.EndGuildCardArg;
    using ResDataType = KKSG.EndGuildCardRes;

    class RpcC2G_EndGuildCard : Rpc
    {
        public ArgDataType oArg = new ArgDataType();
        public ResDataType oRes = new ResDataType();

        public RpcC2G_EndGuildCard()
        {
        }

        public override uint GetRpcType()
        {
            return 13212;
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
            Process_RpcC2G_EndGuildCard.OnReply(oArg, oRes);
        }

        public override void OnTimeout(object args)
        {
            Process_RpcC2G_EndGuildCard.OnTimeout(oArg);
        }
    }
}
