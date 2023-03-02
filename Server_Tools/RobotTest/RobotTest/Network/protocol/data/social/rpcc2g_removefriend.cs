using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2015/9/18 16:13:58

namespace XMainClient
{
	using ArgDataType = KKSG.RemoveFriendArg;
    using ResDataType = KKSG.RemoveFriendRes;

    class RpcC2G_RemoveFriend : Rpc
    {
        public ArgDataType oArg = new ArgDataType();
        public ResDataType oRes = new ResDataType();

        public RpcC2G_RemoveFriend()
        {
        }

        public override uint GetRpcType()
        {
            return 8143;
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
            Process_RpcC2G_RemoveFriend.OnReply(oArg, oRes);
        }

        public override void OnTimeout(object args)
        {
            Process_RpcC2G_RemoveFriend.OnTimeout(oArg);
        }
    }
}
