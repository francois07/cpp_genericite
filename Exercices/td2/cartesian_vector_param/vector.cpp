#include "vector.hh"

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

Vector::Vector()
{
    for (int i = 0; i < NDIM; i++)
    {
        this->coords_[i] = 0;
    }
}

Vector &Vector::operator+=(const Vector &v)
{
    for(int i=0; i < NDIM; i++)
    {
        this->coords_[i] += v.coords_[i];
    }

    return *this;
}

Vector Vector::operator+(const Vector &v)
{
    auto newVec = Vector{};

    for(int i=0; i < NDIM; i++)
    {
        newVec.coords_[i] = this->coords_[i] + v.coords_[i];
    }

    return newVec;
}

Vector &Vector::operator-=(const Vector &v)
{
    for(int i=0; i < NDIM; i++)
    {
        this->coords_[i] -= v.coords_[i];
    }

    return *this;
}

Vector Vector::operator-(const Vector &v)
{
    auto newVec = Vector{};

    for(int i=0; i < NDIM; i++)
    {
        newVec.coords_[i] = this->coords_[i] - v.coords_[i];
    }

    return newVec;
}

value Vector::operator*(const Vector &v)
{
    value dotProduct = 0;

    for(int i = 0; i < NDIM; i++)
    {
        dotProduct += this->coords_[i] * v.coords_[i];
    }

    return dotProduct;
}

Vector &Vector::operator*=(const value &k)
{
    for(int i = 0; i < NDIM; i++)
    {
        this->coords_[i] *= k;
    }

    return *this;
}

Vector Vector::operator*(const value &k)
{
    auto newVec = Vector{};

    for(int i=0; i < NDIM; i++)
    {
        newVec.coords_[i] = this->coords_[i] * k;
    }

    return newVec;
}

Vector &Vector::operator+=(const value &k)
{
    for(int i = 0; i < NDIM; i++)
    {
        this->coords_[i] += k;
    }

    return *this;
}

Vector Vector::operator+(const value &k)
{
    auto newVec = Vector{};

    for(int i=0; i < NDIM; i++)
    {
        newVec.coords_[i] = this->coords_[i] + k;
    }

    return newVec;
}

value &Vector::operator[](int idx)
{
    return this->coords_[idx];
}

const value &Vector::operator[](int idx) const{
    return this->coords_[idx];
};


std::ostream &operator<<(std::ostream &os, const Vector& v)
{
    os << "[";

    for(int i = 0; i < NDIM - 1; i++)
    {
        os << v[i] << ',';
    }

    os << v[NDIM - 1] << "]";

    return os;
}