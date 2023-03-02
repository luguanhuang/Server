using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/11/24 20:44:07

namespace XMainClient
{
    using DataType = KKSG.LargeRoomLoginParam;

	class PtcM2C_LargeRoomLoginParamNtf : Protocol
	{
		public DataType Data = new DataType();

		public PtcM2C_LargeRoomLoginParamNtf()
		{

		}

		public override uint GetProtoType()
		{
			return 51856;
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
			Process_PtcM2C_LargeRoomLoginParamNtf.Process(this);
		}

	}
}
