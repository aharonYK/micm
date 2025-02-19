// Copyright (C) 2023 National Center for Atmospheric Research,
//
// SPDX-License-Identifier: Apache-2.0
#pragma once

#include <cassert>
#include <vector>

#include <micm/util/exit_codes.hpp>

namespace micm
{

  /// @brief A 2D array class with contiguous memory
  template<class T>
  class Matrix
  {
    std::vector<T> data_;
    std::size_t x_dim_;
    std::size_t y_dim_;

    friend class Proxy;
    friend class ConstProxy;

    class Proxy
    {
      Matrix &matrix_;
      std::size_t offset_;
      std::size_t y_dim_;

     public:
      Proxy(Matrix &matrix, std::size_t offset, std::size_t y_dim)
          : matrix_(matrix),
            offset_(offset),
            y_dim_(y_dim)
      {
      }
      Proxy &operator=(const std::vector<T> other)
      {
        // check that this row matches the expected rectangular matrix dimensions
        if (other.size() < y_dim_)
        {
          std::cerr << "Matrix row size mismatch in assignment from vector";
          std::exit(micm::ExitCodes::InvalidMatrixDimension);
        }
        auto other_elem = other.begin();
        for (auto &elem : *this)
        {
          elem = *(other_elem++);
        }
        return *this;
      }
      operator std::vector<T>() const
      {
        return std::vector<T>(this->begin(), this->end());
      }
      std::size_t size() const
      {
        return y_dim_;
      }
      typename std::vector<T>::iterator begin() noexcept
      {
        return std::next(matrix_.data_.begin(), offset_);
      }
      typename std::vector<T>::const_iterator begin() const noexcept
      {
        return std::next(matrix_.data_.cbegin(), offset_);
      }
      typename std::vector<T>::iterator end() noexcept
      {
        return std::next(matrix_.data_.begin(), offset_ + y_dim_);
      }
      typename std::vector<T>::const_iterator end() const noexcept
      {
        return std::next(matrix_.data_.begin(), offset_ + y_dim_);
      }
      T &operator[](std::size_t y)
      {
        return matrix_.data_[offset_ + y];
      }
    };

    class ConstProxy
    {
      const Matrix &matrix_;
      std::size_t offset_;
      std::size_t y_dim_;

     public:
      ConstProxy(const Matrix &matrix, std::size_t offset, std::size_t y_dim)
          : matrix_(matrix),
            offset_(offset),
            y_dim_(y_dim)
      {
      }
      operator std::vector<T>() const
      {
        return std::vector<T>(this->begin(), this->end());
      }
      std::size_t size() const
      {
        return y_dim_;
      }
      typename std::vector<T>::const_iterator begin() const noexcept
      {
        return std::next(matrix_.data_.cbegin(), offset_);
      }
      typename std::vector<T>::const_iterator end() const noexcept
      {
        return std::next(matrix_.data_.begin(), offset_ + y_dim_);
      }
      const T &operator[](std::size_t y) const
      {
        return matrix_.data_[offset_ + y];
      }
    };

   public:
    Matrix()
        : x_dim_(0),
          y_dim_(0),
          data_()
    {
    }

    Matrix(std::size_t x_dim, std::size_t y_dim)
        : x_dim_(x_dim),
          y_dim_(y_dim),
          data_(x_dim * y_dim)
    {
    }

    Matrix(std::size_t x_dim, std::size_t y_dim, T initial_value)
        : x_dim_(x_dim),
          y_dim_(y_dim),
          data_(x_dim * y_dim, initial_value)
    {
    }

    Matrix(const std::vector<std::vector<T>> other)
        : x_dim_(other.size()),
          y_dim_(other.size() == 0 ? 0 : other[0].size()),
          data_(
              [&]() -> std::vector<T>
              {
                std::size_t x_dim = other.size();
                if (x_dim == 0)
                  return std::vector<T>(0);
                std::size_t y_dim = other[0].size();
                std::vector<T> data(x_dim * y_dim);
                auto elem = data.begin();
                for (std::size_t x{}; x < x_dim; ++x)
                {
                  // check that this row matches the expected rectangular matrix dimensions
                  if (other[x].size() != y_dim)
                  {
                    std::cerr << "Invalid vector for matrix assignment\n";
                    std::exit(micm::ExitCodes::InvalidMatrixDimension);
                  }
                  for (std::size_t y{}; y < y_dim; ++y)
                  {
                    *(elem++) = other[x][y];
                  }
                }
                return data;
              }())
    {
    }

    std::size_t size() const
    {
      return x_dim_;
    }

    ConstProxy operator[](std::size_t x) const
    {
      return ConstProxy(*this, x * y_dim_, y_dim_);
    }

    Proxy operator[](std::size_t x)
    {
      return Proxy(*this, x * y_dim_, y_dim_);
    }

    std::vector<T> &AsVector()
    {
      return data_;
    }

    const std::vector<T> &AsVector() const
    {
      return data_;
    }
  };

}  // namespace micm