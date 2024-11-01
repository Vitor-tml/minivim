#include <ftxui/component/screen_interactive.hpp> 
#include <ftxui/component/component.hpp>          // button, renderer, event
#include <ftxui/dom/elements.hpp>                 // text, elements, hbox, etc
#include <ftxui/screen/color.hpp>                 // cores
#include <iostream>
#include <string>

int main()
{
    const std::string hello = {"Olá, Mundo!"};

    // Elemento básico
    ftxui::Element doc = ftxui::hbox (
        ftxui::text(hello) | ftxui::border
    );

    // Objeto da tela
    ftxui::Screen tela = ftxui::Screen::Create(
        ftxui::Dimension::Fixed(hello.length() + 1),
        ftxui::Dimension::Fixed(3)
    );

    // Renderização e atualização da tela
    ftxui::Render(tela, doc);
    tela.Print();

    std::cout << '\n';
    return 0;    
}