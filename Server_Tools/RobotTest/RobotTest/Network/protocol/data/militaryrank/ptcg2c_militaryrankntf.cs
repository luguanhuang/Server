using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2017/4/11 16:08:13

namespace XMainClient
{
    using DataType = KKSG.MilitaryRecord;

	class PtcG2C_MilitaryrankNtf : Protocol
	{
		public DataType Data = new DataType();

		public PtcG2C_MilitaryrankNtf()
		{

		}

		public override uint GetProtoType()
		{
			return 64945;
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
			Process_PtcG2C_MilitaryrankNtf.Process(this);
		}

	}
}
