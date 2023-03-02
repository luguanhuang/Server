using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2017/10/7 15:44:50

namespace XMainClient
{
    using DataType = KKSG.MulActivityIconSys;

	class PtcC2M_MulActivityIconSysReq : Protocol
	{
		public DataType Data = new DataType();

		public PtcC2M_MulActivityIconSysReq()
		{

		}

		public override uint GetProtoType()
		{
			return 64642;
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
