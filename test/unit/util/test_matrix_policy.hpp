// Tests of common matrix functions
#include <gtest/gtest.h>

template<template<class> class MatrixPolicy>
MatrixPolicy<double> testSmallMatrix()
{
  MatrixPolicy<double> matrix(3, 5);

  matrix[1][3] = 64.7;
  matrix[0][0] = 41.2;
  matrix[2][4] = 102.3;

  EXPECT_EQ(matrix[1][3], 64.7);
  EXPECT_EQ(matrix[0][0], 41.2);
  EXPECT_EQ(matrix[2][4], 102.3);

  std::vector<double>& data = matrix.AsVector();

  EXPECT_GE(data.size(), 3);

  return matrix;
}

template<template<class> class MatrixPolicy>
const MatrixPolicy<double> testSmallConstMatrix()
{
  MatrixPolicy<double> matrix(3, 5);

  matrix[1][3] = 64.7;
  matrix[0][0] = 41.2;
  matrix[2][4] = 102.3;

  const MatrixPolicy<double> const_matrix = matrix;

  EXPECT_EQ(const_matrix[1][3], 64.7);
  EXPECT_EQ(const_matrix[0][0], 41.2);
  EXPECT_EQ(const_matrix[2][4], 102.3);

  const std::vector<double>& data = const_matrix.AsVector();

  EXPECT_GE(data.size(), 3);

  return const_matrix;
}

template<template<class> class MatrixPolicy>
MatrixPolicy<double> testInializeMatrix()
{
  MatrixPolicy<double> matrix{ 2, 3, 12.4 };

  EXPECT_EQ(matrix[0][0], 12.4);
  EXPECT_EQ(matrix[1][0], 12.4);
  EXPECT_EQ(matrix[1][2], 12.4);

  return matrix;
}

template<template<class> class MatrixPolicy>
const MatrixPolicy<double> testInializeConstMatrix()
{
  const MatrixPolicy<double> matrix{ 2, 3, 12.4 };

  EXPECT_EQ(matrix[0][0], 12.4);
  EXPECT_EQ(matrix[1][0], 12.4);
  EXPECT_EQ(matrix[1][2], 12.4);

  return matrix;
}

template<template<class> class MatrixPolicy>
MatrixPolicy<int> testLoopOverMatrix()
{
  MatrixPolicy<int> matrix(3, 4, 0);
  for (std::size_t i{}; i < matrix.size(); ++i)
  {
    for (std::size_t j{}; j < matrix[i].size(); ++j)
    {
      matrix[i][j] = i * 100 + j;
    }
  }

  EXPECT_EQ(matrix[0][0], 0);
  EXPECT_EQ(matrix[1][2], 102);
  EXPECT_EQ(matrix[2][3], 203);
  EXPECT_EQ(matrix[0][3], 3);

  return matrix;
}

template<template<class> class MatrixPolicy>
const MatrixPolicy<int> testLoopOverConstMatrix()
{
  MatrixPolicy<int> matrix(3, 4, 0);
  for (std::size_t i{}; i < matrix.size(); ++i)
  {
    for (std::size_t j{}; j < matrix[i].size(); ++j)
    {
      matrix[i][j] = i * 100 + j;
    }
  }

  const MatrixPolicy<int> const_matrix = matrix;

  EXPECT_EQ(const_matrix[0][0], 0);
  EXPECT_EQ(const_matrix[1][2], 102);
  EXPECT_EQ(const_matrix[2][3], 203);
  EXPECT_EQ(const_matrix[0][3], 3);

  return const_matrix;
}

template<template<class> class MatrixPolicy>
MatrixPolicy<double> testConversionToVector()
{
  MatrixPolicy<double> matrix{ 2, 3, 0.0 };

  matrix[1][0] = 13.2;
  matrix[1][1] = 31.2;
  matrix[1][2] = 314.2;

  std::vector<double> slice = matrix[1];

  EXPECT_EQ(slice[0], 13.2);
  EXPECT_EQ(slice[1], 31.2);
  EXPECT_EQ(slice[2], 314.2);

  return matrix;
}

template<template<class> class MatrixPolicy>
const MatrixPolicy<double> testConstConversionToVector()
{
  MatrixPolicy<double> matrix{ 2, 3, 0.0 };

  matrix[1][0] = 13.2;
  matrix[1][1] = 31.2;
  matrix[1][2] = 314.2;

  const MatrixPolicy<double> const_matrix = matrix;
  std::vector<double> slice = const_matrix[1];

  EXPECT_EQ(slice[0], 13.2);
  EXPECT_EQ(slice[1], 31.2);
  EXPECT_EQ(slice[2], 314.2);

  return const_matrix;
}

template<template<class> class MatrixPolicy>
MatrixPolicy<double> testConversionFromVector()
{
  MatrixPolicy<double> zero_matrix = std::vector<std::vector<double>>{};

  EXPECT_EQ(zero_matrix.size(), 0);

  std::vector<std::vector<double>> vec = { { 412.3, 32.4, 41.3 }, { 5.33, -0.3, 31.2 } };

  MatrixPolicy<double> matrix = vec;

  EXPECT_EQ(matrix.size(), 2);
  EXPECT_EQ(matrix[0].size(), 3);
  EXPECT_EQ(matrix[0][0], 412.3);
  EXPECT_EQ(matrix[0][1], 32.4);
  EXPECT_EQ(matrix[0][2], 41.3);
  EXPECT_EQ(matrix[1].size(), 3);
  EXPECT_EQ(matrix[1][0], 5.33);
  EXPECT_EQ(matrix[1][1], -0.3);
  EXPECT_EQ(matrix[1][2], 31.2);

  std::vector<std::vector<int>> bad_vector = { { 3 }, { 4, 5 }, { 5 } };

  MatrixPolicy<int> bad_matrix;
  EXPECT_DEATH(bad_matrix = bad_vector, "Invalid vector for matrix assignment");

  return matrix;
}

template<template<class> class MatrixPolicy>
MatrixPolicy<double> testAssignmentFromVector()
{
  MatrixPolicy<double> matrix{ 4, 3, 0.0 };
  std::vector<double> other = { 12.3, 15.1, 24.3 };
  std::vector<double> big_other = { 14.3, 52.3, 65.7, 16.34 };
  std::vector<double> small_other = { 13.2, 52.8 };

  matrix[2] = other;

  EXPECT_EQ(matrix[0][0], 0.0);
  EXPECT_EQ(matrix[2][0], 12.3);
  EXPECT_EQ(matrix[2][1], 15.1);
  EXPECT_EQ(matrix[2][2], 24.3);
  EXPECT_EQ(matrix[3][0], 0.0);

  matrix[2] = big_other;

  EXPECT_EQ(matrix[0][0], 0.0);
  EXPECT_EQ(matrix[2][0], 14.3);
  EXPECT_EQ(matrix[2][1], 52.3);
  EXPECT_EQ(matrix[2][2], 65.7);
  EXPECT_EQ(matrix[3][0], 0.0);

  EXPECT_DEATH(matrix[2] = small_other, "Matrix row size mismatch in assignment from vector");

  return matrix;
}