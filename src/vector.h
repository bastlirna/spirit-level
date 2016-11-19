#ifndef _VECTOR_H
#define _VECTOR_H

#include <Arduino.h>

typedef float vpart_t;

class Vector{
public:
  Vector(uint8_t _size);
  Vector(vpart_t *_data, uint8_t _size);
  Vector(const Vector& other);
  ~Vector();
  vpart_t get(uint8_t i);
  vpart_t &operator[](uint8_t i);
  Vector operator/(vpart_t scalar);
  Vector operator*(vpart_t scalar);
  Vector operator+(vpart_t scalar);
  Vector operator-(vpart_t scalar);
  Vector operator+(const Vector& other);
  Vector operator-(const Vector& other);
  Vector& operator=(const Vector& other);
  Vector operator/(const Vector& other);
  Vector operator*(const Vector& other);

  vpart_t dot(const Vector& other);

  Vector minimum(const Vector& other);
  Vector maximum(const Vector& other);
  Vector normalize();
  vpart_t length();

  Vector xy();

  void print(Print &p);
  void println(Print &p);
  void fill(vpart_t val);

  vpart_t *data;

  uint8_t size;
protected:
  Vector() {};


};

class Vector4 : public Vector{
public:
  Vector4() : Vector(4) {};
  Vector4(vpart_t x, vpart_t y, vpart_t z, vpart_t a);
};

class Vector3 : public Vector{
public:
  Vector3() : Vector(3) {};
  Vector3(vpart_t x, vpart_t y, vpart_t z);
};


#endif
