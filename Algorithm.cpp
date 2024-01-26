#include "Functions.h"


std::vector<std::vector<double>> CalculateAverageNormalsAndStoreTangent(const std::vector<std::vector<double>> positions, const std::vector<std::vector<double>> normals) {
    std::unordered_map<std::wstring, std::wstring> position_normal_map;

    for (int i = 0; i < positions.size(); i++) {
        std::vector<double> position_double = positions[i];
        std::vector<double> normal_double = normals[i];

        std::wstring formated_position = formatedFloat(position_double[0], 10) + L"_" + formatedFloat(position_double[1], 10) + L"_" + formatedFloat(position_double[2], 10);
        std::wstring formated_normal = formatedFloat(normal_double[0], 10) + L"_" + formatedFloat(normal_double[1], 10) + L"_" + formatedFloat(normal_double[2], 10);

        if (position_normal_map.contains(formated_position)) {
            std::wstring old_formated_normal = position_normal_map[formated_position];
            std::vector<std::wstring> split_results = SplitString(old_formated_normal, L'_');

            Vector3D OldNormalVector = { std::stod(split_results[0]),std::stod(split_results[1]) ,std::stod(split_results[2]) };
            Vector3D NowNormalVector = { normal_double[0], normal_double[1], normal_double[2] };
            Vector3D newVector = NormalizeVector(AddVectors(OldNormalVector, NowNormalVector));

            std::wstring result_formated_normal = formatedFloat(newVector.x, 10) + L"_" + formatedFloat(newVector.y, 10) + L"_" + formatedFloat(newVector.z, 10);
            position_normal_map[formated_position] = result_formated_normal;
        }
        else {
            position_normal_map[formated_position] = formated_normal;
        }

    }

    std::vector<std::vector<double>> tangents;
    for (int i = 0; i < positions.size(); i++) {
        std::vector<double> position_double = positions[i];
        std::wstring formated_position = formatedFloat(position_double[0], 10) + L"_" + formatedFloat(position_double[1], 10) + L"_" + formatedFloat(position_double[2], 10);
        std::wstring formated_normal = position_normal_map[formated_position];
        std::vector<std::wstring> split_results = SplitString(formated_normal, L'_');
        std::vector<double> OldNormalVector = { std::stod(split_results[0]),std::stod(split_results[1]) ,std::stod(split_results[2]) ,0 };
        tangents.push_back(OldNormalVector);
    }

    return tangents;
}


