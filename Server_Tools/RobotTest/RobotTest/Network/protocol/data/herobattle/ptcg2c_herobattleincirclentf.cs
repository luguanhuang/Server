using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/12/17 13:15:14

namespace XMainClient
{
    using DataType = KKSG.HeroBattleInCircle;

	class PtcG2C_HeroBattleInCircleNtf : Protocol
	{
		public DataType Data = new DataType();

		public PtcG2C_HeroBattleInCircleNtf()
		{

		}

		public override uint GetProtoType()
		{
			return 40409;
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
			Process_PtcG2C_HeroBattleInCircleNtf.Process(this);
		}

	}
}
