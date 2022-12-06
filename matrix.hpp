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

#ifndef MATRIX_HPP
#define MATRIX_HPP

#include <utility>

#include <cstddef>
#include <cmath>

template<typename data>
class matrix
{

	public: enum class mode
		{
			all,
			rows,
			cols
		};

	protected:

		data* m_ptr = nullptr;

		size_t m_cols = 0;
		size_t m_rows = 0;

	public:

		matrix(size_t rows, size_t cols, const data& val = 0.0);
		matrix(size_t rows, size_t cols, const std::initializer_list<data>& list);
		matrix(void) = default;

		matrix(const matrix<data>& other);
		matrix(matrix<data>&& other);

		template<typename type>
		matrix(const matrix<type>& other);

		virtual ~matrix(void);

		data& get(size_t row, size_t col);
		bool set(size_t row, size_t col, const data& val);

		data get(size_t row, size_t col) const;
		data mean(size_t n = 0, mode mod = mode::all) const;
		data var(size_t n = 0, mode mod = mode::all) const;
		data std(size_t n = 0, mode mod = mode::all) const;

		bool resize(size_t rows, size_t cols);
		bool clear(void);

		bool is_valid(void) const;
		bool is_vector(void) const;
		bool is_square(void) const;

		size_t rows(void) const;
		size_t cols(void) const;

		matrix<data> transpose(void) const;
		matrix<data> row(size_t n) const;
		matrix<data> col(size_t n) const;

		template<typename type>
		matrix<data> operator+ (const matrix<type>& other) const;

		template<typename type>
		matrix<data> operator- (const matrix<type>& other) const;

		template<typename type>
		matrix<data> operator* (const matrix<type>& other) const;

		template<typename type>
		matrix<data> operator/ (const matrix<type>& other) const;

		template<typename type>
		matrix<data> operator+ (const type& other) const;

		template<typename type>
		matrix<data> operator- (const type& other) const;

		template<typename type>
		matrix<data> operator* (const type& other) const;

		template<typename type>
		matrix<data> operator/ (const type& other) const;

		template<typename type>
		matrix<data>& operator+= (const matrix<type>& other);

		template<typename type>
		matrix<data>& operator-= (const matrix<type>& other);

		template<typename type>
		matrix<data>& operator*= (const matrix<type>& other);

		template<typename type>
		matrix<data>& operator/= (const matrix<type>& other);

		template<typename type>
		matrix<data>& operator+= (const type& other);

		template<typename type>
		matrix<data>& operator-= (const type& other);

		template<typename type>
		matrix<data>& operator*= (const type& other);

		template<typename type>
		matrix<data>& operator/= (const type& other);

		template<typename type>
		matrix<data>& operator= (const matrix<type>& other);

		template<typename type>
		bool operator== (const matrix<type>& other) const;

		template<typename type>
		bool operator!= (const matrix<type>& other) const;

		matrix<data>& operator= (const matrix<data>& other);
		matrix<data>& operator= (matrix<data>&& other);

		template<typename type> friend class matrix;

};

#ifndef MATRIX_CPP
#include "matrix.cpp"
#endif

#endif // MATRIX_HPP
