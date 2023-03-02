using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/12/9 22:40:44

namespace XMainClient
{
	using ArgDataType = KKSG.MentorMyBeAppliedMsgArg;
    using ResDataType = KKSG.MentorMyBeAppliedMsgRes;

    class RpcC2M_MentorMyBeAppliedMsg : Rpc
    {
        public ArgDataType oArg = new ArgDataType();
        public ResDataType oRes = null;

        public RpcC2M_MentorMyBeAppliedMsg()
        {
        }

        public override uint GetRpcType()
        {
            return 45205;
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
            Process_RpcC2M_MentorMyBeAppliedMsg.OnReply(oArg, oRes);
        }

        public override void OnTimeout(object args)
        {
            Process_RpcC2M_MentorMyBeAppliedMsg.OnTimeout(oArg);
        }
    }
}
