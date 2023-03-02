﻿using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/11/17 19:42:25

namespace XMainClient
{
    using DataType = KKSG.NoticeGuildInheritExp;

	class PtcM2C_NoticeGuildInheritExp : Protocol
	{
		public DataType Data = new DataType();

		public PtcM2C_NoticeGuildInheritExp()
		{

		}

		public override uint GetProtoType()
		{
			return 52440;
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
			Process_PtcM2C_NoticeGuildInheritExp.Process(this);
		}

	}
}
