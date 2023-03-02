using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2017/5/5 15:00:45

namespace XMainClient
{
	using ArgDataType = KKSG.KMatchCommonArg;
    using ResDataType = KKSG.KMatchCommonRes;

    class RpcC2M_KMatchCommonReq : Rpc
    {
        public ArgDataType oArg = new ArgDataType();
        public ResDataType oRes = null;

        public RpcC2M_KMatchCommonReq()
        {
        }

        public override uint GetRpcType()
        {
            return 57822;
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
            Process_RpcC2M_KMatchCommonReq.OnReply(oArg, oRes);
        }

        public override void OnTimeout(object args)
        {
            Process_RpcC2M_KMatchCommonReq.OnTimeout(oArg);
        }
    }
}
