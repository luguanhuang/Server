using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/11/24 20:19:24

namespace XMainClient
{
    using DataType = KKSG.LargeRoomRoleParam;

	class PtcM2C_LargeRoomRoleNtf : Protocol
	{
		public DataType Data = new DataType();

		public PtcM2C_LargeRoomRoleNtf()
		{

		}

		public override uint GetProtoType()
		{
			return 36333;
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
			Process_PtcM2C_LargeRoomRoleNtf.Process(this);
		}

	}
}
