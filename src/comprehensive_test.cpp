/**
 * @file comprehensive_test.cpp
 * @brief Complete test suite for fstring v3.0
 * @author zugyonozz
 * @date 2025-11-26
 */

#include <zuu/fstring.hpp>
#include <iostream>
#include <cassert>

using namespace zuu;
using namespace zuu::str;
using namespace zuu::fmt;
using namespace zuu::literals;

// Test counter
int tests_passed = 0;
int tests_failed = 0;

#define TEST(name) \
    void test_##name(); \
    void run_test_##name() { \
        try { \
            test_##name(); \
            std::cout << "✅ " << #name << '\n'; \
            ++tests_passed; \
        } catch (const std::exception& e) { \
            std::cout << "❌ " << #name << ": " << e.what() << '\n'; \
            ++tests_failed; \
        } \
    } \
    void test_##name()

// ==================== Core Tests ====================

TEST(basic_construction) {
    fstring<32> s1;
    assert(s1.empty());
    
    fstring<32> s2 = "hello";
    assert(s2.size() == 5);
    assert(s2 == "hello");
    
    auto s3 = "world"_sfs;
    assert(s3 == "world");
}

TEST(concatenation) {
    fstring<32> s1 = "hello";
    fstring<32> s2 = "world";
    
    auto s3 = s1 + " " + s2;
    assert(s3 == "hello world");
    
    s1 += "!";
    assert(s1 == "hello!");
}

TEST(element_access) {
    fstring<32> s = "test";
    assert(s[0] == 't');
    assert(s.front() == 't');
    assert(s.back() == 't');
    assert(s.at(1) == 'e');
}

// ==================== Trim Tests ====================

TEST(trim_operations) {
    auto s1 = "  hello  "_sfs | trim;
    assert(s1 == "hello");
    
    auto s2 = "  hello  "_sfs | trim_left;
    assert(s2 == "hello  ");
    
    auto s3 = "  hello  "_sfs | trim_right;
    assert(s3 == "  hello");
}

TEST(trim_piping) {
    auto result = "  TEST  "_sfs | trim | to_lower;
    assert(result == "test");
}

// ==================== Case Tests ====================

TEST(case_conversion) {
    auto lower = to_lower("HELLO"_sfs);
    assert(lower == "hello");
    
    auto upper = to_upper("hello"_sfs);
    assert(upper == "HELLO");
    
    auto title = to_title("hello world"_sfs);
    assert(title == "Hello World");
}

TEST(case_piping) {
    auto result = "  hello  "_sfs | trim | to_upper;
    assert(result == "HELLO");
}

TEST(case_insensitive_compare) {
    fstring<32> s1 = "Hello";
    fstring<32> s2 = "HELLO";
    assert(equals_ignore_case(s1, s2));
}

// ==================== Split Tests ====================

TEST(split_char) {
    auto parts = split("a,b,c"_sfs, ',');
    assert(parts.count == 3);
    assert(parts[0] == "a");
    assert(parts[1] == "b");
    assert(parts[2] == "c");
}

TEST(split_string) {
    auto parts = split_by("a::b::c"_sfs, "::"_sfs);
    assert(parts.count == 3);
    assert(parts[0] == "a");
    assert(parts[1] == "b");
    assert(parts[2] == "c");
}

TEST(split_lines) {
    auto parts = "line1\nline2\nline3"_fs | split_lines;
    assert(parts.count == 3);
    assert(parts[0] == "line1");
    assert(parts[1] == "line2");
    assert(parts[2] == "line3");
}

TEST(split_whitespace) {
    auto parts = "a  b\tc\nd"_fs | split_whitespace;
    assert(parts.count == 4);
    assert(parts[0] == "a");
    assert(parts[1] == "b");
    assert(parts[2] == "c");
    assert(parts[3] == "d");
}

TEST(split_piping) {
    auto parts = "  a , b , c  "_fs | trim | split(',');
    assert(parts.count == 3);
}

TEST(partition) {
    auto result = partition("key=value"_sfs, '=');
    assert(result.found);
    assert(result.first == "key");
    assert(result.second == "value");
}

TEST(rsplit) {
    auto parts = rsplit("a.b.c.d"_sfs, '.');
    assert(parts.count == 4);
    assert(parts[0] == "a");
    assert(parts[3] == "d");
}

