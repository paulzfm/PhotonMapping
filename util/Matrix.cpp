#include "Matrix.h"
#include <iomanip>

Matrix::Matrix(const Matrix& mat)
{
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            x[i][j] = mat.x[i][j];
        }
    }
}

void Matrix::inverse()
{
    double det = determinant();
    Matrix inverse;

    inverse.x[0][0] =  det3(x[1][1], x[1][2], x[1][3],
                            x[2][1], x[2][2], x[2][3],
                            x[3][1], x[3][2], x[3][3]) / det;
    inverse.x[0][1] = -det3(x[0][1], x[0][2], x[0][3], 
                            x[2][1], x[2][2], x[2][3], 
                            x[3][1], x[3][2], x[3][3]) / det;
    inverse.x[0][2] =  det3(x[0][1], x[0][2], x[0][3],
                            x[1][1], x[1][2], x[1][3], 
                            x[3][1], x[3][2], x[3][3]) / det;
    inverse.x[0][3] = -det3(x[0][1], x[0][2], x[0][3], 
                            x[1][1], x[1][2], x[1][3], 
                            x[2][1], x[2][2], x[2][3]) / det;

    inverse.x[1][0] = -det3(x[1][0], x[1][2], x[1][3], 
                            x[2][0], x[2][2], x[2][3], 
                            x[3][0], x[3][2], x[3][3]) / det;
    inverse.x[1][1] =  det3(x[0][0], x[0][2], x[0][3], 
                            x[2][0], x[2][2], x[2][3], 
                            x[3][0], x[3][2], x[3][3]) / det;
    inverse.x[1][2] = -det3(x[0][0], x[0][2], x[0][3], 
                            x[1][0], x[1][2], x[1][3], 
                            x[3][0], x[3][2], x[3][3]) / det;
    inverse.x[1][3] =  det3(x[0][0], x[0][2], x[0][3], 
                            x[1][0], x[1][2], x[1][3], 
                            x[2][0], x[2][2], x[2][3]) / det;

    inverse.x[2][0] =  det3(x[1][0], x[1][1], x[1][3], 
                            x[2][0], x[2][1], x[2][3], 
                            x[3][0], x[3][1], x[3][3]) / det;
    inverse.x[2][1] = -det3(x[0][0], x[0][1], x[0][3], 
                            x[2][0], x[2][1], x[2][3], 
                            x[3][0], x[3][1], x[3][3]) / det;
    inverse.x[2][2] =  det3(x[0][0], x[0][1], x[0][3], 
                            x[1][0], x[1][1], x[1][3], 
                            x[3][0], x[3][1], x[3][3]) / det;
    inverse.x[2][3] = -det3(x[0][0], x[0][1], x[0][3], 
                            x[1][0], x[1][1], x[1][3], 
                            x[2][0], x[2][1], x[2][3]) / det;

    inverse.x[3][0] = -det3(x[1][0], x[1][1], x[1][2], 
                            x[2][0], x[2][1], x[2][2], 
                            x[3][0], x[3][1], x[3][2]) / det;
    inverse.x[3][1] =  det3(x[0][0], x[0][1], x[0][2], 
                            x[2][0], x[2][1], x[2][2], 
                            x[3][0], x[3][1], x[3][2]) / det;
    inverse.x[3][2] = -det3(x[0][0], x[0][1], x[0][2], 
                            x[1][0], x[1][1], x[1][2], 
                            x[3][0], x[3][1], x[3][2]) / det;
    inverse.x[3][3] =  det3(x[0][0], x[0][1], x[0][2], 
                            x[1][0], x[1][1], x[1][2], 
                            x[2][0], x[2][1], x[2][2]) / det;

    *this = inverse;
}

Matrix Matrix::getInverse() const
{
    Matrix ret = *this;
    ret.inverse();
    return ret;
}

void Matrix::transpose()
{
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            double tmp = x[i][j];
            x[i][j] = x[j][i];
            x[j][i] = tmp;
        }
    }
}

