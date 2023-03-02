﻿using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/3/30 15:13:26

namespace XMainClient
{
    using DataType = KKSG.TeamFullDataNtf;

	class PtcG2C_TeamFullDataNtf : Protocol
	{
		public DataType Data = new DataType();

		public PtcG2C_TeamFullDataNtf()
		{

		}

		public override uint GetProtoType()
		{
			return 48618;
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
			Process_PtcG2C_TeamFullDataNtf.Process(this);
		}

	}
}
