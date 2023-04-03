#include <type_traits>

template <typename T>
struct remove_reference
{
  using type = T;
};

// implement your specializations here
template <typename T>
struct remove_reference<T&&>
{
  using type = T;
};

template <typename T>
struct remove_reference<T&>
{
  using type = T;
};


// implement remove_pointers here
template <typename T>
struct remove_pointers
{
  using type = T;
};

template <typename T>
struct remove_pointers<T*>
{
  using type = typename remove_pointers<T>::type;
};

template <typename T>
struct remove_pointers<T* const>
{
  using type = typename remove_pointers<T>::type;
};

template <typename T>
using remove_reference_t = typename remove_reference<T>::type;

template <typename T>
using remove_pointers_t = typename remove_pointers<T>::type;


int main()
{
  static_assert(std::is_same<
                remove_reference_t<int&&>, int
                >::value, "remove_reference failed for 'int&&'");
  static_assert(std::is_same<
                remove_reference_t<int&>, int
                >::value, "remove_reference failed for 'int&'");
  static_assert(std::is_same<
                remove_reference_t<int>, int
                >::value, "remove_reference failed for 'int'");
  static_assert(std::is_same<
                remove_reference_t<int const&>, int const
                >::value, "remove_reference failed for 'int const&'");
  
  static_assert(std::is_same<
                remove_pointers_t<int>, int
                >::value, "remove_pointers failed for 'int'");
  static_assert(std::is_same<
                remove_pointers_t<int*>, int
                >::value, "remove_pointers failed for 'int*'");
  static_assert(std::is_same<
                remove_pointers_t<int* const>, int
                >::value, "remove_pointers failed for 'int* const'");
  static_assert(std::is_same<
                remove_pointers_t<int* const*>, int
                >::value, "remove_pointers failed for 'int* const*'");
  static_assert(std::is_same<
                remove_pointers_t<int***>, int
                >::value, "remove_pointers failed for 'int***'");
}
