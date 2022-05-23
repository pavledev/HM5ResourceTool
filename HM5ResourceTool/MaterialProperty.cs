using System;
using System.Linq;
using System.Collections.Generic;
using System.Collections.ObjectModel;

namespace HM5ResourceTool
{
	internal class MaterialProperty
	{
		public string? Name { get; set; }
		public string? Type { get; set; }
		public string? Value { get; set; }
		public List<MaterialProperty>? Children { get; set; }
	}
}
