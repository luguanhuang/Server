using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/12/24 11:57:15

namespace XMainClient
{
	using ArgDataType = KKSG.GetGuildBonusLeftArg;
    using ResDataType = KKSG.GetGuildBonusLeftRes;

    class RpcC2G_GetGuildBonusLeft : Rpc
    {
        public ArgDataType oArg = new ArgDataType();
        public ResDataType oRes = null;

        public RpcC2G_GetGuildBonusLeft()
        {
        }

        public override uint GetRpcType()
        {
            return 9967;
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
            Process_RpcC2G_GetGuildBonusLeft.OnReply(oArg, oRes);
        }

        public override void OnTimeout(object args)
        {
            Process_RpcC2G_GetGuildBonusLeft.OnTimeout(oArg);
        }
    }
}
