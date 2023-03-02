using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/7/20 17:34:01

namespace XMainClient
{
    using DataType = KKSG.MulActivityCha;

	class PtcG2C_MulActivityStateChange : Protocol
	{
		public DataType Data = new DataType();

		public PtcG2C_MulActivityStateChange()
		{

		}

		public override uint GetProtoType()
		{
			return 13448;
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
			Process_PtcG2C_MulActivityStateChange.Process(this);
		}

	}
}
