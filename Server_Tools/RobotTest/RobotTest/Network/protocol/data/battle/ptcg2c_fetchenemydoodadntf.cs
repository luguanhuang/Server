using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2015/12/2 14:51:35

namespace XMainClient
{
    using DataType = KKSG.OtherFetchDoodadRes;

	class PtcG2C_FetchEnemyDoodadNtf : Protocol
	{
		public DataType Data = new DataType();

		public PtcG2C_FetchEnemyDoodadNtf()
		{

		}

		public override uint GetProtoType()
		{
			return 50480;
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
			Process_PtcG2C_FetchEnemyDoodadNtf.Process(this);
		}

	}
}
