#include "Functions.h"


//Use wmain or we can't process path contains wide characters.
std::int32_t wmain(std::int32_t argc, wchar_t* argv[])
{
	if (argc <= 1) {
		std::cout << "Parameter not enough! " << std::endl;
		std::cout << "Usage: Drag your Position.buf on this program's exe file." << std::endl;
		std::cout << "Press any key to quit..." << std::endl;
		std::cin.get();
		return 0;
	}

	std::wstring positionBufPath = argv[1];
	std::wcout <<  L"Processing: " << positionBufPath << std::endl;

    // recalculate TANGENT
    modifyPositionBuf(positionBufPath);

	std::cout << "Recalculate TANGENT using AverageNormal algorithm success!" << std::endl;
	std::cout << "Free Program developed by NicoMico. Github Release: https://github.com/StarBobis/TangentFix" << std::endl;
	std::cout << "Press any key to quit..." << std::endl;
	std::cin.get();
	return 0;
}

