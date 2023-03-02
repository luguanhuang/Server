using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/9/8 16:42:42

namespace XMainClient
{
	using ArgDataType = KKSG.StudyGuildSkillArg;
    using ResDataType = KKSG.StudyGuildSkillRes;

    class RpcC2M_StudyGuildSkillNew : Rpc
    {
        public ArgDataType oArg = new ArgDataType();
        public ResDataType oRes = new ResDataType();

        public RpcC2M_StudyGuildSkillNew()
        {
        }

        public override uint GetRpcType()
        {
            return 45669;
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
            Process_RpcC2M_StudyGuildSkillNew.OnReply(oArg, oRes);
        }

        public override void OnTimeout(object args)
        {
            Process_RpcC2M_StudyGuildSkillNew.OnTimeout(oArg);
        }
    }
}
