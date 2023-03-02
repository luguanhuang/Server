using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/12/24 15:19:44

namespace XMainClient
{
	using ArgDataType = KKSG.GetPartnerInfoArg;
    using ResDataType = KKSG.GetPartnerInfoRes;

    class RpcC2M_GetPartnerInfo : Rpc
    {
        public ArgDataType oArg = new ArgDataType();
        public ResDataType oRes = null;

        public RpcC2M_GetPartnerInfo()
        {
        }

        public override uint GetRpcType()
        {
            return 61123;
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
            Process_RpcC2M_GetPartnerInfo.OnReply(oArg, oRes);
        }

        public override void OnTimeout(object args)
        {
            Process_RpcC2M_GetPartnerInfo.OnTimeout(oArg);
        }
    }
}
