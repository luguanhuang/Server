using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2017/10/9 16:22:33

namespace XMainClient
{
    using DataType = KKSG.WordNotify;

	class PtcG2C_WordNotify : Protocol
	{
		public DataType Data = new DataType();

		public PtcG2C_WordNotify()
		{

		}

		public override uint GetProtoType()
		{
			return 34052;
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
			Process_PtcG2C_WordNotify.Process(this);
		}

	}
}
