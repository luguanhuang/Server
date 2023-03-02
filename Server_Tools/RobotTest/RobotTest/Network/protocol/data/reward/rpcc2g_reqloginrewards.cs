using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/6/16 14:25:59

namespace XMainClient
{
	using ArgDataType = KKSG.LoginRewardReq;
    using ResDataType = KKSG.LoginRewardRet;

    class RpcC2G_ReqLoginRewards : Rpc
    {
        public ArgDataType oArg = new ArgDataType();
        public ResDataType oRes = new ResDataType();

        public RpcC2G_ReqLoginRewards()
        {
        }

        public override uint GetRpcType()
        {
            return 49761;
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
            Process_RpcC2G_ReqLoginRewards.OnReply(oArg, oRes);
        }

        public override void OnTimeout(object args)
        {
            Process_RpcC2G_ReqLoginRewards.OnTimeout(oArg);
        }
    }
}
