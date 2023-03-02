using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/11/27 19:46:14

namespace XMainClient
{
    using DataType = KKSG.ResWarTime;

	class PtcM2C_ResWarTimeNtf : Protocol
	{
		public DataType Data = new DataType();

		public PtcM2C_ResWarTimeNtf()
		{

		}

		public override uint GetProtoType()
		{
			return 36825;
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
			Process_PtcM2C_ResWarTimeNtf.Process(this);
		}

	}
}
