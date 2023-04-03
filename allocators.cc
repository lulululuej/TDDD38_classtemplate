#include <utility>
#include <cassert>
#include <string>
#include <iostream>

template <typename T>
struct New_Allocator
{
  template <typename... Args>
  static T* create(Args&&... args)
  {
    //std::cout << "create data : " << new T{std::forward<Args>(args)...} << std::endl;
    return new T{std::forward<Args>(args)...};
  }
  
  static void destroy(T* data)
  {
    //std::cout << "del data : " << data << std::endl;
    delete data;
  }
};

template <typename T>
struct Tracker_Allocator
{
  template <typename... Args>
  static T* create(Args&&... args)
  {
    T* result {New_Allocator<T>::create(std::forward<Args>(args)...)};
    std::cout << "Object allocated at: " << result << std::endl;
    return result;
  }
  
  static void destroy(T* data)
  {
    std::cout << "Object deallocated: " << data << std::endl;
    New_Allocator<T>::destroy(data);
  }
  
};

template <typename T, template <typename> typename Allocator = New_Allocator>
class Stack
{
  // linked list
  /* value and the addr of next data */
  struct Node;
  
public:

  Stack()
    : head{nullptr} { }

  Stack(Stack const&) = delete;
  Stack(Stack&&) = delete;

  Stack& operator=(Stack const&) = delete;
  Stack& operator=(Stack&&) = delete;
  
  ~Stack()
  {
    while (!empty())
    {
      pop();
    }
  }
  
  void push(T const& value)
  {
    //std::cout << "push value = " << value << std::endl;
    //std::cout << "push head  = " << head << std::endl;
    /* head is the address of the value */
    head = Allocator<Node>::create(value, head);
  }

  T pop()
  {
    T result{top()};
    Node* old {head};
    head = head->next;
    Allocator<Node>::destroy(old);
    return result;
  }

  T& top()
  {
    // -> use in access pointer value
    return head->value;
  }

  T const& top() const
  {
    return head->value;
  }

  bool empty() const
  {
    return head == nullptr;
  }
  
private:
  
  struct Node
  {
    T value;
    Node* next;
  };

  Node* head;
  
};

int main()
{
  {
    Stack<std::string> st {};
    assert(st.empty());
  
    st.push("1");
    assert(!st.empty());
    assert(st.top() == "1");
  
    st.push("2");
    assert(st.top() == "2");
    assert(st.pop() == "2");
  
    assert(st.pop() == "1");

    assert(st.empty());

    st.push("3");
    assert(st.pop() == "3");
    
    st.push("4");
    
    st.push("5");
    assert(st.pop() == "5");
  }
  {
    Stack<std::string, Tracker_Allocator> st {};
    assert(st.empty());
  
    st.push("1");
    assert(!st.empty());
    assert(st.top() == "1");
  
    st.push("2");
    assert(st.top() == "2");
    assert(st.pop() == "2");
  
    assert(st.pop() == "1");

    assert(st.empty());
    
    st.push("3");
    assert(st.pop() == "3");
    
    st.push("4");
    
    st.push("5");
    assert(st.pop() == "5");
  }
}