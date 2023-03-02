using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/9/19 10:27:06

namespace XMainClient
{
    using DataType = KKSG.IdipMessage;

	class PtcM2C_NotifyIdipMessageMs : Protocol
	{
		public DataType Data = new DataType();

		public PtcM2C_NotifyIdipMessageMs()
		{

		}

		public override uint GetProtoType()
		{
			return 48558;
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
			Process_PtcM2C_NotifyIdipMessageMs.Process(this);
		}

	}
}
