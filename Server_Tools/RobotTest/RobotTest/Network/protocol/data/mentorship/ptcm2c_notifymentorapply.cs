using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/12/9 23:10:39

namespace XMainClient
{
    using DataType = KKSG.NotifyMentorApplyData;

	class PtcM2C_NotifyMentorApply : Protocol
	{
		public DataType Data = new DataType();

		public PtcM2C_NotifyMentorApply()
		{

		}

		public override uint GetProtoType()
		{
			return 61023;
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
			Process_PtcM2C_NotifyMentorApply.Process(this);
		}

	}
}
