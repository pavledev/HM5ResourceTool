#pragma once

#include <string>
#include <vector>
#include "rapidjson/prettywriter.h"
#include "rapidjson/stringbuffer.h"

namespace IOI
{
	struct SRMaterialProperties;
}

namespace HM5ResourceTool
{
	namespace IO
	{
		class ZBinaryReader;
	}

	class ZRenderMaterialInstance
	{
	public:
		enum PROPERTY_TYPE
		{
			PT_FLOAT = 0,
			PT_CHAR = 1,
			PT_UINT32 = 2,
			PT_LIST = 3
		};

		struct SProperty
		{
			unsigned int lName;
			unsigned int lData;
			unsigned int lSize;
			unsigned int lType;
		};

		class ZProperty
		{
		public:
			__declspec(dllexport) ZProperty();
			__declspec(dllexport) ~ZProperty();
			__declspec(dllexport) SProperty* GetPropertyInfo();
			__declspec(dllexport) void* GetValue();
			__declspec(dllexport) unsigned int GetValueSize();
			__declspec(dllexport) std::vector<ZProperty*>* GetChildProperties();
			const SProperty* GetPropertyInfo() const;
			const void* GetValue() const;
			const std::vector<ZProperty*>* GetChildProperties() const;
			void SetPropertyInfo(SProperty* propertyInfo);
			void SetValue(void* value);
			void SetValueSize(unsigned int valueSize);
			void AddChildProperty(ZProperty* childProperty);

		private:
			SProperty* propertyInfo;
			void* value;
			unsigned int valueSize;
			std::vector<ZProperty*> childProperties;
		};

		__declspec(dllexport) ZRenderMaterialInstance();
		__declspec(dllexport) ~ZRenderMaterialInstance();
		__declspec(dllexport) IOI::SRMaterialProperties* GetMaterialInfo();
		__declspec(dllexport) ZProperty* GetInstanceProperty();
		const IOI::SRMaterialProperties* GetMaterialInfo() const;
		const ZProperty* GetInstanceProperty() const;
		__declspec(dllexport) void Deserialize(const std::string& filePath);
		__declspec(dllexport) void Deserialize(const void* buffer, const unsigned int dataSize);
		void Deserialize(IO::ZBinaryReader& binaryReader);
		std::string SerializeToJSON();
		void SerializeProperty(ZProperty* property, rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer);
		void ReadProperty(ZProperty* property, IO::ZBinaryReader& binaryReader, unsigned int propertyOffset);

	private:
		IOI::SRMaterialProperties* materialInfo;
		ZProperty* instanceProperty;
	};
}
