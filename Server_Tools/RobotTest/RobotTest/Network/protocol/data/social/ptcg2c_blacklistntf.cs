using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2015/9/22 13:20:46

namespace XMainClient
{
    using DataType = KKSG.BlackListNtf;

	class PtcG2C_BlackListNtf : Protocol
	{
		public DataType Data = new DataType();

		public PtcG2C_BlackListNtf()
		{

		}

		public override uint GetProtoType()
		{
			return 15793;
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
			Process_PtcG2C_BlackListNtf.Process(this);
		}

	}
}
