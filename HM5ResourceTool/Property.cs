using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using HM5ResourceToolCLI;

namespace HM5ResourceTool
{
    public class Enum
    {
        public string SelectedName { get; set; }
        public int SelectedValue { get; set; }
        public List<string> Values { get; set; }

        public Enum(string name, int value, List<string> values)
        {
            SelectedName = name;
            SelectedValue = value;
            Values = values;
        }
    }

    public class BaseType
    {
        public string Text { get; set; }

        public BaseType(string text)
        {
            Text = text;
        }
    }

    public class SVector2
    {
        public string X { get; set; }
        public string Y { get; set; }

        public SVector2(string x, string y)
        {
            X = x;
            Y = y;
        }
    }

    public class SVector3
    {
        public string X { get; set; }
        public string Y { get; set; }
        public string Z { get; set; }

        public SVector3(string x, string y, string z)
        {
            X = x;
            Y = y;
            Z = z;
        }
    }

    public class SColorRGB
    {
        public string R { get; set; }
        public string G { get; set; }
        public string B { get; set; }

        public SColorRGB(string r, string g, string b)
        {
            R = r;
            G = g;
            B = b;
        }
    }

    public class SColorRGBA
    {
        public string R { get; set; }
        public string G { get; set; }
        public string B { get; set; }
        public string A { get; set; }

        public SColorRGBA(string r, string g, string b, string a)
        {
            R = r;
            G = g;
            B = b;
            A = a;
        }
    }

    public class SMatrix43
    {
        public SVector3 Rotation { get; set; }
        public SVector3 Position { get; set; }

        public SMatrix43(SVector3 rotation, SVector3 position)
        {
            Rotation = rotation;
            Position = position;
        }
    }

    public class SEntityTemplateReference
    {
        public string EntityIndex { get; set; }
        public string EntityName { get; set; }
        public string ExposedEntity { get; set; }

        public SEntityTemplateReference(string entityIndex, string entityName, string exposedEntity)
        {
            EntityIndex = entityIndex;
            EntityName = entityName;
            ExposedEntity = exposedEntity;
        }
    }

    public class SBodyPartDamageMultipliers
    {
        float m_fHeadDamageMultiplier;
        float m_fFaceDamageMultiplier;
        float m_fArmDamageMultiplier;
        float m_fHandDamageMultiplier;
        float m_fLegDamageMultiplier;
        float m_fTorsoDamageMultiplier;
    }

    public class ZRuntimeResourceID
    {
        public string ResourceID { get; set; }
        public string Flags { get; set; }

        public ZRuntimeResourceID(string resourceID, string flags)
        {
            ResourceID = resourceID;
            Flags = flags;
        }
    }

    public class SEntityTemplatePropertyAlias
    {
        public string AliasName { get; set; }
        public string EntityName { get; set; }
        public string PropertyName { get; set; }

        public SEntityTemplatePropertyAlias(string aliasName, string entityName, string propertyName)
        {
            AliasName = aliasName;
            EntityName = entityName;
            PropertyName = propertyName;
        }
    }

    internal class Property
    {
        public string? Name { get; set; }
        public object? Value { get; set; }
        public string? HeaderType { get; set; }
        public string? TypeName { get; set; }
    }
}
