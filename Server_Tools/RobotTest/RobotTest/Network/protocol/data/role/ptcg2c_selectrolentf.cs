using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/7/10 13:44:02

namespace XMainClient
{
    using DataType = KKSG.SelectRoleNtfData;

	class PtcG2C_SelectRoleNtf : Protocol
	{
		public DataType Data = new DataType();

		public PtcG2C_SelectRoleNtf()
		{

		}

		public override uint GetProtoType()
		{
			return 19493;
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
			Process_PtcG2C_SelectRoleNtf.Process(this);
		}

	}
}
