// Copyright 2006 Google Inc.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
// ---
//
//
// This file contains some Google extensions to the standard
// <algorithm> C++ header. Many of these algorithms were in the
// original STL before it was proposed for standardization.

#ifndef UTIL_GTL_ALGORITHM_H_
#define UTIL_GTL_ALGORITHM_H_

#include <stddef.h>
#include <algorithm>
#include <functional>
#include <iterator>
#include <utility>

namespace util {
namespace gtl {

// Returns true if [first, last) contains an element equal to value.
// Complexity: linear.
template<typename InputIterator, typename EqualityComparable>
bool contains(InputIterator first, InputIterator last,
              const EqualityComparable& value) {
  return std::find(first, last, value) != last;
}

// There is no contains_if().  Use any() instead.

template<typename InputIterator, typename ForwardIterator>
bool contains_some_of(InputIterator first1, InputIterator last1,
                      ForwardIterator first2, ForwardIterator last2) {
  return std::find_first_of(first1, last1, first2, last2) != last1;
}

template<typename InputIterator, typename ForwardIterator, typename Predicate>
bool contains_some_of(InputIterator first1, InputIterator last1,
                      ForwardIterator first2, ForwardIterator last2,
                      Predicate pred) {
  return std::find_first_of(first1, last1, first2, last2, pred) != last1;
}

template<typename InputIterator, typename EqualityComparable>
typename std::iterator_traits<InputIterator>::pointer
find_or_null(InputIterator first, InputIterator last,
             const EqualityComparable& value) {
  const InputIterator it = std::find(first, last, value);
  return it != last ? &*it : NULL;
}

template<typename InputIterator, typename Predicate>
typename std::iterator_traits<InputIterator>::pointer
find_if_or_null(InputIterator first, InputIterator last, Predicate pred) {
  const InputIterator it = std::find_if(first, last, pred);
  return it != last ? &*it : NULL;
}

// Reorders elements in [first, last), so that for each consecutive group
// of duplicate elements (according to eq predicate) the first one is left and
// others are moved at the end of the range. Returns: iterator middle such that
// [first, middle) contains no two consecutive elements that are duplicates and
// [middle, last) contains elements removed from all groups. It's stable for
// range [first, middle) meaning the order of elements are the same as order of
// their corresponding groups in input, but the order in range [middle, last)
// is not preserved. Function is similar to std::unique, but ensures that
// removed elements are properly copied and accessible at the range's end.
// Complexity: exactly last-first-1 applications of eq; at most middle-first-1
// swap operations.
template <typename ForwardIterator, typename Equals>
ForwardIterator unique_partition(ForwardIterator first, ForwardIterator last,
                                 Equals eq) {
  first = adjacent_find(first, last, eq);
  if (first == last)
    return last;

  // Points to right-most element within range of unique elements being built.
  ForwardIterator result = first;

  // 'first' iterator goes through the sequence starting from element after
  // first equal elements pair (found by adjacent_find above).
  ++first;
  while (++first != last) {
    // If we encounter an element that isn't equal to right-most element in
    // result, then extend the range and swap this element into it.
    // Otherwise just continue incrementing 'first'.
    if (!eq(*result, *first)) {
      using std::swap;
      swap(*++result, *first);
    }
  }
  // Return past-the-end upper-bound of the resulting range.
  return ++result;
}

// Reorders elements in [first, last) range moving duplicates for each
// consecutive group of elements to the end. Equality is checked using ==.
template <typename ForwardIterator>
inline ForwardIterator unique_partition(ForwardIterator first,
                                        ForwardIterator last) {
  typedef typename std::iterator_traits<ForwardIterator>::value_type T;
  return unique_partition(first, last, std::equal_to<T>());
}

// Samples k elements from the next n.
// Elements have the same order in the output stream as they did on input.
//
// This is Algorithm S from section 3.4.2 of Knuth, TAOCP, 2nd edition.
// My k corresponds to Knuth n-m.
// My n corrsponds to Knuth N-t.
//
// RngFunctor is any functor that can be called as:
//   size_t RngFunctor(size_t x)
// The return value is an integral value in the half-open range [0, x)
// such that all values are equally likely.  Behavior is unspecified if x==0.
// (This function never calls RngFunctor with x==0).

template <typename InputIterator, typename OutputIterator, typename RngFunctor>
inline void sample_k_of_n(InputIterator in, size_t k, size_t n,
                          RngFunctor& rng, OutputIterator out) {
  if (k > n) {
    k = n;
  }
  while (k > 0) {
    if (rng(n) < k) {
      *out++ = *in;
      k--;
    }
    ++in;
    --n;
  }
}

// Finds the longest prefix of a range that is a binary max heap with
// respect to a given StrictWeakOrdering.  If first == last, returns last.
// Otherwise, return an iterator it such that [first,it) is a heap but
// no longer prefix is -- in other words, first + i for the lowest i
// such that comp(first[(i-1)/2], first[i]) returns true.
template <typename RandomAccessIterator, typename StrictWeakOrdering>
RandomAccessIterator gtl_is_binary_heap_until(RandomAccessIterator first,
                                              RandomAccessIterator last,
                                              StrictWeakOrdering comp) {
  if (last - first < 2) return last;
  RandomAccessIterator parent = first;
  bool is_right_child = false;
  for (RandomAccessIterator child = first + 1; child != last; ++child) {
    if (comp(*parent, *child)) return child;
    if (is_right_child) ++parent;
    is_right_child = !is_right_child;
  }
  return last;
}

// Special case of gtl_is_binary_heap_until where the order is std::less,
// i.e., where we're working with a simple max heap.
template <typename RandomAccessIterator>
RandomAccessIterator gtl_is_binary_heap_until(RandomAccessIterator first,
                                              RandomAccessIterator last) {
  typedef typename std::iterator_traits<RandomAccessIterator>::value_type T;
  return gtl_is_binary_heap_until(first, last, std::less<T>());
}

// Checks whether a range of values is a binary heap, i.e., checks that
// no node is less (as defined by a StrictWeakOrdering) than a child.
template <typename RandomAccessIterator, typename StrictWeakOrdering>
bool gtl_is_binary_heap(RandomAccessIterator begin,
                        RandomAccessIterator end,
                        StrictWeakOrdering comp) {
  return gtl_is_binary_heap_until(begin, end, comp) == end;
}

// Special case of gtl_is_binary_heap where the order is std::less (i.e.,
// where we're working on a simple max heap).
template <typename RandomAccessIterator>
bool gtl_is_binary_heap(RandomAccessIterator begin,
                        RandomAccessIterator end) {
  return gtl_is_binary_heap_until(begin, end) == end;
}

// Unqualified calls to is_heap are ambiguous with some build types,
// namespace that can clash with names that C++11 added to ::std.
// By calling util::gtl::is_heap, clients can avoid those errors,
// and by using the underlying is_heap call we ensure consistency
// with the standard library's heap implementation just in case a
// standard library ever uses anything other than a binary heap.
#if defined(__GXX_EXPERIMENTAL_CXX0X__) || __cplusplus > 199711L \
  || defined(LIBCXX) || _MSC_VER >= 1600 /* Visual Studio 2010 */
using std::is_heap;
#elif defined(_MSC_VER)  || defined(__ANDROID__) || defined(__LSB_VERSION__) \
  || defined(__QNX__) || defined(__APPLE__)
// For the platforms listed above, we know by inspection that make_heap()
// traffics in binary max heaps, so gtl_is_binary_heap is an acceptable
// implementation for is_heap.
template <typename RandomAccessIterator>
bool is_heap(RandomAccessIterator begin, RandomAccessIterator end) {
  return gtl_is_binary_heap(begin, end);
}

template <typename RandomAccessIterator, typename StrictWeakOrdering>
bool is_heap(RandomAccessIterator begin,
             RandomAccessIterator end,
             StrictWeakOrdering comp) {
  return gtl_is_binary_heap(begin, end, comp);
}
#elif defined __GNUC__
/* using __gnu_cxx::is_heap; */
#else
// We need an implementation of is_heap that matches the library's
// implementation of make_heap() and friends.  gtl_is_binary_heap will
// *probably* work, but let's be safe and not make that assumption.
#error No implementation of is_heap defined for this toolchain.
#endif

}  // namespace gtl
}  // namespace util

#endif  // UTIL_GTL_ALGORITHM_H_
