using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/11/29 16:26:51

namespace XMainClient
{
    using DataType = KKSG.FMSystem;

	class PtcM2C_FMSystemNtf : Protocol
	{
		public DataType Data = new DataType();

		public PtcM2C_FMSystemNtf()
		{

		}

		public override uint GetProtoType()
		{
			return 22361;
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
			Process_PtcM2C_FMSystemNtf.Process(this);
		}

	}
}
