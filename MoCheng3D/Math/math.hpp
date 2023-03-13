#pragma once
#include <variant>
struct Vec2
{
    float x;
    float y;
};

class Mat4
{
  public:
    static Mat4 CreateIdentity();
    static Mat4 CreateOnes();
    static Mat4 CreateOrtho(int left, int right, int top, int bottom, int near, int far);
    static Mat4 CreateTranslate(const Vec2 &);
    static Mat4 CreateScale(const Vec2 &);
    static Mat4 Create(const std::initializer_list<float> &);

    
    Mat4();
    const float *GetData() const
    {
        return data_;
    }
    void Set(int x, int y, float value)
    {
        data_[x * 4 + y] = value;
    }
    float Get(int x, int y) const
    {
        return data_[x * 4 + y];
    }

    Mat4 Mul(const Mat4 &m) const;

  private:
    float data_[4 * 4];
};

struct Rect {
    Vec2 pos;
    Vec2 size;
};