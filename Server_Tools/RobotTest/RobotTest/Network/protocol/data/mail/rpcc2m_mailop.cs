using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/7/22 2:13:53

namespace XMainClient
{
	using ArgDataType = KKSG.MailOpArg;
    using ResDataType = KKSG.MailOpRes;

    class RpcC2M_MailOp : Rpc
    {
        public ArgDataType oArg = new ArgDataType();
        public ResDataType oRes = new ResDataType();

        public RpcC2M_MailOp()
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
            Process_RpcC2M_MailOp.OnReply(oArg, oRes);
        }

        public override void OnTimeout(object args)
        {
            Process_RpcC2M_MailOp.OnTimeout(oArg);
        }
    }
}
