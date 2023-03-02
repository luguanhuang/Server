using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2017/8/1 18:15:41

namespace XMainClient
{
    using DataType = KKSG.ThemeActivityChangeData;

	class PtcG2C_ThemeActivityChangeNtf : Protocol
	{
		public DataType Data = new DataType();

		public PtcG2C_ThemeActivityChangeNtf()
		{

		}

		public override uint GetProtoType()
		{
			return 25642;
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
			Process_PtcG2C_ThemeActivityChangeNtf.Process(this);
		}

	}
}
