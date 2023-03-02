using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/10/28 17:13:37

namespace XMainClient
{
    using DataType = KKSG.ResWarRankData;

	class PtcM2C_ResWarRankDataNtf : Protocol
	{
		public DataType Data = new DataType();

		public PtcM2C_ResWarRankDataNtf()
		{

		}

		public override uint GetProtoType()
		{
			return 6844;
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
			Process_PtcM2C_ResWarRankDataNtf.Process(this);
		}

	}
}
