using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/6/25 15:00:05

namespace XMainClient
{
	using ArgDataType = KKSG.FriendGiftOpArg;
    using ResDataType = KKSG.FriendGiftOpRes;

    class RpcC2G_FriendGiftOp : Rpc
    {
        public ArgDataType oArg = new ArgDataType();
        public ResDataType oRes = new ResDataType();

        public RpcC2G_FriendGiftOp()
        {
        }

        public override uint GetRpcType()
        {
            return 13403;
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
            Process_RpcC2G_FriendGiftOp.OnReply(oArg, oRes);
        }

        public override void OnTimeout(object args)
        {
            Process_RpcC2G_FriendGiftOp.OnTimeout(oArg);
        }
    }
}
