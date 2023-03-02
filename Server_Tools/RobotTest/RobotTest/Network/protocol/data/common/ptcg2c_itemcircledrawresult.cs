using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2015/12/19 16:27:22

namespace XMainClient
{
    using DataType = KKSG.CircleDrawGive;

	class PtcG2C_ItemCircleDrawResult : Protocol
	{
		public DataType Data = new DataType();

		public PtcG2C_ItemCircleDrawResult()
		{

		}

		public override uint GetProtoType()
		{
			return 34574;
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
			Process_PtcG2C_ItemCircleDrawResult.Process(this);
		}

	}
}
