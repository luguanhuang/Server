using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2017/7/13 11:45:10

namespace XMainClient
{
	using ArgDataType = KKSG.GroupChatPlayerApplyC2S;
    using ResDataType = KKSG.GroupChatPlayerApplyS2C;

    class RpcC2M_GroupChatPlayerApply : Rpc
    {
        public ArgDataType oArg = new ArgDataType();
        public ResDataType oRes = null;

        public RpcC2M_GroupChatPlayerApply()
        {
        }

        public override uint GetRpcType()
        {
            return 24788;
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
            Process_RpcC2M_GroupChatPlayerApply.OnReply(oArg, oRes);
        }

        public override void OnTimeout(object args)
        {
            Process_RpcC2M_GroupChatPlayerApply.OnTimeout(oArg);
        }
    }
}
