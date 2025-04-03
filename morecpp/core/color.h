#pragma once

class Color {
public:
    unsigned char r, g, b, a;

    Color(unsigned char r = 255, unsigned char g = 255, unsigned char b = 255, unsigned char a = 255)
        : r(r), g(g), b(b), a(a) {}

    static Color Red() { return Color(255, 0, 0); }
    static Color Green() { return Color(0, 255, 0); }
    static Color Blue() { return Color(0, 0, 255); }
    static Color Yellow() { return Color(255, 255, 0); }
    static Color Black() { return Color(0, 0, 0); }
    static Color White() { return Color(255, 255, 255); }
};