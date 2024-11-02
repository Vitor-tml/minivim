#include <ftxui/component/screen_interactive.hpp> // screens (interativa, relativa, etc)
#include <ftxui/component/component.hpp>          // button, renderer, event
#include <ftxui/dom/elements.hpp>                 // text, elements, hbox, etc
#include <ftxui/screen/color.hpp>                 // cores
#include <iostream>
#include <string>

int main()
{
    const std::string hello = {"Olá, Mundo!"};

    std::string entrada;
    auto input = ftxui::Input(&entrada, L"Escreva aqui: ");
    // Elemento básico
    ftxui::Element doc = ftxui::hbox (
        ftxui::text(hello) | ftxui::border
    );

    ftxui::Component componente = ftxui::Container::Vertical({
        input
    });

    auto renderer = ftxui::Renderer(componente, [&] {
        return ftxui::vbox({
            doc,
            ftxui::separator(),
            input->Render() | ftxui::border,
        });
    }
    );


    // Objeto da tela
    ftxui::ScreenInteractive tela = ftxui::ScreenInteractive::Fullscreen();
    tela.Loop(renderer);

    return 0;    
}