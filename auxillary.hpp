#ifndef LR_SUPERFLAPPYBIRDS_AUXILLARY_H
#define LR_SUPERFLAPPYBIRDS_AUXILLARY_H

#include <stack>

class Auxillary {
public:
    /// Returns a stack with each digit of a number in order.
    /// i.e., 356 -> [3, 5, 6]
    /// \param number The number to seperate into digits.
    /// \return The stack of digits.
    static std::stack<int> digits(int number) {
        std::stack<int> sd;
        while (number > 0) {
            int digit = number % 10;
            number /= 10;
            sd.push(digit);
        }
        if (sd.empty())
            sd.push(0);

        return sd;
    }

    /// Gets the frames for each digit to be drawn.
    /// \param number The number to be drawn.
    /// \param number_frames The frames for each number on the texture, in order.
    /// \param x The x position to be drawn at.
    /// \param y The y position to be drawn at.
    /// \param pixels_between_numbers The gap in pixels between the digits.
    /// \param scale The factor to scale the number by.
    /// \return The frames, each pair consists of the src and dest frame.
    static std::vector<std::pair<Rect, Rect>> getNumberRects(int number, std::vector<Rect>* number_frames, int x, int y, int pixels_between_numbers, float scale = 1) {
        auto ds = digits(number);
        int x_offset = 0;
        std::vector<std::pair<Rect, Rect>> positions;
        while (!ds.empty())
        {
            int digit = ds.top();
            ds.pop();
            Rect frame = number_frames->at(digit);
            Rect dest_rect;
            dest_rect.x = x + x_offset;
            dest_rect.y = y;
            dest_rect.w = (int)(frame.w * scale);
            dest_rect.h = (int)(frame.h * scale);
            positions.push_back({frame, dest_rect});
            x_offset += dest_rect.w + pixels_between_numbers;
        }
        return positions;
    }
};

#endif //LR_SUPERFLAPPYBIRDS_AUXILLARY_H
