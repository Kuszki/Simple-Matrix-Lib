/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                         *
 *  Simple matrix implementation for simulation purposes                   *
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

#include <functional>
#include <utility>
#include <fstream>
#include <string>

#include <cstddef>
#include <cmath>

template<typename data = double>
class matrix
{

     public:

          using fun_type_a = std::function<data (data, size_t, size_t, size_t, size_t)>;
          using fun_type_b = std::function<data (data, size_t, size_t)>;
          using fun_type_c = std::function<data (data)>;

          using data_type = data;

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

          size_t m_ompmin = 1024;

     public:

          explicit matrix(const std::string& file);

          matrix(const std::initializer_list<data>& list);
          matrix(const std::initializer_list<std::initializer_list<data>>& list);

          matrix(size_t rows, size_t cols);
          matrix(size_t rows, size_t cols, const data ptr[]);
          matrix(size_t rows, size_t cols, const data& val);
          matrix(size_t rows, size_t cols, const std::initializer_list<data>& list);

          matrix(void) = default;

          matrix(const matrix<data>& other);
          matrix(matrix<data>&& other);

          template<typename type>
          matrix(const matrix<type>& other);

          data& get_val(size_t row, size_t col);
          const data& get_val(size_t row, size_t col) const;
          const data& get_val(size_t row, size_t col, const data& def) const;
          bool set_val(size_t row, size_t col, const data& val);

          matrix<data> get_row(size_t n) const;
          matrix<data> get_col(size_t n) const;

          size_t rows(void) const;
          size_t cols(void) const;
          size_t size(void) const;

          size_t get_ompmin(void) const;
          bool set_ompmin(size_t ompmin);

          bool resize(size_t rows, size_t cols);
          bool clear(void);

          bool is_valid(size_t row, size_t col) const;

          bool is_empty(void) const;
          bool is_valid(void) const;
          bool is_vector(void) const;
          bool is_square(void) const;

          bool load(const std::string& path);
          bool save(const std::string& path, std::streamsize prec = 6) const;

          bool load(std::istream& stream);
          bool save(std::ostream& stream) const;

          matrix<data> submatrix(size_t row, size_t col) const;
          matrix<data> diagonal(mode mod = mode::rows) const;
          matrix<data> transpose(void) const;

          matrix<data> normalize(const data& val) const&;
          matrix<data> normalize(const data& val) &&;

          matrix<data> normalize(void) const&;
          matrix<data> normalize(void) &&;

          matrix<data> apply(const fun_type_a& fun, bool omp = true) const&;
          matrix<data> apply(const fun_type_a& fun, bool omp = true) &&;

          matrix<data> apply(const fun_type_b& fun, bool omp = true) const&;
          matrix<data> apply(const fun_type_b& fun, bool omp = true) &&;

          matrix<data> apply(const fun_type_c& fun, bool omp = true) const&;
          matrix<data> apply(const fun_type_c& fun, bool omp = true) &&;

          data mean(size_t n = 0, mode mod = mode::all) const;
          data var(size_t n = 0, mode mod = mode::all) const;
          data std(size_t n = 0, mode mod = mode::all) const;

          data max(size_t n = 0, mode mod = mode::all) const;
          data min(size_t n = 0, mode mod = mode::all) const;

          data det(void) const;

          template<typename type>
          bool set_row(size_t n, const matrix<type>& other);

          template<typename type>
          bool set_col(size_t n, const matrix<type>& other);

          template<typename type>
          matrix<data> operator+ (const matrix<type>& other) const&;

          template<typename type>
          matrix<data> operator- (const matrix<type>& other) const&;

          template<typename type>
          matrix<data> operator+ (matrix<type>&& other) const;

          template<typename type>
          matrix<data> operator- (matrix<type>&& other) const;

          template<typename type>
          matrix<data> operator+ (const matrix<type>& other) &&;

          template<typename type>
          matrix<data> operator- (const matrix<type>& other) &&;

          template<typename type>
          matrix<data> operator* (const matrix<type>& other) const;

          template<typename type>
          matrix<data> operator/ (const matrix<type>& other) const;

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

          matrix<data> operator+ (const data& other) const&;
          matrix<data> operator- (const data& other) const&;
          matrix<data> operator* (const data& other) const&;
          matrix<data> operator/ (const data& other) const&;

          matrix<data> operator+ (const data& other) &&;
          matrix<data> operator- (const data& other) &&;
          matrix<data> operator* (const data& other) &&;
          matrix<data> operator/ (const data& other) &&;

          data& operator() (size_t row, size_t col);
          const data& operator() (size_t row, size_t col) const;

          matrix<data> operator- (void) const&;
          matrix<data> operator- (void) &&;

          template<typename type> friend class matrix;

          virtual ~matrix(void);

          static matrix<data> gen_zeros(size_t rows, size_t cols);
          static matrix<data> gen_ones(size_t rows, size_t cols);
          static matrix<data> gen_diag(size_t size, const data& val = data(1));
          static matrix<data> gen_const(size_t rows, size_t cols,
                                        const data& val);
          static matrix<data> gen_linsp(size_t rows, size_t cols,
                                        const data& start,
                                        const data& stop);

};

#ifndef MATRIX_CPP
#include "matrix.cpp"
#endif

#endif // MATRIX_HPP
