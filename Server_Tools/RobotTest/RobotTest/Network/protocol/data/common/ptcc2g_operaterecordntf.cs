using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/10/27 10:53:43

namespace XMainClient
{
    using DataType = KKSG.OperateRecord;

	class PtcC2G_OperateRecordNtf : Protocol
	{
		public DataType Data = new DataType();

		public PtcC2G_OperateRecordNtf()
		{

		}

		public override uint GetProtoType()
		{
			return 56173;
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
