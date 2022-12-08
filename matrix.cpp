/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                         *
 *  {description}                                                          *
 *  Copyright (C) 2022  Łukasz "Kuszki" Dróżdż  lukasz.kuszki@gmail.com    *
 *                                                                         *
 *  This program is free software: you can redistribute it and/or modify   *
 *  it under the terms of the GNU General Public License as published by   *
 *  the  Free Software Foundation, either  version 3 of the  License, or   *
 *  (at your option) any later version.                                    *
 *                                                                         *
 *  This  program  is  distributed  in the hope  that it will be useful,   *
 *  but WITHOUT ANY  WARRANTY;  without  even  the  implied  warranty of   *
 *  MERCHANTABILITY  or  FITNESS  FOR  A  PARTICULAR  PURPOSE.  See  the   *
 *  GNU General Public License for more details.                           *
 *                                                                         *
 *  You should have  received a copy  of the  GNU General Public License   *
 *  along with this program. If not, see http://www.gnu.org/licenses/.     *
 *                                                                         *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#ifndef MATRIX_CPP
#define MATRIX_CPP

#ifndef MATRIX_HPP
#include "matrix.hpp"
#endif

template<typename data>
matrix<data>::matrix(const std::initializer_list<data>& list)
{
	resize(1, list.size()); size_t i = 0;

	for (const auto& n : list) set_val(0, i++, n);
}

template<typename data>
matrix<data>::matrix(const std::initializer_list<std::initializer_list<data>>& list)
{
	resize(list.size(), (*list.begin()).size());

	for (size_t i = 0; const auto& r : list)
	{
		for (size_t j = 0; const auto& n : r) set_val(i, j++, n); ++i;
	}
}

template<typename data>
matrix<data>::matrix(size_t rows, size_t cols, const data ptr[])
{
	resize(rows, cols); const size_t count = rows*cols;

     #pragma omp parallel for
	for (size_t i = 0; i < count; ++i) m_ptr[i] = ptr[i];
}

template<typename data>
matrix<data>::matrix(size_t rows, size_t cols, const data& val)
{
	resize(rows, cols); const size_t count = rows*cols;

     #pragma omp parallel for
	for (size_t i = 0; i < count; ++i) m_ptr[i] = val;
}

template<typename data>
matrix<data>::matrix(size_t rows, size_t cols, const std::initializer_list<data>& list)
{
	resize(rows, cols); const size_t count = rows*cols;

	if (auto j = list.begin(); list.size() == count)
		for (size_t i = 0; i < count; ++i) m_ptr[i] = *j++;
}

template<typename data>
matrix<data>::matrix(const matrix<data>& other)
{
	*this = other;
}

template<typename data>
matrix<data>::matrix(matrix<data>&& other)
{
	*this = std::move(other);
}

template<typename data> template<typename type>
matrix<data>::matrix(const matrix<type>& other)
{
	*this = other;
}

template<typename data>
data& matrix<data>::get_val(size_t row, size_t col)
{
	return m_ptr[m_cols*row + col];
}

template<typename data>
data matrix<data>::get_val(size_t row, size_t col, data def) const
{
	if (row >= m_rows || col >= m_cols) return def;
	else return m_ptr[m_cols*row + col];
}

template<typename data>
bool matrix<data>::set_val(size_t row, size_t col, const data& val)
{
	if (row >= m_rows || col >= m_cols) return false;
	else m_ptr[m_cols*row + col] = val;

	return true;
}

template<typename data>
data matrix<data>::mean(size_t n, matrix<data>::mode mod) const
{
	data out = data();

	if (m_ptr == nullptr) return out;
	else switch (mod)
	{
		case mode::all:
		{
			const size_t count = m_rows * m_cols;

			if (count <= 1) return m_ptr[0];

               #pragma omp parallel for reduction(+:out)
			for (size_t i = 0; i < count; ++i)
			{
				out += m_ptr[i];
			}

			return out / data(count);
		}
		break;
		case mode::rows:
		{
			if (n >= m_rows) return out;
			else if (m_cols <= 1) return get_val(n, m_cols);

               #pragma omp parallel for reduction(+:out)
			for (size_t i = 0; i < m_cols; ++i)
			{
				out += get_val(n, i);
			}

			return out / data(m_cols);
		}
		break;
		case mode::cols:
		{
			if (n >= m_cols) return out;
			else if (m_rows <= 1) return get_val(m_rows, n);

               #pragma omp parallel for reduction(+:out)
			for (size_t i = 0; i < m_rows; ++i)
			{
				out += get_val(i, n);
			}

			return out / data(m_rows);
		}
		break;
	}

	return out;
}

