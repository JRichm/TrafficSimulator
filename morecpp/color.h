#pragma once

class Color {
public:
    unsigned char r, g, b;

    Color() : r(255), g(255), b(255) {}
    Color(unsigned char red, unsigned char green, unsigned char blue) : r(red), g(green), b(blue) {}

    // Predefined colors
    static Color Red() { return Color(255, 0, 0); }
    static Color Green() { return Color(0, 255, 0); }
    static Color Blue() { return Color(0, 0, 255); }
    static Color Yellow() { return Color(255, 255, 0); }  // Added missing Yellow method
    static Color Orange() { return Color(255, 165, 0); }
    static Color Purple() { return Color(128, 0, 128); }
    static Color White() { return Color(255, 255, 255); }
    static Color Black() { return Color(0, 0, 0); }
    static Color Gray() { return Color(128, 128, 128); }
};