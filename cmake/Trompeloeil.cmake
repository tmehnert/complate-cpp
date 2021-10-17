Include(FetchContent)

message("-- Fetching Trompeloeil...")
FetchContent_Declare(
        Trompeloeil
        GIT_REPOSITORY https://github.com/rollbear/trompeloeil.git
        GIT_TAG        v41
)

FetchContent_MakeAvailable(Trompeloeil)
message("-- Fetching Trompeloeil - done")
