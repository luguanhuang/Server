using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2017/9/12 20:02:15

namespace XMainClient
{
	using ArgDataType = KKSG.GetDragonGuildShopRecordArg;
    using ResDataType = KKSG.GetDragonGuildShopRecordRes;

    class RpcC2M_GetDragonGuildShopRecord : Rpc
    {
        public ArgDataType oArg = new ArgDataType();
        public ResDataType oRes = null;

        public RpcC2M_GetDragonGuildShopRecord()
        {
        }

        public override uint GetRpcType()
        {
            return 3114;
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
            Process_RpcC2M_GetDragonGuildShopRecord.OnReply(oArg, oRes);
        }

        public override void OnTimeout(object args)
        {
            Process_RpcC2M_GetDragonGuildShopRecord.OnTimeout(oArg);
        }
    }
}
