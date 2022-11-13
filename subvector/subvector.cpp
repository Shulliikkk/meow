struct subvector {
     int *mas;
     unsigned int top;
     unsigned int capacity;
};

bool init(subvector* sv) {
  sv -> mas = NULL;
  sv -> top = 0;
  sv -> capacity = 0;
  return true;
}

bool resize(subvector* sv, unsigned int new_capacity) {
  sv -> capacity = new_capacity;
  return true;
}

bool push_back(subvector* sv, int d) {
  sv -> top += 1;
  if (sv -> top - 1 == 0) {
    if (sv -> capacity == 0) {
      sv -> capacity = 1;
      int* new_mas = new int[sv -> capacity];
      delete[] sv -> mas;
      sv -> mas = new_mas;
    }
    (sv -> mas)[sv -> top - 1] = d;
  }
  else if (sv -> top > sv -> capacity) {
    unsigned int cur_cap = sv -> capacity;
    resize(sv, cur_cap * 2);
    int* new_mas = new int[sv -> capacity];
    for (unsigned int i = 0; i < (sv -> top - 1); i++) {
      new_mas[i] = (sv -> mas)[i];
    }
    delete[] sv -> mas;
    sv -> mas = new_mas;
    (sv -> mas)[sv -> top - 1] = d;
  }
  else {
    (sv -> mas)[sv -> top - 1] = d;
  }
  return true;
}


int pop_back(subvector* sv) {
  if (sv -> top > 0) {
    sv -> top -= 1;
    return sv -> mas[sv -> top];
  }
  else {
    return -1;
  }
}

void shrink_to_fit(subvector* sv) {
  int* new_mas = new int[sv -> top];
  for (unsigned int i = 0; i < sv -> top; i++) {
    new_mas[i] = (sv -> mas)[i];
  }
  delete[] sv -> mas;
  sv -> mas = new_mas;
  sv -> capacity = sv -> top;
}

void clear(subvector* sv) {
  sv -> top = 0;
}

void destructor(subvector* sv) {
  delete[] sv -> mas;
  init(sv);
}
