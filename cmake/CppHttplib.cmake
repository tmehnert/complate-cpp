Include(FetchContent)

message("-- Fetching cpp-httplib...")
FetchContent_Declare(
        cpp-httplib
        GIT_REPOSITORY https://github.com/yhirose/cpp-httplib.git
        GIT_TAG        v0.9.7
)

FetchContent_MakeAvailable(cpp-httplib)
message("-- Fetching cpp-httplib - done")
