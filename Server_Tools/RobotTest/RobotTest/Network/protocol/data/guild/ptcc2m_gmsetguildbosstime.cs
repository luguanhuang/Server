﻿using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/8/5 17:27:08

namespace XMainClient
{
    using DataType = KKSG.gmguildbosstime;

	class PtcC2M_GMSetGuildBossTime : Protocol
	{
		public DataType Data = new DataType();

		public PtcC2M_GMSetGuildBossTime()
		{

		}

		public override uint GetProtoType()
		{
			return 62392;
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
