/**
 * @file examples.cpp
 * @brief Comprehensive examples for fstring library
 * @author zuudevs (zuudevs@gmail.com)
 * @date 2025-11-24
 */

#include "fstring.hpp"
#include <iostream>
#include <cassert>

using namespace zuu;
using namespace zuu::literals;
using namespace zuu::algorithms;

// ==================== Example 1: Basic Usage ====================
void example_basic_usage() {
    std::cout << "\n=== Basic Usage ===\n";
    
    // Creating fstrings
    fstring<10> str1 = "Hello";
    fstring<10> str2("World");
    auto str3 = "C++"_fs;
    
    std::cout << "str1: " << str1 << "\n";
    std::cout << "str2: " << str2 << "\n";
    std::cout << "str3: " << str3 << "\n";
    
    // Concatenation
    auto greeting = str1 + " " + str2;
    std::cout << "Greeting: " << greeting << "\n";
    
    // Appending
    str1.append("!");
    std::cout << "str1 after append: " << str1 << "\n";
    
    // Length and capacity
    std::cout << "Length: " << str1.length() << ", Capacity: " << str1.max_size() << "\n";
}

// ==================== Example 2: String Operations ====================
void example_string_operations() {
    std::cout << "\n=== String Operations ===\n";
    
    fstring<50> text = "The quick brown fox";
    
    // Finding
    auto pos = text.find("quick");
    std::cout << "Position of 'quick': " << pos << "\n";
    
    // Substring
    auto sub = text.substr<10>(4, 5);
    std::cout << "Substring: " << sub << "\n";
    
    // Contains
    std::cout << "Contains 'fox': " << std::boolalpha << text.contains("fox") << "\n";
    
    // Starts/ends with
    std::cout << "Starts with 'The': " << text.starts_with("The") << "\n";
    std::cout << "Ends with 'fox': " << text.ends_with("fox") << "\n";
    
    // Insert
    text.insert(19, " jumps");
    std::cout << "After insert: " << text << "\n";
    
    // Erase
    text.erase(10, 6);
    std::cout << "After erase: " << text << "\n";
}

// ==================== Example 3: Number Conversions ====================
void example_conversions() {
    std::cout << "\n=== Number Conversions ===\n";
    
    // Integer to string
    auto int_str = to_fstring(42);
    std::cout << "Integer: " << int_str << "\n";
    
    // Float to string
    auto float_str = to_fstring(3.14159, 4);
    std::cout << "Float: " << float_str << "\n";
    
    // Hex conversion
    auto hex_str = to_hex(255);
    std::cout << "Hex: " << hex_str << "\n";
    
    // Binary conversion
    auto bin_str = to_binary(42);
    std::cout << "Binary: " << bin_str << "\n";
    
    // String to number
    fstring<10> num_text = "12345";
    int num = parse_int<int>(num_text);
    std::cout << "Parsed int: " << num << "\n";
    
    fstring<10> float_text = "3.14";
    float fnum = parse_float<float>(float_text);
    std::cout << "Parsed float: " << fnum << "\n";
    
    // Boolean
    auto bool_str = to_fstring(true);
    std::cout << "Boolean: " << bool_str << "\n";
}

// ==================== Example 4: Case Conversion ====================
void example_case_conversion() {
    std::cout << "\n=== Case Conversion ===\n";
    
    fstring<30> text = "Hello World";
    
    auto lower = to_lower(text);
    std::cout << "Lowercase: " << lower << "\n";
    
    auto upper = to_upper(text);
    std::cout << "Uppercase: " << upper << "\n";
    
    fstring<30> mixed = "hello world from cpp";
    auto title = to_title(mixed);
    std::cout << "Title case: " << title << "\n";
}

// ==================== Example 5: Trimming and Padding ====================
void example_trim_pad() {
    std::cout << "\n=== Trimming and Padding ===\n";
    
    fstring<30> text = "  Hello World  ";
    
    auto trimmed = trim(text);
    std::cout << "Trimmed: '" << trimmed << "'\n";
    
    auto left_trimmed = trim_left(text);
    std::cout << "Left trimmed: '" << left_trimmed << "'\n";
    
    auto right_trimmed = trim_right(text);
    std::cout << "Right trimmed: '" << right_trimmed << "'\n";
    
    // Padding
    fstring<20> short_text = "Hi";
    auto padded_left = pad_left(short_text, 10, '*');
    std::cout << "Padded left: '" << padded_left << "'\n";
    
    auto padded_right = pad_right(short_text, 10, '-');
    std::cout << "Padded right: '" << padded_right << "'\n";
    
    auto centered = center(short_text, 10, '=');
    std::cout << "Centered: '" << centered << "'\n";
}

// ==================== Example 6: Replace Operations ====================
void example_replace() {
    std::cout << "\n=== Replace Operations ===\n";
    
    fstring<50> text = "Hello World, Hello C++";
    
    // Replace character
    auto replaced_char = replace(text, 'o', '0');
    std::cout << "Char replaced: " << replaced_char << "\n";
    
    // Replace substring
    fstring<10> from = "Hello";
    fstring<10> to = "Hi";
    auto replaced_str = replace(text, from, to);
    std::cout << "String replaced: " << replaced_str << "\n";
}

