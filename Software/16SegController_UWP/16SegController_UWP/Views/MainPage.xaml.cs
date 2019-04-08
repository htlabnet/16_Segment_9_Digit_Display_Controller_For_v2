using System;

using _16SegController_UWP.ViewModels;

using Windows.UI.Xaml.Controls;

namespace _16SegController_UWP.Views
{
    public sealed partial class MainPage : Page
    {
        public MainViewModel ViewModel { get; } = new MainViewModel();

        public MainPage()
        {
            InitializeComponent();
        }
    }
}
