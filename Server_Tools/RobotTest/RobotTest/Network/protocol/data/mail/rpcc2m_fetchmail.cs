using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/7/22 10:18:01

namespace XMainClient
{
	using ArgDataType = KKSG.FetchMailArg;
    using ResDataType = KKSG.FetchMailRes;

    class RpcC2M_FetchMail : Rpc
    {
        public ArgDataType oArg = new ArgDataType();
        public ResDataType oRes = new ResDataType();

        public RpcC2M_FetchMail()
        {
        }

        public override uint GetRpcType()
        {
            return 12373;
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
            Process_RpcC2M_FetchMail.OnReply(oArg, oRes);
        }

        public override void OnTimeout(object args)
        {
            Process_RpcC2M_FetchMail.OnTimeout(oArg);
        }
    }
}
