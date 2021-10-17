Include(FetchContent)

message("-- Fetching Catch2...")
FetchContent_Declare(
        Catch2
        GIT_REPOSITORY https://github.com/catchorg/Catch2.git
        GIT_TAG        v2.13.7
)

FetchContent_MakeAvailable(Catch2)
message("-- Fetching Catch2 - done")