// ==================== Example 7: Split and Join ====================
void example_split_join() {
    std::cout << "\n=== Split and Join ===\n";
    
    fstring<50> text = "apple,banana,cherry,date";
    
    auto parts = split(text, ',');
    std::cout << "Split result (" << parts.count << " parts):\n";
    for (std::size_t i = 0; i < parts.count; ++i) {
        std::cout << "  " << i << ": " << parts.parts[i] << "\n";
    }
    
    // Join back
    auto joined = join(parts.parts, ',');
    std::cout << "Joined: " << joined << "\n";
}

// ==================== Example 8: String Algorithms ====================
void example_algorithms() {
    std::cout << "\n=== String Algorithms ===\n";
    
    fstring<30> text = "Hello World";
    
    // Reverse
    auto reversed = reverse(text);
    std::cout << "Reversed: " << reversed << "\n";
    
    // Repeat
    fstring<10> small = "abc";
    auto repeated = repeat(small, 3);
    std::cout << "Repeated: " << repeated << "\n";
    
    // Remove character
    fstring<30> with_spaces = "H e l l o";
    auto no_spaces = remove(with_spaces, ' ');
    std::cout << "Spaces removed: " << no_spaces << "\n";
    
    // Character count
    fstring<30> count_text = "hello world";
    auto count_l = count(count_text, 'l');
    std::cout << "Count of 'l': " << count_l << "\n";
    
    // Type checks
    fstring<10> digits = "12345";
    fstring<10> alpha = "abcde";
    std::cout << "Is '12345' all digits? " << std::boolalpha << is_digit(digits) << "\n";
    std::cout << "Is 'abcde' all alpha? " << is_alpha(alpha) << "\n";
}

// ==================== Example 9: Formatting ====================
void example_formatting() {
    std::cout << "\n=== Formatting ===\n";
    
    // Format with padding
    auto formatted = format_int(42, 5, '0');
    std::cout << "Formatted int: " << formatted << "\n";
    
    // Hex
    auto hex = to_hex(255);
    std::cout << "Hex: " << hex << "\n";
    
    // Binary  
    auto bin = to_binary(42);
    std::cout << "Binary: " << bin << "\n";
}

// ==================== Example 10: Builder Pattern ====================
void example_builder() {
    std::cout << "\n=== Builder Pattern ===\n";
    
    fstring<10> part1 = "Hello";
    fstring<10> part2 = " ";
    fstring<10> part3 = "World";
    fstring<10> part4 = "!";
    
    auto result = builder{}(part1, part2, part3, part4);
    std::cout << "Built string: " << result << "\n";
}

// ==================== Example 11: Constexpr Usage ====================
constexpr auto example_constexpr() {
    // All operations can be done at compile time
    fstring<20> str1 = "Hello";
    fstring<20> str2 = "World";
    auto result = str1 + " " + str2;
    return result;
}

void demonstrate_constexpr() {
    std::cout << "\n=== Constexpr Usage ===\n";
    
    constexpr auto compile_time_str = example_constexpr();
    std::cout << "Compile-time string: " << compile_time_str << "\n";
    
    constexpr fstring<10> ct_str = "test";
    constexpr auto ct_len = ct_str.length();
    std::cout << "Compile-time length: " << ct_len << "\n";
}

// ==================== Example 12: Type Aliases ====================
void example_type_aliases() {
    std::cout << "\n=== Type Aliases ===\n";
    
    using namespace types;
    
    name_str name = "John Doe";
    msg_str message = "Hello, World!";
    path_str path = "/usr/local/bin/app";
    uuid_str uuid = "550e8400-e29b-41d4-a716-446655440000";
    ip_str ip = "192.168.1.1";
    datetime_str dt = "2025-11-24 10:30:00";
    
    std::cout << "Name: " << name << "\n";
    std::cout << "Message: " << message << "\n";
    std::cout << "Path: " << path << "\n";
    std::cout << "UUID: " << uuid << "\n";
    std::cout << "IP: " << ip << "\n";
    std::cout << "DateTime: " << dt << "\n";
}

// ==================== Example 13: Comparison ====================
void example_comparison() {
    std::cout << "\n=== Comparison ===\n";
    
    fstring<10> str1 = "apple";
    fstring<10> str2 = "banana";
    fstring<10> str3 = "apple";
    
    std::cout << "str1 == str3: " << std::boolalpha << (str1 == str3) << "\n";
    std::cout << "str1 < str2: " << (str1 < str2) << "\n";
    std::cout << "str1 > str2: " << (str1 > str2) << "\n";
    
    // Case-insensitive comparison
    fstring<10> upper = "APPLE";
    fstring<10> lower = "apple";
    std::cout << "Case-insensitive equal: " << equals_ignore_case(upper, lower) << "\n";
}

// ==================== Main Function ====================
int main() {
    std::cout << "====================================\n";
    std::cout << "  fstring Library Examples v" << version_info.string << "\n";
    std::cout << "====================================\n";
    
    try {
        example_basic_usage();
        example_string_operations();
        example_conversions();
        example_case_conversion();
        example_trim_pad();
        example_replace();
        example_split_join();
        example_algorithms();
        example_formatting();
        example_builder();
        demonstrate_constexpr();
        example_type_aliases();
        example_comparison();
        
        std::cout << "\n====================================\n";
        std::cout << "  All examples completed!\n";
        std::cout << "====================================\n";
        
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }
    
    return 0;
}