/**
 * @file example_advanced.cpp
 * @brief Contoh penggunaan lanjutan dari zuu::string library v2.0
 * 
 * Compile: g++ -std=c++20 -O3 -o example_advanced example_advanced.cpp
 * Dengan optimasi: g++ -std=c++20 -O3 -march=native -o example_advanced example_advanced.cpp
 */

#include "constant_string.hpp"
#include <iostream>
#include <cassert>

using namespace zuu::literals;

// ============================================================================
// 1. Compile-Time String Processing
// ============================================================================

void demo_compile_time() {
    std::cout << "=== Compile-Time Processing ===" << std::endl;
    
    // Semua operasi ini dievaluasi pada compile-time!
    constexpr auto hello = "Hello"_cs;
    constexpr auto world = "World"_cs;
    constexpr auto greeting = hello + " "_cs + world + "!"_cs;
    
    static_assert(greeting.length() == 12);
    static_assert(greeting.starts_with("Hello"));
    static_assert(greeting.ends_with("!"));
    static_assert(greeting.contains("World"));
    
    std::cout << "Greeting: " << greeting.c_str() << std::endl;
    std::cout << "Length: " << greeting.length() << " (computed at compile-time)" << std::endl;
    
    // String transformations at compile-time
    constexpr auto upper = zuu::to_upper(greeting);
    constexpr auto reversed = zuu::reversed(greeting);
    
    static_assert(upper == "HELLO WORLD!"_cs);
    std::cout << "Uppercase: " << upper.c_str() << std::endl;
    std::cout << "Reversed: " << reversed.c_str() << std::endl;
}

// ============================================================================
// 2. Integer Conversions
// ============================================================================

void demo_integer_conversions() {
    std::cout << "\n=== Integer Conversions ===" << std::endl;
    
    // Signed integers
    constexpr auto int_str = zuu::to_string(-12345);
    constexpr auto long_str = zuu::to_string(9876543210LL);
    
    static_assert(int_str == "-12345"_cs);
    std::cout << "Integer: " << int_str.c_str() << std::endl;
    std::cout << "Long: " << long_str.c_str() << std::endl;
    
    // Unsigned integers
    constexpr auto uint_str = zuu::to_string(42u);
    constexpr auto size_str = zuu::to_string(std::size_t(1024));
    
    std::cout << "Unsigned: " << uint_str.c_str() << std::endl;
    std::cout << "Size: " << size_str.c_str() << std::endl;
    
    // Different bases
    constexpr auto hex = zuu::to_string_base(255u, 16);
    constexpr auto bin = zuu::to_string_base(42u, 2);
    constexpr auto oct = zuu::to_string_base(64u, 8);
    
    std::cout << "Hex (255): " << hex.c_str() << std::endl;
    std::cout << "Binary (42): " << bin.c_str() << std::endl;
    std::cout << "Octal (64): " << oct.c_str() << std::endl;
    
    // String to integer
    int parsed;
    if (zuu::from_string(int_str, parsed)) {
        std::cout << "Parsed back: " << parsed << std::endl;
        assert(parsed == -12345);
    }
    
    // Parsing with different bases
    unsigned hex_val;
    if (zuu::from_string(zuu::cstring<10>("ff"), hex_val, 16)) {
        std::cout << "Hex 'ff' parsed: " << hex_val << std::endl;
        assert(hex_val == 255);
    }
}

// ============================================================================
// 3. Floating Point Conversions
// ============================================================================

void demo_float_conversions() {
    std::cout << "\n=== Floating Point Conversions ===" << std::endl;
    
    auto pi = zuu::to_string(3.14159265359, 2);
    auto e = zuu::to_string(2.71828, 4);
    auto small = zuu::to_string(0.000123, 6);
    
    std::cout << "Pi (2 decimals): " << pi.c_str() << std::endl;
    std::cout << "e (4 decimals): " << e.c_str() << std::endl;
    std::cout << "Small (6 decimals): " << small.c_str() << std::endl;
    
    // Special values
    auto nan_str = zuu::to_string(std::numeric_limits<double>::quiet_NaN());
    auto inf_str = zuu::to_string(std::numeric_limits<double>::infinity());
    
    std::cout << "NaN: " << nan_str.c_str() << std::endl;
    std::cout << "Infinity: " << inf_str.c_str() << std::endl;
}

// ============================================================================
// 4. String Search and Manipulation
// ============================================================================

