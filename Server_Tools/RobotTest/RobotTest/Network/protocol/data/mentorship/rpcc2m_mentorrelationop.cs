using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/12/9 22:57:42

namespace XMainClient
{
	using ArgDataType = KKSG.MentorRelationOpArg;
    using ResDataType = KKSG.MentorRelationOpRes;

    class RpcC2M_MentorRelationOp : Rpc
    {
        public ArgDataType oArg = new ArgDataType();
        public ResDataType oRes = null;

        public RpcC2M_MentorRelationOp()
        {
        }

        public override uint GetRpcType()
        {
            return 10644;
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
            Process_RpcC2M_MentorRelationOp.OnReply(oArg, oRes);
        }

        public override void OnTimeout(object args)
        {
            Process_RpcC2M_MentorRelationOp.OnTimeout(oArg);
        }
    }
}
