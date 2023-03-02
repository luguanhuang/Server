using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2017/4/11 17:12:37

namespace XMainClient
{
    using DataType = KKSG.CloseSkyCraftEliNtf;

	class PtcM2C_CloseSkyCraftEliNtf : Protocol
	{
		public DataType Data = new DataType();

		public PtcM2C_CloseSkyCraftEliNtf()
		{

		}

		public override uint GetProtoType()
		{
			return 50236;
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
			Process_PtcM2C_CloseSkyCraftEliNtf.Process(this);
		}

	}
}
