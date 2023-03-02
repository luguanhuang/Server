using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/12/26 17:41:19

namespace XMainClient
{
    using DataType = KKSG.ResWarEnemyTime;

	class PtcM2C_ResWarEnemyTimeNtf : Protocol
	{
		public DataType Data = new DataType();

		public PtcM2C_ResWarEnemyTimeNtf()
		{

		}

		public override uint GetProtoType()
		{
			return 48125;
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
			Process_PtcM2C_ResWarEnemyTimeNtf.Process(this);
		}

	}
}
