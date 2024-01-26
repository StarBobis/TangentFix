#include "Functions.h"


std::wstring to_wide_string(std::string input) {
    if (input.empty()) return L"";

    int size_needed = MultiByteToWideChar(CP_UTF8, 0, input.c_str(), -1, NULL, 0);
    if (size_needed == 0) {
        // Handle error appropriately
        throw std::runtime_error("Failed in MultiByteToWideChar conversion.");
    }

    std::wstring wstrTo(size_needed, L'\0');
    int chars_converted = MultiByteToWideChar(CP_UTF8, 0, input.c_str(), -1, &wstrTo[0], size_needed);
    if (chars_converted == 0) {
        // Handle error appropriately
        throw std::runtime_error("Failed in MultiByteToWideChar conversion.");
    }

    // Remove the null terminator as it is implicitly handled in std::wstring
    wstrTo.pop_back();

    return wstrTo;
}


std::string to_byte_string(std::wstring input) {
    if (input.empty()) return "";

    int size_needed = WideCharToMultiByte(CP_UTF8, 0, input.c_str(), -1, NULL, 0, NULL, NULL);
    if (size_needed == 0) {
        // Handle error appropriately
        throw std::runtime_error("Failed in WideCharToMultiByte conversion.");
    }

    std::string strTo(size_needed, '\0');
    int bytes_converted = WideCharToMultiByte(CP_UTF8, 0, input.c_str(), -1, &strTo[0], size_needed, NULL, NULL);
    if (bytes_converted == 0) {
        // Handle error appropriately
        throw std::runtime_error("Failed in WideCharToMultiByte conversion.");
    }

    // Remove the null terminator as it is implicitly handled in std::string
    strTo.pop_back();

    return strTo;
}


double VectorLength(const Vector3D& vector) {
    return std::sqrt(vector.x * vector.x + vector.y * vector.y + vector.z * vector.z);
}


Vector3D NormalizeVector(const Vector3D vector) {
    double length = VectorLength(vector);
    Vector3D normalizedVector;
    normalizedVector.x = vector.x / length;
    normalizedVector.y = vector.y / length;
    normalizedVector.z = vector.z / length;
    return normalizedVector;
}


Vector3D AddVectors(const Vector3D vector1, const Vector3D vector2) {
    Vector3D result;
    result.x = vector1.x + vector2.x;
    result.y = vector1.y + vector2.y;
    result.z = vector1.z + vector2.z;
    return result;
}


double bytesToFloat(const std::vector<std::byte>& bytes) {
    std::uint32_t value = 0;
    for (std::size_t i = 0; i < sizeof(float); ++i) {
        value |= static_cast<std::uint32_t>(static_cast<std::uint8_t>(bytes[i])) << (8 * i);
    }

    float result;
    std::memcpy(&result, &value, sizeof(float));
    return result;
}


std::vector<std::byte> GetRange(const std::vector<std::byte>& vec, std::size_t startIndex, std::size_t endIndex)
{
    if (startIndex >= vec.size() || startIndex > endIndex)
    {
        return {};
    }

    std::vector<std::byte> rangeVec(endIndex - startIndex);
    std::copy(vec.begin() + startIndex, vec.begin() + endIndex, rangeVec.begin());

    return rangeVec;
}


std::wstring formatedFloat(double originalValue, int reserveCount) {
    std::wstring valueDoubleStr;

    double valueDouble = originalValue;
    if (std::fabs(valueDouble) < 0.0001) {
        valueDouble = 0.0;
    }
    std::ostringstream out_stream_value;
    out_stream_value << std::fixed << std::setprecision(reserveCount) << valueDouble;
    valueDoubleStr = to_wide_string(out_stream_value.str());

    if (valueDouble == 0.0) {
        valueDoubleStr = L"0";
    }
    return valueDoubleStr;
}


std::vector<std::byte> PackNumberR32_FLOAT_littleIndian(float number)
{
    std::vector<std::byte> packedBytes(sizeof(float));
    std::memcpy(packedBytes.data(), &number, sizeof(float));
    return packedBytes;
}


std::vector<std::wstring> SplitString(const std::wstring input, wchar_t delimiter)
{
    std::wstringstream ss(input);
    std::wstring item;
    std::vector<std::wstring> tokens;

    while (std::getline(ss, item, delimiter))
    {
        tokens.push_back(item);
    }
    return tokens;
}


