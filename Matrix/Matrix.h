#pragma once
#include "MathVector.h"

class Matrix : public MathVector<MathVector<double>>
{
	size_t rows_;
	size_t cols_;
public:
	Matrix(size_t rows, size_t cols, const double* array = nullptr);
	Matrix(size_t rows, size_t cols, std::initializer_list<std::initializer_list<double>> list);
	Matrix(size_t rows, size_t cols, std::initializer_list<double> list);
	Matrix(const Matrix& other);
	Matrix(Matrix&& other) noexcept;
	~Matrix() = default;
	Matrix T() const;
	Matrix operator*(const Matrix& other) const;
	size_t rows() const noexcept { return rows_; }
	size_t cols() const noexcept { return cols_; }
};

std::ostream& operator<<(std::ostream& os, const Matrix& mat) {
	for (size_t i = 0; i < mat.size(); ++i) {
		for (size_t j = 0; j < mat[i].size(); ++j) {
			os << mat[i][j] << " ";
		}
		os << '\n';
	}
	return os;
}

std::istream& operator>>(std::istream& is, Matrix& mat) {
	for (size_t i = 0; i < mat.size(); ++i) {
		is >> mat[i];
	}
	return is;
}

Matrix::Matrix(size_t rows, size_t cols, const double* array) : MathVector<MathVector<double>>(rows), rows_(rows), cols_(cols) {
	for (size_t i = 0; i < rows; ++i) {
		const double* row_data = array ? array + i * cols : nullptr;
		(*this)[i] = MathVector<double>(cols, row_data);
	}
}

Matrix::Matrix(size_t rows, size_t cols, std::initializer_list<std::initializer_list<double>> list): MathVector<MathVector<double>>(rows), rows_(rows), cols_(cols) {
	if (list.size() != rows) {
		throw std::invalid_argument("Initializer list size does not match matrix row count");
	}
	size_t i = 0;
	for (const auto& row : list) {
		if (row.size() != cols) {
			throw std::invalid_argument("Initializer list size does not match matrix column count");
		}
		(*this)[i] = MathVector<double>(row);
		++i;
	}
}

Matrix::Matrix(size_t rows, size_t cols, std::initializer_list<double> list) : MathVector<MathVector<double>>(rows), rows_(rows), cols_(cols) {
	if (list.size() != rows * cols) {
		throw std::invalid_argument("Initializer list size does not match matrix dimensions");
	}
	size_t i = 0;
	for (const auto& row : list) {
		(*this)[i] = MathVector<double>(row);
		++i;
	}
}

Matrix::Matrix(const Matrix& other) : MathVector<MathVector<double>>(other), rows_(other.rows_), cols_(other.cols_) {}

Matrix::Matrix(Matrix&& other) noexcept : MathVector<MathVector<double>>(std::move(other)), rows_(other.rows_), cols_(other.cols_){
	other.rows_ = 0;
	other.cols_ = 0;
}

Matrix Matrix::T() const
{
	Matrix res(cols_, rows_);
	for (size_t i = 0; i < rows_; ++i) {
		for (size_t j = 0; j < cols_; ++j) {
			res[j][i] = (*this)[i][j];
		}
	}
	return res;
}

Matrix Matrix::operator*(const Matrix& other) const
{
	if (cols_ != other.rows_) {
		throw std::invalid_argument("Incompatible matrix dimensions");
	}
	Matrix result(rows_, other.cols_);
	for (size_t i = 0; i < rows_; ++i) {
		for (size_t j = 0; j < other.cols_; ++j) {
			result[i][j] = (*this)[i] * other[j];
		}
	}
	return result;
}
