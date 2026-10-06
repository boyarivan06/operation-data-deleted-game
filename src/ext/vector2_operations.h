//
// Created by Иван Бояринцев on 31.10.25.
//

#ifndef VECTOR2_OPERATIONS_H
#define VECTOR2_OPERATIONS_H
#include <iostream>
#include <raylib.h>

bool operator==(const Vector2&, const Vector2&);
Vector2 operator+(const Vector2&, const Vector2&);
Vector2 operator-(const Vector2&, const Vector2&);
void operator+=(Vector2&, const Vector2&);
void operator-=(Vector2&, const Vector2&);
Vector2 operator*(const Vector2&, int);
Vector2 operator*(const Vector2&, float);
Vector2 operator*(int, const Vector2&);
bool operator<(Vector2 v1, Vector2 v2);

void operator*=(Vector2&, int);
std::ostream& operator<<(std::ostream&, const Vector2&);
Vector2 normalize_coords(Vector2 coords); ///< к виду типа (1,0)
Vector2 make_integer(Vector2); ///< к целому числу (позиционирование при перемещении) - ближайшая клетка
float module(Vector2);
Vector2 rotate_destination(Vector2 dest, bool left, int n);
#endif //VECTOR2_OPERATIONS_H
