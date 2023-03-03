﻿using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/4/20 11:58:35

namespace XMainClient
{
    using DataType = KKSG.NewBattleResult;

	class PtcG2C_BattleResultNtf : Protocol
	{
		public DataType Data = new DataType();

		public PtcG2C_BattleResultNtf()
		{

		}

		public override uint GetProtoType()
		{
			return 29609;
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
			Process_PtcG2C_BattleResultNtf.Process(this);
		}

	}
}