std::unordered_map<std::wstring, std::vector<std::byte>> TANGENT_averageNormal(std::unordered_map<std::wstring, std::vector<std::byte>> inputVBCategoryDataMap) {
    std::unordered_map<std::wstring, std::vector<std::byte>> newVBCategoryDataMap;
    std::vector<std::byte> PositionCategoryValues = inputVBCategoryDataMap[L"Position"];

    std::vector<std::vector<double>> POSITION_VALUES;
    std::vector<std::vector<double>> NORMAL_VALUES;

    for (std::size_t i = 0; i < PositionCategoryValues.size(); i = i + 40)
    {
        std::vector<std::byte> POSITION_X = GetRange(PositionCategoryValues, i, i + 4);
        std::vector<std::byte> POSITION_Y = GetRange(PositionCategoryValues, i + 4, i + 8);
        std::vector<std::byte> POSITION_Z = GetRange(PositionCategoryValues, i + 8, i + 12);
        std::vector<double> POSITION_DOUBLE = { bytesToFloat(POSITION_X), bytesToFloat(POSITION_Y) ,bytesToFloat(POSITION_Z) };
        POSITION_VALUES.push_back(POSITION_DOUBLE);

        std::vector<std::byte> NORMAL_X = GetRange(PositionCategoryValues, i + 12, i + 16);
        std::vector<std::byte> NORMAL_Y = GetRange(PositionCategoryValues, i + 16, i + 20);
        std::vector<std::byte> NORMAL_Z = GetRange(PositionCategoryValues, i + 20, i + 24);
        std::vector<double> NORMAL_DOUBLE = { bytesToFloat(NORMAL_X), bytesToFloat(NORMAL_Y) ,bytesToFloat(NORMAL_Z) };
        NORMAL_VALUES.push_back(NORMAL_DOUBLE);
    }


    std::vector<std::vector<double>> TANGENT_VALUES = CalculateAverageNormalsAndStoreTangent(POSITION_VALUES, NORMAL_VALUES);

    std::vector<std::byte> newPositionCategoryValues;
    int count = 0;
    for (std::size_t i = 0; i < PositionCategoryValues.size(); i = i + 40)
    {
        std::vector<std::byte> POSITION_NORMAL_VALUES = GetRange(PositionCategoryValues, i, i + 24);

        std::vector<double> TANGENT_DOUBLES = TANGENT_VALUES[count];

        std::vector<std::byte> TANGENT_X;
        std::vector<std::byte> TANGENT_Y;
        std::vector<std::byte> TANGENT_Z;
        std::vector<std::byte> TANGENT_W;

        double tangent_double_x = TANGENT_DOUBLES[0];

        TANGENT_X = PackNumberR32_FLOAT_littleIndian((float)TANGENT_DOUBLES[0]);
        TANGENT_Y = PackNumberR32_FLOAT_littleIndian((float)TANGENT_DOUBLES[1]);
        TANGENT_Z = PackNumberR32_FLOAT_littleIndian((float)TANGENT_DOUBLES[2]);
        TANGENT_W = PackNumberR32_FLOAT_littleIndian((float)TANGENT_DOUBLES[3]);


        newPositionCategoryValues.insert(newPositionCategoryValues.end(), POSITION_NORMAL_VALUES.begin(), POSITION_NORMAL_VALUES.end());

        newPositionCategoryValues.insert(newPositionCategoryValues.end(), TANGENT_X.begin(), TANGENT_X.end());
        newPositionCategoryValues.insert(newPositionCategoryValues.end(), TANGENT_Y.begin(), TANGENT_Y.end());
        newPositionCategoryValues.insert(newPositionCategoryValues.end(), TANGENT_Z.begin(), TANGENT_Z.end());
        newPositionCategoryValues.insert(newPositionCategoryValues.end(), TANGENT_W.begin(), TANGENT_W.end());
        count = count + 1;
    }

    newVBCategoryDataMap = inputVBCategoryDataMap;
    newVBCategoryDataMap[L"Position"] = newPositionCategoryValues;
    return newVBCategoryDataMap;
};


void modifyPositionBuf(std::wstring bufFilePath) {
    // read buf content into buffer file.
    std::ifstream bufFileStream(bufFilePath, std::ios::binary);
    bufFileStream.seekg(0, std::ios::end);
    std::streampos VBFileSize = bufFileStream.tellg();

    if (VBFileSize % 40 != 0) {
        std::cout << "Wrong buffer file or unsupported format! Please use Position.buf which stride is 40." << std::endl;
        std::cout << "Press any key to quit..." << std::endl;
        std::cin.get();
        exit(0);
    }

    bufFileStream.seekg(0);
    std::vector<std::byte> buffer(VBFileSize);
    bufFileStream.read(reinterpret_cast<char*>(buffer.data()), VBFileSize);
    bufFileStream.close();

    std::unordered_map<std::wstring, std::vector<std::byte>> categoryBufferMap;
    categoryBufferMap[L"Position"] = buffer;

    std::unordered_map<std::wstring, std::vector<std::byte>> recalculatedTangentCategoryBufferMap;
    recalculatedTangentCategoryBufferMap = TANGENT_averageNormal(categoryBufferMap);
    categoryBufferMap = recalculatedTangentCategoryBufferMap;

    // Remove old position buf file.
    std::filesystem::remove(to_byte_string(bufFilePath).c_str());
    // Write new buf file.
    std::ofstream outputFile(to_byte_string(bufFilePath), std::ios::binary);
    std::vector<std::byte> positionCategoryData = categoryBufferMap[L"Position"];
    outputFile.write(reinterpret_cast<const char*>(positionCategoryData.data()), positionCategoryData.size());
    outputFile.close();
}