void demo_search_operations() {
    std::cout << "\n=== Search Operations ===" << std::endl;
    
    constexpr zuu::cstring<100> text("The quick brown fox jumps over the lazy dog");
    
    // Find operations
    constexpr auto pos1 = text.find("quick");
    constexpr auto pos2 = text.find("fox");
    constexpr auto pos3 = text.find("cat"); // Not found
    
    std::cout << "Text: " << text.c_str() << std::endl;
    std::cout << "Position of 'quick': " << pos1 << std::endl;
    std::cout << "Position of 'fox': " << pos2 << std::endl;
    std::cout << "Position of 'cat': " << (pos3 == text.npos ? "Not found" : std::to_string(pos3)) << std::endl;
    
    // Character search
    constexpr auto first_o = text.find('o');
    constexpr auto last_o = text.rfind('o');
    constexpr auto count_o = text.count('o');
    
    std::cout << "First 'o' at: " << first_o << std::endl;
    std::cout << "Last 'o' at: " << last_o << std::endl;
    std::cout << "Count of 'o': " << count_o << std::endl;
    
    // Prefix/Suffix checking
    static_assert(text.starts_with("The"));
    static_assert(text.ends_with("dog"));
    static_assert(text.contains("brown fox"));
    
    std::cout << "Starts with 'The': " << std::boolalpha << text.starts_with("The") << std::endl;
    std::cout << "Ends with 'dog': " << text.ends_with("dog") << std::endl;
    std::cout << "Contains 'brown fox': " << text.contains("brown fox") << std::endl;
}

// ============================================================================
// 5. String Building and Manipulation
// ============================================================================

void demo_string_building() {
    std::cout << "\n=== String Building ===" << std::endl;
    
    // Progressive building
    zuu::cstring<200> builder;
    builder += "Start";
    builder += " -> ";
    builder += "Middle";
    builder += " -> ";
    builder += "End";
    
    std::cout << "Built: " << builder.c_str() << std::endl;
    
    // Insert
    zuu::cstring<50> text("Hello World");
    text.insert(5, " Beautiful", 10);
    std::cout << "After insert: " << text.c_str() << std::endl;
    
    // Erase
    zuu::cstring<50> text2("Hello Beautiful World");
    text2.erase(6, 10); // Remove "Beautiful "
    std::cout << "After erase: " << text2.c_str() << std::endl;
    
    // Replace
    zuu::cstring<50> text3("Hello World");
    text3.replace(6, 5, "Universe", 8);
    std::cout << "After replace: " << text3.c_str() << std::endl;
}

// ============================================================================
// 6. Case and Whitespace Operations
// ============================================================================

void demo_transformations() {
    std::cout << "\n=== String Transformations ===" << std::endl;
    
    zuu::cstring<50> mixed("   HeLLo WoRLd   \n");
    std::cout << "Original: [" << mixed.c_str() << "]" << std::endl;
    
    auto upper = zuu::to_upper(mixed);
    auto lower = zuu::to_lower(mixed);
    auto trimmed = zuu::trimmed(mixed);
    
    std::cout << "Uppercase: [" << upper.c_str() << "]" << std::endl;
    std::cout << "Lowercase: [" << lower.c_str() << "]" << std::endl;
    std::cout << "Trimmed: [" << trimmed.c_str() << "]" << std::endl;
    
    // In-place transformations
    zuu::cstring<50> text("  test string  ");
    text.trim().to_upper();
    std::cout << "Trim + Upper: [" << text.c_str() << "]" << std::endl;
}

// ============================================================================
// 7. STL Interoperability
// ============================================================================

void demo_stl_interop() {
    std::cout << "\n=== STL Interoperability ===" << std::endl;
    
    // To std::string
    zuu::cstring<50> cs("Hello from zuu::string");
    std::string std_str = zuu::to_std_string(cs);
    std::cout << "std::string: " << std_str << std::endl;
    
    // To std::string_view
    auto view = zuu::to_string_view(cs);
    std::cout << "string_view: " << view << std::endl;
    
    // From std::string
    std::string source("Hello from std::string");
    auto back = zuu::from_std_string<50>(source);
    std::cout << "Back to zuu: " << back.c_str() << std::endl;
    
    // STL algorithms
    zuu::cstring<20> text("hello");
    std::transform(text.begin(), text.end(), text.begin(), 
                   [](char c) { return std::toupper(c); });
    std::cout << "After std::transform: " << text.c_str() << std::endl;
    
    // Range-based for loop
    std::cout << "Characters: ";
    for (char c : text) {
        std::cout << c << " ";
    }
    std::cout << std::endl;
}

// ============================================================================
// 8. Hash and Comparison
// ============================================================================

