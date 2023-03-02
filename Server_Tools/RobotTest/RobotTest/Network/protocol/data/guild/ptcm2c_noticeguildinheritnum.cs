using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/11/19 10:47:43

namespace XMainClient
{
    using DataType = KKSG.NoticeGuildInheritNum;

	class PtcM2C_NoticeGuildInheritNum : Protocol
	{
		public DataType Data = new DataType();

		public PtcM2C_NoticeGuildInheritNum()
		{

		}

		public override uint GetProtoType()
		{
			return 51650;
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
			Process_PtcM2C_NoticeGuildInheritNum.Process(this);
		}

	}
}
