using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2017/2/3 21:24:45

namespace XMainClient
{
	using ArgDataType = KKSG.ChangeSkillSetArg;
    using ResDataType = KKSG.ChangeSkillSetRes;

    class RpcC2G_ChangeSkillSet : Rpc
    {
        public ArgDataType oArg = new ArgDataType();
        public ResDataType oRes = null;

        public RpcC2G_ChangeSkillSet()
        {
        }

        public override uint GetRpcType()
        {
            return 51116;
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
            Process_RpcC2G_ChangeSkillSet.OnReply(oArg, oRes);
        }

        public override void OnTimeout(object args)
        {
            Process_RpcC2G_ChangeSkillSet.OnTimeout(oArg);
        }
    }
}
