using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/3/12 17:47:16

namespace XMainClient
{
    using DataType = KKSG.TeleportNoticeState;

	class PtcG2C_TeleportNotice : Protocol
	{
		public DataType Data = new DataType();

		public PtcG2C_TeleportNotice()
		{

		}

		public override uint GetProtoType()
		{
			return 27305;
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
			Process_PtcG2C_TeleportNotice.Process(this);
		}

	}
}
