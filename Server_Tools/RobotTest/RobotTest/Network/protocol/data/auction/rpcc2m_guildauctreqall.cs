using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/11/1 16:07:35

namespace XMainClient
{
	using ArgDataType = KKSG.GuildAuctReqArg;
    using ResDataType = KKSG.GuildAuctReqRes;

    class RpcC2M_GuildAuctReqAll : Rpc
    {
        public ArgDataType oArg = new ArgDataType();
        public ResDataType oRes = new ResDataType();

        public RpcC2M_GuildAuctReqAll()
        {
        }

        public override uint GetRpcType()
        {
            return 41964;
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
            Process_RpcC2M_GuildAuctReqAll.OnReply(oArg, oRes);
        }

        public override void OnTimeout(object args)
        {
            Process_RpcC2M_GuildAuctReqAll.OnTimeout(oArg);
        }
    }
}
