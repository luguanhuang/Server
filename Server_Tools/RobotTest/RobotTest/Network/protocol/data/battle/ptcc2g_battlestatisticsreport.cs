﻿using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2017/7/25 20:35:42

namespace XMainClient
{
    //using DataType = KKSG.;

	class PtcC2G_BattleStatisticsReport : Protocol
	{
		//public DataType Data = new DataType();

		public PtcC2G_BattleStatisticsReport()
		{

		}

		public override uint GetProtoType()
		{
			return 3612;
		}

		public override void Serialize(MemoryStream stream)
		{
			//Serializer.Serialize(stream, Data);
		}

		public override void DeSerialize(MemoryStream stream)
		{
			//Data = Serializer.Deserialize<DataType>(stream);
		}

		public override void Process()
		{
			throw new Exception("Send only protocol can not call process");
		}

	}
}
