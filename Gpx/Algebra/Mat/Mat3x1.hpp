#pragma once

namespace gpx
{

struct mat3x1_data
{
    mat3x1_data(f32 v11, f32 v21, f32 v31) :
        _r1(v11), _r2(v21), _r3(v31)
    {
    }

    mat3x1_data(const mat3x1_data& m31) :
        _r1(m31._r1), _r2(m31._r2), _r3(m31._r3)
    {
    }

    mat3x1_data& operator =(const mat3x1_data& rhs)
    {
        _r1 = rhs._r1;
        _r2 = rhs._r2;
        _r3 = rhs._r3;
        return *this;
    }

    f32 _r1;
    f32 _r2;
    f32 _r3;
};

class mat3x1
{
public:
    explicit mat3x1(
        f32 v11 = 0,
        f32 v21 = 0,
        f32 v31 = 0);

    mat3x1(const mat3x1& rhs);

    mat3x1& operator =(const mat3x1& rhs);

public:
    f32 r1() const;

    f32 r2() const;

    f32 r3() const;

    vec3 c1() const;

public:
    mat3x1_data _data;
};

}