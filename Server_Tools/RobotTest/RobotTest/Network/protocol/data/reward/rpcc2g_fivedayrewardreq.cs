using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/6/16 14:25:54

namespace XMainClient
{
	using ArgDataType = KKSG.FiveRewardRes;
    using ResDataType = KKSG.FiveRewardRet;

    class RpcC2G_FiveDayRewardReq : Rpc
    {
        public ArgDataType oArg = new ArgDataType();
        public ResDataType oRes = new ResDataType();

        public RpcC2G_FiveDayRewardReq()
        {
        }

        public override uint GetRpcType()
        {
            return 63999;
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
            Process_RpcC2G_FiveDayRewardReq.OnReply(oArg, oRes);
        }

        public override void OnTimeout(object args)
        {
            Process_RpcC2G_FiveDayRewardReq.OnTimeout(oArg);
        }
    }
}
