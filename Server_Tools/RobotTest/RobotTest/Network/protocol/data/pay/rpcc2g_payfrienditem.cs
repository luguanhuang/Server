using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2017/3/14 10:25:06

namespace XMainClient
{
	using ArgDataType = KKSG.PayFriendItemArg;
    using ResDataType = KKSG.PayFriendItemRes;

    class RpcC2G_PayFriendItem : Rpc
    {
        public ArgDataType oArg = new ArgDataType();
        public ResDataType oRes = null;

        public RpcC2G_PayFriendItem()
        {
        }

        public override uint GetRpcType()
        {
            return 29289;
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
            Process_RpcC2G_PayFriendItem.OnReply(oArg, oRes);
        }

        public override void OnTimeout(object args)
        {
            Process_RpcC2G_PayFriendItem.OnTimeout(oArg);
        }
    }
}
