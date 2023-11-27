cmake_minimum_required(VERSION 3.10)

add_subdirectory(thirdparty/json)

add_library(StraitXJson 
	"./sources/json/json.cpp"
)

target_include_directories(StraitXJson 
	PUBLIC "./sources"
)

target_link_libraries(StraitXJson
	PUBLIC StraitXCore
	PUBLIC nlohmann_json
)