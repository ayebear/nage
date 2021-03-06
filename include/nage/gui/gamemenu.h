// Copyright (C) 2014-2015 Eric Hebert (ayebear)
// This code is licensed under LGPLv3, see LICENSE.txt for details.

#ifndef GAMEMENU_H
#define GAMEMENU_H

#include <SFML/Graphics.hpp>
#include <functional>
#include <string>
#include <vector>
#include "nage/graphics/colorcode.h"
#include "nage/graphics/spriteloader.h"
#include "nage/actions/actionhandler.h"

namespace ng
{

/*
An SFML menu designed for use with games.
You can configure various settings in the config file.
Buttons can have different settings for each state.
A transition time can be set to animate the button state transitions.

How to use:
    Use addItem() to add menu items linked to callbacks.
    Whenever the item is clicked, the callback will be called.
    Note: Make sure your callbacks eventually return, otherwise there is
        potential for a stack overflow to eventually occur.

Example:
    void pressedStart()
    {
        std::cout << "'Start' was pressed.\n";
    }
...
    GameMenu menu(window, "menu.cfg");
    menu.addItem("Start", pressedStart);
*/
class GameMenu: public sf::Drawable
{
    public:

        using CallbackType = std::function<void()>;

        // Setup and adding/accessing menu items
        GameMenu(sf::RenderWindow& window, const std::string& configFilename);
        size_t addItem(const std::string& name, CallbackType callback);
        void setLabel(size_t index, const std::string& name);
        const std::string& getLabel(size_t index) const;
        void setEnabled(size_t index, bool state);

        // These should be called while running
        void handleEvent(const sf::Event& event);
        void update(float dt);
        void draw(sf::RenderTarget& target, sf::RenderStates states) const;

    private:

        int getSelectedItem() const;
        void mapMousePos(const sf::Vector2i& pos);
        void selectMenuItem(int index);
        void loadSettings(const std::string& filename);
        sf::Color averageColors(const sf::Color& startColor, const sf::Color& endColor, float ratio) const;
        float interpolate(float start, float end, float ratio) const;
        void moveUp();
        void moveDown();

        // Returns the new "ratio" or point in time of the animation
        float updateDt(float& itemDt, float dt, float animTime, bool hovered);

        struct MenuItem
        {
            MenuItem(const std::string& name, CallbackType callback);
            sf::FloatRect rect;
            sf::RectangleShape shape;
            sf::Text label;
            std::string name;
            CallbackType callback;
            float dt{0.0f};
            float dtText{0.0f};
            bool enabled{true};
        };

        struct ButtonSettings
        {
            unsigned fontSize;
            unsigned outlineThickness;
            ColorCode colorFill;
            ColorCode colorOutline;
            ColorCode fontColor;
        };

        static const int NO_SELECTION = -1;

        // Main objects/variables
        sf::RenderWindow& window;
        int currentItem;
        std::vector<MenuItem> menuItems;
        sf::Vector2f mousePos;
        sf::View view;
        sf::Vector2f viewSize;
        sf::Sprite backgroundSprite;
        sf::Sprite foregroundSprite;
        bool mouseMoved;
        ng::ActionHandler actions;

        // Settings
        float transitionTime;
        float textTransitionTime;
        int padding;
        int textPaddingTop;
        unsigned width;
        unsigned height;
        unsigned fontSize;
        sf::Font font;
        ButtonSettings buttonSettings[2];

        // Calculated settings
        sf::Vector2f firstButton;
};

}

#endif
