using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/10/28 22:22:56

namespace XMainClient
{
    using DataType = KKSG.ErrorInfo;

	class PtcT2C_ErrorNotify : Protocol
	{
		public DataType Data = new DataType();

		public PtcT2C_ErrorNotify()
		{

		}

		public override uint GetProtoType()
		{
			return 21940;
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
			Process_PtcT2C_ErrorNotify.Process(this);
		}

	}
}
