#pragma once

#include <iostream>
#include <fstream>
#include <filesystem>
#include <Windows.h>
#include <unordered_map>
#include <clocale>
#include <codecvt> 


// wide and byte
std::wstring to_wide_string(std::string input);
std::string to_byte_string(std::wstring input);

// vector abstract
struct Vector3D {
    double x;
    double y;
    double z;
};
double VectorLength(const Vector3D& vector);
Vector3D NormalizeVector(const Vector3D vector);
Vector3D AddVectors(const Vector3D vector1, const Vector3D vector2);

//yeah, we don't have split even in C++20 standard.
std::vector<std::wstring> SplitString(const std::wstring input, wchar_t delimiter);

//data type process
double bytesToFloat(const std::vector<std::byte>& bytes);
std::vector<std::byte> GetRange(const std::vector<std::byte>& vec, std::size_t startIndex, std::size_t endIndex);
std::wstring formatedFloat(double originalValue, int reserveCount);
std::vector<std::byte> PackNumberR32_FLOAT_littleIndian(float number);

std::vector<std::vector<double>> CalculateAverageNormalsAndStoreTangent(const std::vector<std::vector<double>> positions, const std::vector<std::vector<double>> normals);
void modifyPositionBuf(std::wstring bufFilePath);