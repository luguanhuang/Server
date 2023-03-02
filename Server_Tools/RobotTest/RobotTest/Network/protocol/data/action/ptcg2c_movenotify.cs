using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2015/12/16 14:56:29

namespace XMainClient
{
    using DataType = KKSG.MoveInfo;

	class PtcG2C_MoveNotify : Protocol
	{
		public DataType Data = new DataType();

		public PtcG2C_MoveNotify()
		{

		}

		public override uint GetProtoType()
		{
			return 33850;
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
			Process_PtcG2C_MoveNotify.Process(this);
		}

	}
}
