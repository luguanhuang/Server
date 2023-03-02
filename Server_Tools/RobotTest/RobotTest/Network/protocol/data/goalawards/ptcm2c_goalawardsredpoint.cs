using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2017/9/1 19:48:09

namespace XMainClient
{
    using DataType = KKSG.GoalAwardsRedPointNtf;

	class PtcM2C_GoalAwardsRedPoint : Protocol
	{
		public DataType Data = new DataType();

		public PtcM2C_GoalAwardsRedPoint()
		{

		}

		public override uint GetProtoType()
		{
			return 11570;
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
			Process_PtcM2C_GoalAwardsRedPoint.Process(this);
		}

	}
}
