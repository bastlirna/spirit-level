#include "matrix.h"
#include "app_asserts.h"

#define indexPart(r, c) ((colls * r) + c)
#define indexPartN(r, c, n) ((n * r) + c)

Matrix::Matrix(uint8_t _rows, uint8_t _colls) : rows(_rows), colls(_colls)
{
  data = (vpart_t*)malloc(sizeof(vpart_t) * rows * colls);
  fill(0);
}

Matrix::Matrix(vpart_t *_data, uint8_t _rows, uint8_t _colls) : rows(_rows), colls(_colls)
{
  uint8_t r, c;
  data = (vpart_t*)malloc(sizeof(vpart_t) * rows * colls);

  for(r = 0; r < rows; r++){
    for(c = 0; c < colls; c++){
      data[indexPart(r, c)] = _data[(colls * r) + c];
    }
  }
}

Matrix Matrix::part(uint8_t _rows, uint8_t _colls)
{
  appAssert(_rows <= rows);
  appAssert(_colls <= colls);

  Matrix matrix(_rows, _colls);

  uint8_t r, c;
  for(r = 0; r < _rows; r++){
    for(c = 0; c < _colls; c++){
      matrix.data[indexPartN(r, c, matrix.colls)] = data[indexPart(r, c)];
    }
  }

  return matrix;
}

Vector Matrix::solve(Vector &vect)
{
  appAssert(vect.size == rows);

  Vector ret(colls);
  uint8_t r, c;
  vpart_t d = det();

  appAssert(d != 0);

  if(d == 0){
    return ret;
  }

  for(c = 0; c < colls; c++)
  {
    Matrix m(*this);
    for(r = 0; r < rows; r++){
      m.data[indexPart(r, c)] = vect[r];
    }
    ret[c] = m.det() / d;
  }

  return ret;
}

Vector Matrix::solve()
{
  Vector vect(rows);
  return solve(vect);
}

Matrix::Matrix(const Matrix& other) : Matrix(other.data, other.rows, other.colls)
{
}

void Matrix::fill(vpart_t val)
{
  uint8_t r, c;
  for(r = 0; r < rows; r++){
    for(c = 0; c < colls; c++){
      data[indexPart(r, c)] = val;
    }
  }
}

void Matrix::diagonalOne()
{
  uint8_t r, c;
  for(r = 0; r < rows; r++){
    for(c = 0; c < colls; c++){
      data[indexPart(r, c)] = (r == c?1:0);
    }
  }
}

void Matrix::save(vpart_t *ptr, uint8_t size)
{
  appAssert(size == (rows * colls));
  uint8_t r, c;
  for(r = 0; r < rows; r++){
    for(c = 0; c < colls; c++){
      *ptr = data[indexPart(r, c)];
      ptr++;
    }
  }
}

void Matrix::load(vpart_t *ptr, uint8_t size)
{
  appAssert(size == (rows * colls));
  uint8_t r, c;
  for(r = 0; r < rows; r++){
    for(c = 0; c < colls; c++){
      data[indexPart(r, c)] = *ptr;
      ptr++;
    }
  }
}


void Matrix::transpose()
{
  vpart_t *newdata = (vpart_t*)malloc(sizeof(vpart_t) * rows * colls);
  uint8_t tmp = rows;
  uint8_t r, c;

  for(r = 0; r < rows; r++){
    for(c = 0; c < colls; c++){
      newdata[indexPartN(c, r, rows)] = data[indexPart(r, c)];
    }
  }

  rows = colls;
  colls = tmp;
  free(data);
  data = newdata;
}

Matrix Matrix::subMatrix(uint8_t row, uint8_t collumn)
{
  uint8_t r, c;
  Matrix mat(colls - 1, rows - 1);

  for(r = 0; r < rows; r++){
    for(c = 0; (c < colls) && (r != row); c++){
      if(c != collumn){
        mat.data[indexPartN((r<row?r:r-1), (c<collumn?c:c-1), mat.colls)] = data[indexPart(r, c)];
      }
    }
  }

  return mat;
}

