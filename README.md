# **Custom Vector Implementation in C++**  
**University Assignment | STL-compliant Vector Container**  

## **Project Overview**  
A complete implementation of a dynamic array container (`Vector`) in C++, modeled after `std::vector`. The project demonstrates core C++ concepts including template programming, memory management, iterator patterns, and exception safety.

## **Key Features**  
✅ STL-compliant interface with iterators  
✅ Template-based implementation for any data type  
✅ Dynamic memory management with geometric resizing  
✅ Full iterator support (const and non-const)  
✅ Exception-safe operations  
✅ Copy semantics support  

## **Technical Implementation**  

### **Core Components**  
```cpp
template <typename T>
class Vector {
    // Memory management
    void realloc(size_type new_capacity);
    
    // Element access
    reference operator[](size_type index);
    const_reference operator[](size_type index) const;
    
    // Modifiers
    void push_back(const_reference value);
    void pop_back();
    iterator insert(const_iterator pos, const_reference val);
    iterator erase(const_iterator pos);
    
    // Iterators
    iterator begin();
    iterator end();
    const_iterator begin() const;
    const_iterator end() const;
    
    // Capacity
    size_type size() const;
    size_type capacity() const;
    void reserve(size_type new_cap);
    void shrink_to_fit();
};
```

### **Iterator Implementation**  
- Full compliance with STL iterator requirements  
- Bound checking and validity verification  

## **Key Technical Aspects**  
1. **Memory Management**  
   - Geometric resizing (capacity ×2 when full)  
   - Custom `realloc()` implementation  
   - Proper handling of object copying during reallocation  

2. **STL Compliance**  
   - Standard typedefs (`value_type`, `size_type`, etc.)  
   - Proper iterator categories  
   - Exception safety guarantees   

## **Usage Example**  
```cpp
Vector<int> v{1, 2, 3};
v.push_back(4);
v.insert(v.begin(), 0);

for (auto& x : v) {
    std::cout << x << " ";  // 0 1 2 3 4
}
```

## **Academic Purpose**  
This implementation was developed as a university exercise to demonstrate:  
- Deep understanding of container internals  
- Proper memory management in C++  
- Template metaprogramming techniques  
- Iterator design patterns  


