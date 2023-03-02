using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/9/12 11:37:12

namespace XMainClient
{
	using ArgDataType = KKSG.LearnGuildSkillAgr;
    using ResDataType = KKSG.LearnGuildSkillRes;

    class RpcC2G_LearnGuildSkill : Rpc
    {
        public ArgDataType oArg = new ArgDataType();
        public ResDataType oRes = new ResDataType();

        public RpcC2G_LearnGuildSkill()
        {
        }

        public override uint GetRpcType()
        {
            return 62806;
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
            Process_RpcC2G_LearnGuildSkill.OnReply(oArg, oRes);
        }

        public override void OnTimeout(object args)
        {
            Process_RpcC2G_LearnGuildSkill.OnTimeout(oArg);
        }
    }
}