vpart_t Matrix::det()
{
  appAssert(colls == rows);

  switch(colls){
    case 0:
      return 0;
    case 1:
      return data[0];
    case 2:
      return data[indexPart(0, 0)] * data[indexPart(1, 1)] - data[indexPart(0, 1)] * data[indexPart(1, 0)];
  }

  // Laplace
  uint8_t r;
  vpart_t det = 0;

  for(r = 0; r < rows; r++){
    vpart_t subDet = this->subMatrix(r, 0).det();
    vpart_t val = data[indexPart(r, 0)];
    if(val != 0){
      det += ((vpart_t)pow(-1, r)) * val * subDet;
    }
  }

  return det;
}

Matrix Matrix::inv()
{
  appAssert(colls == rows);
  Matrix mat(colls, rows);
  uint8_t r, c;

  vpart_t det = 0;

  for(r = 0; r < rows; r++){
    for(c = 0; c < colls; c++){
      vpart_t p = pow(-1, r + c);
      vpart_t subDet = this->subMatrix(c, r).det();
      mat.data[indexPart(r, c)] = subDet * p;
      if(c == 0){
        det += p * data[indexPart(c, r)] * subDet;
      }
    }
  }

  if(det == 0){
    return *this;
  }

  return mat / det;
}

Matrix& Matrix::operator/(vpart_t scalar)
{
  appAssert(scalar != 0);
  uint8_t r, c;

  for(r = 0; r < rows; r++){
    for(c = 0; c < colls; c++){
      data[indexPart(r, c)] /= scalar;
    }
  }

  return *this;
}

Matrix& Matrix::operator*(vpart_t scalar)
{
  uint8_t r, c;

  for(r = 0; r < rows; r++){
    for(c = 0; c < colls; c++){
      data[indexPart(r, c)] *= scalar;
    }
  }

  return *this;
}

Matrix Matrix::operator*(Matrix &other)
{
  appAssert(colls == other.rows);

  uint8_t r, c, i;
  Matrix mat(rows, other.colls);

  for(r = 0; r < mat.rows; r++){

    for(c = 0; c < mat.colls; c++){
      vpart_t sum = 0;

      for(i = 0; i < colls; i++){
      /*    Serial.print(F("a["));
          Serial.print(r);
          Serial.print(F("x"));
          Serial.print(i);
          Serial.print(F("] * b["));
          Serial.print(i);
          Serial.print(F("x"));
          Serial.print(c);
          Serial.print(F("] => "));

          Serial.print(data[indexPart(r, i)],8);
          Serial.print(F(" * "));
          Serial.print(other.data[indexPartN(i, c, mat.colls)],8);
          Serial.print(F(" = "));
          Serial.println(data[indexPart(r, i)] * other.data[indexPartN(i, c, mat.colls)],8);
*/
          sum += data[indexPart(r, i)] * other.data[indexPartN(i, c, mat.colls)];
      }
      mat.data[indexPartN(r, c, mat.colls)] = sum;
      //delay(20000);
    }
  }

  return mat;
}

RowProxy Matrix::operator[](uint8_t i)
{
  appAssert(i < rows);
  RowProxy row(&data[indexPart(i, 0)], colls);
  return row;
}

void Matrix::println(Print &p)
{
  print(p);
  p.println();
}

void Matrix::print(Print &p)
{
  uint8_t r, c;

  p.print(F("Mat"));
  p.print(rows);
  p.print(F("x"));
  p.print(colls);
  p.print(F(":"));
  p.println(F("  "));

  for(r = 0; r < rows; r++){
    if(r){
      p.println(F("  "));
    }
    for(c = 0; c < colls; c++){
      if(c){
        p.print(F(" "));
      }
      p.print(data[indexPart(r, c)], 4);
    }
  }
}

RowProxy::RowProxy(vpart_t *_data, uint8_t _size) : data(_data), size(_size)
{
}

RowProxy& RowProxy::operator=(Vector& other)
{
  uint8_t i;

  for(i = 0; i < min(size, other.size); i++){
    data[i] = other[i];
  }

  return *this;
}

Vector RowProxy::vector()
{
  return Vector(data, size);
}

vpart_t& RowProxy::operator[](uint8_t i)
{
  appAssert(i < size);
  return data[i];
}

Matrix::~Matrix()
{
  free(data);
}
