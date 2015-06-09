// 3 by 3 or 4 by 4 Matrix, assume it is nonsingular.

#ifndef MATRIX_H_
#define MATRIX_H_

#include "Vector.h"
#include <iostream>

class Matrix
{
public:
    Matrix() {}
    Matrix(const Matrix& mat);

    void inverse();        // M = M^{-1}
    void transpose();      // M = M^T
    Matrix getInverse() const;
    Matrix getTranspose() const;
    
    double determinant() const;  // det(M)

    Matrix operator + (const Matrix& that);  // M1 + M2
    Matrix operator - (const Matrix& that);  // M1 - M2
    Matrix operator * (const Matrix& that);  // M1 * M2
    Matrix operator * (double t);            // tM
    Vector operator * (const Vector& v);     // M * v

    void operator += (const Matrix& that);
    void operator -= (const Matrix& that);
    void operator *= (const Matrix& that);
    void operator *= (double t);

    static Matrix zero();       // M_ij = 0
    static Matrix identity();   // M_ii = 1
    static Matrix view(const Vector& eye, const Vector& gaze, const Vector& up);

    friend std::ostream& operator << (std::ostream& os, const Matrix& mat);

    double x[4][4];

private:
    static double det3(double a, double b, double c,
                       double d, double e, double f,
                       double g, double h, double i);
};

#endif