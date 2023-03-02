using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2017/7/27 19:55:28

namespace XMainClient
{
	using ArgDataType = KKSG.GroupChatChangeNameC2S;
    using ResDataType = KKSG.GroupChatChangeNameS2C;

    class RpcC2M_GroupChatChangeName : Rpc
    {
        public ArgDataType oArg = new ArgDataType();
        public ResDataType oRes = null;

        public RpcC2M_GroupChatChangeName()
        {
        }

        public override uint GetRpcType()
        {
            return 44170;
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
            Process_RpcC2M_GroupChatChangeName.OnReply(oArg, oRes);
        }

        public override void OnTimeout(object args)
        {
            Process_RpcC2M_GroupChatChangeName.OnTimeout(oArg);
        }
    }
}
