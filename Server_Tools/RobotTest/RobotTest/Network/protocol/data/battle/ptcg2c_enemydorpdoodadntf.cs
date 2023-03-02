using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2015/11/25 11:40:41

namespace XMainClient
{
    using DataType = KKSG.EnemyDropDoodadInfo;

	class PtcG2C_EnemyDorpDoodadNtf : Protocol
	{
		public DataType Data = new DataType();

		public PtcG2C_EnemyDorpDoodadNtf()
		{

		}

		public override uint GetProtoType()
		{
			return 55996;
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
			Process_PtcG2C_EnemyDorpDoodadNtf.Process(this);
		}

	}
}
