using System;
using System.Collections.Generic;

using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2014/12/10 17:00:01

namespace XMainClient
{
    using DataType = KKSG.SkillCeaseInfo;

	class PtcC2G_SkillCeaseReq : Protocol
	{
		public DataType Data = new DataType();

		public PtcC2G_SkillCeaseReq()
		{
			//Data.Position = new KKSG.Vec3();

		}

		public override uint GetProtoType()
		{
			return 52535;
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
