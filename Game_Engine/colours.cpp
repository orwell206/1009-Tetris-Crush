#pragma once
typedef uint8_t u8;

struct Color
{
private:
    // Color variables - RGBA
    u8 r;                           // Color red
    u8 g;                           // Clor green
    u8 b;                           // Color blue
    u8 a;                           // Color alpha

public:
    Color(u8 r, u8 g, u8 b, u8 a);  // Parameterised constructor.
    u8 get_r();                     // Gets the color red from RGBA.
    u8 get_g();                     // Gets the color green from RGBA.
    u8 get_b();                     // Gets the color blue from RGBA.
    u8 get_a();                     // Gets the color alpha from RGBA.
};

Color::Color(u8 r, u8 g, u8 b, u8 a){
    /*
        Desc: Parameterised constructor.

        Params: u8 (r), u8 (g), u8 (b), u8 (a)
    */
    this->r = r;
    this->g = g;
    this->b = b;
    this->a = a;
}

u8 Color::get_r(){
    /*
        Desc: Gets the color red from RGBA.

        Params: None.
    */
    return this->r;
}

u8 Color::get_g(){
    /*
        Desc: Gets the color green from RGBA.

        Params: None.
    */
    return this->g;
}

u8 Color::get_b(){
    /*
        Desc: Gets the color blue from RGBA.

        Params: None.
    */
    return this->b;
}

u8 Color::get_a(){
    /*
        Desc: Gets the color alpha from RGBA.

        Params: None.
    */
    return this->a;
}
