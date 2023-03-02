using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/9/1 16:54:24

namespace XMainClient
{
	using ArgDataType = KKSG.IBQueryItemReq;
    using ResDataType = KKSG.IBQueryItemRes;

    class RpcC2G_QueryIBItem : Rpc
    {
        public ArgDataType oArg = new ArgDataType();
        public ResDataType oRes = new ResDataType();

        public RpcC2G_QueryIBItem()
        {
        }

        public override uint GetRpcType()
        {
            return 35378;
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
            Process_RpcC2G_QueryIBItem.OnReply(oArg, oRes);
        }

        public override void OnTimeout(object args)
        {
            Process_RpcC2G_QueryIBItem.OnTimeout(oArg);
        }
    }
}
