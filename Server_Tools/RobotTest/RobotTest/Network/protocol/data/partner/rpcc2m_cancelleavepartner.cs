using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/12/12 15:47:10

namespace XMainClient
{
	using ArgDataType = KKSG.CancelLeavePartnerArg;
    using ResDataType = KKSG.CancelLeavePartnerRes;

    class RpcC2M_CancelLeavePartner : Rpc
    {
        public ArgDataType oArg = new ArgDataType();
        public ResDataType oRes = null;

        public RpcC2M_CancelLeavePartner()
        {
        }

        public override uint GetRpcType()
        {
            return 27794;
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
            Process_RpcC2M_CancelLeavePartner.OnReply(oArg, oRes);
        }

        public override void OnTimeout(object args)
        {
            Process_RpcC2M_CancelLeavePartner.OnTimeout(oArg);
        }
    }
}
