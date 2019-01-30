#ifndef LR_SUPERFLAPPYBIRDS_POS_HPP
#define LR_SUPERFLAPPYBIRDS_POS_HPP

struct Pos
{
    Pos() : x(0), y(0) {}
    Pos(int x, int y) : x(x), y(y) {}

    Pos& operator+=(Pos pos)       { x += pos.x; y += pos.y; return *this; }
    Pos& operator-=(Pos pos)       { x -= pos.x; y -= pos.y; return *this; }
    Pos& operator*=(Pos pos)       { x *= pos.x; y *= pos.y; return *this; }
    Pos& operator/=(int div)       { x /= div; y /= div; return *this; }
    Pos  operator+ (Pos pos) const { return { x + pos.x, y + pos.y }; }
    Pos  operator- (Pos pos) const { return { x - pos.x, y - pos.y }; }
    Pos  operator* (Pos pos) const { return { x * pos.x, y * pos.y }; }
    Pos  operator/ (int div) const { return { x / div, y / div }; }
    bool operator==(Pos pos) const { return x == pos.x && y == pos.y; }
    bool operator!=(Pos pos) const { return !(*this == pos); }

    int x, y;
};

#endif //LR_SUPERFLAPPYBIRDS_POS_HPP
