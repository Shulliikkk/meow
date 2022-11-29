struct subforwardlist {
     int data;
     subforwardlist* next;
};

bool init(subforwardlist** sfl) {
  *sfl = NULL;
  return true;
}

bool push_back(subforwardlist** sfl, int d) {
  if (*sfl == NULL) {
    subforwardlist* temp = new subforwardlist;
    temp -> data = d;
    temp -> next = NULL;
    *sfl = temp;
  }
  else {
    subforwardlist* temp = *sfl;
    while (temp -> next) {
      temp = temp -> next;
    }
    subforwardlist* temp_2 = new subforwardlist;
    temp_2 -> data = d;
    temp_2 -> next = NULL;
    temp -> next = temp_2;
  }
  return true;
}

int pop_back(subforwardlist** sfl) {
  if (*sfl == NULL) {
    return 0;
  }
  else {
    if ((*sfl) -> next == NULL) {
      int res = (*sfl) -> data;
      delete *sfl;
      (*sfl) = NULL;
      return res;
    }
    else {
      subforwardlist* temp = *sfl;
      while (temp -> next -> next) {
        temp = temp -> next;
      }
      int res = temp -> next -> data;
      delete temp -> next;
      temp -> next = NULL;
      return res;
    }
  }
}

bool push_forward(subforwardlist** sfl, int d) {
    subforwardlist* temp = new subforwardlist;
    temp -> data = d;
    temp -> next = *sfl;
    *sfl = temp;
    return true;
}

int pop_forward(subforwardlist** sfl) {
  if (*sfl == NULL) {
    return 0;
  }
  else {
    subforwardlist* temp = (*sfl) -> next;
    int res = (*sfl) -> data;
    delete *sfl;
    *sfl = temp;
    return res;
  }
}

bool push_where(subforwardlist** sfl, unsigned int where, int d) {
  subforwardlist* temp = *sfl;
  for (unsigned int i = 0; i < where; i++) {
    temp = temp -> next;
  }
  subforwardlist* temp_2 = new subforwardlist;
  temp_2 -> next = temp -> next;
  temp_2 -> data = d;
  temp -> next = temp_2;
  return true;
}

int erase_where(subforwardlist **sfl, unsigned int where) {
  int d = 0;
  subforwardlist* temp = *sfl;
  for (unsigned int i = 0; i < where; i++) {
    temp = temp -> next;
  }
  subforwardlist* temp_2 = temp -> next -> next;
  d = temp -> next -> data;
  delete temp -> next;
  temp -> next = temp_2;
  return d;
}

unsigned int size(subforwardlist  **sfl) {
  unsigned int res = 1;
  if (*sfl == NULL)
    return 0;
  subforwardlist* temp = *sfl;
  while (temp -> next != NULL) {
    temp = temp -> next;
    res++;
  }
  return res;
}

void clear(subforwardlist  **sfl) {
  while (*sfl != NULL)
    pop_forward(sfl);
}
