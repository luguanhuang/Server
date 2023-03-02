using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/8/11 19:44:19

namespace XMainClient
{
	using ArgDataType = KKSG.PayCardAwardArg;
    using ResDataType = KKSG.PayCardAwardRes;

    class RpcC2G_PayCardAward : Rpc
    {
        public ArgDataType oArg = new ArgDataType();
        public ResDataType oRes = new ResDataType();

        public RpcC2G_PayCardAward()
        {
        }

        public override uint GetRpcType()
        {
            return 20470;
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
            Process_RpcC2G_PayCardAward.OnReply(oArg, oRes);
        }

        public override void OnTimeout(object args)
        {
            Process_RpcC2G_PayCardAward.OnTimeout(oArg);
        }
    }
}
