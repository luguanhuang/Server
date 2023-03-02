using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2017/5/11 10:13:19

namespace XMainClient
{
	using ArgDataType = KKSG.GetGuildPartyReceiveInfoArg;
    using ResDataType = KKSG.GetGuildPartyReceiveInfoRes;

    class RpcC2G_GetGuildPartyReceiveInfo : Rpc
    {
        public ArgDataType oArg = new ArgDataType();
        public ResDataType oRes = null;

        public RpcC2G_GetGuildPartyReceiveInfo()
        {
        }

        public override uint GetRpcType()
        {
            return 58154;
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
            Process_RpcC2G_GetGuildPartyReceiveInfo.OnReply(oArg, oRes);
        }

        public override void OnTimeout(object args)
        {
            Process_RpcC2G_GetGuildPartyReceiveInfo.OnTimeout(oArg);
        }
    }
}
