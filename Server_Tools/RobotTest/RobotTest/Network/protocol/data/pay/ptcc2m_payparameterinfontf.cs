using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/12/19 22:30:55

namespace XMainClient
{
    using DataType = KKSG.PayParameterInfo;

	class PtcC2M_PayParameterInfoNtf : Protocol
	{
		public DataType Data = new DataType();

		public PtcC2M_PayParameterInfoNtf()
		{

		}

		public override uint GetProtoType()
		{
			return 1181;
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
