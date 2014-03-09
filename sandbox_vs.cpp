#include <cereal/access.hpp>
#include <cereal/details/traits.hpp>
#include <cereal/details/helpers.hpp>
#include <cereal/types/base_class.hpp>
#include <cereal/cereal.hpp>

#include <cereal/types/array.hpp>
#include <cereal/types/bitset.hpp>
#include <cereal/types/boost_variant.hpp>
#include <cereal/types/chrono.hpp>
#include <cereal/types/common.hpp>
#include <cereal/types/complex.hpp>
#include <cereal/types/deque.hpp>
#include <cereal/types/forward_list.hpp>
#include <cereal/types/list.hpp>
#include <cereal/types/map.hpp>
#include <cereal/types/memory.hpp>

#include <cereal/details/util.hpp>

#include <cereal/details/polymorphic_impl.hpp>
#include <cereal/types/polymorphic.hpp>

#include <cereal/types/queue.hpp>
#include <cereal/types/set.hpp>
#include <cereal/types/stack.hpp>
#include <cereal/types/string.hpp>
#include <cereal/types/tuple.hpp>
#include <cereal/types/unordered_map.hpp>
#include <cereal/types/unordered_set.hpp>
#include <cereal/types/utility.hpp>
#include <cereal/types/vector.hpp>

#include <cereal/archives/binary.hpp>
#include <cereal/archives/portable_binary.hpp>
#include <cereal/archives/xml.hpp>
#include <cereal/archives/json.hpp>

#include <iostream>
#include <type_traits>
#include <functional>

struct Archive {};

struct Test
{
  template <class Archive>
  void serialzize( Archive & )
  {
    std::cout << "hey there" << std::endl;
  }

  template <class Archive>
  void save( Archive & ) const
  {
    std::cout << "saved by the bell" << std::endl;
  }

  template <class Archive>
  void load( Archive & )
  {
    std::cout << "locked and loaded" << std::endl;
  }

  template <class Archive>
  static Test * load_and_construct( Archive & )
  {
    return new Test();
  }
};

template <class Archive>
void serialize( Archive &, Test & )
{ }

template <class Archive>
void load( Archive &, Test & )
{ }

template <class Archive>
void save( Archive &, Test const & )
{ }

namespace cereal
{
  template <>
  struct LoadAndConstruct<Test>
  {
    template <class Archive>
    static void load_and_construct( Archive &, cereal::construct<Test> & construct )
    {
      construct();
    }
  };
}

struct A
{
  virtual void foo() = 0;
};

struct B : A
{
  void foo() {}

  template <class Archive>
  void serialize( Archive & )
  {
    std::cout << "i'm in your b" << std::endl;
  }
};

struct C
{
  char a;
};

//CEREAL_REGISTER_TYPE(B);

template <class T, class A>
static auto test(int) -> decltype( cereal::access::member_serialize( std::declval<A&>(), std::declval<T&>() ), std::true_type())
{ return {}; }

template <class T, class A>
static auto test(...) -> std::false_type
{ return {}; }

int main()
{
  typedef Test T;
  std::cout << std::boolalpha;

  // Test Load and Construct internal/external
  std::cout << "\tload_and_construct" << std::endl;
  std::cout << cereal::traits::has_member_load_and_construct<T, Archive>::value << std::endl;
  std::cout << cereal::traits::has_non_member_load_and_construct<T, Archive>::value << std::endl;

  // serialize
  std::cout << "\tserialize" << std::endl;
  std::cout << cereal::traits::has_member_serialize<T, Archive>::value << std::endl;
  std::cout << cereal::traits::has_non_member_serialize<T, Archive>::value << std::endl;
  std::cout << test<T, Archive>(0) << std::endl;

  // load
  std::cout << "\tload" << std::endl;
  std::cout << cereal::traits::has_member_load<T, Archive>::value << std::endl;
  std::cout << cereal::traits::has_non_member_load<T, Archive>::value << std::endl;

  // save
  std::cout << "\tsave" << std::endl;
  std::cout << cereal::traits::has_member_save<T, Archive>::value << std::endl;
  std::cout << cereal::traits::has_non_member_save<T, Archive>::value << std::endl;

  // splittable
  std::cout << "\t splittable" << std::endl;
  std::cout << cereal::traits::has_member_split<T, Archive, Archive>::value << std::endl;
  std::cout << cereal::traits::has_non_member_split<T, Archive, Archive>::value << std::endl;

  // serialiable
  std::cout << "\toutput serializable" << std::endl;
  std::cout << cereal::traits::is_output_serializable<T, Archive>::value << std::endl;
  std::cout << cereal::traits::is_input_serializable<T, Archive>::value << std::endl;

  // specialized
  std::cout << "\tspecialized" << std::endl;
  std::cout << cereal::traits::detail::is_specialized_member_serialize<T, Archive>::value << std::endl;
  std::cout << cereal::traits::detail::is_specialized_member_load_save<T, Archive>::value << std::endl;
  std::cout << cereal::traits::detail::is_specialized_non_member_serialize<T, Archive>::value << std::endl;
  std::cout << cereal::traits::detail::is_specialized_non_member_load_save<T, Archive>::value << std::endl;
  std::cout << cereal::traits::detail::is_specialized_error<T, Archive>::value << std::endl;
  std::cout << cereal::traits::is_specialized<T, Archive>::value << std::endl;

  // array size
  std::cout << typeid(A).name() << std::endl;
  std::cout << typeid(cereal::traits::has_load_and_construct<int, bool>).name() << std::endl;

  // extra testing

  return 0;
}