// ==================== Join Tests ====================

TEST(join_char) {
    fstring<16> arr[] = {"a"_sfs, "b"_sfs, "c"_sfs};
    auto result = join(arr, ',');
    assert(result == "a,b,c");
}

TEST(join_string) {
    auto parts = split("a,b,c"_sfs, ',');
    auto rejoined = join(parts, ", "_sfs);
    assert(rejoined == "a, b, c");
}

TEST(join_split_roundtrip) {
    fstring<64> original = "apple,banana,cherry";
    auto parts = split(original, ',');
    auto rejoined = join(parts, ',');
    assert(original == rejoined);
}

// ==================== Find Tests ====================

TEST(contains_operations) {
    fstring<32> s = "hello world";
    
    assert(contains(s, 'o'));
    assert(!contains(s, 'x'));
    
    assert(contains(s, "world"));
    assert(!contains(s, "xyz"));
}

TEST(starts_ends_with) {
    fstring<32> s = "hello world";
    
    assert(starts_with(s, "hello"));
    assert(!starts_with(s, "world"));
    
    assert(ends_with(s, "world"));
    assert(!ends_with(s, "hello"));
    
    assert(starts_with(s, 'h'));
    assert(ends_with(s, 'd'));
}

TEST(find_operations) {
    fstring<32> s = "hello world";
    
    auto pos1 = find(s, 'o');
    assert(pos1 == 4);
    
    auto pos2 = rfind(s, 'o');
    assert(pos2 == 7);
    
    auto pos3 = find(s, "world");
    assert(pos3 == 6);
}

TEST(count_operations) {
    fstring<32> s = "hello world";
    
    assert(count(s, 'l') == 3);
    assert(count(s, 'o') == 2);
    assert(count(s, 'x') == 0);
    
    assert(count(s, "l") == 3);
    assert(count(s, "ll") == 1);
}

TEST(find_first_of) {
    fstring<32> s = "hello world";
    
    auto pos = find_first_of(s, "aeiou");
    assert(pos == 1); // 'e'
    
    auto pos2 = find_last_of(s, "aeiou");
    assert(pos2 == 7); // 'o'
}

TEST(contains_any) {
    fstring<32> s = "hello";
    
    assert(contains_any(s, "aeiou"));
    assert(!contains_any(s, "xyz"));
}

// ==================== Formatting Tests ====================

TEST(integer_formatting) {
    assert(to_fstring(42) == "42");
    assert(to_fstring(-123) == "-123");
    assert(to_fstring(0) == "0");
}

TEST(hex_formatting) {
    assert(to_fstring(hex(255)) == "0xff");
    assert(to_fstring(hex(255, true)) == "0xFF");
    assert(to_fstring(hex(0)) == "0x0");
}

TEST(binary_formatting) {
    assert(to_fstring(bin(5)) == "0b101");
    assert(to_fstring(bin(0)) == "0b0");
    assert(to_fstring(bin(255)) == "0b11111111");
}

TEST(padding_formatting) {
    assert(to_fstring(pad_left(7, 3, '0')) == "007");
    assert(to_fstring(pad_left(42, 5, ' ')) == "   42");
}

TEST(float_formatting) {
    auto f1 = to_fstring(3.14, 2);
    assert(f1 == "3.14");
    
    auto f2 = to_fstring(2.71828, 3);
    assert(f2 == "2.718");
}

TEST(bool_formatting) {
    assert(to_fstring(true) == "true");
    assert(to_fstring(false) == "false");
}

// ==================== Parsing Tests ====================

TEST(parse_int) {
    assert(parse_int<int>("42"_sfs) == 42);
    assert(parse_int<int>("-123"_sfs) == -123);
    assert(parse_int<int>("0"_sfs) == 0);
}

TEST(parse_float) {
    float f1 = parse_float<float>("3.14"_sfs);
    assert(f1 > 3.13 && f1 < 3.15);
    
    float f2 = parse_float<float>("2.71"_sfs);
    assert(f2 > 2.70 && f2 < 2.72);
}

// ==================== Complex Pipeline Tests ====================

TEST(complex_pipeline_1) {
    auto result = "  HELLO, WORLD!  "_fs 
        | trim 
        | to_lower 
        | [](auto s) { 
            fstring<64> r = "["; 
            r += s; 
            r += "]"; 
            return r; 
          };
    
    assert(result == "[hello, world!]");
}

