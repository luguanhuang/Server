using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2015/11/2 11:26:44

namespace XMainClient
{
    using DataType = KKSG.PositionCheckList;

	class PtcG2C_PositionDebug : Protocol
	{
		public DataType Data = new DataType();

		public PtcG2C_PositionDebug()
		{

		}

		public override uint GetProtoType()
		{
			return 42493;
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
			Process_PtcG2C_PositionDebug.Process(this);
		}

	}
}
