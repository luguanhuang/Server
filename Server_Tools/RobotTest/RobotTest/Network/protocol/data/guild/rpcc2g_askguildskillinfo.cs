using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/7/2 20:07:10

namespace XMainClient
{
	using ArgDataType = KKSG.AskGuildSkillInfoArg;
    using ResDataType = KKSG.AskGuildSkillInfoReq;

    class RpcC2G_AskGuildSkillInfo : Rpc
    {
        public ArgDataType oArg = new ArgDataType();
        public ResDataType oRes = new ResDataType();

        public RpcC2G_AskGuildSkillInfo()
        {
        }

        public override uint GetRpcType()
        {
            return 50475;
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
            Process_RpcC2G_AskGuildSkillInfo.OnReply(oArg, oRes);
        }

        public override void OnTimeout(object args)
        {
            Process_RpcC2G_AskGuildSkillInfo.OnTimeout(oArg);
        }
    }
}
