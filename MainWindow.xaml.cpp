#include "pch.h"
#include "MainWindow.xaml.h"
#include <winrt/Microsoft.UI.Windowing.h>
#if __has_include("MainWindow.g.cpp")
#include "MainWindow.g.cpp"
#endif

using namespace winrt;
using namespace Microsoft::UI::Xaml;

// To learn more about WinUI, the WinUI project structure,
// and more about our project templates, see: http://aka.ms/winui-project-info.

namespace winrt::SocialNetworkApp::implementation
{
    MainWindow::MainWindow()
    {
        InitializeComponent();
        SetupTitleBar();

        LoadAnimation().Begin();
        DisplayOutput(L"🚀 Социальная сеть запущена\n");
        DisplayOutput(L"━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
        DisplayOutput(L"Добро пожаловать! Начните добавлять пользователей.\n\n");
    }

    void MainWindow::SetupTitleBar()
    {
        auto titleBar = this->AppWindow().TitleBar();
        titleBar.ExtendsContentIntoTitleBar(true);
        titleBar.ButtonBackgroundColor(Microsoft::UI::Colors::Transparent());
        titleBar.ButtonInactiveBackgroundColor(Microsoft::UI::Colors::Transparent());

        this->SetTitleBar(AppTitleBar());
    }

    void MainWindow::AddUserButton_Click(IInspectable const&, RoutedEventArgs const&)
    {
        auto userName = UserNameTextBox().Text();

        if (userName.empty())
        {
            DisplayOutput(L"❌ Ошибка: Введите имя пользователя!\n\n");
            return;
        }

        AddUser(std::wstring(userName));
        UserNameTextBox().Text(L"");
    }

    void MainWindow::AddFriendshipButton_Click(IInspectable const&, RoutedEventArgs const&)
    {
        auto user1 = User1TextBox().Text();
        auto user2 = User2TextBox().Text();

        if (user1.empty() || user2.empty())
        {
            DisplayOutput(L"❌ Ошибка: Введите имена обоих пользователей!\n\n");
            return;
        }

        AddFriendship(std::wstring(user1), std::wstring(user2));
        User1TextBox().Text(L"");
        User2TextBox().Text(L"");
    }

    void MainWindow::ShowFriendsButton_Click(IInspectable const&, RoutedEventArgs const&)
    {
        auto userName = SearchUserTextBox().Text();

        if (userName.empty())
        {
            DisplayOutput(L"❌ Ошибка: Введите имя пользователя!\n\n");
            return;
        }

        auto friends = GetFriends(std::wstring(userName));

        std::wstringstream output;
        output << L"\n┌─────────────────────────────────────────┐\n";
        output << L"│ 👥 Друзья пользователя: " << std::wstring(userName) << L"\n";
        output << L"└─────────────────────────────────────────┘\n";

        if (friends.empty())
        {
            output << L"  У этого пользователя пока нет друзей.\n";
        }
        else
        {
            int count = 1;
            for (const auto& friendName : friends)
            {
                output << L"  " << count++ << L". " << friendName << L"\n";
            }
        }

        output << L"\n";
        DisplayOutput(output.str());
    }

    void MainWindow::FindCommonButton_Click(IInspectable const&, RoutedEventArgs const&)
    {
        auto user1 = CommonUser1TextBox().Text();
        auto user2 = CommonUser2TextBox().Text();

        if (user1.empty() || user2.empty())
        {
            DisplayOutput(L"❌ Ошибка: Введите имена обоих пользователей!\n\n");
            return;
        }

        auto commonFriends = FindCommonFriends(std::wstring(user1), std::wstring(user2));

        std::wstringstream output;
        output << L"\n┌─────────────────────────────────────────┐\n";
        output << L"│ 🤝 Общие друзья\n";
        output << L"│ " << std::wstring(user1) << L" ⟷ " << std::wstring(user2) << L"\n";
        output << L"└─────────────────────────────────────────┘\n";

        if (commonFriends.empty())
        {
            output << L"  Общих друзей не найдено.\n";
        }
        else
        {
            int count = 1;
            for (const auto& friendName : commonFriends)
            {
                output << L"  " << count++ << L". " << friendName << L"\n";
            }
        }

        output << L"\n";
        DisplayOutput(output.str());
    }

    void MainWindow::AddUser(const std::wstring& userName)
    {
        if (socialGraph.find(userName) != socialGraph.end())
        {
            DisplayOutput(L"⚠️  Пользователь '" + userName + L"' уже существует!\n\n");
            return;
        }

        socialGraph[userName] = std::set<std::wstring>();

        std::wstringstream output;
        output << L"✅ Пользователь добавлен: " << userName << L"\n";
        output << GetAllUsers();
        DisplayOutput(output.str());
    }

    void MainWindow::AddFriendship(const std::wstring& user1, const std::wstring& user2)
    {
        if (socialGraph.find(user1) == socialGraph.end())
        {
            DisplayOutput(L"❌ Ошибка: Пользователь '" + user1 + L"' не найден!\n\n");
            return;
        }

        if (socialGraph.find(user2) == socialGraph.end())
        {
            DisplayOutput(L"❌ Ошибка: Пользователь '" + user2 + L"' не найден!\n\n");
            return;
        }

        if (user1 == user2)
        {
            DisplayOutput(L"❌ Ошибка: Пользователь не может дружить с самим собой!\n\n");
            return;
        }

        if (socialGraph[user1].find(user2) != socialGraph[user1].end())
        {
            DisplayOutput(L"⚠️  Дружба между '" + user1 + L"' и '" + user2 + L"' уже существует!\n\n");
            return;
        }

        socialGraph[user1].insert(user2);
        socialGraph[user2].insert(user1);

        std::wstringstream output;
        output << L"✅ Дружба создана: " << user1 << L" ⟷ " << user2 << L"\n";
        output << L"   Граф расширен: +2 связи\n\n";
        DisplayOutput(output.str());
    }

    std::set<std::wstring> MainWindow::GetFriends(const std::wstring& userName)
    {
        if (socialGraph.find(userName) == socialGraph.end())
        {
            DisplayOutput(L"❌ Ошибка: Пользователь '" + userName + L"' не найден!\n\n");
            return std::set<std::wstring>();
        }

        return socialGraph[userName];
    }

    std::set<std::wstring> MainWindow::FindCommonFriends(const std::wstring& user1,
        const std::wstring& user2)
    {
        std::set<std::wstring> common;

        if (socialGraph.find(user1) == socialGraph.end())
        {
            DisplayOutput(L"❌ Ошибка: Пользователь '" + user1 + L"' не найден!\n\n");
            return common;
        }

        if (socialGraph.find(user2) == socialGraph.end())
        {
            DisplayOutput(L"❌ Ошибка: Пользователь '" + user2 + L"' не найден!\n\n");
            return common;
        }

        auto& friends1 = socialGraph[user1];
        auto& friends2 = socialGraph[user2];

        for (const auto& friendName : friends1)
        {
            if (friends2.find(friendName) != friends2.end())
            {
                common.insert(friendName);
            }
        }

        return common;
    }

    void MainWindow::DisplayOutput(const std::wstring& message)
    {
        auto currentText = OutputTextBlock().Text();
        OutputTextBlock().Text(currentText + message);
    }

    std::wstring MainWindow::GetAllUsers()
    {
        std::wstringstream output;
        output << L"\n📊 Статистика сети:\n";
        output << L"   Пользователей: " << socialGraph.size() << L"\n";

        size_t totalConnections = 0;
        for (const auto& pair : socialGraph)
        {
            totalConnections += pair.second.size();
        }
        totalConnections /= 2;

        output << L"   Связей дружбы: " << totalConnections << L"\n";
        output << L"\n📋 Список пользователей:\n";

        for (const auto& pair : socialGraph)
        {
            output << L"   • " << pair.first << L" (друзей: "
                << pair.second.size() << L")\n";
        }

        output << L"\n";
        return output.str();
    }
    void MainWindow::ShowNotification(const std::wstring& message, int severity)
    {
        NotificationBar().Message(message);

        // 0 = Success, 1 = Warning, 2 = Error, 3 = Informational
        switch (severity)
        {
        case 0: NotificationBar().Severity(Microsoft::UI::Xaml::Controls::InfoBarSeverity::Success); break;
        case 1: NotificationBar().Severity(Microsoft::UI::Xaml::Controls::InfoBarSeverity::Warning); break;
        case 2: NotificationBar().Severity(Microsoft::UI::Xaml::Controls::InfoBarSeverity::Error); break;
        case 3: NotificationBar().Severity(Microsoft::UI::Xaml::Controls::InfoBarSeverity::Informational); break;
        }

        NotificationBar().IsOpen(true);
    }
    void MainWindow::ShowTipButton_Click(winrt::Windows::Foundation::IInspectable const&, winrt::Microsoft::UI::Xaml::RoutedEventArgs const&)
    {
        AddUserTip().IsOpen(true);
    }
    void MainWindow::RefreshButton_Click(winrt::Windows::Foundation::IInspectable const&, winrt::Microsoft::UI::Xaml::RoutedEventArgs const&)
    {
        // Пока пустой или логика обновления
        OutputTextBlock().Text(L"Граф обновлён\n");
    }

}