void demo_hash_comparison() {
    std::cout << "\n=== Hash and Comparison ===" << std::endl;
    
    constexpr auto str1 = "apple"_cs;
    constexpr auto str2 = "banana"_cs;
    constexpr auto str3 = "apple"_cs;
    
    // Hashing
    constexpr auto hash1 = str1.hash();
    constexpr auto hash2 = str2.hash();
    constexpr auto hash3 = str3.hash();
    
    std::cout << "Hash('apple'): " << hash1 << std::endl;
    std::cout << "Hash('banana'): " << hash2 << std::endl;
    std::cout << "Hash('apple'): " << hash3 << " (same)" << std::endl;
    
    static_assert(hash1 == hash3);
    static_assert(hash1 != hash2);
    
    // Comparison
    static_assert(str1 == str3);
    static_assert(str1 != str2);
    static_assert(str1 < str2);
    static_assert(str2 > str1);
    
    std::cout << "\nComparison:" << std::endl;
    std::cout << "'apple' == 'apple': " << std::boolalpha << (str1 == str3) << std::endl;
    std::cout << "'apple' < 'banana': " << (str1 < str2) << std::endl;
    std::cout << "compare('apple', 'banana'): " << str1.compare(str2) << std::endl;
}

// ============================================================================
// 9. Performance Benchmarks
// ============================================================================

void demo_performance() {
    std::cout << "\n=== Performance Characteristics ===" << std::endl;
    
    // Compile-time evaluation
    constexpr auto compile_time = []() {
        auto s = "test"_cs;
        for (int i = 0; i < 10; ++i) {
            s += " "_cs;
            s += zuu::to_string(i);
        }
        return s;
    }();
    
    std::cout << "Compile-time built: " << compile_time.c_str() << std::endl;
    std::cout << "Length: " << compile_time.length() << " (known at compile-time)" << std::endl;
    
    // Stack allocation (no heap)
    std::cout << "\nMemory characteristics:" << std::endl;
    std::cout << "sizeof(zuu::cstring<10>): " << sizeof(zuu::cstring<10>) << " bytes" << std::endl;
    std::cout << "sizeof(zuu::cstring<100>): " << sizeof(zuu::cstring<100>) << " bytes" << std::endl;
    std::cout << "sizeof(std::string): " << sizeof(std::string) << " bytes (+ heap)" << std::endl;
}

// ============================================================================
// 10. Advanced Use Cases
// ============================================================================

// Compile-time URL parsing
constexpr auto parse_protocol(const char* url) {
    zuu::cstring<20> result;
    for (std::size_t i = 0; url[i] != '\0'; ++i) {
        if (url[i] == ':') break;
        result.push_back(url[i]);
    }
    return result;
}

void demo_advanced() {
    std::cout << "\n=== Advanced Use Cases ===" << std::endl;
    
    // Compile-time parsing
    constexpr auto protocol = parse_protocol("https://example.com");
    static_assert(protocol == "https"_cs);
    std::cout << "Protocol: " << protocol.c_str() << std::endl;
    
    // Template string with placeholders
    zuu::cstring<100> template_str("Hello {}, you have {} messages");
    // (Simplified - for real template use format function)
    
    // Path manipulation
    zuu::cstring<256> path("/home/user/documents/file.txt");
    auto last_slash = path.rfind('/');
    if (last_slash != path.npos) {
        auto filename = path.substr(last_slash + 1);
        std::cout << "Filename: " << filename.c_str() << std::endl;
    }
    
    // Simple tokenization
    zuu::cstring<100> csv("apple,banana,cherry");
    std::size_t start = 0;
    std::cout << "CSV tokens: ";
    while (start < csv.length()) {
        auto comma = csv.find(',', start);
        if (comma == csv.npos) {
            std::cout << csv.substr(start).c_str();
            break;
        }
        std::cout << csv.substr(start, comma - start).c_str() << " | ";
        start = comma + 1;
    }
    std::cout << std::endl;
}

// ============================================================================
// Main
// ============================================================================

int main() {
    std::cout << "====================================" << std::endl;
    std::cout << "zuu::string Library v" << zuu::version::string() << std::endl;
    std::cout << "Advanced Examples and Benchmarks" << std::endl;
    std::cout << "====================================\n" << std::endl;
    
    try {
        demo_compile_time();
        demo_integer_conversions();
        demo_float_conversions();
        demo_search_operations();
        demo_string_building();
        demo_transformations();
        demo_stl_interop();
        demo_hash_comparison();
        demo_performance();
        demo_advanced();
        
        std::cout << "\n====================================" << std::endl;
        std::cout << "All tests passed successfully!" << std::endl;
        std::cout << "====================================" << std::endl;
        
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}
