using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/7/28 23:00:49

namespace XMainClient
{
	using ArgDataType = KKSG.DoAddFriendArg;
    using ResDataType = KKSG.DoAddFriendRes;

    class RpcC2M_DoAddFriendNew : Rpc
    {
        public ArgDataType oArg = new ArgDataType();
        public ResDataType oRes = new ResDataType();

        public RpcC2M_DoAddFriendNew()
        {
        }

        public override uint GetRpcType()
        {
            return 23397;
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
            Process_RpcC2M_DoAddFriendNew.OnReply(oArg, oRes);
        }

        public override void OnTimeout(object args)
        {
            Process_RpcC2M_DoAddFriendNew.OnTimeout(oArg);
        }
    }
}
