using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2017/5/2 21:51:43

namespace XMainClient
{
	using ArgDataType = KKSG.GuildCampExchangeOperateArg;
    using ResDataType = KKSG.GuildCampExchangeOperateRes;

    class RpcC2G_GuildCampExchangeOperate : Rpc
    {
        public ArgDataType oArg = new ArgDataType();
        public ResDataType oRes = null;

        public RpcC2G_GuildCampExchangeOperate()
        {
        }

        public override uint GetRpcType()
        {
            return 31811;
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
            Process_RpcC2G_GuildCampExchangeOperate.OnReply(oArg, oRes);
        }

        public override void OnTimeout(object args)
        {
            Process_RpcC2G_GuildCampExchangeOperate.OnTimeout(oArg);
        }
    }
}
