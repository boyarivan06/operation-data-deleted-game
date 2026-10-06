//
// Created by Иван Бояринцев on 31.10.25.
//
#include <raylib.h>
#include "vector2_operations.h"
#include <iostream>
#include <format>

#include "exceptions.h"

bool operator==(const Vector2 & lhs, const Vector2 & rhs) {
    return lhs.x==rhs.x && lhs.y == rhs.y;
}

Vector2 operator+(const Vector2 & lhs, const Vector2 & rhs) {
    return {lhs.x+rhs.x, lhs.y+rhs.y};
}

Vector2 operator-(const Vector2 &vector1, const Vector2 &vector2) {
    return {vector1.x-vector2.x, vector1.y - vector2.y};
}

void operator+=(Vector2& lhs, const Vector2& rhs) {
    lhs = lhs+rhs;
}

void operator-=(Vector2 & v1, const Vector2 & v2) {
    v1 = v1-v2;
}

Vector2 operator*(const Vector2 &vector2, int i) {
    return {vector2.x * i, vector2.y * i};
}

Vector2 operator*(const Vector2 &vector2, float i) {
    return {vector2.x * i, vector2.y * i};
}

Vector2 operator*(int i, const Vector2 &vector2) {
    return vector2 * i;
}

void operator*=(Vector2& lhs, int n) {
    lhs = lhs*n;
}
bool operator<(Vector2 v1, Vector2 v2) {
    return module(v1) < module(v2);
}

std::ostream & operator<<(std::ostream & output, const Vector2& vector) {
    output << std::format("{}, {}", vector.x, vector.y);
    return output;
}
Vector2 normalize_coords(Vector2 coords) {
    float sqrt2 = sqrtf(2)/2;
    float x = coords.x;
    float y = coords.y;
    float hyp = std::hypot(x, y);

    float cos_val = 0.0f;
    float sin_val = 0.0f;

    if (hyp != 0.0f) {
        cos_val = x / hyp;
        sin_val = y / hyp;
    }
    Vector2 res{};
    if (cos_val >= sqrt2) res.x = 1;
    else if (cos_val <= -sqrt2) res.x = -1;
    else res.x = 0;
    if (sin_val >= sqrt2) res.y = 1;
    else if (sin_val <= -sqrt2) res.y = -1;
    else res.y = 0;
    return res;
}

Vector2 make_integer(Vector2 vector) {
    Vector2 res = {float(int(vector.x)), float(int(vector.y))};
    if (vector.x - res.x >= 0.5f) res.x++;
    if (vector.y - res.y >= 0.5f) res.y++;
    return res;
}

float module(Vector2 vec) {
    return sqrtf(vec.x * vec.x + vec.y * vec.y);
}
Vector2 rotate_destination(Vector2 dest, bool left, int n) {
    if (std::abs(dest.x) > 1 || std::abs(dest.x) > 1) throw ObjectParameterException("not normalized coords given");
    const std::vector<Vector2> circle = {
        {1,0},
        {1,-1},
        {0,-1},
        {-1,-1},
        {-1,0},
        {-1,1},
        {0,1},
        {1,1}
    };
    int index = -1;
    for (int i = 0; i < 8; i++) if (circle[i] == dest) index = i;
    if (index == -1) throw ObjectParameterException("not normalized coords given");
    for (int i = 0; i < n; i++) {
        if (left) index++;
        else index--;
        if (index == 8) index = 0;
        else if (index == -1) index = 7;
    }
    return circle[index];
}