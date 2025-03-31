#pragma once

class Color {
public:
    unsigned char r, g, b, a;

    Color(unsigned char r = 255, unsigned char g = 255, unsigned char b = 255, unsigned char a = 255);

    static Color Red();
    static Color Green();
    static Color Blue();
    static Color Yellow();
    static Color Black();
    static Color White();
};