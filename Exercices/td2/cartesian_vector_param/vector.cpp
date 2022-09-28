#include "vector.hh"

Vector::Vector()
{
}

Vector::Vector(std::initializer_list<value> l)
{
    if (l.size() != NDIM)
    {
        std::runtime_error("Wrong size");
    }

    auto it = l.begin();

    for (int i = 0; i < l.size(); i++)
    {
        this->coords_[i] = *(it + i);
    }
}

Vector &Vector::operator+=(const Vector &v)
{
    this->u_ += v.u_;
    this->v_ += v.v_;

    return *this;
}

Vector &Vector::operator+(const Vector &v)
{
    return Vector{this->u_ + v.u_, this->v_ + v.v_};
}

Vector &Vector::operator-=(const Vector &v)
{
    this->u_ -= v.u_;
    this->v_ -= v.v_;

    return *this;
}

Vector &Vector::operator-(const Vector &v)
{
    return Vector{this->u_ - v.u_, this->v_ - v.v_};
}

value Vector::operator*(const Vector &v)
{
    return (this->u_ * v.u_) + (this->v_ * v.v_);
}

Vector &Vector::operator*=(const value &k)
{
    this->u_ *= k;
    this->v_ *= k;

    return *this;
}

Vector &Vector::operator*(const value &k)
{
    return Vector{this->u_ * k, this->v_ * k};
}

Vector &Vector::operator+=(const value &k)
{
    this->u_ += k;
    this->v_ += k;

    return *this;
}