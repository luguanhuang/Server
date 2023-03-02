﻿using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/10/9 11:19:18

namespace XMainClient
{
    using DataType = KKSG.QARoomRankNtf;

	class PtcM2C_QARoomRankNtf : Protocol
	{
		public DataType Data = new DataType();

		public PtcM2C_QARoomRankNtf()
		{

		}

		public override uint GetProtoType()
		{
			return 36888;
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
			Process_PtcM2C_QARoomRankNtf.Process(this);
		}

	}
}
