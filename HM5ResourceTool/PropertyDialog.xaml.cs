using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Shapes;
using MahApps.Metro.Controls;

namespace HM5ResourceTool
{
    /// <summary>
    /// Interaction logic for PropertyDialog.xaml
    /// </summary>
    public partial class PropertyDialog : MetroWindow
    {
        public PropertyDialog(Settings? settings)
        {
            InitializeComponent();

            if (settings != null && settings.Theme != null)
            {
                Utility.ChangeTheme(settings, this, false);
            }

            cbPropertyTypes.Items.Add("bool");
            cbPropertyTypes.Items.Add("int8");
            cbPropertyTypes.Items.Add("char");
            cbPropertyTypes.Items.Add("uint8");
            cbPropertyTypes.Items.Add("int16");
            cbPropertyTypes.Items.Add("uint16");
            cbPropertyTypes.Items.Add("int32");
            cbPropertyTypes.Items.Add("uint32");
            cbPropertyTypes.Items.Add("int64");
            cbPropertyTypes.Items.Add("uint64");
            cbPropertyTypes.Items.Add("float32");
            cbPropertyTypes.Items.Add("float64");
            cbPropertyTypes.Items.Add("SVector2");
            cbPropertyTypes.Items.Add("SVector3");
            cbPropertyTypes.Items.Add("SMatrix43");
            cbPropertyTypes.Items.Add("SColorRGB");
            cbPropertyTypes.Items.Add("SColorRGBA");
            cbPropertyTypes.Items.Add("SEntityTemplateReference");
            cbPropertyTypes.Items.Add("SBodyPartDamageMultipliers");
            cbPropertyTypes.Items.Add("ZRuntimeResourceID");
            cbPropertyTypes.Items.Add("ZString");
            cbPropertyTypes.Items.Add("ZCurve");
            cbPropertyTypes.Items.Add("ZGameTime");
            cbPropertyTypes.Items.Add("ZSpeakerLevels");
            cbPropertyTypes.Items.Add("TArray<SEntityTemplateReference>");
            cbPropertyTypes.Items.Add("TArray<float32>");
            cbPropertyTypes.Items.Add("TArray<ZGameTime>");
            cbPropertyTypes.Items.Add("TArray<SVector2>");
            cbPropertyTypes.Items.Add("TArray<bool>");
            cbPropertyTypes.Items.Add("TArray<ZSharedSensorDef.SVisibilitySetting>");
            cbPropertyTypes.Items.Add("TArray<ZString>");
            cbPropertyTypes.Items.Add("TArray<SSettingsParamMultiplier>");
            cbPropertyTypes.Items.Add("TArray<SColorRGB>");
            cbPropertyTypes.Items.Add("TArray<ECameraState>");
        }
    }
}
