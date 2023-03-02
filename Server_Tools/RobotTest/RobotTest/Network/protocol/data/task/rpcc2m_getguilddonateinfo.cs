using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/11/16 19:30:20

namespace XMainClient
{
	using ArgDataType = KKSG.GetGuildDonateInfoArg;
    using ResDataType = KKSG.GetGuildDonateInfoRes;

    class RpcC2M_GetGuildDonateInfo : Rpc
    {
        public ArgDataType oArg = new ArgDataType();
        public ResDataType oRes = new ResDataType();

        public RpcC2M_GetGuildDonateInfo()
        {
        }

        public override uint GetRpcType()
        {
            return 14656;
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
            Process_RpcC2M_GetGuildDonateInfo.OnReply(oArg, oRes);
        }

        public override void OnTimeout(object args)
        {
            Process_RpcC2M_GetGuildDonateInfo.OnTimeout(oArg);
        }
    }
}
