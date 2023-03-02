using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/12/15 11:36:44

namespace XMainClient
{
	using ArgDataType = KKSG.GetPartnerShopArg;
    using ResDataType = KKSG.GetPartnerShopRes;

    class RpcC2M_GetPartnerShop : Rpc
    {
        public ArgDataType oArg = new ArgDataType();
        public ResDataType oRes = null;

        public RpcC2M_GetPartnerShop()
        {
        }

        public override uint GetRpcType()
        {
            return 46131;
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
            Process_RpcC2M_GetPartnerShop.OnReply(oArg, oRes);
        }

        public override void OnTimeout(object args)
        {
            Process_RpcC2M_GetPartnerShop.OnTimeout(oArg);
        }
    }
}
