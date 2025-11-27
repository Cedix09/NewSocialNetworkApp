#pragma once

#include "MainWindow.g.h"
#include <map>
#include <set>
#include <vector>
#include <string>
#include <sstream>


namespace winrt::SocialNetworkApp::implementation
{
    struct MainWindow : MainWindowT<MainWindow>
    {
        MainWindow();

        void AddUserButton_Click(Windows::Foundation::IInspectable const& sender,
            Microsoft::UI::Xaml::RoutedEventArgs const& e);
        void AddFriendshipButton_Click(Windows::Foundation::IInspectable const& sender,
            Microsoft::UI::Xaml::RoutedEventArgs const& e);
        void ShowFriendsButton_Click(Windows::Foundation::IInspectable const& sender,
            Microsoft::UI::Xaml::RoutedEventArgs const& e);
        void FindCommonButton_Click(Windows::Foundation::IInspectable const& sender,
            Microsoft::UI::Xaml::RoutedEventArgs const& e);
        void ShowTipButton_Click(winrt::Windows::Foundation::IInspectable const& sender,
            winrt::Microsoft::UI::Xaml::RoutedEventArgs const& args);
        void RefreshButton_Click(winrt::Windows::Foundation::IInspectable const& sender, 
            winrt::Microsoft::UI::Xaml::RoutedEventArgs const& args);
     



    private:
        std::map<std::wstring, std::set<std::wstring>> socialGraph;

        void AddUser(const std::wstring& userName);
        void AddFriendship(const std::wstring& user1, const std::wstring& user2);
        std::set<std::wstring> GetFriends(const std::wstring& userName);
        std::set<std::wstring> FindCommonFriends(const std::wstring& user1,
            const std::wstring& user2);
        void DisplayOutput(const std::wstring& message);
        std::wstring GetAllUsers();
        void SetupTitleBar();
        void ShowNotification(const std::wstring& message, int severity);
    };
}

namespace winrt::SocialNetworkApp::factory_implementation
{
    struct MainWindow : MainWindowT<MainWindow, implementation::MainWindow>
    {
    };
}
