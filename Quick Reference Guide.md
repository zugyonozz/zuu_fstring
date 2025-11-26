# 📖 fstring v3.0 - Quick Reference Guide

## 🚀 Getting Started

```cpp
#include <zuu/fstring.hpp>

using namespace zuu;
using namespace zuu::str;
using namespace zuu::fmt;
using namespace zuu::literals;
```

---

## 📝 Basic Operations

### Construction
```cpp
fstring<32> s1;                    // Empty string
fstring<32> s2 = "hello";          // From literal
fstring<32> s3("world", 5);        // From pointer + length
auto s4 = "test"_fs;               // User-defined literal (256 cap)
auto s5 = "hi"_sfs;                // Small literal (32 cap)
```

### Concatenation
```cpp
auto s1 = "hello"_sfs + " " + "world"_sfs;
s1 += "!";
```

### Element Access
```cpp
char c1 = s[0];        // No bounds check
char c2 = s.at(0);     // With bounds check
char first = s.front();
char last = s.back();
```

### Capacity
```cpp
bool is_empty = s.empty();
size_t len = s.length();      // or s.size()
size_t cap = s.max_size();    // Compile-time capacity
size_t avail = s.available();
bool is_full = s.full();
```

---

## 🔧 String Algorithms

### Trimming (Whitespace Removal)

```cpp
// Traditional
auto result = trim(str);
auto result = trim_left(str);
auto result = trim_right(str);

// Piped (modern)
auto result = str | trim;
auto result = str | trim_left;
auto result = str | trim_right;

// Custom predicate
auto result = str | trim_if([](char c) { return c == '*'; });
```

### Case Conversion

```cpp
// Traditional
auto lower = to_lower("HELLO"_sfs);
auto upper = to_upper("hello"_sfs);
auto title = to_title("hello world"_sfs);

// Piped
auto lower = "HELLO"_sfs | to_lower;
auto upper = "hello"_sfs | to_upper;
auto title = "hello world"_sfs | to_title;

// Toggle case
auto toggled = "HeLLo"_sfs | toggle_case;  // "hEllO"

// Case-insensitive compare
bool equal = equals_ignore_case("Hello"_sfs, "HELLO"_sfs);
```

### Split Operations

```cpp
// Split by character
auto parts = split("a,b,c"_sfs, ',');
for (const auto& part : parts) {
    std::cout << part << '\n';
}

// Split by string
auto parts = split_by("a::b::c"_sfs, "::"_sfs);

// Split lines (handles \n, \r, \r\n)
auto lines = "line1\nline2\nline3"_fs | split_lines;

// Split whitespace (space, tab, newline)
auto words = "hello  world\tthere"_fs | split_whitespace;

// Partition (split into 2 parts)
auto result = partition("key=value"_sfs, '=');
if (result.found) {
    std::cout << result.first << " -> " << result.second << '\n';
}

// Reverse split (from right)
auto parts = rsplit("a.b.c.d"_sfs, '.');
```

### Join Operations

```cpp
// Join with character
fstring<16> arr[] = {"a"_sfs, "b"_sfs, "c"_sfs};
auto result = join(arr, ',');  // "a,b,c"

// Join with string
auto result = join(arr, ", "_sfs);  // "a, b, c"

// Join split result
auto parts = split("a,b,c"_sfs, ',');
auto rejoined = join(parts, ',');  // "a,b,c"
```

### Find & Search

```cpp
fstring<32> s = "hello world";

// Contains
bool has_char = contains(s, 'o');
bool has_str = contains(s, "world");

// Starts/Ends with
bool starts = starts_with(s, "hello");
bool ends = ends_with(s, "world");
bool starts_ch = starts_with(s, 'h');

// Find position
size_t pos = find(s, 'o');              // First 'o'
size_t rpos = rfind(s, 'o');            // Last 'o'
size_t str_pos = find(s, "world");

// Count occurrences
size_t cnt = count(s, 'l');             // 3
size_t cnt_str = count(s, "ll");        // 1

// Find first/last of any character
size_t pos = find_first_of(s, "aeiou"); // First vowel
size_t pos = find_last_of(s, "aeiou");  // Last vowel

// Contains any
bool has_vowel = contains_any(s, "aeiou");
```

---

## 🎨 Formatting System

### Integer Formatting

```cpp
auto dec = to_fstring(42);                    // "42"
auto hex = to_fstring(hex(255));              // "0xff"
auto hex_upper = to_fstring(hex(255, true));  // "0xFF"
auto bin = to_fstring(bin(5));                // "0b101"
auto padded = to_fstring(pad_left(7, 3));     // "007"
```

### Float Formatting

```cpp
auto f1 = to_fstring(3.14159);        // "3.141590" (default 6 digits)
auto f2 = to_fstring(3.14159, 2);     // "3.14"
auto f3 = to_fstring(2.71828, 4);     // "2.7182"
```

### Boolean Formatting

```cpp
auto t = to_fstring(true);   // "true"
auto f = to_fstring(false);  // "false"
```

### Parsing

```cpp
int i = parse_int<int>("42"_sfs);
int hex_val = parse_int<int>("ff"_sfs, 16);
float f = parse_float<float>("3.14"_sfs);
```

---

## 🔗 Functional Piping

### Basic Piping

```cpp
// Single operation
auto result = "  HELLO  "_sfs | trim;

// Chain operations
auto result = "  HELLO  "_sfs | trim | to_lower;

// With custom lambda
auto result = "test"_sfs 
    | trim 
    | to_upper 
    | [](auto s) { s += "!!!"; return s; };
```

### Composable Pipelines

