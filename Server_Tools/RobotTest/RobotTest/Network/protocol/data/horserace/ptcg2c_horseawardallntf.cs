using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/12/6 20:12:56

namespace XMainClient
{
    using DataType = KKSG.HorseAwardAll;

	class PtcG2C_HorseAwardAllNtf : Protocol
	{
		public DataType Data = new DataType();

		public PtcG2C_HorseAwardAllNtf()
		{

		}

		public override uint GetProtoType()
		{
			return 5990;
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
			Process_PtcG2C_HorseAwardAllNtf.Process(this);
		}

	}
}
