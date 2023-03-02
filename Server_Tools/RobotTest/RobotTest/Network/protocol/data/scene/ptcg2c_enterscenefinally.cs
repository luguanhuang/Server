using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2015/11/30 21:07:17

namespace XMainClient
{
    using DataType = KKSG.EnterSceneFinally;

	class PtcG2C_EnterSceneFinally : Protocol
	{
		public DataType Data = new DataType();

		public PtcG2C_EnterSceneFinally()
		{

		}

		public override uint GetProtoType()
		{
			return 15108;
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
			Process_PtcG2C_EnterSceneFinally.Process(this);
		}

	}
}
