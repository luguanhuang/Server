﻿using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/12/25 18:48:34

namespace XMainClient
{
    using DataType = KKSG.HeroBattleData;

	class PtcG2C_HeroBattleDataNtf : Protocol
	{
		public DataType Data = new DataType();

		public PtcG2C_HeroBattleDataNtf()
		{

		}

		public override uint GetProtoType()
		{
			return 60769;
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
			Process_PtcG2C_HeroBattleDataNtf.Process(this);
		}

	}
}
