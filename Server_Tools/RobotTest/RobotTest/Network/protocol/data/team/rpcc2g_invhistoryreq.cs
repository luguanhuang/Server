using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/7/8 19:54:02

namespace XMainClient
{
	using ArgDataType = KKSG.InvHistoryArg;
    using ResDataType = KKSG.InvHistoryRes;

    class RpcC2G_InvHistoryReq : Rpc
    {
        public ArgDataType oArg = new ArgDataType();
        public ResDataType oRes = new ResDataType();

        public RpcC2G_InvHistoryReq()
        {
        }

        public override uint GetRpcType()
        {
            return 23442;
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
            Process_RpcC2G_InvHistoryReq.OnReply(oArg, oRes);
        }

        public override void OnTimeout(object args)
        {
            Process_RpcC2G_InvHistoryReq.OnTimeout(oArg);
        }
    }
}
