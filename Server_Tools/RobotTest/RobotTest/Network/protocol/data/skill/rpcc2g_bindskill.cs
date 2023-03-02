using System;
using System.Collections.Generic;

using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2015/4/14 16:22:17

namespace XMainClient
{
	using ArgDataType = KKSG.BingSkillArg;
    using ResDataType = KKSG.BindSkillRes;

    class RpcC2G_BindSkill : Rpc
    {
        public ArgDataType oArg = new ArgDataType();
        public ResDataType oRes = new ResDataType();

        public RpcC2G_BindSkill()
        {
        }

        public override uint GetRpcType()
        {
            return 48236;
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
            Process_RpcC2G_BindSkill.OnReply(oArg, oRes);
        }

        public override void OnTimeout(object args)
        {
            Process_RpcC2G_BindSkill.OnTimeout(oArg);
        }
    }
}
