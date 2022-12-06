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
matrix<data>::matrix(size_t rows, size_t cols, const data& val)
{
	resize(rows, cols); const size_t count = rows*cols;

     #pragma omp parallel for
	for (size_t i = 0; i < count; ++i)
		m_ptr[i] = val;
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
data& matrix<data>::get(size_t row, size_t col)
{
	return m_ptr[m_cols*row + col];
}

template<typename data>
bool matrix<data>::set(size_t row, size_t col, const data& val)
{
	if (row > m_rows || col > m_cols) return false;
	else m_ptr[m_cols*row + col] = val;

	return true;
}

template<typename data>
data matrix<data>::get(size_t row, size_t col) const
{
	return m_ptr[m_cols*row + col];
}

template<typename data>
data matrix<data>::mean(size_t n, matrix<data>::mode mod) const
{
	data out = 0.0;

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
			else if (m_cols <= 1) return get(n, m_cols);

               #pragma omp parallel for reduction(+:out)
			for (size_t i = 0; i < m_cols; ++i)
			{
				out += get(n, i);
			}

			return out / data(m_cols);
		}
		break;
		case mode::cols:
		{
			if (n >= m_cols) return out;
			else if (m_rows <= 1) return get(m_rows, n);

               #pragma omp parallel for reduction(+:out)
			for (size_t i = 0; i < m_rows; ++i)
			{
				out += get(i, n);
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
	data out = 0.0;

	if (m_ptr == nullptr) return out;
	else switch (mod)
	{
		case mode::all:
		{
			const size_t count = m_rows * m_cols;

			if (count == 1) return 0.0;

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
			else if (m_cols == 1) return 0.0;

               #pragma omp parallel for reduction(+:out)
			for (size_t i = 0; i < m_cols; ++i)
			{
				data diff = get(n, i) - m;

				diff = diff * diff;
				out += diff;
			}

			return out / data(m_cols - 1);
		}
		break;
		case mode::cols:
		{
			if (n >= m_cols) return out;
			else if (m_rows == 1) return 0.0;

               #pragma omp parallel for reduction(+:out)
			for (size_t i = 0; i < m_rows; ++i)
			{
				data diff = get(i, n) - m;

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
matrix<data> matrix<data>::transpose(void) const
{
	matrix tmp(m_cols, m_rows);

     #pragma omp parallel for collapse(2)
	for (size_t i = 0; i < m_rows; ++i)
		for (size_t j = 0; j < m_cols; ++j)
			tmp.set(j, i, get(i, j));

	return tmp;
}

template<typename data>
matrix<data> matrix<data>::row(size_t n) const
{
	matrix tmp(1, m_cols);

     #pragma omp parallel for
	for (size_t i = 0; i < m_cols; ++i)
		tmp.set(0, i, get(n, i));

	return tmp;
}

template<typename data>
matrix<data> matrix<data>::col(size_t n) const
{
	matrix tmp(m_rows, 1);

     #pragma omp parallel for
	for (size_t i = 0; i < m_rows; ++i)
		tmp.set(i, 0, get(i, n));

	return tmp;
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
				res.get(j, i) += get(j, k) * other.get(k, i);

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
				res.get(j, i) += get(j, k)*other.get(k, i);

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
