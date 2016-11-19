#ifndef _MATRIX_H
#define _MATRIX_H

#include <Arduino.h>
#include "vector.h"

class RowProxy{
public:
  RowProxy(vpart_t *_data, uint8_t _size);
  vpart_t& operator[](uint8_t i);
  RowProxy& operator=(Vector& other);
  Vector vector();
private:
  vpart_t *data;
  uint8_t size;
};

class Matrix{
public:

  Matrix(uint8_t _rows, uint8_t _colls);
  Matrix(vpart_t *_data, uint8_t _rows, uint8_t _colls);
  Matrix(const Matrix& other);
  ~Matrix();

  void save(vpart_t *ptr, uint8_t size);
  void load(vpart_t *ptr, uint8_t size);
  void fill(vpart_t val);
  RowProxy operator[](uint8_t i);
  Matrix& operator/(vpart_t scalar);
  Matrix& operator*(vpart_t scalar);
  Matrix operator*(Matrix &other);
  Matrix part(uint8_t rows, uint8_t colls);
  Vector solve(Vector &vect);
  Vector solve();

  void print(Print &p);
  void println(Print &p);

  Matrix inv();
  vpart_t det();
  void transpose();
  void diagonalOne();
private:
  Matrix subMatrix(uint8_t u, uint8_t j);

  vpart_t *data;
  uint8_t rows;
  uint8_t colls;

};




#endif
