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

#ifndef MATRIX_CPP
#define MATRIX_CPP

#ifndef MATRIX_HPP
#include "matrix.hpp"
#endif

template<typename data>
matrix<data>::matrix(const std::string& file)
{
     load(file);
}

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
matrix<data>::matrix(size_t rows, size_t cols)
{
     resize(rows, cols);
}

template<typename data>
matrix<data>::matrix(size_t rows, size_t cols, const data ptr[])
{
     resize(rows, cols); const size_t count = rows*cols;

     #pragma omp parallel for if(count > m_ompmin)
     for (size_t i = 0; i < count; ++i) m_ptr[i] = ptr[i];
}

template<typename data>
matrix<data>::matrix(size_t rows, size_t cols, const data& val)
{
     resize(rows, cols); const size_t count = rows*cols;

     #pragma omp parallel for if(count > m_ompmin)
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
const data& matrix<data>::get_val(size_t row, size_t col) const
{
     return m_ptr[m_cols*row + col];
}

template<typename data>
const data& matrix<data>::get_val(size_t row, size_t col, const data& def) const
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
data matrix<data>::mean(size_t n, mode mod) const
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
data matrix<data>::var(size_t n, mode mod) const
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
data matrix<data>::std(size_t n, mode mod) const
{
     return std::sqrt(var(n, mod));
}

template<typename data>
data matrix<data>::max(size_t n, mode mod) const
{
     if (m_ptr == nullptr) return data();
     else switch (mod)
     {
          case mode::all:
          {
               const size_t count = m_rows * m_cols;
               data out = m_ptr[0];

               for (size_t i = 1; i < count; ++i)
                    if (out < m_ptr[i]) out = m_ptr[i];

               return out;
          }
          break;
          case mode::rows:
          {
               if (n >= m_rows) return data();

               size_t ci = n * m_cols + 1;
               data out = get_val(n, 0);

               for (size_t i = 1; i < m_cols; ++i, ++ci)
                    if (out < m_ptr[ci]) out = m_ptr[ci];

               return out;
          }
          break;
          case mode::cols:
          {
               if (n >= m_cols) return data();

               size_t ci = n + m_cols;
               data out = get_val(n, 0);

               for (size_t i = 1; i < m_rows; ++i, ci += m_cols)
                    if (out < m_ptr[ci]) out = m_ptr[ci];

               return out;
          }
          break;
     }

     return data();
}

template<typename data>
data matrix<data>::min(size_t n, mode mod) const
{
     if (m_ptr == nullptr) return data();
     else switch (mod)
     {
          case mode::all:
          {
               const size_t count = m_rows * m_cols;
               data out = m_ptr[0];

               for (size_t i = 1; i < count; ++i)
                    if (out > m_ptr[i]) out = m_ptr[i];

               return out;
          }
          break;
          case mode::rows:
          {
               if (n >= m_rows) return data();

               size_t ci = n * m_cols + 1;
               data out = get_val(n, 0);

               for (size_t i = 1; i < m_cols; ++i, ++ci)
                    if (out > m_ptr[ci]) out = m_ptr[ci];

               return out;
          }
          break;
          case mode::cols:
          {
               if (n >= m_cols) return data();

               size_t ci = n + m_cols;
               data out = get_val(0, n);

               for (size_t i = 1; i < m_rows; ++i, ci += m_cols)
                    if (out > m_ptr[ci]) out = m_ptr[ci];

               return out;
          }
          break;
     }

     return data();
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
     if (rows == m_rows && cols == m_cols) return false;
     else if (rows > 0 && cols > 0) clear();
     else return false;

     const size_t count = cols * rows;
     void* ptr = std::malloc(count * sizeof(data));

     if (ptr)
     {
          m_ptr = static_cast<data*>(ptr);
          m_cols = cols;
          m_rows = rows;
     }

     return m_ptr != nullptr;
}

template<typename data>
bool matrix<data>::clear(void)
{
     if (m_ptr) std::free(m_ptr);
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
bool matrix<data>::load(const std::string& path)
{
     std::ifstream file(path);

     return load(file);
}

template<typename data>
bool matrix<data>::save(const std::string& path, std::streamsize prec) const
{
     std::ofstream file(path, std::ios::trunc);
     file.precision(prec);

     return save(file);
}

template<typename data>
bool matrix<data>::load(std::istream& stream)
{
     if (!stream.good()) return false;

     size_t count = 0, cnum = 0, step;
     size_t length = step = 1024;
     double val = 0.0;

     void* mem = std::malloc(length * sizeof(data));
     data* ptr = static_cast<data*>(mem);

     while (stream >> val)
     {
          ptr[count++] = val;

          if (count == length)
          {
               length += step;
               mem = std::realloc(ptr, length * sizeof(data));
               step *= 2;

               if (!mem) { std::free(ptr); return false; }
               else ptr = static_cast<data*>(mem);
          }

          if (cnum == 0)
          {
               char c = '\0'; stream.get(c);
               if (c == '\n') cnum = count;
          }
     }

     if (cnum) count -= count % cnum;
     else cnum = count;

     if (count) mem = std::realloc(ptr, count * sizeof(data));
     else if (ptr) { std::free(ptr); mem = ptr = nullptr; }

     if (mem != nullptr) clear(); else return false;

     m_ptr = static_cast<data*>(mem);
     m_rows = count / cnum;
     m_cols = cnum;

     return true;
}

template<typename data>
bool matrix<data>::save(std::ostream& stream) const
{
     if (!stream.good()) return false;

     for (size_t i = 0; i < m_rows; ++i)
          for (size_t j = 0; j < m_cols; ++j)
               stream << get_val(i, j) << (j+1 == m_cols ? '\n' : '\t');

     return !stream.fail();
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
size_t matrix<data>::get_ompmin(void) const
{
     return m_ompmin;
}

template<typename data>
bool matrix<data>::set_ompmin(size_t ompmin)
{
     return m_ompmin = ompmin;
}

template<typename data>
matrix<data> matrix<data>::submatrix(size_t row, size_t col) const
{
     if (m_cols < 2 || m_rows < 2) return matrix<data>();
     else if (row >= m_rows || col >= m_cols) return *this;

     const size_t count = m_rows * m_cols;
     matrix<data> res(m_rows - 1, m_cols - 1);

     #pragma omp parallel for collapse(2) if(count > m_ompmin)
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
     const size_t count = m_rows * m_cols;
     matrix<data> out(m_cols, m_rows);

     #pragma omp parallel for collapse(2) if(count > m_ompmin)
     for (size_t i = 0; i < m_rows; ++i)
          for (size_t j = 0; j < m_cols; ++j)
               out.set_val(j, i, get_val(i, j));

     return out;
}

template<typename data>
matrix<data> matrix<data>::normalize(const data& val) const&
{
     if (m_ptr == nullptr) return matrix<data>();

     const size_t count = m_rows * m_cols;
     matrix<data> out(m_rows, m_cols);

     #pragma omp parallel for if(count > m_ompmin)
     for (size_t i = 0; i < count; ++i)
          out.m_ptr[i] = m_ptr[i] / val;

     return out;
}

template<typename data>
matrix<data> matrix<data>::normalize(const data& val) &&
{
     if (m_ptr == nullptr) return matrix<data>();

     const size_t count = m_rows * m_cols;

     #pragma omp parallel for if(count > m_ompmin)
     for (size_t i = 0; i < count; ++i)  m_ptr[i] /= val;

     return std::move(*this);
}

template<typename data>
matrix<data> matrix<data>::normalize(void) const&
{
     if (m_ptr == nullptr) return matrix<data>();

     const size_t count = m_rows * m_cols;
     matrix<data> out(m_rows, m_cols);
     data max = m_ptr[0];

     for (size_t i = 1; i < count; ++i)
          if (max < m_ptr[i]) max = m_ptr[i];

     #pragma omp parallel for if(count > m_ompmin)
     for (size_t i = 0; i < count; ++i)
          out.m_ptr[i] = m_ptr[i] / max;

     return out;
}

template<typename data>
matrix<data> matrix<data>::normalize(void) &&
{
     if (m_ptr == nullptr) return matrix<data>();

     const size_t count = m_rows * m_cols;
     data max = m_ptr[0];

     for (size_t i = 1; i < count; ++i)
          if (max < m_ptr[i]) max = m_ptr[i];

     #pragma omp parallel for if(count > m_ompmin)
     for (size_t i = 0; i < count; ++i)  m_ptr[i] /= max;

     return std::move(*this);
}

template<typename data>
matrix<data> matrix<data>::apply(const fun_type_a& fun, bool omp) const&
{
     if (m_ptr == nullptr) return matrix<data>();

     const size_t count = m_rows * m_cols;
     matrix<data> out(m_rows, m_cols);

     #pragma omp parallel for collapse(2) if(omp && count > m_ompmin)
     for (size_t i = 0; i < m_rows; ++i)
          for (size_t j = 0; j < m_cols; ++j)
          {
               out.get_val(i, j) = fun(get_val(i, j), i, j, m_rows, m_cols);
          }

     return out;
}

template<typename data>
matrix<data> matrix<data>::apply(const fun_type_a& fun, bool omp) &&
{
     if (m_ptr == nullptr) return matrix<data>();

     const size_t count = m_rows * m_cols;

     #pragma omp parallel for collapse(2) if(omp && count > m_ompmin)
     for (size_t i = 0; i < m_rows; ++i)
          for (size_t j = 0; j < m_cols; ++j)
          {
               get_val(i, j) = fun(get_val(i, j), i, j, m_rows, m_cols);
          }

     return std::move(*this);
}

template<typename data>
matrix<data> matrix<data>::apply(const fun_type_b& fun, bool omp) const&
{
     if (m_ptr == nullptr) return matrix<data>();

     const size_t count = m_rows * m_cols;
     matrix<data> out(m_rows, m_cols);

     #pragma omp parallel for if(omp && count > m_ompmin)
     for (size_t i = 0; i < count; ++i)
     {
          out.m_ptr[i] = fun(m_ptr[i], i, count);
     }

     return out;
}

template<typename data>
matrix<data> matrix<data>::apply(const fun_type_b& fun, bool omp) &&
{
     if (m_ptr == nullptr) return matrix<data>();

     const size_t count = m_rows * m_cols;

     #pragma omp parallel for if(omp && count > m_ompmin)
     for (size_t i = 0; i < count; ++i)
     {
          m_ptr[i] = fun(m_ptr[i], i, count);
     }

     return std::move(*this);
}

template<typename data>
matrix<data> matrix<data>::apply(const fun_type_c& fun, bool omp) const&
{
     if (m_ptr == nullptr) return matrix<data>();

     const size_t count = m_rows * m_cols;
     matrix<data> out(m_rows, m_cols);

     #pragma omp parallel for if(omp && count > m_ompmin)
     for (size_t i = 0; i < count; ++i)
     {
          out.m_ptr[i] = fun(m_ptr[i]);
     }

     return out;
}

template<typename data>
matrix<data> matrix<data>::apply(const fun_type_c& fun, bool omp) &&
{
     if (m_ptr == nullptr) return matrix<data>();

     const size_t count = m_rows * m_cols;

     #pragma omp parallel for if(omp && count > m_ompmin)
     for (size_t i = 0; i < count; ++i)
     {
          m_ptr[i] = fun(m_ptr[i]);
     }

     return std::move(*this);
}

template<typename data>
matrix<data> matrix<data>::diagonal(matrix<data>::mode mod) const
{
     if (m_rows != m_cols) return matrix<data>();

     matrix<data> out = mod == mode::rows ?
                             matrix<data>(1, m_cols) :
                             matrix<data>(m_rows, 1);

     #pragma omp parallel for if(m_cols >= m_ompmin)
     for (size_t i = 0; i < m_cols; ++i) out.m_ptr[i] = get_val(i, i);

     return out;
}

template<typename data>
matrix<data> matrix<data>::get_row(size_t n) const
{
     if (n >= m_rows) return matrix<data>();

     matrix<data> res(1, m_cols);

     #pragma omp parallel for if(m_cols >= m_ompmin)
     for (size_t i = 0; i < m_cols; ++i) res.m_ptr[i] = get_val(n, i);

     return res;
}

template<typename data>
matrix<data> matrix<data>::get_col(size_t n) const
{
     if (n >= m_cols) return matrix<data>();

     matrix<data> res(m_rows, 1);

     #pragma omp parallel for if(m_rows >= m_ompmin)
     for (size_t i = 0; i < m_rows; ++i) res.m_ptr[i] = get_val(i, n);

     return res;
}

template<typename data>
data& matrix<data>::operator() (size_t row, size_t col)
{
     return get_val(row, col);
}

template<typename data>
const data& matrix<data>::operator() (size_t row, size_t col) const
{
     return get_val(row, col);
}

template<typename data>
matrix<data> matrix<data>::operator- (void) const&
{
     const size_t count = m_rows * m_cols;
     matrix<data> res(m_rows, m_cols);

     #pragma omp parallel for if(count > m_ompmin)
     for (size_t i = 0; i < count; ++i) res.m_ptr[i] = -m_ptr[i];

     return res;
}

template<typename data>
matrix<data> matrix<data>::operator- (void) &&
{
     const size_t count = m_rows * m_cols;
     #pragma omp parallel for if(count > m_ompmin)
     for (size_t i = 0; i < count; ++i) m_ptr[i] = -m_ptr[i];

     return std::move(*this);
}

template<typename data> template<typename type>
bool matrix<data>::set_row(size_t n, const matrix<type>& other)
{
     if (n >= m_rows) return false;
     if (other.m_rows != 1 && other.m_cols != 1) return false;
     if (other.m_rows != m_cols && other.m_cols != m_cols) return false;

     #pragma omp parallel for if(m_cols >= m_ompmin)
     for (size_t i = 0; i < m_cols; ++i) set_val(n, i, other.m_ptr[i]);

     return true;
}

template<typename data> template<typename type>
bool matrix<data>::set_col(size_t n, const matrix<type>& other)
{
     if (n >= m_cols) return false;
     if (other.m_rows != 1 && other.m_cols != 1) return false;
     if (other.m_rows != m_rows && other.m_cols != m_rows) return false;

     #pragma omp parallel for if(m_rows >= m_ompmin)
     for (size_t i = 0; i < m_rows; ++i) set_val(i, n, other.m_ptr[i]);

     return true;
}

template<typename data> template<typename type>
matrix<data>& matrix<data>::operator= (const matrix<type>& other)
{
     if (static_cast<const void*>(&other) == this) return *this;
     else resize(other.m_rows, other.m_cols);

     const size_t count = m_rows * m_cols;

     #pragma omp parallel for if(count > m_ompmin)
     for (size_t i = 0; i < count; ++i) m_ptr[i] = other.m_ptr[i];

     return *this;
}

template<typename data>
matrix<data>& matrix<data>::operator= (const matrix<data>& other)
{
     if (&other == this) return *this;
     else resize(other.m_rows, other.m_cols);

     const size_t count = m_rows * m_cols;

     #pragma omp parallel for if(count > m_ompmin)
     for (size_t i = 0; i < count; ++i) m_ptr[i] = other.m_ptr[i];

     return *this;
}

template<typename data>
matrix<data>& matrix<data>::operator= (matrix<data>&& other)
{
     if (&other != this) clear();
     else return *this;

     m_cols = other.m_cols;
     m_rows = other.m_rows;
     m_ptr = other.m_ptr;

     other.m_ptr = nullptr;
     other.m_cols = 0;
     other.m_rows = 0;

     return *this;
}

template<typename data> template<typename type>
matrix<data> matrix<data>::operator+ (const matrix<type>& other) const&
{
     if (m_rows != other.m_rows ||
         m_cols != other.m_cols) return matrix<data>();

     matrix<data> out(m_rows, m_cols);
     const size_t count = m_rows * m_cols;

     #pragma omp parallel for if(count > m_ompmin)
     for (size_t i = 0; i < count; ++i) out.m_ptr[i] = m_ptr[i] + other.m_ptr[i];

     return out;
}

template<typename data> template<typename type>
matrix<data> matrix<data>::operator+ (matrix<type>&& other) const
{
     if (m_rows != other.m_rows ||
         m_cols != other.m_cols) return matrix<data>();

     const size_t count = m_rows * m_cols;

     #pragma omp parallel for if(count > m_ompmin)
     for (size_t i = 0; i < count; ++i) other.m_ptr[i] += m_ptr[i];

     return std::move(other);
}

template<typename data> template<typename type>
matrix<data> matrix<data>::operator+ (const matrix<type>& other) &&
{
     if (m_rows != other.m_rows ||
         m_cols != other.m_cols) return matrix<data>();

     const size_t count = m_rows * m_cols;

     #pragma omp parallel for if(count > m_ompmin)
     for (size_t i = 0; i < count; ++i) m_ptr[i] += other.m_ptr[i];

     return std::move(*this);
}

template<typename data> template<typename type>
matrix<data> matrix<data>::operator- (const matrix<type>& other) const&
{
     if (m_rows != other.m_rows ||
         m_cols != other.m_cols) return matrix<data>();

     matrix<data> out(m_rows, m_cols);
     const size_t count = m_rows * m_cols;

     #pragma omp parallel for if(count > m_ompmin)
     for (size_t i = 0; i < count; ++i) out.m_ptr[i] = m_ptr[i] - other.m_ptr[i];

     return out;
}

template<typename data> template<typename type>
matrix<data> matrix<data>::operator- (matrix<type>&& other) const
{
     if (m_rows != other.m_rows ||
         m_cols != other.m_cols) return matrix<data>();

     const size_t count = m_rows * m_cols;

     #pragma omp parallel for if(count > m_ompmin)
     for (size_t i = 0; i < count; ++i) other.m_ptr[i] -= m_ptr[i];

     return std::move(other);
}

template<typename data> template<typename type>
matrix<data> matrix<data>::operator- (const matrix<type>& other) &&
{
     if (m_rows != other.m_rows ||
         m_cols != other.m_cols) return matrix<data>();

     const size_t count = m_rows * m_cols;

     #pragma omp parallel for if(count > m_ompmin)
     for (size_t i = 0; i < count; ++i) m_ptr[i] -= other.m_ptr[i];

     return std::move(*this);
}

template<typename data> template<typename type>
matrix<data> matrix<data>::operator* (const matrix<type>& other) const
{
     if (m_cols != other.m_rows) return matrix<data>();

     matrix<data> res(m_rows, other.m_cols, data(0));
     const size_t count = res.m_rows * res.m_cols;

     #pragma omp parallel for if(count > m_ompmin)
     for (size_t i = 0; i < res.m_rows; ++i)
          for (size_t k = 0; k < m_cols; ++k)
          {
               const data& mul = m_ptr[i * m_cols + k];
               for (size_t j = 0; j < res.m_cols; ++j)
               {
                    res(i, j) += mul * other(k, j);
               }
          }

     return res;
}

template<typename data>
matrix<data> matrix<data>::operator+ (const data& other) const&
{
     matrix<data> res(m_rows, m_cols);

     const size_t count = m_rows * m_cols;

     #pragma omp parallel for if(count > m_ompmin)
     for (size_t i = 0; i < count; ++i) res.m_ptr[i] = m_ptr[i] + other;

     return res;
}

template<typename data>
matrix<data> matrix<data>::operator+ (const data& other) &&
{
     const size_t count = m_rows * m_cols;

     #pragma omp parallel for if(count > m_ompmin)
     for (size_t i = 0; i < count; ++i) m_ptr[i] += other;

     return std::move(*this);
}

template<typename data>
matrix<data> matrix<data>::operator- (const data& other) const&
{
     matrix<data> res(m_rows, m_cols);

     const size_t count = m_rows * m_cols;

     #pragma omp parallel for if(count > m_ompmin)
     for (size_t i = 0; i < count; ++i) res.m_ptr[i] = m_ptr[i] - other;

     return res;
}

template<typename data>
matrix<data> matrix<data>::operator- (const data& other) &&
{
     const size_t count = m_rows * m_cols;

     #pragma omp parallel for if(count > m_ompmin)
     for (size_t i = 0; i < count; ++i) m_ptr[i] -= other;

     return std::move(*this);
}

template<typename data>
matrix<data> matrix<data>::operator* (const data& other) const&
{
     matrix<data> res(m_rows, m_cols);

     const size_t count = m_rows * m_cols;

     #pragma omp parallel for if(count > m_ompmin)
     for (size_t i = 0; i < count; ++i) res.m_ptr[i] = m_ptr[i] * other;

     return res;
}

template<typename data>
matrix<data> matrix<data>::operator* (const data& other) &&
{
     const size_t count = m_rows * m_cols;

     #pragma omp parallel for if(count > m_ompmin)
     for (size_t i = 0; i < count; ++i) m_ptr[i] *= other;

     return std::move(*this);
}

template<typename data>
matrix<data> matrix<data>::operator/ (const data& other) const&
{
     matrix<data> res(m_rows, m_cols);

     const size_t count = m_rows * m_cols;

     #pragma omp parallel for if(count > m_ompmin)
     for (size_t i = 0; i < count; ++i) res.m_ptr[i] = m_ptr[i] / other;

     return res;
}

template<typename data>
matrix<data> matrix<data>::operator/ (const data& other) &&
{
     const size_t count = m_rows * m_cols;

     #pragma omp parallel for if(count > m_ompmin)
     for (size_t i = 0; i < count; ++i) m_ptr[i] /= other;

     return std::move(*this);
}

template<typename data> template<typename type>
matrix<data>& matrix<data>::operator+= (const matrix<type>& other)
{
     if (m_rows != other.m_rows ||
         m_cols != other.m_cols) return *this;

     const size_t count = m_rows * m_cols;

     #pragma omp parallel for if(count > m_ompmin)
     for (size_t i = 0; i < count; ++i) m_ptr[i] += other.m_ptr[i];

     return *this;
}

template<typename data> template<typename type>
matrix<data>& matrix<data>::operator-= (const matrix<type>& other)
{
     if (m_rows != other.m_rows ||
         m_cols != other.m_cols) return *this;

     const size_t count = m_rows * m_cols;

     #pragma omp parallel for if(count > m_ompmin)
     for (size_t i = 0; i < count; ++i) m_ptr[i] -= other.m_ptr[i];

     return *this;
}

template<typename data> template<typename type>
matrix<data>& matrix<data>::operator*= (const matrix<type>& other)
{
     if (m_cols != other.m_rows) return *this;
     else return *this = *this * other;
}

template<typename data> template<typename type>
matrix<data>& matrix<data>::operator+= (const type& other)
{
     const size_t count = m_rows * m_cols;

     #pragma omp parallel for if(count > m_ompmin)
     for (size_t i = 0; i < count; ++i) m_ptr[i] += other;

     return *this;
}

template<typename data> template<typename type>
matrix<data>& matrix<data>::operator-= (const type& other)
{
     const size_t count = m_rows * m_cols;

     #pragma omp parallel for if(count > m_ompmin)
     for (size_t i = 0; i < count; ++i) m_ptr[i] -= other;

     return *this;
}

template<typename data> template<typename type>
matrix<data>& matrix<data>::operator*= (const type& other)
{
     const size_t count = m_rows * m_cols;

     #pragma omp parallel for if(count > m_ompmin)
     for (size_t i = 0; i < count; ++i) m_ptr[i] *= other;

     return *this;
}

template<typename data> template<typename type>
matrix<data>& matrix<data>::operator/= (const type& other)
{
     const size_t count = m_rows * m_cols;

     #pragma omp parallel for if(count > m_ompmin)
     for (size_t i = 0; i < count; ++i) m_ptr[i] /= other;

     return *this;
}

template<typename data> template<typename type>
bool matrix<data>::operator== (const matrix<type>& other) const
{
     if (m_rows != other.m_rows || m_cols != other.m_cols) return false;

     const size_t count = m_rows * m_cols;
     for (size_t i = 0; i < count; ++i)
          if (m_ptr[i] != other.m_ptr[i])
               return false;

     return true;
}

template<typename data> template<typename type>
bool matrix<data>::operator!= (const matrix<type>& other) const
{
     if (m_rows != other.m_rows || m_cols != other.m_cols) return true;

     const size_t count = m_rows * m_cols;
     for (size_t i = 0; i < count; ++i)
          if (m_ptr[i] != other.m_ptr[i])
               return true;

     return false;
}

template<typename data>
matrix<data>::~matrix(void)
{
     if (m_ptr) std::free(m_ptr);
}

template<typename data>
matrix<data> matrix<data>::gen_zeros(size_t rows, size_t cols)
{
     return matrix<data>(rows, cols, data(0));
}

template<typename data>
matrix<data> matrix<data>::gen_ones(size_t rows, size_t cols)
{
     return matrix<data>(rows, cols, data(1));
}

template<typename data>
matrix<data> matrix<data>::gen_diag(size_t size, const data& val)
{
     matrix<data> out(size, size, data(0));

     #pragma omp parallel for if (size > out.m_ompmin)
     for (size_t i = 0; i < size; ++i) out.set_val(i, i, data(1));

     return out;
}

template<typename data>
matrix<data> matrix<data>::gen_const(size_t rows, size_t cols, const data& val)
{
     return matrix<data>(rows, cols, val);
}

template<typename data>
matrix<data> matrix<data>::gen_linsp(size_t rows, size_t cols, const data& start, const data& stop)
{
     const size_t count = rows * cols;
     const data dt = (stop - start);
     matrix<data> out(rows, cols);

     #pragma omp parallel for if (count > out.m_ompmin)
     for (size_t i = 0; i < count; ++i)
     {
          out.m_ptr[i] = start + (dt * i) / (count - 1);
     }

     return out;
}

template<typename data>
matrix<data> operator+ (const data& other, const matrix<data>& mat)
{
     return mat + other;
}

template<typename data>
matrix<data> operator+ (const data& other, matrix<data>&& mat)
{
     return std::move(mat) + other;
}

template<typename data>
matrix<data> operator- (const data& other, const matrix<data>& mat)
{
     return mat - other;
}

template<typename data>
matrix<data> operator- (const data& other, matrix<data>&& mat)
{
     return -std::move(mat) + other;
}

template<typename data>
matrix<data> operator* (const data& other, const matrix<data>& mat)
{
     return mat * other;
}

template<typename data>
matrix<data> operator* (const data& other, matrix<data>&& mat)
{
     return std::move(mat) * other;
}

#endif
