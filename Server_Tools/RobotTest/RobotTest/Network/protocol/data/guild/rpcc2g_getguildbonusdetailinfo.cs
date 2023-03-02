using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2015/10/8 20:03:53

namespace XMainClient
{
	using ArgDataType = KKSG.GetGuildBonusDetailInfoArg;
    using ResDataType = KKSG.GetGuildBonusDetailInfoResult;

    class RpcC2G_GetGuildBonusDetailInfo : Rpc
    {
        public ArgDataType oArg = new ArgDataType();
        public ResDataType oRes = new ResDataType();

        public RpcC2G_GetGuildBonusDetailInfo()
        {
        }

        public override uint GetRpcType()
        {
            return 20027;
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
            Process_RpcC2G_GetGuildBonusDetailInfo.OnReply(oArg, oRes);
        }

        public override void OnTimeout(object args)
        {
            Process_RpcC2G_GetGuildBonusDetailInfo.OnTimeout(oArg);
        }
    }
}
