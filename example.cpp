#include <ftxui/component/component.hpp>  // for Button, Renderer, Input, Container
#include <ftxui/component/screen_interactive.hpp>  // for ScreenInteractive
#include <ftxui/dom/elements.hpp>  // for text, Element, hbox, vbox, border
#include <ftxui/screen/color.hpp>  // for Color
#include <iostream>
int main() {
    using namespace ftxui;

    // Variável para armazenar o texto digitado pelo usuário
    std::string input_text;

    // Cria um componente Input para capturar a entrada de texto
    auto input = Input(&input_text, L"Enter text...");

    // Cria um botão que, ao ser clicado, imprime o texto digitado
    auto button = Button(L"Print Text", [&] {
        std::cout << "Input text: " << input_text << std::endl;
    });

    // Cria um container vertical para organizar os componentes
    auto component = Container::Vertical({
        input,
        button
    });

    // Define o layout e a renderização dos componentes
    auto renderer = Renderer(component, [&] {
        return vbox({
            text(L"Please enter some text:") | border,
            input->Render() | border,
            button->Render() | border,
        });
    });

    // Cria uma tela interativa e inicia o loop de eventos
    auto screen = ScreenInteractive::FitComponent();
    screen.Loop(renderer);

    return 0;
}
