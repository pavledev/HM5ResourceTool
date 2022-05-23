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
using System.Windows.Navigation;
using System.Windows.Shapes;
using MahApps.Metro.Controls;

namespace HM5ResourceTool
{
    /// <summary>
    /// Interaction logic for ProgressWindow.xaml
    /// </summary>
    public partial class ProgressWindow : MetroWindow
    {
        public ProgressWindow(Settings? settings)
        {
            InitializeComponent();

            if (settings != null && settings.Theme != null)
            {
                Utility.ChangeTheme(settings, this, false);
            }
        }
    }
}
