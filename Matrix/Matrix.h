#pragma once
#include "MathVector.h"

class Matrix : public MathVector<MathVector<double>>
{
	size_t rows_;
	size_t cols_;
public:
	Matrix(size_t rows = 0, size_t cols = 0, const double* array = nullptr);
	Matrix(size_t rows, size_t cols, std::initializer_list<std::initializer_list<double>> list);
	Matrix(size_t rows, size_t cols, std::initializer_list<double> list);
	Matrix(const Matrix& other);
	Matrix(const MathVector<MathVector<double>>& other);
	Matrix(Matrix&& other) noexcept;
	~Matrix() = default;
	Matrix T() const;
	Matrix operator*(const Matrix& other) const;
	Matrix operator*(double scalar) const;
	size_t rows() const noexcept { return rows_; }
	size_t cols() const noexcept { return cols_; }
	MathVector<double> getRow(size_t row) const; // Отдельный метод для получения копии строки
	MathVector<double> getColumn(size_t col) const; // Отдельный метод для получения копии столбца
	void setRow(size_t row, const MathVector<double>& newRow);
	void setColumn(size_t col, const MathVector<double>& newCol);
	bool isSquare() const noexcept { return rows_ == cols_; }
	double determinant() const;
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

Matrix::Matrix(const MathVector<MathVector<double>>& other): MathVector<MathVector<double>>(other), rows_(other.size()), cols_(other.size() > 0 ? other[0].size() : 0) {
}

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
	Matrix otherT = other.T();
	Matrix result(rows_, other.cols_);
	for (size_t i = 0; i < rows_; ++i) {
		for (size_t j = 0; j < other.cols_; ++j) {
			result[i][j] = (*this)[i] * otherT[j];
		}
	}
	return result;
}

Matrix Matrix::operator*(double scalar) const
{
	Matrix res(*this);
	return res*=scalar;
}

MathVector<double> Matrix::getRow(size_t row) const
{
	if (row >= rows_) {
		throw std::out_of_range("Row index out of range");
	}
	return (*this)[row];
}

MathVector<double> Matrix::getColumn(size_t col) const
{
	if (col >= cols_) {
		throw std::out_of_range("Column index out of range");
	}
	MathVector<double> column(rows_);
	for (size_t i = 0; i < rows_; ++i) {
		column[i] = (*this)[i][col];
	}
	return column;
}

void Matrix::setRow(size_t row, const MathVector<double>& newRow) {
	if (row >= rows_) {
		throw std::out_of_range("Row index out of range");
	}
	if (newRow.size() != cols_) {
		throw std::invalid_argument("New row size does not match matrix column count");
	}
	(*this)[row] = newRow;
}

void Matrix::setColumn(size_t col, const MathVector<double>& newCol) {
	if (col >= cols_) {
		throw std::out_of_range("Column index out of range");
	}
	if (newCol.size() != rows_) {
		throw std::invalid_argument("New column size does not match matrix row count");
	}
	for (size_t i = 0; i < rows_; ++i) {
		(*this)[i][col] = newCol[i];
	}
}

double Matrix::determinant() const
{
	if (rows_ != cols_) {
		throw std::logic_error("Determinant is only defined for square matrices");
	}
	if (rows_ == 1) {
		return (*this)[0][0];
	}
	if (rows_ == 2) {
		return (*this)[0][0] * (*this)[1][1] - (*this)[0][1] * (*this)[1][0];
	}
	// For larger matrices, use cofactor expansion
	double det = 0.0;
	for (size_t j = 0; j < cols_; ++j) {
		Matrix minor(rows_ - 1, cols_ - 1);
		for (size_t i = 1; i < rows_; ++i) {
			size_t k = 0;
			for (size_t l = 0; l < cols_; ++l) {
				if (l != j) {
					minor[i - 1][k] = (*this)[i][l];
					++k;
				}
			}
		}
		det += (*this)[0][j] * minor.determinant() * (j % 2 == 0 ? 1.0 : -1.0);
	}
	return det;
}