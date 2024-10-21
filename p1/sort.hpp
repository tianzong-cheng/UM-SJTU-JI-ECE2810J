#ifndef VE281P1_SORT_HPP
#define VE281P1_SORT_HPP

#include <functional>
#include <stdlib.h>
#include <vector>

template <typename T, typename Compare>
void bubble_sort(std::vector<T> &vector, Compare comp = std::less<T>()) {
  for (unsigned long i = 0; i < vector.size(); i++) {
    for (unsigned long j = 0; j < vector.size() - i - 1; j++) {
      if (comp(vector[j + 1], vector[j])) {
        std::swap(vector[j], vector[j + 1]);
      }
    }
  }
}

template <typename T, typename Compare>
void insertion_sort(std::vector<T> &vector, Compare comp = std::less<T>()) {
  for (unsigned long i = 1; i < vector.size(); i++) {
    T key = vector[i];
    unsigned long j = i - 1;
    while (comp(key, vector[j])) {
      vector[j + 1] = vector[j];
      if (j == 0) {
        j--;
        break;
      }
      j--;
    }
    vector[j + 1] = key;
  }
}

template <typename T, typename Compare>
void selection_sort(std::vector<T> &vector, Compare comp = std::less<T>()) {
  if (vector.size() <= 1) {
    return;
  }

  unsigned long left = 0;
  unsigned long right = vector.size() - 1;

  while (left < right) {
    unsigned long min = left;
    unsigned long max = left;
    for (unsigned long i = left + 1; i <= right; i++) {
      if (comp(vector[i], vector[min])) {
        min = i;
      }
      if (comp(vector[max], vector[i])) {
        max = i;
      }
    }
    std::swap(vector[left], vector[min]);
    if (max == left) {
      max = min;
    }
    std::swap(vector[right], vector[max]);
    left++;
    right--;
  }
}

template <typename T, typename Compare>
void merge_sort(std::vector<T> &vector, Compare comp = std::less<T>()) {
  if (vector.size() <= 1) {
    return;
  }
  std::vector<T> left(vector.begin(), vector.begin() + vector.size() / 2);
  std::vector<T> right(vector.begin() + vector.size() / 2, vector.end());
  merge_sort(left, comp);
  merge_sort(right, comp);
  unsigned long i = 0, j = 0, k = 0;
  while (i < left.size() && j < right.size()) {
    if (comp(right[j], left[i])) {
      vector[k] = right[j];
      j++;
    } else {
      vector[k] = left[i];
      i++;
    }
    k++;
  }
  while (i < left.size()) {
    vector[k] = left[i];
    i++;
    k++;
  }
  while (j < right.size()) {
    vector[k] = right[j];
    j++;
    k++;
  }
}

template <typename T, typename Compare>
void quick_sort_extra(std::vector<T> &vector, Compare comp = std::less<T>()) {
  // quick sort using extra array for partitioning
  if (vector.size() <= 1) {
    return;
  }
  std::vector<T> left, right;
  T pivot = vector[0];
  for (unsigned long i = 1; i < vector.size(); i++) {
    if (comp(vector[i], pivot)) {
      left.push_back(vector[i]);
    } else {
      right.push_back(vector[i]);
    }
  }
  left.shrink_to_fit();
  right.shrink_to_fit();

  vector.clear();
  vector.shrink_to_fit();

  quick_sort_extra(left, comp);
  quick_sort_extra(right, comp);

  vector.insert(vector.end(), left.begin(), left.end());
  vector.push_back(pivot);
  vector.insert(vector.end(), right.begin(), right.end());
  vector.shrink_to_fit();
}

template <typename T, typename Compare>
void quick_sort_inplace_helper(std::vector<T> &vector, unsigned long left,
                               unsigned long right, Compare comp) {
  if (left >= right) {
    return;
  }
  unsigned long pivot = left;
  unsigned long i = left + 1;
  unsigned long j = right;
  while (i <= j) {
    if (comp(vector[i], vector[pivot])) {
      i++;
    } else if (comp(vector[pivot], vector[j])) {
      j--;
    } else {
      std::swap(vector[i], vector[j]);
      i++;
      j--;
    }
  }
  std::swap(vector[pivot], vector[j]);
  if (j > 0) {
    quick_sort_inplace_helper(vector, left, j - 1, comp);
  }
  quick_sort_inplace_helper(vector, j + 1, right, comp);
}

template <typename T, typename Compare>
void quick_sort_inplace(std::vector<T> &vector, Compare comp = std::less<T>()) {
  // quick sort with in-place partitioning
  if (vector.size() <= 1) {
    return;
  }
  quick_sort_inplace_helper(vector, 0, vector.size() - 1, comp);
}

#endif // VE281P1_SORT_HPP
