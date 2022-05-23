#pragma once

#include <msclr\marshal_cppstd.h>
#include "ManagedObject.hpp"
#include "../../HM5ResourceToolLibrary/include/SEntityTemplate.hpp"

using namespace System::Collections::Generic;
using namespace System::Runtime::InteropServices;
using namespace msclr::interop;

namespace HM5ResourceToolCLI
{
	public ref struct SEntityTemplate : public ManagedObject<HM5ResourceTool::SEntityTemplate>
	{
		ref struct SSubEntityTemplate
		{
			//List<SSubEntityTemplate^>^ children;
			HM5ResourceTool::SEntityTemplate::SSubEntityTemplate* subEntityTemplate;

			SSubEntityTemplate(HM5ResourceTool::SEntityTemplate::SSubEntityTemplate* subEntityTemplate);
			int GetIndex();
			String^ GetEntityName();
			List<SSubEntityTemplate^>^ GetChildren();
		};

		//List<SSubEntityTemplate^>^ entityTemplates;

		SEntityTemplate(HM5ResourceTool::SEntityTemplate* entityTemplate);
		SSubEntityTemplate^ GetRootEntityTemplate();
		String^ GetEntityDetails(int index);
		String^ GetEntityName(int index);
		String^ GetTemplateEntityResourceID(int index);
		String^ GetTemplateEntityBlueprintResourceID(int index);
		List<int>^ SearchContent(String^ content, bool exactMatch);
		Dictionary<String^, Dictionary<String^, List<String^>^>^>^ GetPropertyTypesAndValues(int idnex);
		Dictionary<String^, Dictionary<String^, List<String^>^>^>^ GetPostInitPropertyTypesAndValues(int idnex);
		List<String^>^ GetEntityReferences(int index);
		List<String^>^ GetEntityBackReferences(int index);
		List<String^>^ GetPropertyAliases(int index);
	};
}
