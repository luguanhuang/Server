using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2015/6/16 15:59:12

namespace XMainClient
{
    using DataType = KKSG.FashionChanged;

	class PtcG2C_ChangeFashionNotify : Protocol
	{
		public DataType Data = new DataType();

		public PtcG2C_ChangeFashionNotify()
		{

		}

		public override uint GetProtoType()
		{
			return 1731;
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
			Process_PtcG2C_ChangeFashionNotify.Process(this);
		}

	}
}
