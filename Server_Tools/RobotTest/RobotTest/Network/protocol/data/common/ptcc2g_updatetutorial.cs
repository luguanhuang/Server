using System;
using System.Collections.Generic;

using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2015/2/2 14:42:56

namespace XMainClient
{
    using DataType = KKSG.TutorialInfo;

	class PtcC2G_UpdateTutorial : Protocol
	{
		public DataType Data = new DataType();

		public PtcC2G_UpdateTutorial()
		{

		}

		public override uint GetProtoType()
		{
			return 31917;
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
			throw new Exception("Send only protocol can not call process");
		}

	}
}