```cpp
// Define reusable pipeline
auto sanitize = trim | to_lower;

// Use it multiple times
auto s1 = "  INPUT1  "_sfs | sanitize;
auto s2 = "  INPUT2  "_sfs | sanitize;

// Compose pipelines
auto process = trim | to_lower | [](auto s) { 
    return s + "!"; 
};
```

### Complex Example

```cpp
// CSV processing
auto line = "  John , 30 , Developer  "_fs;
auto fields = line | trim | split(',');

for (auto& field : fields) {
    field = field | trim;
}

std::cout << fields[0] << ", " 
          << fields[1] << ", " 
          << fields[2] << '\n';
```

---

## 📦 Type Aliases (Semantic Names)

```cpp
using namespace types;

name_str username = "alice";           // 64 capacity
path_str file_path = "/usr/local";     // 260 capacity
uuid_str unique_id = "550e8400...";    // 36 capacity
ip_str address = "192.168.1.1";        // 45 capacity
email_str contact = "user@example";    // 254 capacity
url_str website = "https://...";       // 2048 capacity
datetime_str timestamp = "2025-11-26"; // 32 capacity
msg_str message = "Hello!";            // 256 capacity

// Standard sizes
str8, str16, str32, str64, str128, str256, str512, str1k
```

---

## ⚡ Constexpr Support

```cpp
// Compile-time string operations
constexpr auto compile_time_fn() {
    fstring<32> s = "test";
    return s | to_upper;  // All at compile-time!
}

static_assert(compile_time_fn() == "TEST");

constexpr fstring<16> ct = "hello";
static_assert(ct.size() == 5);
static_assert(!ct.empty());
```

---

## 🎯 Common Patterns

### Pattern 1: CSV Parsing
```cpp
auto line = "name,age,city"_fs;
auto fields = line | split(',');
for (const auto& field : fields) {
    process(field);
}
```

### Pattern 2: Path Manipulation
```cpp
auto path = "/usr/local/bin/app"_fs;
auto parts = split(path, '/');
auto filename = parts[parts.count - 1];
```

### Pattern 3: String Cleaning
```cpp
auto clean = [](auto str) {
    return str | trim | to_lower | remove_whitespace;
};

auto result = clean("  MESSY INPUT  ");
```

### Pattern 4: Format Builder
```cpp
auto format_id = [](int id) {
    return to_fstring(pad_left(id, 6, '0'));
};

auto id_str = format_id(42);  // "000042"
```

### Pattern 5: Validation Pipeline
```cpp
auto validate_email = [](auto email) {
    return (email | contains('@')) 
        && (email | contains('.'))
        && !email.empty();
};

bool valid = validate_email("user@example.com"_fs);
```

---

## 🚫 Common Pitfalls

### ❌ Don't: Capacity Overflow
```cpp
fstring<5> s = "hello world";  // Only "hello" stored!
```

### ✅ Do: Check Capacity
```cpp
fstring<32> s = "hello world";  // Plenty of space
assert(!s.full());
```

---

### ❌ Don't: Modify During Iteration
```cpp
for (auto& part : parts) {
    parts = split(part, ' ');  // BAD: Modifying container
}
```

### ✅ Do: Create New Container
```cpp
split_result<char, 32> new_parts;
for (const auto& part : parts) {
    auto sub = split(part, ' ');
    // Process sub parts
}
```

---

### ❌ Don't: Assume Mutable Split Result
```cpp
auto parts = split(str, ',');
parts[0] += "suffix";  // OK: parts are mutable
```

### ✅ Do: Understand Split Result
```cpp
auto parts = split(str, ',');
// parts.parts[] are mutable fstrings
// parts.count tells you how many are valid
```

---

## 🔍 Performance Tips

1. **Use appropriate capacity**: Don't use `fstring<1024>` for short strings
2. **Prefer piping**: Compiler optimizes chains better
3. **Use `string_view` internally**: Algorithms already do this
4. **Enable optimizations**: `-O2` or higher
5. **Constexpr when possible**: Moves work to compile-time

```cpp
// Bad: Too large
fstring<1024> name = "John";

// Good: Right-sized
name_str name = "John";  // 64 capacity
```

---

## 📚 Full Example

```cpp
#include <zuu/fstring.hpp>
#include <iostream>

int main() {
    using namespace zuu;
    using namespace zuu::str;
    using namespace zuu::fmt;
    using namespace zuu::literals;
    
    // Process CSV data
    auto data = "  Alice , 30 , Engineer  \n  Bob , 25 , Designer  "_fs;
    
    // Split into lines
    auto lines = data | split_lines;
    
    // Process each line
    for (const auto& line : lines) {
        auto fields = line | trim | split(',');
        
        for (auto& field : fields) {
            field = field | trim;
        }
        
        std::cout << "Name: " << fields[0] 
                  << ", Age: " << fields[1]
                  << ", Role: " << fields[2] << '\n';
    }
    
    // Format output
    for (int i = 0; i < 10; ++i) {
        auto id = to_fstring(pad_left(i, 3, '0'));
        auto hex_id = to_fstring(hex(i));
        std::cout << "ID: " << id << " (hex: " << hex_id << ")\n";
    }
    
    return 0;
}
```

---

## 🔗 Additional Resources

- **Full Documentation**: https://fstring.readthedocs.io
- **GitHub**: https://github.com/zugyonozz/fstring
- **Migration Guide**: See `MIGRATION.md`
- **Troubleshooting**: See troubleshooting artifact
- **Examples**: See `examples/` directory

---

**Version:** 3.0.0  
**Last Updated:** 2025-11-26