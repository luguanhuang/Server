using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/12/12 17:11:03

namespace XMainClient
{
	using ArgDataType = KKSG.EnhanceItemArg;
    using ResDataType = KKSG.EnhanceItemRes;

    class RpcC2G_EnhanceItem : Rpc
    {
        public ArgDataType oArg = new ArgDataType();
        public ResDataType oRes = null;

        public RpcC2G_EnhanceItem()
        {
        }

        public override uint GetRpcType()
        {
            return 3744;
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
            Process_RpcC2G_EnhanceItem.OnReply(oArg, oRes);
        }

        public override void OnTimeout(object args)
        {
            Process_RpcC2G_EnhanceItem.OnTimeout(oArg);
        }
    }
}
