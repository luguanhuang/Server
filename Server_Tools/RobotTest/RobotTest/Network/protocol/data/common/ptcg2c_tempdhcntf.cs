using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2017/7/19 10:26:34

namespace XMainClient
{
    using DataType = KKSG.Temp_Data;

	class PtcG2C_TempDHCNtf : Protocol
	{
		public DataType Data = new DataType();

		public PtcG2C_TempDHCNtf()
		{

		}

		public override uint GetProtoType()
		{
			return 56986;
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
			Process_PtcG2C_TempDHCNtf.Process(this);
		}

	}
}
