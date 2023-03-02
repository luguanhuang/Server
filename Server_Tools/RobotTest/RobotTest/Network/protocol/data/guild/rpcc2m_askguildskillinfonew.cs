using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/9/8 16:31:16

namespace XMainClient
{
	using ArgDataType = KKSG.AskGuildSkillInfoArg;
    using ResDataType = KKSG.AskGuildSkillInfoReq;

    class RpcC2M_AskGuildSkillInfoNew : Rpc
    {
        public ArgDataType oArg = new ArgDataType();
        public ResDataType oRes = new ResDataType();

        public RpcC2M_AskGuildSkillInfoNew()
        {
        }

        public override uint GetRpcType()
        {
            return 35479;
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
            Process_RpcC2M_AskGuildSkillInfoNew.OnReply(oArg, oRes);
        }

        public override void OnTimeout(object args)
        {
            Process_RpcC2M_AskGuildSkillInfoNew.OnTimeout(oArg);
        }
    }
}