template<typename data>
data matrix<data>::var(size_t n, matrix<data>::mode mod) const
{
	const data m = mean(n, mod);
	data out = data();

	if (m_ptr == nullptr) return out;
	else switch (mod)
	{
		case mode::all:
		{
			const size_t count = m_rows * m_cols;

			if (count == 1) return data();

               #pragma omp parallel for reduction(+:out)
			for (size_t i = 0; i < count; ++i)
			{
				data diff = m_ptr[i] - m;

				diff = diff * diff;
				out += diff;
			}

			return out / data(count - 1);
		}
		break;
		case mode::rows:
		{
			if (n >= m_rows) return out;
			else if (m_cols == 1) return data();

               #pragma omp parallel for reduction(+:out)
			for (size_t i = 0; i < m_cols; ++i)
			{
				data diff = get_val(n, i) - m;

				diff = diff * diff;
				out += diff;
			}

			return out / data(m_cols - 1);
		}
		break;
		case mode::cols:
		{
			if (n >= m_cols) return out;
			else if (m_rows == 1) return data();

               #pragma omp parallel for reduction(+:out)
			for (size_t i = 0; i < m_rows; ++i)
			{
				data diff = get_val(i, n) - m;

				diff = diff * diff;
				out += diff;
			}

			return out / data(m_rows - 1);
		}
		break;
	}

	return out;
}

template<typename data>
data matrix<data>::std(size_t n, matrix<data>::mode mod) const
{
	return std::sqrt(var(n, mod));
}

template<typename data>
data matrix<data>::det(void) const
{
	if (m_rows != m_cols) return data();
	else if (m_rows == 1) return m_ptr[0];
	else if (m_rows == 2) return
	          m_ptr[0] * m_ptr[3] -
	          m_ptr[1] * m_ptr[2];

	data out = data();

     #pragma omp parallel for reduction(+: out)
	for (size_t i = 0; i < m_rows; ++i)
	{
		const data mul = (i+1) % 2 ? 1 : -1;

		out += mul * get_val(0, i) * submatrix(0, i).det();
	}

	return out;
}

template<typename data>
bool matrix<data>::resize(size_t rows, size_t cols)
{
	if (rows > 0 && cols > 0) clear();
	else return false;

	const size_t count = cols*rows;

	m_ptr = new data[count];
	m_cols = cols;
	m_rows = rows;

	return true;
}

template<typename data>
bool matrix<data>::clear(void)
{
	if (m_ptr) delete [] m_ptr;
	else return false;

	m_cols = m_rows = 0;

	return true;
}

template<typename data>
bool matrix<data>::is_valid(size_t row, size_t col) const
{
	return row < m_rows && col < m_cols;
}

template<typename data>
bool matrix<data>::is_empty(void) const
{
	return m_ptr == nullptr;
}

template<typename data>
bool matrix<data>::is_valid(void) const
{
	return m_ptr != nullptr;
}

template<typename data>
bool matrix<data>::is_vector(void) const
{
	return m_rows == 1 || m_cols == 1;
}

template<typename data>
bool matrix<data>::is_square(void) const
{
	return m_rows == m_cols;
}

template<typename data>
size_t matrix<data>::rows(void) const
{
	return m_rows;
}

template<typename data>
size_t matrix<data>::cols(void) const
{
	return m_cols;
}

template<typename data>
size_t matrix<data>::size(void) const
{
	return m_rows * m_cols;
}

template<typename data>
matrix<data> matrix<data>::submatrix(size_t row, size_t col) const
{
	if (m_cols < 2 || m_rows < 2) return matrix<data>();
	else if (row >= m_rows || col >= m_cols) return *this;

	matrix<data> res(m_rows - 1, m_cols - 1);

	//#pragma omp parallel for collapse(2)
	for (size_t i = 0; i < m_rows; ++i)
		for (size_t j = 0; j < m_cols; ++j)
		{
			if (i == row || j == col) continue;

			const size_t r = i < row ? i : i - 1;
			const size_t c = j < col ? j : j - 1;

			res.set_val(r, c, get_val(i, j));
		}

	return res;
}

template<typename data>
matrix<data> matrix<data>::transpose(void) const
{
	matrix tmp(m_cols, m_rows);

     #pragma omp parallel for collapse(2)
	for (size_t i = 0; i < m_rows; ++i)
		for (size_t j = 0; j < m_cols; ++j)
			tmp.set_val(j, i, get_val(i, j));

	return tmp;
}

template<typename data>
matrix<data> matrix<data>::diagonal(void) const
{
	if (m_rows != m_cols) return matrix<data>();

	matrix tmp(1, m_cols);

     #pragma omp parallel for
	for (size_t i = 0; i < m_cols; ++i)
		tmp.set_val(0, i, get_val(i, i));

	return tmp;
}

template<typename data>
matrix<data> matrix<data>::get_row(size_t n) const
{
	if (n >= m_rows) return matrix<data>();

	matrix res(1, m_cols);

     #pragma omp parallel for
	for (size_t i = 0; i < m_cols; ++i)
		res.m_ptr[i] = get_val(n, i);

	return res;
}

