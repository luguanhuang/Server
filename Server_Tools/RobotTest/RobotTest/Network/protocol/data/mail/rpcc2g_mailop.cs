using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/5/3 15:07:39

namespace XMainClient
{
	using ArgDataType = KKSG.MailOpArg;
    using ResDataType = KKSG.MailOpRes;

    class RpcC2G_MailOp : Rpc
    {
        public ArgDataType oArg = new ArgDataType();
        public ResDataType oRes = new ResDataType();

        public RpcC2G_MailOp()
        {
        }

        public override uint GetRpcType()
        {
            return 50122;
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
            Process_RpcC2G_MailOp.OnReply(oArg, oRes);
        }

        public override void OnTimeout(object args)
        {
            Process_RpcC2G_MailOp.OnTimeout(oArg);
        }
    }
}
