﻿using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2015/12/16 11:08:35

namespace XMainClient
{
    using DataType = KKSG.SkillResultReqUnit;

	class PtcC2G_SkillResultReq : Protocol
	{
		public DataType Data = new DataType();

		public PtcC2G_SkillResultReq()
		{
			Data.ResultAt = new KKSG.Vec3();
			Data.Pos = new KKSG.Vec3();

		}

		public override uint GetProtoType()
		{
			return 41958;
		}

		public override void Serialize(MemoryStream stream)
		{
			Serializer.Serialize(stream, Data);
		}

		public override void DeSerialize(MemoryStream stream)
		{
			Data = Serializer.Deserialize<DataType>(stream);
		}

		public override void Process()
		{
			throw new Exception("Send only protocol can not call process");
		}

	}
}
