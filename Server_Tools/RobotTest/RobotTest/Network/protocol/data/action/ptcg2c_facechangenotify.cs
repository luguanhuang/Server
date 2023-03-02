using System;
using System.Collections.Generic;

using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2014/12/17 10:41:06

namespace XMainClient
{
    using DataType = KKSG.LocationInfo;

	class PtcG2C_FaceChangeNotify : Protocol
	{
		public DataType Data = new DataType();

		public PtcG2C_FaceChangeNotify()
		{

		}

		public override uint GetProtoType()
		{
			return 51791;
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
			Process_PtcG2C_FaceChangeNotify.Process(this);
		}

	}
}
