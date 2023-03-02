using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2015/6/12 10:21:12

namespace XMainClient
{
	using ArgDataType = KKSG.UseItemArg;
    using ResDataType = KKSG.UseItemRes;

    class RpcC2G_UseItem : Rpc
    {
        public ArgDataType oArg = new ArgDataType();
        public ResDataType oRes = new ResDataType();

        public RpcC2G_UseItem()
        {
        }

        public override uint GetRpcType()
        {
            return 64132;
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
            Process_RpcC2G_UseItem.OnReply(oArg, oRes);
        }

        public override void OnTimeout(object args)
        {
            Process_RpcC2G_UseItem.OnTimeout(oArg);
        }
    }
}
