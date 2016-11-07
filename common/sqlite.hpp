// SQLite wrapper
// Copyright (C) 2014 Daniel Beer <dlbeer@gmail.com>
//
// Permission to use, copy, modify, and/or distribute this software for
// any purpose with or without fee is hereby granted, provided that the
// above copyright notice and this permission notice appear in all
// copies.
//
// THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL
// WARRANTIES WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED
// WARRANTIES OF MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE
// AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL
// DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR
// PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER
// TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
// PERFORMANCE OF THIS SOFTWARE.

#ifndef IO_SQLITE_HPP_
#define IO_SQLITE_HPP_

#include <exception>
#include <algorithm>
#include <cstring>
#include <cstdint>
#include <string>
#include <sqlite3.h>

namespace io {
namespace sqlite {

// Exception thrown on error
class error : public std::exception {
public:
    error(int c) : _code(c) { }
    virtual ~error() throw ();

    const char *what() const throw ();

    int code() const
    {
	return _code;
    }

private:
    int _code;
};

// Check return code, throw error if not ok
namespace impl {

static inline void check(int c)
{
    if (c != SQLITE_OK)
	throw error(c);
}

void destroy_blob(void *blob);
void destroy_text(void *blob);

}

// Database handle
class db {
public:
    db() : _db(nullptr) { }

    db(const std::string& filename)
    {
	impl::check(::sqlite3_open(filename.c_str(), &_db));
    }

    db(const char *filename)
    {
	impl::check(::sqlite3_open(filename, &_db));
    }

    ~db()
    {
	if (_db)
	    ::sqlite3_close(_db);
    }

    db(const db&) = delete;
    db& operator=(const db&) = delete;

    void swap(db& r)
    {
	std::swap(_db, r._db);
    }

    db(db&& r) : _db(r._db)
    {
	r._db = nullptr;
    }

    db& operator=(db&& r)
    {
	db m(std::move(r));

	swap(m);
	return *this;
    }

    ::sqlite3 *get()
    {
	return _db;
    }

    const ::sqlite3 *get() const
    {
	return _db;
    }

    // Number of changes due to the most recent statement.
    unsigned int changes() const
    {
	return ::sqlite3_changes(_db);
    }

    // Execute a simple statement
    void exec(const std::string& text)
    {
	impl::check(::sqlite3_exec(_db, text.c_str(),
				   nullptr, nullptr, nullptr));
    }

    void exec(const char *text)
    {
	impl::check(::sqlite3_exec(_db, text,
				   nullptr, nullptr, nullptr));
    }

private:
    ::sqlite3 *_db;
};

// Statement
class stmt {
public:
    stmt() : _stmt(nullptr) { }

    stmt(db& db, const char *sql)
    {
	impl::check(::sqlite3_prepare_v2(db.get(), sql, -1, &_stmt, nullptr));
    }

    ~stmt()
    {
	if (_stmt)
	    ::sqlite3_finalize(_stmt);
    }

    stmt(const stmt&) = delete;
    stmt& operator=(const stmt&) = delete;

    void swap(stmt& r)
    {
	std::swap(_stmt, r._stmt);
    }

    stmt(stmt&& r) : _stmt(r._stmt)
    {
	r._stmt = nullptr;
    }

    stmt& operator=(stmt&& r)
    {
	stmt m(std::move(r));

	swap(m);
	return *this;
    }

    ::sqlite3_stmt *get()
    {
	return _stmt;
    }

    const ::sqlite3_stmt *get() const
    {
	return _stmt;
    }

    class binder {
    public:
	binder(stmt& s) : _stmt(s._stmt) { }

	binder& blob(unsigned int i, const void *data, size_t len)
	{
	    uint8_t *copy = new uint8_t[len];

	    ::memcpy(copy, data, len);
	    impl::check(::sqlite3_bind_blob(_stmt, i, copy, len,
					    impl::destroy_blob));
	    return *this;
	}

	binder& blob_ref(unsigned int i, const void *data, size_t len)
	{
	    impl::check(::sqlite3_bind_blob(_stmt, i,
		data, len, nullptr));
	    return *this;
	}

	binder& real(unsigned int i, double value)
	{
	    impl::check(::sqlite3_bind_double(_stmt, i, value));
	    return *this;
	}

	binder& int32(unsigned int i, int32_t value)
	{
	    impl::check(::sqlite3_bind_int(_stmt, i, value));
	    return *this;
	}

	binder& int64(unsigned int i, int64_t value)
	{
	    impl::check(::sqlite3_bind_int64(_stmt, i, value));
	    return *this;
	}

	binder& null(unsigned int i)
	{
	    impl::check(::sqlite3_bind_null(_stmt, i));
	    return *this;
	}

	binder& text(unsigned int i, const char *orig)
	{
	    const size_t len = ::strlen(orig);
	    char *copy = new char[len];

	    ::memcpy(copy, orig, len);
	    impl::check(::sqlite3_bind_text(_stmt, i, copy, len,
			impl::destroy_text));
	    return *this;
	}

	binder& text(unsigned int i, const std::string& value)
	{
	    const char *orig = value.c_str();
	    const size_t len = value.size();
	    char *copy = new char[len];

	    ::memcpy(copy, orig, len);
	    impl::check(::sqlite3_bind_text(_stmt, i, copy, len,
			impl::destroy_text));
	    return *this;
	}

	binder& text_ref(unsigned int i, const std::string& value)
	{
	    impl::check(::sqlite3_bind_text(_stmt, i,
			value.c_str(), value.size(),
			nullptr));
	    return *this;
	}

	binder& text_ref(unsigned int i, const char *value)
	{
	    impl::check(::sqlite3_bind_text(_stmt, i,
			value, -1, nullptr));
	    return *this;
	}

	void clear()
	{
	    impl::check(::sqlite3_clear_bindings(_stmt));
	}

    private:
	::sqlite3_stmt *_stmt;
    };

    binder bind()
    {
	return binder(*this);
    }

    bool step()
    {
	const int c = ::sqlite3_step(_stmt);

	if (c == SQLITE_ROW)
	    return true;

	if (c == SQLITE_DONE)
	    return false;

	throw error(c);
    }

    void exec()
    {
	while (step());
    }

    void reset()
    {
	impl::check(::sqlite3_reset(_stmt));
    }

    class reader {
    public:
	reader(stmt& s) : _stmt(s._stmt) { }

	const void *blob(unsigned int i)
	{
	    return ::sqlite3_column_blob(_stmt, i);
	}

	size_t size(unsigned int i)
	{
	    return ::sqlite3_column_bytes(_stmt, i);
	}

	double real(unsigned int i)
	{
	    return ::sqlite3_column_double(_stmt, i);
	}

	int32_t int32(unsigned int i)
	{
	    return ::sqlite3_column_int(_stmt, i);
	}

	int64_t int64(unsigned int i)
	{
	    return ::sqlite3_column_int64(_stmt, i);
	}

	const char *cstr(unsigned int i)
	{
	    return reinterpret_cast<const char *>
		    (::sqlite3_column_text(_stmt, i));
	}

	std::string text(unsigned int i)
	{
	    return std::string(cstr(i), size(i));
	}

    private:
	::sqlite3_stmt *_stmt;
    };

    reader row()
    {
	return reader(*this);
    }

private:
    ::sqlite3_stmt *_stmt;
};

}
}

#endif
