﻿using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/9/2 22:07:34

namespace XMainClient
{
    using DataType = KKSG.BossRushPara;

	class PtcG2C_BossRushOneFinishNtf : Protocol
	{
		public DataType Data = new DataType();

		public PtcG2C_BossRushOneFinishNtf()
		{

		}

		public override uint GetProtoType()
		{
			return 21034;
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
			Process_PtcG2C_BossRushOneFinishNtf.Process(this);
		}

	}
}
