using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2015/10/8 19:39:29

namespace XMainClient
{
	using ArgDataType = KKSG.GetGuildBonusListArg;
    using ResDataType = KKSG.GetGuildBonusListResult;

    class RpcC2G_GetGuildBonusList : Rpc
    {
        public ArgDataType oArg = new ArgDataType();
        public ResDataType oRes = new ResDataType();

        public RpcC2G_GetGuildBonusList()
        {
        }

        public override uint GetRpcType()
        {
            return 43440;
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
            Process_RpcC2G_GetGuildBonusList.OnReply(oArg, oRes);
        }

        public override void OnTimeout(object args)
        {
            Process_RpcC2G_GetGuildBonusList.OnTimeout(oArg);
        }
    }
}
