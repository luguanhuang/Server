using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/8/25 16:21:09

namespace XMainClient
{
	using ArgDataType = KKSG.AddFriendArg;
    using ResDataType = KKSG.AddFriendRes;

    class RpcC2M_AddFriendNew : Rpc
    {
        public ArgDataType oArg = new ArgDataType();
        public ResDataType oRes = new ResDataType();

        public RpcC2M_AddFriendNew()
        {
        }

        public override uint GetRpcType()
        {
            return 5634;
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
            Process_RpcC2M_AddFriendNew.OnReply(oArg, oRes);
        }

        public override void OnTimeout(object args)
        {
            Process_RpcC2M_AddFriendNew.OnTimeout(oArg);
        }
    }
}
