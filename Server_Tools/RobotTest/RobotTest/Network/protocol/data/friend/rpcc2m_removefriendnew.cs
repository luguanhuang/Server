using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/7/28 23:01:40

namespace XMainClient
{
	using ArgDataType = KKSG.RemoveFriendArg;
    using ResDataType = KKSG.RemoveFriendRes;

    class RpcC2M_RemoveFriendNew : Rpc
    {
        public ArgDataType oArg = new ArgDataType();
        public ResDataType oRes = new ResDataType();

        public RpcC2M_RemoveFriendNew()
        {
        }

        public override uint GetRpcType()
        {
            return 2841;
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
            Process_RpcC2M_RemoveFriendNew.OnReply(oArg, oRes);
        }

        public override void OnTimeout(object args)
        {
            Process_RpcC2M_RemoveFriendNew.OnTimeout(oArg);
        }
    }
}
