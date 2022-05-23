using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.IO;
using System.Windows.Media;
using MahApps.Metro.Controls;
using ControlzEx.Theming;
using Newtonsoft.Json;

namespace HM5ResourceTool
{
    internal class Utility
    {
		public static void ChangeTheme(Settings? settings, MetroWindow metroWindow, bool updateSettings = true)
		{
			if (settings == null || settings.Theme == null)
			{
				return;
			}

			bool hasLightTheme = settings.Theme.Split('/')[0].Equals("Light");
			string color = settings.Theme.Split('/')[1];
			Theme? theme2 = null;

			if (hasLightTheme)
			{
				switch (color)
				{
					case "Blue":
						theme2 = RuntimeThemeGenerator.Current.GenerateRuntimeTheme("Light", Colors.DodgerBlue);
						break;
					case "Brown":
						theme2 = RuntimeThemeGenerator.Current.GenerateRuntimeTheme("Light", Colors.BurlyWood);
						break;
					case "Green":
						theme2 = RuntimeThemeGenerator.Current.GenerateRuntimeTheme("Light", Colors.Green);
						break;
					case "Orange":
						theme2 = RuntimeThemeGenerator.Current.GenerateRuntimeTheme("Light", Colors.Orange);
						break;
					case "Purple":
						theme2 = RuntimeThemeGenerator.Current.GenerateRuntimeTheme("Light", Colors.Purple);
						break;
					case "Red":
						theme2 = RuntimeThemeGenerator.Current.GenerateRuntimeTheme("Light", Colors.Red);
						break;
					case "Yellow":
						theme2 = RuntimeThemeGenerator.Current.GenerateRuntimeTheme("Light", Colors.Yellow);
						break;
				}
			}
			else
			{
				switch (color)
				{
					case "Blue":
						theme2 = RuntimeThemeGenerator.Current.GenerateRuntimeTheme("Dark", Colors.DodgerBlue);
						break;
					case "Brown":
						theme2 = RuntimeThemeGenerator.Current.GenerateRuntimeTheme("Dark", Colors.BurlyWood);
						break;
					case "Green":
						theme2 = RuntimeThemeGenerator.Current.GenerateRuntimeTheme("Dark", Colors.Green);
						break;
					case "Orange":
						theme2 = RuntimeThemeGenerator.Current.GenerateRuntimeTheme("Dark", Colors.Orange);
						break;
					case "Purple":
						theme2 = RuntimeThemeGenerator.Current.GenerateRuntimeTheme("Dark", Colors.Purple);
						break;
					case "Red":
						theme2 = RuntimeThemeGenerator.Current.GenerateRuntimeTheme("Dark", Colors.Red);
						break;
					case "Yellow":
						theme2 = RuntimeThemeGenerator.Current.GenerateRuntimeTheme("Dark", Colors.Yellow);
						break;
					case "White":
						theme2 = RuntimeThemeGenerator.Current.GenerateRuntimeTheme("Dark", Colors.White);
						break;
				}
			}

			if (theme2 != null)
			{
				ThemeManager.Current.ChangeTheme(metroWindow, ThemeManager.Current.AddTheme(theme2));
			}

			if (updateSettings)
			{
				JsonSerializer serializer = new JsonSerializer();
				using StreamWriter streamWriter = new StreamWriter("HM5ResourceTool.json");
				using JsonWriter writer = new JsonTextWriter(streamWriter);

				serializer.Serialize(streamWriter, settings);
			}
		}
	}
}
