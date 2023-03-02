using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/10/28 16:43:47

namespace XMainClient
{
    using DataType = KKSG.SpActivityOffsetDay;

	class PtcG2C_SpActivityOffsetDayNtf : Protocol
	{
		public DataType Data = new DataType();

		public PtcG2C_SpActivityOffsetDayNtf()
		{

		}

		public override uint GetProtoType()
		{
			return 4059;
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
			Process_PtcG2C_SpActivityOffsetDayNtf.Process(this);
		}

	}
}
