using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/12/19 17:28:19

namespace XMainClient
{
    using DataType = KKSG.HeroBattleOverTimeData;

	class PtcG2C_HeroBattleOverTime : Protocol
	{
		public DataType Data = new DataType();

		public PtcG2C_HeroBattleOverTime()
		{

		}

		public override uint GetProtoType()
		{
			return 2950;
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
			Process_PtcG2C_HeroBattleOverTime.Process(this);
		}

	}
}
