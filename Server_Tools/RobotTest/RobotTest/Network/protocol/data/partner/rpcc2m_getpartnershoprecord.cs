using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/12/17 12:14:19

namespace XMainClient
{
	using ArgDataType = KKSG.GetPartnerShopRecordArg;
    using ResDataType = KKSG.GetPartnerShopRecordRes;

    class RpcC2M_GetPartnerShopRecord : Rpc
    {
        public ArgDataType oArg = new ArgDataType();
        public ResDataType oRes = null;

        public RpcC2M_GetPartnerShopRecord()
        {
        }

        public override uint GetRpcType()
        {
            return 56970;
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
            Process_RpcC2M_GetPartnerShopRecord.OnReply(oArg, oRes);
        }

        public override void OnTimeout(object args)
        {
            Process_RpcC2M_GetPartnerShopRecord.OnTimeout(oArg);
        }
    }
}
