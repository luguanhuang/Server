using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/10/24 10:24:28

namespace XMainClient
{
    using DataType = KKSG.SpActivityState;

	class PtcG2C_UpdateSpActivityState : Protocol
	{
		public DataType Data = new DataType();

		public PtcG2C_UpdateSpActivityState()
		{

		}

		public override uint GetProtoType()
		{
			return 48406;
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
			Process_PtcG2C_UpdateSpActivityState.Process(this);
		}

	}
}
