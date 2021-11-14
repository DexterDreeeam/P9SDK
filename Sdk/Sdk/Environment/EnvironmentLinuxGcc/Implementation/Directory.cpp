
#include "../EnvironmentHeader.hpp"
#include "../../Interface.hpp"

void directory::build_path(char* path, s64 len)
{
    assert(path);
    assert_info(path[len - 1] == '/', "directory path should end with \'/\'");

    ::mkdir(path, S_IRWXU | S_IRWXG | S_IRWXO);
}

struct directory_cursor_context
{
    DIR*             _dir;
    dirent*          _dir_ptr;
    char*            _folder;
};

boole directory_cursor::init(const char* directory_path)
{
    assert(directory_path);
    assert(_ctx == nullptr);

    s64 path_len = str_len(directory_path);
    assert_info(
        directory_path[path_len - 1] == '/',
        "directory path should end with \'/\'");

    DIR* dir = ::opendir(directory_path);
    if (dir == nullptr)
    {
        return boole::False;
    }

    auto* ctx = new directory_cursor_context();
    ctx->_dir = dir;
    ctx->_dir_ptr = readdir(dir);
    if (ctx->_dir_ptr == nullptr)
    {
        ::closedir(dir);
        delete ctx;
        return boole::False;
    }
    ctx->_folder = memory::alloc_copy<char>(
        directory_path, path_len + 1, path_len + 1);

    _ctx = ctx;
    if (valid_or_move_next())
    {
        // valid or move
        return boole::True;
    }
    else
    {
        uninit();
        return boole::False;
    }
}

boole directory_cursor::uninit()
{
    auto* ctx = pointer_convert(_ctx, 0, directory_cursor_context*);
    assert(ctx);
    assert(ctx->_folder);
    assert(ctx->_dir);

    if (::closedir(ctx->_dir))
    {
        memory::free(ctx->_folder);
        delete ctx;
        _ctx = nullptr;
        return boole::True;
    }
    else
    {
        return boole::False;
    }
}

const char* directory_cursor::directory_name() const
{
    auto* ctx = pointer_convert(_ctx, 0, directory_cursor_context*);
    assert(ctx);
    assert(ctx->_folder);
    assert(ctx->_dir);

    return ctx->_folder;
}

const char* directory_cursor::name() const
{
    auto* ctx = pointer_convert(_ctx, 0, directory_cursor_context*);
    assert(ctx);
    assert(ctx->_folder);
    assert(ctx->_dir);

    return ctx->_dir_ptr->d_name;
}

boole directory_cursor::is_file() const
{
    auto* ctx = pointer_convert(_ctx, 0, directory_cursor_context*);
    assert(ctx);
    assert(ctx->_folder);
    assert(ctx->_dir);

    return ctx->_dir_ptr->d_type == DT_REG;
}

boole directory_cursor::is_folder() const
{
    auto* ctx = pointer_convert(_ctx, 0, directory_cursor_context*);
    assert(ctx);
    assert(ctx->_folder);
    assert(ctx->_dir);

    return ctx->_dir_ptr->d_type == DT_DIR;
}

boole directory_cursor::move_next()
{
    auto* ctx = pointer_convert(_ctx, 0, directory_cursor_context*);
    assert(ctx);
    assert(ctx->_folder);
    assert(ctx->_dir);

    if (ctx->_dir_ptr)
    {
        ctx->_dir_ptr = ::readdir(ctx->_dir);
        return valid_or_move_next();
    }
    else
    {
        return boole::False;
    }
}

boole directory_cursor::valid_or_move_next()
{
    auto* ctx = pointer_convert(_ctx, 0, directory_cursor_context*);
    assert(ctx);
    assert(ctx->_folder);
    assert(ctx->_dir);

    if (ctx->_dir_ptr == nullptr)
    {
        return boole::False;
    }
    else if (
        str_equal(ctx->_dir_ptr->d_name, ".") ||
        str_equal(ctx->_dir_ptr->d_name, "..") ||
        (ctx->_dir_ptr->d_type != DT_REG && ctx->_dir_ptr->d_type != DT_DIR))
    {
        // invalid
        // try move next
        return move_next();
    }
    else
    {
        // normal file or folder
        return boole::True;
    }
}