using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2017/5/6 18:50:33

namespace XMainClient
{
	using ArgDataType = KKSG.ReplyPartyExchangeItemOptArg;
    using ResDataType = KKSG.ReplyPartyExchangeItemOptRes;

    class RpcC2G_ReplyPartyExchangeItemOpt : Rpc
    {
        public ArgDataType oArg = new ArgDataType();
        public ResDataType oRes = null;

        public RpcC2G_ReplyPartyExchangeItemOpt()
        {
        }

        public override uint GetRpcType()
        {
            return 13740;
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
            Process_RpcC2G_ReplyPartyExchangeItemOpt.OnReply(oArg, oRes);
        }

        public override void OnTimeout(object args)
        {
            Process_RpcC2G_ReplyPartyExchangeItemOpt.OnTimeout(oArg);
        }
    }
}
