using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/12/14 21:27:09

namespace XMainClient
{
    using DataType = KKSG.DoodadItemAddNtf;

	class PtcG2C_DoodadItemAddNtf : Protocol
	{
		public DataType Data = new DataType();

		public PtcG2C_DoodadItemAddNtf()
		{

		}

		public override uint GetProtoType()
		{
			return 16613;
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
			Process_PtcG2C_DoodadItemAddNtf.Process(this);
		}

	}
}
