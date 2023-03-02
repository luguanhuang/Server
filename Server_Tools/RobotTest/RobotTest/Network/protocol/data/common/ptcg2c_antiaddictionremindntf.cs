using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/12/6 19:34:15

namespace XMainClient
{
    using DataType = KKSG.AntiAddictionRemindInfo;

	class PtcG2C_AntiAddictionRemindNtf : Protocol
	{
		public DataType Data = new DataType();

		public PtcG2C_AntiAddictionRemindNtf()
		{

		}

		public override uint GetProtoType()
		{
			return 17999;
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
			Process_PtcG2C_AntiAddictionRemindNtf.Process(this);
		}

	}
}
