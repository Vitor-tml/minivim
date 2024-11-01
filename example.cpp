#include <ftxui/component/component.hpp>  // for Button, Renderer, CatchEvent, etc.
#include <ftxui/component/screen_interactive.hpp>  // for ScreenInteractive
#include <ftxui/dom/elements.hpp>  // for text, Element, hbox, vbox, border
#include <ftxui/screen/color.hpp>  // for Color

int main() {
    using namespace ftxui;
    
    // Variáveis para armazenar o estado
    bool checked = false;

    // Cria um botão
    auto button = Button(L"Click me", [] { std::cout << "Button clicked!\n"; });

    // Cria uma checkbox
    auto checkbox = Checkbox(L"Option", &checked);

    // Combina elementos em um layout vertical
    auto component = Renderer(
        Container::Vertical({
            button,
            checkbox
        }),
        [&] {
            return vbox({
                text(L"Hello, FTXUI!") | border,
                separator(),
                button->Render(),
                separator(),
                checkbox->Render() | border,
            });
        }
    );

    // Cria uma tela interativa
    auto screen = ScreenInteractive::FitComponent();
    screen.Loop(component);

    return 0;
}