template<typename data>
matrix<data> matrix<data>::get_col(size_t n) const
{
	if (n >= m_cols) return matrix<data>();

	matrix res(m_rows, 1);

     #pragma omp parallel for
	for (size_t i = 0; i < m_rows; ++i)
		res.m_ptr[i] = get_val(i, n);

	return res;
}

template<typename data>
data& matrix<data>::operator()(size_t row, size_t col)
{
	return get_val(row, col);
}

template<typename data>
data matrix<data>::operator()(size_t row, size_t col) const
{
	return get_val(row, col);
}

template<typename data>
matrix<data> matrix<data>::operator- (void) const
{
	const size_t count = m_rows * m_cols;
	matrix res(m_rows, m_cols);

     #pragma omp parallel for
	for (size_t i = 0; i < count; ++i) res.m_ptr[i] = -m_ptr[i];

	return res;
}

template<typename data> template<typename type>
bool matrix<data>::set_row(size_t n, const matrix<type>& other)
{
	if (n >= m_rows) return false;
	if (other.m_rows != 1 && other.m_cols != 1) return false;
	if (other.m_rows != m_cols && other.m_cols != m_cols) return false;

     #pragma omp parallel for
	for (size_t i = 0; i < m_cols; ++i) set_val(n, i, other.m_ptr[i]);

	return true;
}

template<typename data> template<typename type>
bool matrix<data>::set_col(size_t n, const matrix<type>& other)
{
	if (n >= m_cols) return false;
	if (other.m_rows != 1 && other.m_cols != 1) return false;
	if (other.m_rows != m_rows && other.m_cols != m_rows) return false;

     #pragma omp parallel for
	for (size_t i = 0; i < m_rows; ++i) set_val(i, n, other.m_ptr[i]);

	return true;
}

template<typename data> template<typename type>
matrix<data>& matrix<data>::operator= (const matrix<type>& other)
{
	if (static_cast<const void*>(this) == &other) return *this;
	else resize(other.m_rows, other.m_cols);

	const size_t count = m_rows * m_cols;

     #pragma omp parallel for
	for (size_t i = 0; i < count; ++i)
		m_ptr[i] = other.m_ptr[i];

	return *this;
}

template<typename data>
matrix<data>& matrix<data>::operator= (const matrix<data>& other)
{
	if (static_cast<const void*>(this) == &other) return *this;
	else resize(other.m_rows, other.m_cols);

	const size_t count = m_rows * m_cols;

     #pragma omp parallel for
	for (size_t i = 0; i < count; ++i)
		m_ptr[i] = other.m_ptr[i];

	return *this;
}

template<typename data>
matrix<data>& matrix<data>::operator= (matrix<data>&& other)
{
	if (&other == this) return *this;

	m_cols = other.m_cols;
	m_rows = other.m_rows;
	m_ptr = other.m_ptr;

	other.m_ptr = nullptr;

	return *this;
}

template<typename data> template<typename type>
matrix<data> matrix<data>::operator+ (const matrix<type>& other) const
{
	if (m_rows != other.m_rows ||
	    m_cols != other.m_cols) return *this;

	matrix<data> out(m_rows, m_cols);

	const size_t count = m_rows * m_cols;

     #pragma omp parallel for
	for (size_t i = 0; i < count; ++i)
		out.m_ptr[i] += m_ptr[i] + other.m_ptr[i];

	return out;
}

template<typename data> template<typename type>
matrix<data> matrix<data>::operator- (const matrix<type>& other) const
{
	if (m_rows != other.m_rows ||
	    m_cols != other.m_cols) return *this;

	matrix<data> out(m_rows, m_cols);

	const size_t count = m_rows * m_cols;

     #pragma omp parallel for
	for (size_t i = 0; i < count; ++i)
		out.m_ptr[i] += m_ptr[i] - other.m_ptr[i];

	return out;
}

template<typename data> template<typename type>
matrix<data> matrix<data>::operator* (const matrix<type>& other) const
{
	if (m_cols != other.m_rows) return *this;

	matrix<data> res(m_rows, other.m_cols);

     #pragma omp parallel for collapse(2)
	for (size_t i = 0; i < res.m_cols; ++i)
		for (size_t j = 0; j < res.m_rows; ++j)
			for (size_t k = 0; k < m_cols; ++k)
				res.get_val(j, i) += get_val(j, k) * other.get_val(k, i);

	return res;
}

template<typename data> template<typename type>
matrix<data> matrix<data>::operator+ (const type& other) const
{
	matrix<data> res(m_rows, m_cols);

	const size_t count = m_rows * m_cols;

     #pragma omp parallel for
	for (size_t i = 0; i < count; ++i)
		res.m_ptr[i] = m_ptr[i] + other;

	return res;
}

