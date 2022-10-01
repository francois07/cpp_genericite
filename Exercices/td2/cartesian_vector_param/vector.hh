#pragma once

#include <ostream>

#include "config.h"

class Vector
{
public:
    // DO NOT CHANGE THIS
    Vector(const Vector &) = default;
    Vector &operator=(const Vector &) = default;
    ~Vector() = default;
    //

    // Add suitable constructors
    Vector();
    Vector(float u, float v);
    Vector(std::initializer_list<value> l);

    // possibly more

    // Public Member functions here
    Vector &operator+=(const Vector &v);
    Vector operator+(const Vector &v);

    Vector &operator-=(const Vector &v);
    Vector operator-(const Vector &v);

    value &operator*=(const Vector &v);
    value operator*(const Vector &v);

    Vector &operator*=(const value &k);
    Vector operator*(const value &k);

    Vector &operator+=(const value &k);
    Vector operator+(const value &k);

    value &operator[](int idx);
    const value &operator[](int idx) const;
    // More to go

private:
    // Private Member functions here
    value coords_[NDIM];
    // Member variables are ALWAYS private, and they go here
};

// Nonmember function operators go here
std::ostream &operator<<(std::ostream &os, const Vector& v);
// value &operator+=(const value k, const Vector &v);