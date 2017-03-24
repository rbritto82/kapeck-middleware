#ifndef _SEMAPHORE_H_
#define _SEMAPHORE_H_

class semaphore
{
 private:
  int id;
  static bool stop_on_errors;
  int create(int key, int flags1, int flags2);
  int changevalue(int val, bool block, const char *msg);
 public:
  semaphore(void);
  void set_stop_on_errors(void);
  void unset_stop_on_errors(void);
  int create_only(int key, int flags=0666, int val=0);
  int connect_only(int key);
  int connect_or_create(int key, int flags=0666, int val=0);
  int set_value(int val=0);
  int remove(void);
  int increment(int val=1,bool block=true);
  int decrement(int val=1,bool block=true);
  int test_zero(bool block=true);
};


#endif
