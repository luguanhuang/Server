using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/12/12 0:15:52

namespace XMainClient
{
	using ArgDataType = KKSG.GetPartnerDetailInfoArg;
    using ResDataType = KKSG.GetPartnerDetailInfoRes;

    class RpcC2M_GetPartnerDetailInfo : Rpc
    {
        public ArgDataType oArg = new ArgDataType();
        public ResDataType oRes = null;

        public RpcC2M_GetPartnerDetailInfo()
        {
        }

        public override uint GetRpcType()
        {
            return 31275;
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
            Process_RpcC2M_GetPartnerDetailInfo.OnReply(oArg, oRes);
        }

        public override void OnTimeout(object args)
        {
            Process_RpcC2M_GetPartnerDetailInfo.OnTimeout(oArg);
        }
    }
}
