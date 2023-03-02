using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/10/27 15:24:47

namespace XMainClient
{
	using ArgDataType = KKSG.AskGuildWageInfoArg;
    using ResDataType = KKSG.AskGuildWageInfoRes;

    class RpcC2M_AskGuildWageInfo : Rpc
    {
        public ArgDataType oArg = new ArgDataType();
        public ResDataType oRes = new ResDataType();

        public RpcC2M_AskGuildWageInfo()
        {
        }

        public override uint GetRpcType()
        {
            return 17779;
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
            Process_RpcC2M_AskGuildWageInfo.OnReply(oArg, oRes);
        }

        public override void OnTimeout(object args)
        {
            Process_RpcC2M_AskGuildWageInfo.OnTimeout(oArg);
        }
    }
}
