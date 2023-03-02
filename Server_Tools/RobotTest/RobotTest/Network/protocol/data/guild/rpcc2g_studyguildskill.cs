﻿using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/7/3 15:49:02

namespace XMainClient
{
	using ArgDataType = KKSG.StudyGuildSkillArg;
    using ResDataType = KKSG.StudyGuildSkillRes;

    class RpcC2G_StudyGuildSkill : Rpc
    {
        public ArgDataType oArg = new ArgDataType();
        public ResDataType oRes = new ResDataType();

        public RpcC2G_StudyGuildSkill()
        {
        }

        public override uint GetRpcType()
        {
            return 25461;
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
            Process_RpcC2G_StudyGuildSkill.OnReply(oArg, oRes);
        }

        public override void OnTimeout(object args)
        {
            Process_RpcC2G_StudyGuildSkill.OnTimeout(oArg);
        }
    }
}