TEST(complex_pipeline_2) {
    auto parts = "a,b,c"_sfs | split(',');
    assert(parts.count == 3);
    
    // Transform each part
    for (auto& part : parts) {
        part = part | to_upper;
    }
    
    auto rejoined = join(parts, '-');
    assert(rejoined == "A-B-C");
}

TEST(complex_pipeline_3) {
    // CSV-like processing
    auto line = "  John , 30 , Developer  "_fs;
    auto fields = line | trim | split(',');
    
    assert(fields.count == 3);
    
    for (auto& field : fields) {
        field = field | trim;
    }
    
    assert(fields[0] == "John");
    assert(fields[1] == "30");
    assert(fields[2] == "Developer");
}

// ==================== Constexpr Tests ====================

constexpr auto compile_time_test() {
    fstring<32> s = "test";
    return s.size() == 4;
}

TEST(constexpr_operations) {
    static_assert(compile_time_test());
    
    constexpr fstring<16> ct = "compile";
    static_assert(ct.size() == 7);
    static_assert(!ct.empty());
}

// ==================== Type Aliases Tests ====================

TEST(type_aliases) {
    using namespace types;
    
    name_str name = "Alice";
    assert(name.max_size() == 64);
    
    path_str path = "/usr/local/bin";
    assert(path.max_size() == 260);
    
    uuid_str uuid = "550e8400-e29b-41d4-a716-446655440000";
    assert(uuid.size() == 36);
    
    ip_str ip = "192.168.1.1";
    assert(ip.max_size() == 45);
}

// ==================== Edge Cases ====================

TEST(empty_string_operations) {
    fstring<32> empty;
    
    assert(empty.empty());
    assert(empty.size() == 0);
    
    auto trimmed = empty | trim;
    assert(trimmed.empty());
    
    auto upper = empty | to_upper;
    assert(upper.empty());
}

TEST(full_capacity) {
    fstring<5> s = "12345";
    assert(s.full());
    
    s += "6"; // Should not append (full)
    assert(s.size() == 5);
    assert(s == "12345");
}

TEST(special_characters) {
    fstring<32> s = "hello\nworld\t!";
    assert(s.size() == 13);
    
    auto lines = s | split_lines;
    assert(lines.count == 2);
}

// ==================== Main ====================

int main() {
    std::cout << "====================================\n";
    std::cout << "  fstring v" << version::string << " - Test Suite\n";
    std::cout << "====================================\n\n";
    
    // Run all tests
    run_test_basic_construction();
    run_test_concatenation();
    run_test_element_access();
    
    run_test_trim_operations();
    run_test_trim_piping();
    
    run_test_case_conversion();
    run_test_case_piping();
    run_test_case_insensitive_compare();
    
    run_test_split_char();
    run_test_split_string();
    run_test_split_lines();
    run_test_split_whitespace();
    run_test_split_piping();
    run_test_partition();
    run_test_rsplit();
    
    run_test_join_char();
    run_test_join_string();
    run_test_join_split_roundtrip();
    
    run_test_contains_operations();
    run_test_starts_ends_with();
    run_test_find_operations();
    run_test_count_operations();
    run_test_find_first_of();
    run_test_contains_any();
    
    run_test_integer_formatting();
    run_test_hex_formatting();
    run_test_binary_formatting();
    run_test_padding_formatting();
    run_test_float_formatting();
    run_test_bool_formatting();
    
    run_test_parse_int();
    run_test_parse_float();
    
    run_test_complex_pipeline_1();
    run_test_complex_pipeline_2();
    run_test_complex_pipeline_3();
    
    run_test_constexpr_operations();
    
    run_test_type_aliases();
    
    run_test_empty_string_operations();
    run_test_full_capacity();
    run_test_special_characters();
    
    // Summary
    std::cout << "\n====================================\n";
    std::cout << "  Tests Passed: " << tests_passed << '\n';
    std::cout << "  Tests Failed: " << tests_failed << '\n';
    std::cout << "====================================\n";
    
    if (tests_failed == 0) {
        std::cout << "\n🎉 All tests passed!\n";
        return 0;
    } else {
        std::cout << "\n❌ Some tests failed!\n";
        return 1;
    }
}