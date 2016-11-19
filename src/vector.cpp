#include "vector.h"
#include "app_asserts.h"

Vector::Vector(vpart_t *_data, uint8_t _size)
{
  uint8_t i;
  size = _size;
  data = (vpart_t*)malloc(sizeof(vpart_t) * _size);

  for(i = 0; i < _size; i++){
    data[i] = _data[i];
  }
}

Vector::Vector(uint8_t _size)
{
  size = _size;
  data = (vpart_t*)malloc(sizeof(vpart_t) * _size);
  fill(0);
}

Vector::Vector(const Vector& other) : Vector(other.data, other.size)
{
}

vpart_t Vector::length()
{
  uint8_t i;
  vpart_t sum = 0;

  for(i = 0; i < size; i++){
    sum += data[i]*data[i];
  }

  return sqrt(sum);
}

void Vector::fill(vpart_t val)
{
  uint8_t i;
  for(i = 0; i < size; i++){
    data[i] = val;
  }
}

vpart_t &Vector::operator[](uint8_t i)
{
  appAssert(i < size);
  return data[i];
}

vpart_t Vector::get(uint8_t i)
{
  appAssert(i < size);
  return data[i];
}

Vector Vector::operator/(vpart_t scalar)
{
  uint8_t i;
  Vector v = Vector(data, size);

  for(i = 0; i < size; i++){
    v.data[i] /= scalar;
  }

  return v;
}

Vector Vector::operator*(vpart_t scalar)
{
  uint8_t i;
  Vector v = Vector(data, size);

  for(i = 0; i < size; i++){
    v.data[i] *= scalar;
  }

  return v;
}

Vector Vector::operator+(vpart_t scalar)
{
  uint8_t i;
  Vector v = Vector(data, size);

  for(i = 0; i < size; i++){
    v.data[i] += scalar;
  }

  return v;
}

Vector Vector::operator-(vpart_t scalar)
{
  uint8_t i;
  Vector v = Vector(data, size);

  for(i = 0; i < size; i++){
    v.data[i] -= scalar;
  }

  return v;
}

Vector& Vector::operator=(const Vector& other)
{
  uint8_t i;

  appAssert(other.size == size);

  for(i = 0; i < size; i++){
    data[i] = other.data[i];
  }

  return *this;
}

Vector Vector::operator+(const Vector& other)
{
  appAssert(other.size == size);
  uint8_t i;
  Vector v = Vector(*this);

  for(i = 0; i < size; i++){
    v.data[i] += other.data[i];
  }

  return v;
}

Vector Vector::operator-(const Vector& other)
{
  appAssert(other.size == size);
  uint8_t i;
  Vector v = Vector(*this);

  for(i = 0; i < size; i++){
    v.data[i] -= other.data[i];
  }

  return v;
}

Vector Vector::operator/(const Vector& other)
{
  appAssert(other.size == size);

  uint8_t i;
  Vector v = Vector(data, size);

  for(i = 0; i < size; i++){
    v.data[i] /= other.data[i];
  }

  return v;
}

Vector Vector::operator*(const Vector& other)
{
  appAssert(other.size == size);

  uint8_t i;
  Vector v = Vector(data, size);

  for(i = 0; i < size; i++){
    v.data[i] /= other.data[i];
  }

  return v;
}

vpart_t Vector::dot(const Vector& other)
{
  vpart_t ret = 0;
  uint8_t i;

  for(i = 0; i < size; i++){
    ret += data[i] * other.data[i];
  }

  return ret;
}

Vector Vector::minimum(const Vector& other)
{
  appAssert(other.size == size);
  Vector v = Vector(data, size);
  uint8_t i;

  for(i = 0; i < size; i++){
    if(v.data[i] > other.data[i]){
      v.data[i] = other.data[i];
    }
  }

  return v;
}

Vector Vector::maximum(const Vector& other)
{
  appAssert(other.size == size);
  Vector v = Vector(data, size);
  uint8_t i;

  for(i = 0; i < size; i++){
    if(v.data[i] < other.data[i]){
      v.data[i] = other.data[i];
    }
  }

  return v;
}

Vector Vector::normalize()
{
  Vector v = Vector(data, size);
  return v / length();
}

void Vector::println(Print &p)
{
  print(p);
  p.println();
}

void Vector::print(Print &p)
{
  uint8_t i;

  p.print(F("Vec"));
  p.print(size);
  p.print(F("("));
  for(i = 0; i < size; i++){
    if(i){
      p.print(F("x"));
    }
    p.print(data[i], 4);
  }
  p.print(F(")"));
}

Vector Vector::xy()
{
  return Vector(data, 2);
}

Vector::~Vector()
{
  free(data);
}

Vector4::Vector4(vpart_t x, vpart_t y, vpart_t z, vpart_t a) : Vector(4){
  data[0] = x;
  data[1] = y;
  data[2] = z;
  data[3] = a;
}

Vector3::Vector3(vpart_t x, vpart_t y, vpart_t z) : Vector(3){
  data[0] = x;
  data[1] = y;
  data[2] = z;
}
