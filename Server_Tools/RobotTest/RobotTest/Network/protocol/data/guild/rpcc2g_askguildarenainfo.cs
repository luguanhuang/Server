using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/7/14 16:51:02

namespace XMainClient
{
	using ArgDataType = KKSG.AskGuildArenaInfoArg;
    using ResDataType = KKSG.AskGuildArenaInfoReq;

    class RpcC2G_AskGuildArenaInfo : Rpc
    {
        public ArgDataType oArg = new ArgDataType();
        public ResDataType oRes = new ResDataType();

        public RpcC2G_AskGuildArenaInfo()
        {
        }

        public override uint GetRpcType()
        {
            return 33973;
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
            Process_RpcC2G_AskGuildArenaInfo.OnReply(oArg, oRes);
        }

        public override void OnTimeout(object args)
        {
            Process_RpcC2G_AskGuildArenaInfo.OnTimeout(oArg);
        }
    }
}