Matrix Matrix::getTranspose() const
{
    Matrix ret = *this;
    ret.transpose();
    return ret;
}

double Matrix::determinant() const
{
    double det;
    det =  x[0][0] * det3(x[1][1], x[1][2], x[1][3],
                          x[2][1], x[2][2], x[2][3],
                          x[3][1], x[3][2], x[3][3]);
    det -= x[0][1] * det3(x[1][0], x[1][2], x[1][3], 
                          x[2][0], x[2][2], x[2][3],
                          x[3][0], x[3][2], x[3][3]);
    det += x[0][2] * det3(x[1][0], x[1][1], x[1][3], 
                          x[2][0], x[2][1], x[2][3],
                          x[3][0], x[3][1], x[3][3]);
    det -= x[0][3] * det3(x[1][0], x[1][1], x[1][2], 
                          x[2][0], x[2][1], x[2][2],
                          x[3][0], x[3][1], x[3][2]);
    return det;
}

Matrix Matrix::operator + (const Matrix& that)
{
    Matrix mat;
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            mat.x[i][j] = this->x[i][j] + that.x[i][j];
        }
    }
    return mat;
}

Matrix Matrix::operator - (const Matrix& that)
{
    Matrix mat;
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            mat.x[i][j] = this->x[i][j] - that.x[i][j];
        }
    }
    return mat;
}

Matrix Matrix::operator * (const Matrix& that)
{
    Matrix mat;
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            double sum = 0;
            for (int k = 0; k < 4; k++) {
                sum += this->x[i][k] * that.x[k][j];
            }
            mat.x[i][j] = sum;
        }
    }
    return mat;
}

Matrix Matrix::operator * (double t)
{
    Matrix mat;
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            mat.x[i][j] = this->x[i][j] * t;
        }
    }
    return mat;
}

Vector Matrix::operator * (const Vector& v)
{
    Vector ret;
    ret.x  = v[0] * this->x[0][0] + v[1] * this->x[0][1] 
           + v[2] * this->x[0][2] + this->x[0][3];
    ret.y  = v[0] * this->x[1][0] + v[1] * this->x[1][1] 
           + v[2] * this->x[1][2] + this->x[1][3];
    ret.z  = v[0] * this->x[2][0] + v[1] * this->x[2][1] 
           + v[2] * this->x[2][2] + this->x[2][3];
    ret /= v[0] * this->x[3][0] + v[1] * this->x[3][1] 
         + v[2] * this->x[3][2] + this->x[3][3];
    return ret;
}

void Matrix::operator += (const Matrix& that)
{
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            this->x[i][j] += that.x[i][j];
        }
    }
}

void Matrix::operator -= (const Matrix& that)
{
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            this->x[i][j] -= that.x[i][j];
        }
    }
}

void Matrix::operator *= (const Matrix& that)
{
    *this = *this * that;
}

void Matrix::operator *= (double t)
{
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            this->x[i][j] *= t;
        }
    }
}

double Matrix::det3(double a, double b, double c,
                    double d, double e, double f,
                    double g, double h, double i)
{
    return a * e * i + d * h * c + g * b * f 
         - g * e * c - d * b * i - a * h * f; 
}

Matrix Matrix::zero()
{
    Matrix ret;
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            ret.x[i][j] = 0;
        }
    }
    return ret;
}

Matrix Matrix::identity()
{
    Matrix ret;
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if (i == j) {
                ret.x[i][j] = 1;
            } else {
                ret.x[i][j] = 0;
            }
        }
    }
    return ret;
}

Matrix Matrix::view(const Vector& eye, const Vector& gaze, const Vector& up)
{
    Matrix ret;
    return ret;
}

std::ostream& operator << (std::ostream& os, const Matrix& mat)
{
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            os << std::setprecision(3) << std::setw(8) << mat.x[i][j];
        }
        os << std::endl;
    }

    return os;
}

