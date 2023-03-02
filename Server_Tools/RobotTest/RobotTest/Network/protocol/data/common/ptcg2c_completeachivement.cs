using System;
using System.Collections.Generic;

using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2015/2/4 19:48:20

namespace XMainClient
{
    using DataType = KKSG.AchivementInfo;

	class PtcG2C_CompleteAchivement : Protocol
	{
		public DataType Data = new DataType();

		public PtcG2C_CompleteAchivement()
		{

		}

		public override uint GetProtoType()
		{
			return 26346;
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
			Process_PtcG2C_CompleteAchivement.Process(this);
		}

	}
}
