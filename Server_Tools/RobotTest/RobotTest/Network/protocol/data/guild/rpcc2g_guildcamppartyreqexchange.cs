using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2017/4/26 19:08:18

namespace XMainClient
{
	using ArgDataType = KKSG.GuildCampPartyReqExchangeReq;
    using ResDataType = KKSG.GuildCampPartyReqExchangeRes;

    class RpcC2G_GuildCampPartyReqExchange : Rpc
    {
        public ArgDataType oArg = new ArgDataType();
        public ResDataType oRes = null;

        public RpcC2G_GuildCampPartyReqExchange()
        {
        }

        public override uint GetRpcType()
        {
            return 51021;
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
            Process_RpcC2G_GuildCampPartyReqExchange.OnReply(oArg, oRes);
        }

        public override void OnTimeout(object args)
        {
            Process_RpcC2G_GuildCampPartyReqExchange.OnTimeout(oArg);
        }
    }
}
