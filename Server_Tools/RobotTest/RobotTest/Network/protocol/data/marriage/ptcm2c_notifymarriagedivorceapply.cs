using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2017/7/27 10:19:04

namespace XMainClient
{
    using DataType = KKSG.NotifyMarriageDivorceApplyData;

	class PtcM2C_NotifyMarriageDivorceApply : Protocol
	{
		public DataType Data = new DataType();

		public PtcM2C_NotifyMarriageDivorceApply()
		{

		}

		public override uint GetProtoType()
		{
			return 32886;
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
			Process_PtcM2C_NotifyMarriageDivorceApply.Process(this);
		}

	}
}
