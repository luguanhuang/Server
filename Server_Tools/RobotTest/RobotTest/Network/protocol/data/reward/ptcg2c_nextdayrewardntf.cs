﻿using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2015/10/20 18:37:39

namespace XMainClient
{
    using DataType = KKSG.NextDayRewardNtf;

	class PtcG2C_NextDayRewardNtf : Protocol
	{
		public DataType Data = new DataType();

		public PtcG2C_NextDayRewardNtf()
		{

		}

		public override uint GetProtoType()
		{
			return 50036;
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
			Process_PtcG2C_NextDayRewardNtf.Process(this);
		}

	}
}
