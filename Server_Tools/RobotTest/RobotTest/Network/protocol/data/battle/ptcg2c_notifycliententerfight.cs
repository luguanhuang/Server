using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/7/25 16:29:24

namespace XMainClient
{
    using DataType = KKSG.NotifyEnemyEnterFight;

	class PtcG2C_NotifyClientEnterFight : Protocol
	{
		public DataType Data = new DataType();

		public PtcG2C_NotifyClientEnterFight()
		{

		}

		public override uint GetProtoType()
		{
			return 65191;
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
			Process_PtcG2C_NotifyClientEnterFight.Process(this);
		}

	}
}
