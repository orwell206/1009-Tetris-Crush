#pragma once
typedef uint8_t u8;

struct Color
{
private:
    u8 r;
    u8 g;
    u8 b;
    u8 a;

public:
    Color(u8 r, u8 g, u8 b, u8 a){
        this->r = r;
        this->g = g;
        this->b = b;
        this->a = a;
    }

    u8 get_r(){
        return this->r;
    }

    u8 get_g(){
        return this->g;
    }

    u8 get_b(){
        return this->b;
    }

    u8 get_a(){
        return this->a;
    }
};