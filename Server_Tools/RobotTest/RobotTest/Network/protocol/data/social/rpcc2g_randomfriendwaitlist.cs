using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2015/9/21 17:02:00

namespace XMainClient
{
	using ArgDataType = KKSG.RandomFriendWaitListArg;
    using ResDataType = KKSG.RandomFriendWaitListRes;

    class RpcC2G_RandomFriendWaitList : Rpc
    {
        public ArgDataType oArg = new ArgDataType();
        public ResDataType oRes = new ResDataType();

        public RpcC2G_RandomFriendWaitList()
        {
        }

        public override uint GetRpcType()
        {
            return 40431;
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
            Process_RpcC2G_RandomFriendWaitList.OnReply(oArg, oRes);
        }

        public override void OnTimeout(object args)
        {
            Process_RpcC2G_RandomFriendWaitList.OnTimeout(oArg);
        }
    }
}
