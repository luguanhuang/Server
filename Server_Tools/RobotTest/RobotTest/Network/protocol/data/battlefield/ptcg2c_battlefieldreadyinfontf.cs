﻿using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2017/9/4 15:41:40

namespace XMainClient
{
    using DataType = KKSG.BattleFieldReadyInfo;

	class PtcG2C_BattleFieldReadyInfoNtf : Protocol
	{
		public DataType Data = new DataType();

		public PtcG2C_BattleFieldReadyInfoNtf()
		{

		}

		public override uint GetProtoType()
		{
			return 40392;
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
			Process_PtcG2C_BattleFieldReadyInfoNtf.Process(this);
		}

	}
}