template<typename data> template<typename type>
matrix<data> matrix<data>::operator- (const type& other) const
{
	matrix<data> res(m_rows, m_cols);

	const size_t count = m_rows * m_cols;

     #pragma omp parallel for
	for (size_t i = 0; i < count; ++i)
		res.m_ptr[i] = m_ptr[i] - other;

	return res;
}

template<typename data> template<typename type>
matrix<data> matrix<data>::operator* (const type& other) const
{
	matrix<data> res(m_rows, m_cols);

	const size_t count = m_rows * m_cols;

     #pragma omp parallel for
	for (size_t i = 0; i < count; ++i)
		res.m_ptr[i] = m_ptr[i] * other;

	return res;
}

template<typename data> template<typename type>
matrix<data> matrix<data>::operator/ (const type& other) const
{
	matrix<data> res(m_rows, m_cols);

	const size_t count = m_rows * m_cols;

     #pragma omp parallel for
	for (size_t i = 0; i < count; ++i)
		res.m_ptr[i] = m_ptr[i] / other;

	return res;
}

template<typename data> template<typename type>
matrix<data>& matrix<data>::operator+= (const matrix<type>& other)
{
	if (m_rows != other.m_rows ||
	    m_cols != other.m_cols) return *this;

	const size_t count = m_rows * m_cols;

     #pragma omp parallel for
	for (size_t i = 0; i < count; ++i)
		m_ptr[i] += other.m_ptr[i];

	return *this;
}

template<typename data> template<typename type>
matrix<data>& matrix<data>::operator-= (const matrix<type>& other)
{
	if (m_rows != other.m_rows ||
	    m_cols != other.m_cols) return *this;

	const size_t count = m_rows * m_cols;

     #pragma omp parallel for
	for (size_t i = 0; i < count; ++i)
		m_ptr[i] -= other.m_ptr[i];

	return *this;
}

template<typename data> template<typename type>
matrix<data>& matrix<data>::operator*= (const matrix<type>& other)
{
	if (m_cols != other.m_rows) return *this;

	matrix<data> res(m_rows, other.m_cols);

     #pragma omp parallel for collapse(2)
	for (size_t i = 0; i < res.m_cols; ++i)
		for (size_t j = 0; j < res.m_rows; ++j)
			for (size_t k = 0; k < m_cols; ++k)
				res.get_val(j, i) += get_val(j, k)*other.get_val(k, i);

	return *this = res;
}

template<typename data> template<typename type>
matrix<data>& matrix<data>::operator+= (const type& other)
{
	const size_t count = m_rows * m_cols;

     #pragma omp parallel for
	for (size_t i = 0; i < count; ++i)
		m_ptr[i] += other;

	return *this;
}

template<typename data> template<typename type>
matrix<data>& matrix<data>::operator-= (const type& other)
{
	const size_t count = m_rows * m_cols;

     #pragma omp parallel for
	for (size_t i = 0; i < count; ++i)
		m_ptr[i] -= other;

	return *this;
}

template<typename data> template<typename type>
matrix<data>& matrix<data>::operator*= (const type& other)
{
	const size_t count = m_rows * m_cols;

     #pragma omp parallel for
	for (size_t i = 0; i < count; ++i)
		m_ptr[i] *= other;

	return *this;
}

template<typename data> template<typename type>
matrix<data>& matrix<data>::operator/= (const type& other)
{
	const size_t count = m_rows * m_cols;

     #pragma omp parallel for
	for (size_t i = 0; i < count; ++i)
		m_ptr[i] /= other;

	return *this;
}

template<typename data> template<typename type>
bool matrix<data>::operator== (const matrix<type>& other) const
{
	if (m_rows != other.m_rows || m_cols != other.m_cols) return false;

	const size_t count = m_rows * m_cols;
	for (size_t i = 0; i < count; ++i)
		if (m_ptr[i] != other.m_ptr[i]) return false;

	return true;
}

template<typename data> template<typename type>
bool matrix<data>::operator!= (const matrix<type>& other) const
{
	if (m_rows != other.m_rows || m_cols != other.m_cols) return true;

	const size_t count = m_rows * m_cols;
	for (size_t i = 0; i < count; ++i)
		if (m_ptr[i] != other.m_ptr[i]) return true;

	return false;
}

template<typename data>
matrix<data>::~matrix(void)
{
	if (m_ptr) delete [] m_ptr;
}

template class matrix<float>;
template class matrix<double>;
template class matrix<long double>;

template class matrix<int>;
template class matrix<unsigned>;

template class matrix<short int>;
template class matrix<short unsigned>;

template class matrix<long int>;
template class matrix<long unsigned>;

template class matrix<long long int>;
template class matrix<long long unsigned>;

#endif
