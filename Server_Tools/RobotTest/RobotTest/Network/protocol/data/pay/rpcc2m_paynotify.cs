using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/10/24 11:50:20

namespace XMainClient
{
	using ArgDataType = KKSG.PayNotifyArg;
    using ResDataType = KKSG.PayNotifyRes;

    class RpcC2M_PayNotify : Rpc
    {
        public ArgDataType oArg = new ArgDataType();
        public ResDataType oRes = new ResDataType();

        public RpcC2M_PayNotify()
        {
        }

        public override uint GetRpcType()
        {
            return 32125;
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
            Process_RpcC2M_PayNotify.OnReply(oArg, oRes);
        }

        public override void OnTimeout(object args)
        {
            Process_RpcC2M_PayNotify.OnTimeout(oArg);
        }
    }
}
