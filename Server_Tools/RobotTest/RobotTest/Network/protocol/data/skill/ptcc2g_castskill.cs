using System;
using System.Collections.Generic;

using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2014/12/3 19:53:01

namespace XMainClient
{
    using DataType = KKSG.SkillDataUnit;

	class PtcC2G_CastSkill : Protocol
	{
		public DataType Data = new DataType();

		public PtcC2G_CastSkill()
		{
			//Data.Position = new KKSG.Vec3();

		}

		public override uint GetProtoType()
		{
			return 49584;
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
