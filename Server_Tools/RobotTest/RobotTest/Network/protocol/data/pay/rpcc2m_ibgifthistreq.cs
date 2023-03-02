using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2017/4/17 17:25:57

namespace XMainClient
{
	using ArgDataType = KKSG.IBGiftHistAllItemArg;
    using ResDataType = KKSG.IBGiftHistAllItemRes;

    class RpcC2M_IbGiftHistReq : Rpc
    {
        public ArgDataType oArg = new ArgDataType();
        public ResDataType oRes = null;

        public RpcC2M_IbGiftHistReq()
        {
        }

        public override uint GetRpcType()
        {
            return 27050;
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
            Process_RpcC2M_IbGiftHistReq.OnReply(oArg, oRes);
        }

        public override void OnTimeout(object args)
        {
            Process_RpcC2M_IbGiftHistReq.OnTimeout(oArg);
        }
    }
}
