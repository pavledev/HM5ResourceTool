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
using System.IO;
using MahApps.Metro.Controls;
using HM5ResourceToolCLI;
using System.Threading;

namespace HM5ResourceTool
{
    /// <summary>
    /// Interaction logic for VideoPlayer.xaml
    /// </summary>
    public partial class VideoPlayer : MetroWindow
    {
        private string filePath;
        private LibVLCSharp.Shared.LibVLC? libVLC;
        private LibVLCSharp.Shared.MediaPlayer? mediaPlayer;
        private LibVLCSharp.Shared.StreamMediaInput? mediaInput;
        private MemoryStream? memoryStream;

        public VideoPlayer(string filePath)
        {
            InitializeComponent();

            this.filePath = filePath;

            videoView.Loaded += VideoView_Loaded;
        }

        private void VideoView_Loaded(object sender, RoutedEventArgs e)
        {
            LibVLCSharp.Shared.Core.Initialize();

            libVLC = new LibVLCSharp.Shared.LibVLC();
            mediaPlayer = new LibVLCSharp.Shared.MediaPlayer(libVLC);

            if (!ZVideo.ConvertPCBinkVidToBINK(filePath, out byte[] binkData, out uint binkSize))
            {
                MessageBox.Show("Failed to convert pc_binkvid to BINK.", "HM5 Resource Tool", MessageBoxButton.OK, MessageBoxImage.Error);

                return;
            }

            memoryStream = new MemoryStream(binkData);
            mediaInput = new LibVLCSharp.Shared.StreamMediaInput(memoryStream);
            mediaPlayer.Media = new LibVLCSharp.Shared.Media(libVLC, mediaInput);

            videoView.MediaPlayer = mediaPlayer;

            mediaPlayer.LengthChanged += MediaPlayer_LengthChanged;
            mediaPlayer.TimeChanged += MediaPlayer_TimeChanged;
            mediaPlayer.EndReached += MediaPlayer_EndReached;
        }

        private void BtnTogglePlay_Click(object sender, RoutedEventArgs e)
        {
            if (mediaPlayer == null)
            {
                return;
            }

            if (btnTogglePlay.Content.Equals("Play"))
            {
                mediaPlayer.Play();

                btnTogglePlay.Content = "Pause";
            }
            else
            {
                mediaPlayer.Pause();

                btnTogglePlay.Content = "Play";
            }
        }

        private void MediaPlayer_LengthChanged(object? sender, LibVLCSharp.Shared.MediaPlayerLengthChangedEventArgs e)
        {
            if (mediaPlayer == null)
            {
                return;
            }

            TimeSpan timeSpan = TimeSpan.FromMilliseconds(mediaPlayer.Length);
            string duration = string.Format("{0:D2}:{1:D2}:{2:D2}", timeSpan.Hours, timeSpan.Minutes, timeSpan.Seconds);

            lblDuration.Dispatcher.Invoke(() =>
            {
                lblDuration.Content = duration;
            });

            progressBar.Dispatcher.Invoke(() =>
            {
                progressBar.Minimum = 0;
                progressBar.Maximum = mediaPlayer.Length;
            });
        }

        private void MediaPlayer_TimeChanged(object? sender, LibVLCSharp.Shared.MediaPlayerTimeChangedEventArgs e)
        {
            TimeSpan timeSpan = TimeSpan.FromMilliseconds(e.Time);
            string time = string.Format("{0:D2}:{1:D2}:{2:D2}", timeSpan.Hours, timeSpan.Minutes, timeSpan.Seconds);

            lblTime.Dispatcher.Invoke(() =>
            {
                lblTime.Content = time;
            });

            progressBar.Dispatcher.Invoke(() =>
            {
                progressBar.Value = e.Time;
            });
        }

        private void ProgressBar_MouseLeftButtonUp(object sender, MouseButtonEventArgs e)
        {
            if (mediaPlayer == null)
            {
                return;
            }

            double mousePosition = e.GetPosition(progressBar).X;
            double ratio = mousePosition / progressBar.ActualWidth;
            TimeSpan timeSpan = TimeSpan.FromMilliseconds(ratio * progressBar.Maximum);

            mediaPlayer.SeekTo(timeSpan);
        }

        private void MediaPlayer_EndReached(object? sender, EventArgs e)
        {
            if (mediaPlayer == null)
            {
                return;
            }

            ThreadPool.QueueUserWorkItem(state => mediaPlayer.Stop());

            lblTime.Dispatcher.Invoke(() =>
            {
                lblTime.Content = "00:00:00";
            });

            progressBar.Dispatcher.Invoke(() =>
            {
                progressBar.Value = 0;
            });

            btnTogglePlay.Dispatcher.Invoke(() =>
            {
                btnTogglePlay.Content = "Play";
            });
        }
    }
}
